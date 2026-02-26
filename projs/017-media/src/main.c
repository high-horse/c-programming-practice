#include <libavcodec/codec.h>
#include <libavcodec/codec_par.h>
#include <libavcodec/packet.h>
#include <libavutil/avutil.h>
#include <libavutil/frame.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <time.h>

int main(int argc, char *argv[]) 
{
    char *media_file = "/home/camel/Desktop/extra/C/C-Programming-Practice/projs/017-media/sample.mp4";
    if(argc == 2){
        media_file = argv[1];
    }
    printf("reading file %s\n", media_file);
    
    AVFormatContext *pfmt_ctx = NULL;
    if(avformat_open_input(&pfmt_ctx, media_file, NULL, NULL) != EXIT_SUCCESS){
        perror("Failed to open input:");
        exit(EXIT_FAILURE);
    }
    
    int video_stream_index = -1;
    for (int i = 0; pfmt_ctx->nb_streams; i++) {
        if(pfmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_index = i;
            break;
        }
    }
    
    AVCodecParameters *pcodec_params = pfmt_ctx->streams[video_stream_index]->codecpar;
    const AVCodec *pcodec = avcodec_find_decoder(pcodec_params->codec_id);
    AVCodecContext *pcodec_ctx = avcodec_alloc_context3(pcodec);
    
    avcodec_parameters_to_context(pcodec_ctx, pcodec_params);
    avcodec_open2(pcodec_ctx, pcodec, NULL);
    

    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    
    struct SwsContext *sws_context = sws_getContext(
        pcodec_ctx->width, 
        pcodec_ctx->height, 
        pcodec_ctx->pix_fmt,
        pcodec_ctx->width,
        pcodec_ctx->height,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR, NULL, NULL, NULL
    );
    
    int yuyv_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pcodec_ctx->width, pcodec_ctx->height, 1);
    uint8_t *yuyv_buffer = av_malloc(yuyv_size);
    
    int frame_count = 0;
    while(av_read_frame(pfmt_ctx, packet) == EXIT_SUCCESS) {
        if(packet->stream_index == video_stream_index){
            avcodec_send_packet(pcodec_ctx, packet);
            while(avcodec_receive_frame(pcodec_ctx, frame) == 0){
                // ++frame_count;
                // printf("frame %d\n", pcodec_ctx->frame_number);
                uint8_t *dest_data[4] = {yuyv_buffer, NULL, NULL, NULL};
                int dest_linesize[4] = {2 * pcodec_ctx->width, 0, 0, 0};
                
                sws_scale(sws_context, 
                    (const uint8_t *const *)frame->data, 
                    frame->linesize, 
                    0, pcodec_ctx->height, dest_data, dest_linesize);
                printf("converted frame %d YUYV\n", ++frame_count);
            }
        }
        av_packet_unref(packet);
    }
    
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&pcodec_ctx);
    avformat_close_input(&pfmt_ctx);

	return EXIT_SUCCESS;
}