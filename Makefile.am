AM_CFLAGS = $(WARNCFLAGS) $(DEBUGCFLAGS)
AM_CXXFLAGS = $(WARNCXXFLAGS) $(DEBUGCFLAGS)

AM_CPPFLAGS = \
			  -I$(top_srcdir)/../ngtcp2/lib/includes \
			  -I$(top_builddir)/../ngtcp2/lib/includes \
			  -I$(top_srcdir)/../ngtcp2/crypto/includes \
			  @OPENSSL_LIBS@ \
			  @LIBEV_LIBS@ \
			  @DEFS@

LDADD = $(top_builddir)/../ngtcp2/crypto/openssl/libngtcp2_crypto_openssl.la \
		$(top_builddir)/../ngtcp2/lib/libngtcp2.la \
		@OPENSSL_LIBS@ \
		@LIBEV_LIBS@ \
		@LIBNGHTTP3_LIBS@


bin_PROGRAMS = quicsy
quicsy_SOURCES = quicsy.cc
#quicsy.h debug.h utils.h quicsy_tls.h quicsy_tls.cc

#client_SOURCES = quic_client.cc quic_client.h
