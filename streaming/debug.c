#include "debug.h"

#include <inttypes.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/timestamp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dbg_log(const char *fmt, ...)
{
    va_list args;
    fprintf( stdout, "LOG: " );
    va_start( args, fmt );
    vfprintf( stdout, fmt, args );
    va_end( args );
    fprintf( stdout, "\n" );
}

void err_log(const char *fmt, ...)
{
    va_list args;
    fprintf( stderr, "ERROR: " );
    va_start( args, fmt );
    vfprintf( stderr, fmt, args );
    va_end( args );
    fprintf( stderr, "\n" );
}

void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
	AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

	dbg_log("pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s "
			"stream_index:%d",
			av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
			av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
			av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
			pkt->stream_index);
}

void print_timing(char *name, AVFormatContext *avf, AVCodecContext *avc,
				  AVStream *avs)
{
	dbg_log("=================================================");
	dbg_log("%s", name);

	dbg_log("\tAVFormatContext");
	if (avf != NULL)
	{
		dbg_log("\t\tstart_time=%d duration=%d bit_rate=%d start_time_realtime=%d",
				avf->start_time, avf->duration, avf->bit_rate,
				avf->start_time_realtime);
	}
	else
	{
		dbg_log("\t\t->NULL");
	}

	dbg_log("\tAVCodecContext");
	if (avc != NULL)
	{
		dbg_log("\t\tbit_rate=%d ticks_per_frame=%d width=%d height=%d gop_size=%d "
				"keyint_min=%d sample_rate=%d profile=%d level=%d ",
				avc->bit_rate, avc->ticks_per_frame, avc->width, avc->height,
				avc->gop_size, avc->keyint_min, avc->sample_rate, avc->profile,
				avc->level);
		dbg_log("\t\tavc->time_base=num/den %d/%d", avc->time_base.num,
				avc->time_base.den);
		dbg_log("\t\tavc->framerate=num/den %d/%d", avc->framerate.num,
				avc->framerate.den);
		dbg_log("\t\tavc->pkt_timebase=num/den %d/%d", avc->pkt_timebase.num,
				avc->pkt_timebase.den);
	}
	else
	{
		dbg_log("\t\t->NULL");
	}

	dbg_log("\tAVStream");
	if (avs != NULL)
	{
		dbg_log("\t\tindex=%d start_time=%d duration=%d ", avs->index,
				avs->start_time, avs->duration);
		dbg_log("\t\tavs->time_base=num/den %d/%d", avs->time_base.num,
				avs->time_base.den);
		dbg_log("\t\tavs->sample_aspect_ratio=num/den %d/%d",
				avs->sample_aspect_ratio.num, avs->sample_aspect_ratio.den);
		dbg_log("\t\tavs->avg_frame_rate=num/den %d/%d", avs->avg_frame_rate.num,
				avs->avg_frame_rate.den);
		dbg_log("\t\tavs->r_frame_rate=num/den %d/%d", avs->r_frame_rate.num,
				avs->r_frame_rate.den);
	}
	else
	{
		dbg_log("\t\t->NULL");
	}

	dbg_log("=================================================");
}
