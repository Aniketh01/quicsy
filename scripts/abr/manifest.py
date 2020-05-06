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

source = 'Big_Buck_Bunny_1080_10s_10MB.mp4'
prefix = '/dash/'
framerate = 60

def check_and_create(dir_path):
	print ("Checking: %s" % dir_path)
	if not os.path.isdir(dir_path):
		print ('Destination directory: %s does not exist, creating one' % dir_path)
		os.mkdir(dir_path)
	else:
		print ('Found directory: %s ' % dir_path)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--prefix', '-p', help='Prefix')
    parser.add_argument('--fps', help="Frames per second to use for re-encoding")
    parser.add_argument('-i', '--input', required=True,
                        help='The path to the video file (required).')
    args = parser.parse_args()

    if args.prefix:
        if not args.prefix.endswith('/'):
            prefix = args.prefix + '/'
        else:
            prefix = args.prefix
    else:
        print ("No prefix given")
    
    if args.input:
        source = args.input 

    if args.fps:
        framerate = args.fps


    logging.debug("input: " + args.input + ", datetime: " + str(datetime.datetime.now()))


if __name__ == "__main__":
    sys.exit(main())