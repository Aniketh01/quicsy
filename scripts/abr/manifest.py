import argparse
import sys
import logging
import datetime
import time
import os
from threading import Thread

start_time = time.time()

logging.basicConfig(filename='manifest_creation-' +
                    str(start_time) + '.log', level=logging.DEBUG)


def check_and_create(dir_path):
	print ("Checking: %s" % dir_path)
	if not os.path.isdir(dir_path):
		print ('Destination directory: %s does not exist, creating one' % dir_path)
		os.mkdir(dir_path)
	else:
		print ('Found directory: %s ' % dir_path)


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('-i', '--input', required=True,
                        help='The path to the video file (required).')
    args = parser.parse_args()

    logging.debug("input: " + args.input + ", datetime: " + str(datetime.datetime.now()))


if __name__ == "__main__":
    sys.exit(main())