#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>
#include <gst/gst.h>
#include <glib.h>
#include <unistd.h>
#include <signal.h>

typedef struct {
    DBusConnection *conn;
    char *filename;
    GstElement *pipeline;
    GMainLoop *loop;
    guint timeout_id;
    dbus_bool_t is_recording;
} ScreenRecorder;

static ScreenRecorder *g_recorder = NULL;

/**
 * Initialize GStreamer
 */
gboolean init_gstreamer(int *argc, char ***argv) {
    GError *error = NULL;
    gboolean ret = gst_init_check(argc, argv, &error);
    
    if (!ret) {
        fprintf(stderr, "Failed to initialize GStreamer: %s\n",
                error ? error->message : "unknown error");
        if (error) g_error_free(error);
        return FALSE;
    }
    
    printf("✓ GStreamer initialized\n");
    return TRUE;
}

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
        fprintf(stderr, "D-Bus connection note: %s\n", error.message);
        dbus_error_free(&error);
    }

    recorder->filename = malloc(strlen(output_file) + 1);
    strcpy(recorder->filename, output_file);

    recorder->pipeline = NULL;
    recorder->loop = g_main_loop_new(NULL, FALSE);
    recorder->timeout_id = 0;
    recorder->is_recording = FALSE;

    printf("✓ D-Bus connected\n");
    printf("✓ Screen recorder initialized\n");
    return recorder;
}

/**
 * Query PipeWire via D-Bus
 */
void query_pipewire_dbus(ScreenRecorder *recorder) {
    DBusError error;
    DBusMessage *msg, *reply;
    dbus_error_init(&error);

    printf("\n=== Querying PipeWire via D-Bus ===\n");

    msg = dbus_message_new_method_call(
        "org.PipeWire.Core1",
        "/org/pipewire/core1",
        "org.freedesktop.DBus.Properties",
        "GetAll"
    );

    if (!msg) {
        printf("Note: PipeWire D-Bus interface not available\n");
        printf("====================================\n\n");
        return;
    }

    const char *interface = "org.PipeWire.Core1";
    dbus_message_append_args(msg, DBUS_TYPE_STRING, &interface, DBUS_TYPE_INVALID);

    reply = dbus_connection_send_with_reply_and_block(
        recorder->conn, msg, 1000, &error
    );

    dbus_message_unref(msg);

    if (dbus_error_is_set(&error)) {
        printf("PipeWire query: %s (continuing)\n", error.message);
        dbus_error_free(&error);
    } else if (reply) {
        printf("✓ PipeWire Core1 accessible via D-Bus\n");
        dbus_message_unref(reply);
    }

    printf("====================================\n\n");
}

/**
 * GStreamer bus message handler
 */
static gboolean bus_call(GstBus *bus, GstMessage *msg, gpointer data) {
    ScreenRecorder *recorder = (ScreenRecorder *)data;

    switch (GST_MESSAGE_TYPE(msg)) {
        case GST_MESSAGE_EOS:
            printf("\n✓ End of stream\n");
            g_main_loop_quit(recorder->loop);
            break;

        case GST_MESSAGE_ERROR: {
            GError *err = NULL;
            gchar *debug = NULL;
            gst_message_parse_error(msg, &err, &debug);
            fprintf(stderr, "\n✗ Error: %s\n", err->message);
            if (debug) {
                fprintf(stderr, "  Debug: %s\n", debug);
                g_free(debug);
            }
            g_error_free(err);
            g_main_loop_quit(recorder->loop);
            break;
        }

        case GST_MESSAGE_WARNING: {
            GError *err = NULL;
            gchar *debug = NULL;
            gst_message_parse_warning(msg, &err, &debug);
            printf("⚠ Warning: %s\n", err->message);
            g_error_free(err);
            if (debug) g_free(debug);
            break;
        }

        case GST_MESSAGE_STATE_CHANGED: {
            GstState old_state, new_state, pending_state;
            gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
            if (GST_MESSAGE_SRC(msg) == GST_OBJECT(recorder->pipeline)) {
                printf("Pipeline: %s → %s\n",
                       gst_element_state_get_name(old_state),
                       gst_element_state_get_name(new_state));
            }
            break;
        }

        default:
            break;
    }

    return TRUE;
}

/**
 * Timeout callback
 */
static gboolean timeout_callback(gpointer data) {
    ScreenRecorder *recorder = (ScreenRecorder *)data;
    printf("\n⏱ Duration reached\n");
    
    if (recorder->pipeline) {
        gst_element_send_event(recorder->pipeline, gst_event_new_eos());
    }
    
    return FALSE;
}

