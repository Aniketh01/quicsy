extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <inttypes.h>
}

typedef struct decoder_st {
    // Public things for other parts of the program to read from
    int width = 0, height = 0;
    AVRational time_base;

    // Private internal state
    AVFormatContext* av_format_ctx = NULL;
    AVCodecContext* av_codec_ctx = NULL;
    int video_stream_index = -1;
    AVFrame* av_frame = NULL;
    AVPacket* av_packet = NULL;
    SwsContext* sws_scaler_ctx = NULL;
} decoder_t;

bool quicsy_decoder_open(decoder_t* dec, const char* filename);
bool quicsy_decoder_read_frame(decoder_t* dec, uint8_t* frame_buffer, int64_t* pts);
void quicsy_decoder_close(decoder_t* dec);

static void log(const char *fmt, ...)
{
    va_list args;
    fprintf( stdout, "LOG: " );
    va_start( args, fmt );
    vfprintf( stdout, fmt, args );
    va_end( args );
    fprintf( stdout, "\n" );
}

static void err_log(const char *fmt, ...)
{
    va_list args;
    fprintf( stderr, "ERROR: " );
    va_start( args, fmt );
    vfprintf( stderr, fmt, args );
    va_end( args );
    fprintf( stderr, "\n" );
}