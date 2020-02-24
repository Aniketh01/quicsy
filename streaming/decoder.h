extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

typedef struct decoder_st {
    // Public things for other parts of the program to read from
    int width, height;
    AVRational time_base;

    // Private internal state
    AVFormatContext* av_format_ctx;
    AVCodecContext* av_codec_ctx;
    int video_stream_index;
    AVFrame* av_frame;
    AVPacket* av_packet;
    SwsContext* sws_scaler_ctx;
} decoder_t;

bool quicsy_decoder_open(decoder_t* dec, const char* filename);
bool quicsy_decoder_read_frame(decoder_t* dec, uint8_t* frame_buffer, int64_t* pts);
void quicsy_decoder_close(decoder_t* dec);