#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/videodev2.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <signal.h>
#include <string.h> 


#define FHD_WIDTH 1920
#define FHD_HEIGHT 1080
#define WIDTH 640
#define HEIGHT 480
#define PIXEL_FORMAT V4L2_PIX_FMT_YUYV
#define BYTES_PER_PIXEL_YUYV 2 // YUYV is 16 bits per pixel

volatile sig_atomic_t keep_running = 1;
void handle_signal(int sig) {
    (void)sig;
    keep_running = 0;  // just stop the loop
}

void paint_image(void *buffer, size_t buffer_size) {
    // Fill the buffer with a simple test pattern (e.g., a gradient)
    uint8_t *pixel_data = (uint8_t *)buffer;
    for(size_t i = 0; i < buffer_size; i += BYTES_PER_PIXEL_YUYV) {
        uint8_t value = (i / BYTES_PER_PIXEL_YUYV) % 256; // Simple gradient pattern
        pixel_data[i] = value; // Y component (luma)
        pixel_data[i + 1] = 128; // U component (chroma)
        pixel_data[i + 2] = value; // Y component (luma)
        pixel_data[i + 3] = 128; // V component (chroma)
    }
}

int run_command(char *const argv[]){
    pid_t pid = fork();
    if(pid == 0) {
        execvp(argv[0], argv);
        perror("executiion failed:");
        exit(EXIT_FAILURE);
    } else if(pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return status;
    } else {
        perror("fork failed:");
        return EXIT_FAILURE;
    }
}

int startup() {
    char *load[] = {
        "modprobe",
        "v4l2loopback",
        "video_nr=9",
        "card_label=My Fake Webcam",
        "exclusive_caps=1",
        NULL
    };
    if(run_command(load) != 0) {
        fprintf(stderr, "Failed to load v4l2loopback module\n");
        return EXIT_FAILURE;
    } else {
        printf("Loaded v4l2loopback module successfully.\n");
        return EXIT_SUCCESS;
    }
}

void cleanup() {
    // kill any process using the device first
    char *fuser_kill[] = {"fuser", "-k", "/dev/video9", NULL};
    run_command(fuser_kill);
    
    char *unload[] = {"modprobe", "-r", "v4l2loopback", NULL};
    if(run_command(unload) != 0) {
        fprintf(stderr, "Failed to unload v4l2loopback module\n");
    } else {
        printf("Unloaded v4l2loopback module successfully.\n");
    }
}

void cleanup_() {
    char *unload[] = {
        "modprobe",
        "-r",
        "v4l2loopback",
        NULL
    };
    if(run_command(unload) != 0) {
        fprintf(stderr, "Failed to unload v4l2loopback module\n");
    } else {
        printf("Unloaded v4l2loopback module successfully.\n");
    }
}

int main(int argc, char *argv[])
{

    struct sigaction act = {0};
    act.sa_handler = handle_signal;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT,  &act, NULL);  // Ctrl+C
    sigaction(SIGTERM, &act, NULL);  // kill

    // fork and spin up dev device
    const char *filename ;
    if(argc != 2) {
        if(startup() != 0) {
            return EXIT_FAILURE;
        }

        filename = "/dev/video9";
        printf("Loaded v4l2loopback module successfully, using %s\n", filename);

    } else {
        filename = argv[1];
    }
    
    int cam_fd = open(filename, O_RDWR);
    if(cam_fd < 0) {
        perror("FAILED TO OPEN FILE DESCRIPTOR:");
        return EXIT_FAILURE;
    }

    printf("File desc got %d\n", cam_fd);  

    // add capabilities to the device
    // define the format for the images ( videosstream)
    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    // format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = WIDTH;
    format.fmt.pix.height = HEIGHT;
    format.fmt.pix.pixelformat = PIXEL_FORMAT;
    format.fmt.pix.field = V4L2_FIELD_NONE;
    format.fmt.pix.bytesperline = 0; // WIDTH * 2; // YUYV is 16 bits per pixel 
    format.fmt.pix.sizeimage = WIDTH * HEIGHT * BYTES_PER_PIXEL_YUYV; // WIDTH * HEIGHT * 2; // YUYV is 16 bits per pixel
    format.fmt.pix.colorspace = V4L2_COLORSPACE_DEFAULT;

    int err_code = ioctl(cam_fd, VIDIOC_S_FMT, &format);
    if(err_code < 0) {
        printf("FAILED TO SET FORMAT, ERR CODE: %d\n", err_code);
        perror("FAILED TO SET FORMAT:");
        close(cam_fd);
        cleanup();
        return EXIT_FAILURE;
    }

    // feed stream
    // void *image_buffer = calloc(WIDTH * HEIGHT, BYTES_PER_PIXEL_YUYV);
    // double target_fps = 30;
    // while(true) {

    //     paint_image(image_buffer, format.fmt.pix.sizeimage); // Fill the image buffer with pixel data
    //     if(write(cam_fd, image_buffer, format.fmt.pix.sizeimage) < 0) {
    //         perror("FAILED TO WRITE FRAME:");
    //         close(cam_fd);
    //         return EXIT_FAILURE;
    //     }
    //     usleep(1.0 /target_fps * 1000 * 1000); // Sleep to maintain target FPS
    // }

    unsigned char frame[WIDTH * HEIGHT * BYTES_PER_PIXEL_YUYV];
    int frame_id = 0;
    while(keep_running) {
        frame_id++;
        // generate a simple test pattern (e.g., a gradient)
        for(int y = 0; y < HEIGHT; y++) {
            for(int x = 0; x < WIDTH; x++) {
                int index = (y * WIDTH + x) * BYTES_PER_PIXEL_YUYV;
                
                uint8_t value = (x + y + frame_id) % 256;

                // frame[index] = value; // Y component (luma)
                // frame[index + 1] = 128; // U component (chroma)
                // frame[index + 2] = value; // Y component (luma)
                // frame[index + 3] = 128; // V component (chroma)

                uint8_t y_val = (x + frame_id) % 256;
                uint8_t u_val = (y * 2 + frame_id) % 256;
                uint8_t v_val = (x + y + frame_id * 2) % 256;

                frame[index]     = y_val;
                frame[index + 1] = u_val;
                frame[index + 2] = y_val;
                frame[index + 3] = v_val;
            }
        }

        ssize_t bytes_written = write(cam_fd, frame, sizeof(frame));
        if(bytes_written < 0) {
            perror("FAILED TO WRITE FRAME:");
            break;
        }

        // Sleep for a short duration to simulate a frame rate (e.g., 30 FPS)
        struct timespec ts = {0, 33333333}; // ~33ms
        nanosleep(&ts, NULL);
    }


    // fork and close the dev device.
    close(cam_fd);
    if(argc != 2) {
        cleanup();
        // err_code = atexit(cleanup);
        // if (err_code != 0) {
        //     fprintf(stderr, "cannot set exit function\n");
        //     exit(EXIT_FAILURE);
        // }

    }

    return EXIT_SUCCESS;
}