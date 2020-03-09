AM_CFLAGS = $(WARNCFLAGS) $(DEBUGCFLAGS)
AM_CXXFLAGS = $(WARNCXXFLAGS) $(DEBUGCFLAGS)

SUBDIRS = third-party include src

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# Very hacky way to make use of autotools
# Using Makefile as a script isn't what it is meant for. 
streamer:
	cc streaming/main_window.cpp streaming/decoder.cpp -lstdc++ -lavcodec -lavformat -lavutil -lswresample -lswscale -lglfw -lGL -o bin/player
tcp_streaming:
	cc streaming/tcp_streaming/main_window.cpp streaming/tcp_streaming/decoder.cpp streaming/tcp_streaming/tcp_client.c -lstdc++ -lavcodec -lavformat -lavutil -lswresample -lswscale -lglfw -lGL -o bin/tcp_player && cc streaming/tcp_streaming/tcp_server.c -o tcp_server
transcoder:
	cc streaming/transcoder.c streaming/debug.c -lstdc++ -lavcodec -lavformat -lavutil -lswresample -lswscale -lglfw -lGL -o bin/transcoder

#noinst_PROGRAMS = quicsy_client quicsy_server

#quicsy_client_SOURCES = quicsy_client.h quicsy_client.cc \
#				 keylog.h keylog.cc \
#				 util.h util.cc \
#				 network.h template.h \
#				 shared.h shared.cc \
#				 debug.h debug.cc \
#				 http.h http.cc

#quicsy_server_SOURCES = quicsy_server.h quicsy_server.cc \
#				 keylog.h keylog.cc \
#				 util.h util.cc \
#				 network.h template.h \
#				 shared.h shared.cc \
#				 debug.h debug.cc \
#				 http.h http.cc

#quicsy.h debug.h utils.h quicsy_tls.h quicsy_tls.cc

#client_SOURCES = quic_client.cc quic_client.h
