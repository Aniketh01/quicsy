import argparse
import sys
import logging
import datetime
import time

start_time = time.time()

logging.basicConfig(filename='manifest_creation-' +
                    str(start_time) + '.log', level=logging.DEBUG)


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('-i', '--input', required=True,
                        help='The path to the video file (required).')
    args = parser.parse_args()

    logging.debug("input: " + args.input
                  + ", datetime: " + str(datetime.datetime.now()))


if __name__ == "__main__":
    sys.exit(main())
