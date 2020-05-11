import argparse
import sys
import logging
import datetime
import time
import json
import os
from threading import Thread

start_time = time.time()

logging.basicConfig(filename='manifest_creation-' +
                    str(start_time) + '.log', level=logging.DEBUG)

source = 'Big_Buck_Bunny_1080_10s_10MB.mp4'
prefix = 'dash/'
framerate = 60

frame_type = {'I-Frame': 'PICT_TYPE_I', 'P-Frame': 'PICT_TYPE_P', 'B-Frame': 'PICT_TYPE_B'}
resolutions=['640x360', '854x480', '1280x720', '1920x1080']#, '2560x1440']
bitrates=[1.5, 4, 7.5, 12]#, 24]


def load_json(path):
    with open(path) as file:
        obj = json.load(file)
    return obj


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


def segmentize(in_source, dst_dir):
    print('in:%s out:%s' % (in_source, dst_dir))
    for name, type in frame_type.items():
        cmd = ("ffmpeg -i " + in_source + " -f image2 -vf " + """"select='eq(pict_type,""" +
               type + """)'""" + "\" -vsync vfr " + dst_dir + "/" + name + "%03d.png")
        os.system(cmd)


# Assumes script is ran within the video roots direcoty
def main_segmentize():
    for resolution in resolutions:
        quality = resolution.split('x')[1]
        #dst = res + '/out'
        in_source = ('%s%s/bbb_%s_%s.mp4' % (prefix, quality, quality, framerate))
        check_and_create('%s%s/out' % (prefix, quality))
        out_dir = '%s%s/out' % (prefix, quality)
        segmentize(in_source, out_dir)


def prepare_mpd(seg_duration):
    bitrates_kbps = []
    for b in bitrates:
        bitrates_kbps.append(b*1000)

    manifest = {
        #"Segment_duration_ms": 10000,
        "segment_duration_ms": seg_duration,
        "bitrates_kbps": bitrates_kbps
    }

    with open('bbb_m.json', 'w') as f:
        json.dump(manifest, f, indent=4)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--prefix', '-p', help='Prefix')
    parser.add_argument('--seg_duration', '-sd', help='segment duration')
    parser.add_argument('--action', required=True, help='Action to be performed by the script. Possible actions are: encode, segmentation, mpd')
    parser.add_argument('--fps', help="Frames per second to use for re-encoding")
    parser.add_argument('-i', '--input',
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

    print ('Running "%s" script with arguemnts: prefix(%s) source(%s) fps(%s)' % (args.action, prefix, source, framerate))
    if args.action == 'segmentation':
        main_segmentize()
    elif args.action == 'encode':
        main_encode()
    elif args.action == 'mpd':
        prepare_mpd(args.seg_duration)
    else:
        print("Unknown action requested. Specify one of: truncate, encode")


if __name__ == "__main__":
    sys.exit(main())