/**
 * Build GStreamer pipeline - simplified to work with pipewiresrc
 */
GstElement* build_pipeline(ScreenRecorder *recorder) {
    GstElement *pipeline, *pipewiresrc, *videoconvert, *capsfilter;
    GstElement *videorate, *x264enc, *h264parse, *mp4mux, *filesink;
    GstCaps *caps;

    printf("\n=== Building GStreamer Pipeline ===\n");

    // Create pipeline
    pipeline = gst_pipeline_new("screen-recorder");
    if (!pipeline) {
        fprintf(stderr, "Failed to create pipeline\n");
        return NULL;
    }

    // Create elements
    pipewiresrc = gst_element_factory_make("pipewiresrc", "source");
    videoconvert = gst_element_factory_make("videoconvert", "convert");
    capsfilter = gst_element_factory_make("capsfilter", "filter");
    videorate = gst_element_factory_make("videorate", "rate");
    x264enc = gst_element_factory_make("x264enc", "encoder");
    h264parse = gst_element_factory_make("h264parse", "parse");
    mp4mux = gst_element_factory_make("mp4mux", "mux");
    filesink = gst_element_factory_make("filesink", "sink");

    // Check elements
    if (!pipewiresrc || !videoconvert || !capsfilter || !videorate ||
        !x264enc || !h264parse || !mp4mux || !filesink) {
        fprintf(stderr, "✗ Failed to create elements\n");
        if (pipeline) gst_object_unref(pipeline);
        return NULL;
    }

    printf("✓ All elements created\n");

    // Configure pipewiresrc
    g_object_set(G_OBJECT(pipewiresrc),
                 "do-timestamp", TRUE,
                 "always-copy", FALSE,
                 NULL);

    // Configure capsfilter - let pipewiresrc output any format, then convert
    caps = gst_caps_new_simple("video/x-raw",
                               "width", G_TYPE_INT, 1920,
                               "height", G_TYPE_INT, 1080,
                               NULL);
    g_object_set(G_OBJECT(capsfilter), "caps", caps, NULL);
    gst_caps_unref(caps);

    // Configure videorate
    g_object_set(G_OBJECT(videorate),
                 "rate", 30.0,
                 NULL);

    // Configure x264enc
    g_object_set(G_OBJECT(x264enc),
                 "speed-preset", 0,  // ultrafast
                 "bitrate", 5000,
                 "aud", FALSE,
                 NULL);

    // Configure filesink
    g_object_set(G_OBJECT(filesink),
                 "location", recorder->filename,
                 NULL);

    // Add elements to bin
    gst_bin_add_many(GST_BIN(pipeline),
                     pipewiresrc, videoconvert, capsfilter, videorate,
                     x264enc, h264parse, mp4mux, filesink, NULL);

    printf("✓ Elements added\n");

    // Link elements
    if (!gst_element_link(pipewiresrc, videoconvert)) {
        fprintf(stderr, "Failed to link pipewiresrc → videoconvert\n");
        gst_object_unref(pipeline);
        return NULL;
    }

    if (!gst_element_link(videoconvert, capsfilter)) {
        fprintf(stderr, "Failed to link videoconvert → capsfilter\n");
        gst_object_unref(pipeline);
        return NULL;
    }

    if (!gst_element_link(capsfilter, videorate)) {
        fprintf(stderr, "Failed to link capsfilter → videorate\n");
        gst_object_unref(pipeline);
        return NULL;
    }

    if (!gst_element_link(videorate, x264enc)) {
        fprintf(stderr, "Failed to link videorate → x264enc\n");
        gst_object_unref(pipeline);
        return NULL;
    }

    if (!gst_element_link(x264enc, h264parse)) {
        fprintf(stderr, "Failed to link x264enc → h264parse\n");
        gst_object_unref(pipeline);
        return NULL;
    }

    if (!gst_element_link(h264parse, mp4mux)) {
        fprintf(stderr, "Failed to link h264parse → mp4mux\n");
        gst_object_unref(pipeline);
        return NULL;
    }

    if (!gst_element_link(mp4mux, filesink)) {
        fprintf(stderr, "Failed to link mp4mux → filesink\n");
        gst_object_unref(pipeline);
        return NULL;
    }

    printf("✓ All elements linked\n");
    printf("Pipeline: pipewiresrc ! videoconvert ! capsfilter ! videorate ! x264enc ! h264parse ! mp4mux ! filesink\n");
    printf("====================================\n\n");

    return pipeline;
}

/**
 * Start recording
 */
