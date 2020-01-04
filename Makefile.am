AM_CFLAGS = $(WARNCFLAGS) $(DEBUGCFLAGS)
AM_CXXFLAGS = $(WARNCXXFLAGS) $(DEBUGCFLAGS)

AM_CPPFLAGS = \
			  -I$(top_srcdir)/../ngtcp2/lib/includes \
			  -I$(top_builddir)/../ngtcp2/lib/includes \
			  -I$(top_srcdir)/../ngtcp2/crypto/includes \
			  -I$(top_srcdir)/third-party \
			  @OPENSSL_LIBS@ \
			  @LIBEV_LIBS@ \
			  @DEFS@

LDADD = $(top_builddir)/../ngtcp2/crypto/openssl/libngtcp2_crypto_openssl.la \
		$(top_builddir)/../ngtcp2/lib/libngtcp2.la \
		$(top_builddir)/third-party/libhttp-parser.la \
		@OPENSSL_LIBS@ \
		@LIBEV_LIBS@ \
		@LIBNGHTTP3_LIBS@


bin_PROGRAMS = quicsy
quicsy_SOURCES = quicsy.h quicsy.cc \
				 keylog.h keylog.cc \
				 util.h util.cc \
				 network.h template.h \
				 shared.h shared.cc \
				 debug.h debug.cc \
				 http.h http.cc

#quicsy.h debug.h utils.h quicsy_tls.h quicsy_tls.cc

#client_SOURCES = quic_client.cc quic_client.h
