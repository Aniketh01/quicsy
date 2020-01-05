from threading import Thread
import os
import time
import sys
import argparse

# ffmpeg -i ../bbb_fragmented.mp4 -vf scale=%s -b:v %sM -bufsize %sM -c:v libx264 -x264opts 'keyint=30:min-keyint=30:no-scenecut' -crf 0 -preset veryslow -c:a copy %s/bbb_%s_%s.mp4

# ffmpeg -i bbb_360_60.mp4 -codec copy -f dash -use_template 0 -min_seg_duration 30 -use_timeline 0 -init_seg_name '$RepresentationID$-init.m4s' -media_seg_name '$RepresentationID$-$Time$.m4s' out/output.mpd

# The 2nd command would chunk the file into m4s segments, where each starts with an I-frame. Size of segments is not consistent!
# the -min_seg_duration flag, forces segments to be split on every I-frame. The intention of this flag is NOT this and it is DEPRECATED! read more on this

resolutions=['640x360', '854x480', '1280x720', '1920x1080']#, '2560x1440']
bitrates=[1.5, 4, 7.5, 12]#, 24]

source = 'bbb_sunflower_1080p_60fps_normal.mp4'
prefix = '/quicsy_dash/'
framerate = 60

def check_and_create(dir_path):
	print ("Checking: %s" % dir_path)
	if not os.path.isdir(dir_path):
		print ('Destination directory: %s does not exist, creating one' % dir_path)
		os.mkdir(dir_path)
	else:
		print ('Found directory: %s ' % dir_path)


def truncate(in_source, dst_dir):
	print('in:%s out:%s' % (in_source, dst_dir))
	os.system("ffmpeg -i " + in_source + " -codec copy -f dash -min_seg_duration 30 -use_template 0 -use_timeline 0 -init_seg_name '$RepresentationID$-init.m4s' -media_seg_name '$RepresentationID$-$Number%05d$.m4s' " + dst_dir + "/output.mpd")


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


def main_truncate():
	# Assumes script is ran within the video roots direcoty
	for resolution in resolutions:
		quality = resolution.split('x')[1]
		#dst = res + '/out'
		in_source = ('%s%s/bbb_%s_%s.mp4' % (prefix, quality, quality, framerate) )				
		check_and_create('%s%s/out' % (prefix, quality) )
		out_dir = '%s%s/out' % (prefix, quality)
		truncate(in_source, out_dir)


#TODO: Should really make a use of a built-in xml library for this
def process_mpds():
	stiched_mpds = []	
	
	for i, resolution in enumerate(resolutions):
		width, height = resolution.split('x')
		#<Representation id="0" mimeType="video/mp4" codecs="avc1.64001f" bandwidth="1409668" width="640" height="360" frameRate="60/1">
		
		segment_dir = '%s%s/out' % (prefix, height)
		# Get the bandwidth, currently got no better way of doing this
		with open(segment_dir + '/output.mpd', 'r') as f:
			content = f.read()
			bandwidth = content.split('bandwidth="')[1].split('"')[0]
			seg_duration = content.split('duration="')[1].split('"')[0]

		representation_tag = '\t'*3 + '<Representation id="%s" mimeType="video/mp4" codecs="avc1.64001f" bandwidth="%s" width="%s" height="%s" frameRate="60/1">\n' % (i, bandwidth, width, height)		
		stiched_mpds.append(representation_tag)
		print (representation_tag) 

		files = sorted(os.listdir(segment_dir))
		
		# Duration should NOT be a hardcoded value!
		# Duration is the length of each individual segment duration (math.ciel(mediaPresentationDuration / # of segments))
		segmentlist_tag = '\t'*4 + ('<SegmentList timescale="1000000" duration="%s" startNumber="1">\n' % seg_duration)  
		stiched_mpds.append(segmentlist_tag)
		print (segmentlist_tag)

		initseg_tag = '\t'*5 + '<Initialization sourceURL="' + segment_dir + '/0-init.m4s" />\n'
		stiched_mpds.append(initseg_tag)		
		print (initseg_tag)

		
		for f in files:
			if f.startswith('0') and 'init' not in f: # if the file is video and is not the initial segment
				media_url = '%s/%s' % ( segment_dir, f)
				segment_tag = '\t'*5 + '<SegmentURL media="' + media_url + '" />\n'
				stiched_mpds.append(segment_tag)
				#print ()

#				print("%s/%s" % (segment_dir, f))
		# close segment_list tag
		stiched_mpds.append('\t'*4 + '</SegmentList>\n')
		# close representation tag
		stiched_mpds.append('\t'*3 + '</Representation>\n')
	
	# Make a string out of the stiched_mpds list
	stiched_mpds = ''.join(x for x in stiched_mpds)
	with open('%sbbb.mpd' % prefix, 'w') as f:
		# Put all tags prior to the <representation> tag in the mpd
		f.write('''<?xml version="1.0" encoding="utf-8"?>
<MPD xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns="urn:mpeg:dash:schema:mpd:2011"
	xmlns:xlink="http://www.w3.org/1999/xlink"
	xsi:schemaLocation="urn:mpeg:DASH:schema:MPD:2011 http://standards.iso.org/ittf/PubliclyAvailableStandards/MPEG-DASH_schema_files/DASH-MPD.xsd"
	profiles="urn:mpeg:dash:profile:isoff-live:2011"
	type="static"
	mediaPresentationDuration="PT10M34.5S"
	minBufferTime="PT10.0S">
	<ProgramInformation>
		<Title>Big Buck Bunny, Sunflower version</Title>
	</ProgramInformation>
	<Period id="0" start="PT0.0S">
		<AdaptationSet id="0" contentType="video" segmentAlignment="true" bitstreamSwitching="true" frameRate="60/1" lang="und">\n''') # opening tags

		f.write(stiched_mpds)

		f.write('\t'*2 + '''</AdaptationSet>
	</Period>
</MPD>''') # closing tags


if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Video utility script')
	parser.add_argument('--prefix', '-p', help='Prefix')

	parser.add_argument('--source', '-s', help='Mp4 video source')

	parser.add_argument('--fps', help="Frames per second to use for re-encoding")

	parser.add_argument('--action', required=True, help='Action to be performed by the script. Possible actions are: encode, truncate(segment), and mpd (to generate an MPD file)')

	args = parser.parse_args()
	
	if args.prefix:
		if not args.prefix.endswith('/'):
			prefix = args.prefix + '/'
		else:
			prefix = args.prefix
	else:
		print ("No prefix given")
	if args.source:
		source = args.source 
	if args.fps:
		framerate = args.fps

	print ('Running "%s" script with arguemnts: prefix(%s) source(%s) fps(%s)' % (args.action, prefix, source, framerate))
	if args.action == 'truncate':
		main_truncate()
	elif args.action == 'encode':
		main_encode()
	elif args.action == 'mpd':
		process_mpds()
	else:
		print("Unknown action requested. Specify one of: truncate, encode, or mpd")

