#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
    DBusConnection *conn;
    char *filename;
    pid_t ffmpeg_pid;
    char *pipewire_node;
    dbus_bool_t is_recording;
} ScreenRecorder;

/**
 * Initialize D-Bus connection
 */
ScreenRecorder* screenrecorder_new(const char *output_file) {
    ScreenRecorder *recorder = malloc(sizeof(ScreenRecorder));
    if (!recorder) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    DBusError error;
    dbus_error_init(&error);

    recorder->conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Failed to connect to session bus: %s\n", error.message);
        dbus_error_free(&error);
        free(recorder);
        return NULL;
    }

    recorder->filename = malloc(strlen(output_file) + 1);
    strcpy(recorder->filename, output_file);

    recorder->pipewire_node = NULL;
    recorder->ffmpeg_pid = -1;
    recorder->is_recording = FALSE;

    printf("Connected to D-Bus session\n");
    return recorder;
}

/**
 * List available PipeWire sources using pw-dump
 */
void list_pipewire_sources() {
    printf("\n=== Available PipeWire Sources ===\n");
    system("pw-dump | grep -A 5 '\"type\": \"PipeWire:Interface:Node\"' | grep -E '\"id\"|\"name\"'");
    printf("===================================\n\n");
}

/**
 * Get available screen capture source using DBus portal
 */
char* get_screencast_source(ScreenRecorder *recorder) {
    DBusError error;
    DBusMessage *msg, *reply;
    dbus_error_init(&error);

    // Call SelectSources method on screencast portal
    msg = dbus_message_new_method_call(
        "org.freedesktop.portal.Desktop",
        "/org/freedesktop/portal/desktop",
        "org.freedesktop.portal.ScreenCast",
        "SelectSources"
    );

    if (!msg) {
        fprintf(stderr, "Failed to create SelectSources message\n");
        return NULL;
    }

    // Add session handle (empty for now)
    const char *session = "";
    DBusMessageIter iter, dict_iter;
    dbus_message_iter_init_append(msg, &iter);

    // Add empty options dict
    if (!dbus_message_iter_open_container(&iter, DBUS_TYPE_ARRAY,
                                          "{sv}", &dict_iter)) {
        dbus_message_unref(msg);
        return NULL;
    }
    dbus_message_iter_close_container(&iter, &dict_iter);

    reply = dbus_connection_send_with_reply_and_block(
        recorder->conn, msg, -1, &error
    );

    dbus_message_unref(msg);

    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Portal error: %s\n", error.message);
        dbus_error_free(&error);
        return NULL;
    }

    if (reply) {
        dbus_message_unref(reply);
        return strdup("portal");
    }

    return NULL;
}

/**
 * Record using FFmpeg with PipeWire
 */
dbus_bool_t screenrecorder_start(ScreenRecorder *recorder, int duration) {
    if (!recorder) {
        fprintf(stderr, "Invalid recorder\n");
        return FALSE;
    }

    printf("Starting Wayland screen recording with PipeWire...\n");

    recorder->ffmpeg_pid = fork();
    
    if (recorder->ffmpeg_pid == -1) {
        fprintf(stderr, "Failed to fork\n");
        return FALSE;
    }

    if (recorder->ffmpeg_pid == 0) {
        // Child process - run ffmpeg
        char duration_str[32];
        snprintf(duration_str, sizeof(duration_str), "%d", duration);

        printf("Launching ffmpeg with PipeWire input\n");

        // Use gst-launch or ffmpeg with PipeWire source
        // Method 1: Using ffmpeg with pipewire plugin (if available)
        execlp("ffmpeg",
               "ffmpeg",
               "-f", "pipewire",
               "-i", "default_output",
               "-c:v", "libx264",
               "-preset", "ultrafast",
               "-crf", "23",
               "-t", duration_str,
               "-y",
               recorder->filename,
               NULL);

        // Fallback to gst-launch if ffmpeg fails
        execlp("gst-launch-1.0",
               "gst-launch-1.0",
               "-e",
               "pipewiresrc",
               "!",
               "video/x-raw,format=NV12",
               "!",
               "x264enc",
               "speed-preset=ultrafast",
               "!",
               "mp4mux",
               "!",
               "filesink", "location=" recorder->filename,
               NULL);

        perror("ffmpeg/gst-launch");
        exit(1);
    } else {
        recorder->is_recording = TRUE;
        printf("Screen recording started (PID: %d)\n", recorder->ffmpeg_pid);
        printf("Output: %s\n", recorder->filename);
        return TRUE;
    }
}

