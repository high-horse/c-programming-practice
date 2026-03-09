#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/videodev2.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

int main(int argc, char *argv[]) {
    char *video_path = "test.mp4";
    if(argc == 2) {
        video_path = argv[1];
    }
    printf("video file is %s\n", video_path);
    const char *v4l2_path = "/dev/video9";

    // 1. Open Video File
    AVFormatContext *fmt_ctx = NULL;
    avformat_open_input(&fmt_ctx, video_path, NULL, NULL);
    avformat_find_stream_info(fmt_ctx, NULL);
    
    printf("video file opened\n");

    int stream_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    AVCodecParameters *params = fmt_ctx->streams[stream_idx]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(params->codec_id);
    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, params);
    avcodec_open2(codec_ctx, codec, NULL);

    // 2. Setup V4L2 Loopback Device
    int v4l2_fd = open(v4l2_path, O_RDWR);
    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    fmt.fmt.pix.width = codec_ctx->width;
    fmt.fmt.pix.height = codec_ctx->height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // 4:2:2 Packed
    fmt.fmt.pix.field = V4L2_FIELD_NONE;
    ioctl(v4l2_fd, VIDIOC_S_FMT, &fmt);

    // 3. Setup Scaler (Converts Decoded Frame -> YUYV)
    struct SwsContext *sws_ctx = sws_getContext(
        codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
        codec_ctx->width, codec_ctx->height, AV_PIX_FMT_YUYV422,
        SWS_BILINEAR, NULL, NULL, NULL
    );

    AVFrame *frame = av_frame_alloc();
    AVPacket *pkt = av_packet_alloc();
    
    // Allocate YUYV buffer (width * height * 2 bytes)
    int yuyv_size = codec_ctx->width * codec_ctx->height * 2;
    uint8_t *yuyv_buffer = malloc(yuyv_size);

    // 4. Decode and Write Loop
    while (av_read_frame(fmt_ctx, pkt) >= 0) {
        if (pkt->stream_index == stream_idx) {
            if (avcodec_send_packet(codec_ctx, pkt) == 0) {
                while (avcodec_receive_frame(codec_ctx, frame) == 0) {
                    // Convert to YUYV
                    uint8_t *dst_data[4] = {yuyv_buffer, NULL, NULL, NULL};
                    int dst_linesize[4] = {codec_ctx->width * 2, 0, 0, 0};
                    sws_scale(sws_ctx, (const uint8_t *const *)frame->data, frame->linesize, 
                              0, codec_ctx->height, dst_data, dst_linesize);
                    

                    // Write to camera
                    write(v4l2_fd, yuyv_buffer, yuyv_size);
                    usleep(50000);
                }
            }
        }
        av_packet_unref(pkt);
    }

    // Cleanup
    free(yuyv_buffer);
    close(v4l2_fd);
    return 0;
}
