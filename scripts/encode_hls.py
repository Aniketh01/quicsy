import argparse
import datetime
import sys
import time
import logging

import ffmpeg_streaming

from ffmpeg_streaming import Representation

start_time = time.time()
logging.basicConfig(filename='Transcoding-' + str(start_time) + '.log', level=logging.DEBUG)


start_time = time.time()


def per_to_time_left(percentage):
    if percentage != 0:
        diff_time = time.time() - start_time
        seconds_left = 100 * diff_time / percentage - diff_time
        time_left = str(datetime.timedelta(seconds=int(seconds_left))) + ' left'
    else:
        time_left = 'calculating...'

    return time_left


def transcode_progress(per, ffmpeg):
    # You can update a field in your database or can log it to a file
    # You can also create a socket connection and show a progress bar to users
    logging.debug(ffmpeg)
    sys.stdout.write("\rTranscoding...(%s%%) %s [%s%s]" % (per, per_to_time_left(per), '#' * per, '-' * (100 - per)))
    sys.stdout.flush()


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('-i', '--input', required=True, help='The path to the video file (required).')
    parser.add_argument('-o', '--output', default=None, help='The output to write files.')

    args = parser.parse_args()

    logging.debug("input: " + args.input
                  + ", output: " + str(args.output)
                  + ", datetime: " + str(datetime.datetime.now()))

    r_360p = Representation(width=640, height=360, kilo_bitrate=276)
    r_480p = Representation(width=854, height=480, kilo_bitrate=750)
    r_720p = Representation(width=1280, height=720, kilo_bitrate=2048)
    r_1080p = Representation(width=1920, height=1080, kilo_bitrate=4096)

    (
        ffmpeg_streaming
            .hls(args.input, hls_time=10, hls_allow_cache=1)
            .format('libx264')
            .add_rep(r_360p, r_480p, r_720p, r_1080p)
            .package(args.output, progress=transcode_progress)
    )


if __name__ == "__main__":
    sys.exit(main())