/**
 * Stop recording
 */
dbus_bool_t screenrecorder_stop(ScreenRecorder *recorder) {
    if (!recorder || recorder->ffmpeg_pid <= 0) {
        fprintf(stderr, "Recording not in progress\n");
        return FALSE;
    }

    printf("Stopping screen recording...\n");
    
    if (kill(recorder->ffmpeg_pid, SIGTERM) == 0) {
        int status;
        waitpid(recorder->ffmpeg_pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Recording stopped\n");
            recorder->is_recording = FALSE;
            recorder->ffmpeg_pid = -1;
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * Cleanup
 */
void screenrecorder_free(ScreenRecorder *recorder) {
    if (!recorder) return;

    if (recorder->ffmpeg_pid > 0) {
        kill(recorder->ffmpeg_pid, SIGKILL);
    }
    if (recorder->conn) {
        dbus_connection_unref(recorder->conn);
    }
    if (recorder->filename) free(recorder->filename);
    if (recorder->pipewire_node) free(recorder->pipewire_node);
    free(recorder);
}

/**
 * Signal handler
 */
static ScreenRecorder *g_recorder = NULL;

void signal_handler(int sig) {
    printf("\nReceived signal %d, stopping recording...\n", sig);
    if (g_recorder) {
        screenrecorder_stop(g_recorder);
        screenrecorder_free(g_recorder);
    }
    exit(0);
}

/**
 * Main program
 */
int main(int argc, char *argv[]) {
    const char *output_file = "screencast.mp4";
    int recording_duration = 10;

    if (argc > 1) {
        output_file = argv[1];
    }
    if (argc > 2) {
        recording_duration = atoi(argv[2]);
    }

    printf("=== Wayland Screen Recorder (PipeWire + D-Bus) ===\n");
    printf("Output file: %s\n", output_file);
    printf("Recording duration: %d seconds\n\n", recording_duration);

    // Check Wayland
    const char *session_type = getenv("XDG_SESSION_TYPE");
    if (!session_type || strcmp(session_type, "wayland") != 0) {
        fprintf(stderr, "WARNING: Not running on Wayland (XDG_SESSION_TYPE=%s)\n", 
                session_type ? session_type : "not set");
    }

    // Check dependencies
    if (system("which ffmpeg > /dev/null 2>&1") != 0) {
        fprintf(stderr, "ffmpeg not found. Install it with: sudo apt install ffmpeg\n");
        return EXIT_FAILURE;
    }

    if (system("which gst-launch-1.0 > /dev/null 2>&1") != 0) {
        printf("WARNING: gst-launch-1.0 not found. Install with: sudo apt install gstreamer1.0-tools\n");
    }

    if (system("which pw-dump > /dev/null 2>&1") != 0) {
        fprintf(stderr, "PipeWire tools not found. Install with: sudo apt install pipewire-tools\n");
    }

    list_pipewire_sources();

    ScreenRecorder *recorder = screenrecorder_new(output_file);
    if (!recorder) {
        return EXIT_FAILURE;
    }

    g_recorder = recorder;
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    if (!screenrecorder_start(recorder, recording_duration)) {
        screenrecorder_free(recorder);
        return EXIT_FAILURE;
    }

    printf("Recording in progress (press Ctrl+C to stop)...\n\n");
    sleep(recording_duration);

    screenrecorder_stop(recorder);

    printf("\nRecording saved to: %s\n", output_file);
    printf("Verify with: ffprobe %s\n", output_file);
    
    screenrecorder_free(recorder);

    return EXIT_SUCCESS;
}