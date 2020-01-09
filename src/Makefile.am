AM_CPPFLAGS = \
			  -I$(top_srcdir)/include \
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

bin_PROGRAMS = $(top_builddir)/bin/quicsy_client $(top_builddir)/bin/quicsy_server

__top_builddir__bin_quicsy_client_SOURCES = quicsy_client.cc keylog.cc \
                 util.cc shared.cc debug.cc http.cc

__top_builddir__bin_quicsy_server_SOURCES = quicsy_server.cc keylog.cc \
                  util.cc shared.cc debug.cc http.cc
