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
prefix = 'dash/'
framerate = 60

resolutions=['640x360', '854x480', '1280x720', '1920x1080']#, '2560x1440']
bitrates=[1.5, 4, 7.5, 12]#, 24]

def check_and_create(dir_path):
    dir_path = os.getcwd() + '/' + dir_path
    print ("Checking: %s" % dir_path)
    if not os.path.isdir(dir_path):
        print ('Destination directory: %s does not exist, creating one' % dir_path)
        os.mkdir(dir_path)
    else:
        print ('Found directory: %s ' % dir_path)

def encode(idx):
	quality = resolutions[idx].split('x')[1]
	destination = ( prefix if prefix else '' ) + '%s/bbb_%s_%s.mp4' % (quality, quality, framerate)
	print ('dest: %s' % destination)
	dst_dir = destination.rsplit('/', 1)[0]
	if dst_dir:
		check_and_create(dst_dir)

	#print("ffmpeg -i " + source + " -vf scale=" + resolutions[idx] + "
	cmd = "ffmpeg -i " + source + " -vf scale=" + resolutions[idx] + " -b:v " + str(bitrates[idx]) + "M -bufsize " + str(bitrates[idx]/2) + "M -c:v libx264 -x264opts 'keyint=60:min-keyint=60:no-scenecut' -c:a copy " + destination
	print("Encoding %s: " % cmd)
	os.system(cmd)

	print ('Done encoding %sp' % quality) 

def main_encode():
	for i in range(len(resolutions)):
		print('Starting %s thread' % resolutions[i])
		t = Thread(target=encode, args=(i,))
		t.start()

	print ('Started all threads')

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--prefix', '-p', help='Prefix')
    parser.add_argument('--fps', help="Frames per second to use for re-encoding")
    parser.add_argument('-i', '--input', required=True,
                        help='The path to the video file (required).')
    args = parser.parse_args()

    global prefix
    global source
    global framerate

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
    check_and_create(prefix)

    main_encode()

if __name__ == "__main__":
    sys.exit(main())