dbus_bool_t screenrecorder_start(ScreenRecorder *recorder, int duration) {
    if (!recorder) {
        fprintf(stderr, "Invalid recorder\n");
        return FALSE;
    }

    printf("=== Starting Recording ===\n\n");

    // Build pipeline
    recorder->pipeline = build_pipeline(recorder);
    if (!recorder->pipeline) {
        return FALSE;
    }

    // Set up bus
    GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(recorder->pipeline));
    gst_bus_add_watch(bus, bus_call, recorder);
    gst_object_unref(bus);

    // Set to PLAYING
    printf("Setting state to PLAYING...\n");
    GstStateChangeReturn ret = gst_element_set_state(recorder->pipeline, GST_STATE_PLAYING);

    if (ret == GST_STATE_CHANGE_FAILURE) {
        fprintf(stderr, "✗ Failed to set PLAYING state\n");
        
        // Try to get error
        GstBus *err_bus = gst_pipeline_get_bus(GST_PIPELINE(recorder->pipeline));
        GstMessage *err_msg = gst_bus_pop_filtered(err_bus, GST_MESSAGE_ERROR);
        
        if (err_msg) {
            GError *error = NULL;
            gst_message_parse_error(err_msg, &error, NULL);
            if (error) {
                fprintf(stderr, "  Error: %s\n", error->message);
                g_error_free(error);
            }
            gst_message_unref(err_msg);
        }
        
        gst_object_unref(err_bus);
        gst_element_set_state(recorder->pipeline, GST_STATE_NULL);
        gst_object_unref(recorder->pipeline);
        recorder->pipeline = NULL;
        return FALSE;
    }

    recorder->is_recording = TRUE;

    printf("✓ Recording started\n");
    printf("  Output: %s\n", recorder->filename);
    printf("  Duration: %d seconds\n\n", duration);

    // Schedule timeout
    recorder->timeout_id = g_timeout_add_seconds(duration, timeout_callback, recorder);

    return TRUE;
}

/**
 * Stop recording
 */
dbus_bool_t screenrecorder_stop(ScreenRecorder *recorder) {
    if (!recorder || !recorder->pipeline) {
        return FALSE;
    }

    printf("\nStopping...\n");

    if (recorder->timeout_id) {
        g_source_remove(recorder->timeout_id);
        recorder->timeout_id = 0;
    }

    gst_element_send_event(recorder->pipeline, gst_event_new_eos());
    recorder->is_recording = FALSE;
    
    return TRUE;
}

/**
 * Cleanup
 */
void screenrecorder_free(ScreenRecorder *recorder) {
    if (!recorder) return;

    if (recorder->pipeline) {
        gst_element_set_state(recorder->pipeline, GST_STATE_NULL);
        gst_object_unref(recorder->pipeline);
    }

    if (recorder->loop) {
        if (g_main_loop_is_running(recorder->loop)) {
            g_main_loop_quit(recorder->loop);
        }
        g_main_loop_unref(recorder->loop);
    }

    if (recorder->conn) {
        dbus_connection_unref(recorder->conn);
    }

    if (recorder->filename) {
        free(recorder->filename);
    }

    free(recorder);
}

/**
 * Signal handler
 */
void signal_handler(int sig) {
    printf("\n⚠ Signal %d received\n", sig);
    if (g_recorder) {
        screenrecorder_stop(g_recorder);
    }
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

    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║    Wayland Screen Recorder                        ║\n");
    printf("║    D-Bus + PipeWire + GStreamer                   ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n\n");

    printf("Output: %s\n", output_file);
    printf("Duration: %d seconds\n\n", recording_duration);

    const char *session = getenv("XDG_SESSION_TYPE");
    const char *desktop = getenv("XDG_CURRENT_DESKTOP");
    printf("Session: %s\n", session ? session : "unknown");
    printf("Desktop: %s\n\n", desktop ? desktop : "unknown");

    if (!init_gstreamer(&argc, &argv)) {
        return EXIT_FAILURE;
    }

    ScreenRecorder *recorder = screenrecorder_new(output_file);
    if (!recorder) {
        return EXIT_FAILURE;
    }

    g_recorder = recorder;
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    query_pipewire_dbus(recorder);

    if (!screenrecorder_start(recorder, recording_duration)) {
        screenrecorder_free(recorder);
        return EXIT_FAILURE;
    }

    printf("Press Ctrl+C to stop\n\n");

    g_main_loop_run(recorder->loop);

    printf("\n✓ Recording complete!\n");
    printf("  Saved: %s\n", output_file);
    printf("  Play: ffplay %s\n\n", output_file);

    screenrecorder_free(recorder);

    return EXIT_SUCCESS;
}