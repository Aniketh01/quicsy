#include "decoder.h"

bool quicsy_decoder_open(decoder_t* dec, const char* filename) {

    // Unpack members of state
    auto& width = dec->width;
    auto& height = dec->height;
    auto& time_base = dec->time_base;
    auto& av_format_ctx = dec->av_format_ctx;
    auto& av_codec_ctx = dec->av_codec_ctx;
    auto& video_stream_index = dec->video_stream_index;
    auto& av_frame = dec->av_frame;
    auto& av_packet = dec->av_packet;

    av_register_all();

    // Open the file using libavformat
    av_format_ctx = avformat_alloc_context();
    if (!av_format_ctx) {
        err_log("Couldn't created AVFormatContext");
        return false;
    }

    if (avformat_open_input(&av_format_ctx, filename, NULL, NULL) != 0) {
        err_log("Decoder: Couldn't open video file");
        return false;
    }

	log("format %s, duration %lld us, bit_rate %lld", av_format_ctx->iformat->name, av_format_ctx->duration, av_format_ctx->bit_rate);


    // Find the first valid video stream inside the file
    video_stream_index = -1;
    AVCodecParameters* av_codec_params;
    AVCodec* av_codec;
    for (int i = 0; i < av_format_ctx->nb_streams; ++i) {
        av_codec_params = av_format_ctx->streams[i]->codecpar;

		log("AVStream->time_base before open coded %d/%d", av_format_ctx->streams[i]->time_base.num, av_format_ctx->streams[i]->time_base.den);
		log("AVStream->r_frame_rate before open coded %d/%d", av_format_ctx->streams[i]->r_frame_rate.num, av_format_ctx->streams[i]->r_frame_rate.den);
		log("AVStream->start_time %" PRId64, av_format_ctx->streams[i]->start_time);
		log("AVStream->duration %" PRId64, av_format_ctx->streams[i]->duration);

		log("finding the proper decoder (CODEC)");

		av_codec = avcodec_find_decoder(av_codec_params->codec_id);
        if (av_codec == NULL) {
			err_log("CODEC: couldn't find proper decoder");
            break;
        }
        if (av_codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            width = av_codec_params->width;
            height = av_codec_params->height;
            time_base = av_format_ctx->streams[i]->time_base;
			log("Video Codec: resolution %d x %d", width, height);

            break;
        }

	log("\tCodec %s ID %d bit_rate %lld", av_codec->name, av_codec->id, av_codec_params->bit_rate);
    }

    if (video_stream_index == -1) {
        err_log("Couldn't find valid video stream inside file\n");
        return false;
    }

    // Set up a codec context for the decoder
    av_codec_ctx = avcodec_alloc_context3(av_codec);
    if (!av_codec_ctx) {
        err_log("Couldn't create AVCodecContext\n");
        return false;
    }
    if (avcodec_parameters_to_context(av_codec_ctx, av_codec_params) < 0) {
        err_log("Couldn't initialize AVCodecContext\n");
        return false;
    }
    if (avcodec_open2(av_codec_ctx, av_codec, NULL) < 0) {
        err_log("Couldn't open codec\n");
        return false;
    }

    av_frame = av_frame_alloc();
    if (!av_frame) {
        err_log("Couldn't allocate AVFrame\n");
        return false;
    }
    av_packet = av_packet_alloc();
    if (!av_packet) {
        err_log("Couldn't allocate AVPacket\n");
        return false;
    }

    return true;
}

bool quicsy_decoder_read_frame(decoder_t* dec, uint8_t* frame_buffer, int64_t* pts) {

    // Unpack members of state
    auto& width = dec->width;
    auto& height = dec->height;
    auto& av_format_ctx = dec->av_format_ctx;
    auto& av_codec_ctx = dec->av_codec_ctx;
    auto& video_stream_index = dec->video_stream_index;
    auto& av_frame = dec->av_frame;
    auto& av_packet = dec->av_packet;
    auto& sws_scaler_ctx = dec->sws_scaler_ctx;

    // Decode one frame
	int response;
	bool finished = false;

	while (!finished)
	{
		if (response = (av_read_frame(av_format_ctx, av_packet)) < 0)
		{
			if (response == AVERROR_EOF)
			{
				finished = true;
			}
			else
			{
				exit(0);
			}
		}

		if (av_packet->stream_index != video_stream_index) {
			log("AVPacket->pts %" PRId64, av_packet->pts);
            av_packet_unref(av_packet);
            continue;
        }

        response = avcodec_send_packet(av_codec_ctx, av_packet);
        if (response < 0) {
			err_log("Error while sending a frame from the decoder");
            // FIXME: err_log("Error while receiving a frame from the decoder: %s", av_err2str(response));
            return false;
        }

        response = avcodec_receive_frame(av_codec_ctx, av_frame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
            av_packet_unref(av_packet);
            continue;
		} else if (response < 0) {
			log("Error while receiving a frame from the decoder");
			//FIXME: err_log("Error while receiving a frame from the decoder: %s", av_err2str(response));
			return false;
        }

        av_packet_unref(av_packet);
        break;
	}

	*pts = av_frame->pts;

	// Set up sws scaler
	if (!sws_scaler_ctx)
	{
		sws_scaler_ctx = sws_getContext(width, height, av_codec_ctx->pix_fmt,
										width, height, AV_PIX_FMT_RGB0,
										SWS_BILINEAR, NULL, NULL, NULL);
	}

	if (!sws_scaler_ctx) {
        printf("Couldn't initialize sw scaler\n");
        return false;
    }

    uint8_t* dest[4] = { frame_buffer, NULL, NULL, NULL };
    int dest_linesize[4] = { width * 4, 0, 0, 0 };
    sws_scale(sws_scaler_ctx, av_frame->data, av_frame->linesize, 0, av_frame->height, dest, dest_linesize);

    log(
		"Frame %d (type=%c, size=%d bytes) pts %d key_frame %d [DTS %d]",
        av_codec_ctx->frame_number,
        av_get_picture_type_char(av_frame->pict_type),
        av_frame->pkt_size,
          av_frame->pts,
          av_frame->key_frame,
          av_frame->coded_picture_number
      );

    return true;
}

void quicsy_decoder_close(decoder_t* dec) {
   	sws_freeContext(dec->sws_scaler_ctx);
    avformat_close_input(&dec->av_format_ctx);
    avformat_free_context(dec->av_format_ctx);
    av_frame_free(&dec->av_frame);
    av_packet_free(&dec->av_packet);
    avcodec_free_context(&dec->av_codec_ctx);
}