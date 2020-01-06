#!/bin/sh -e
echo "Run configure..."
PKG_CONFIG_PATH=/home/devbox/devel/quic_research/build_openssl/lib/pkgconfig:/home/devbox/devel/quic_research/nghttp3_build/lib/pkgconfig:/home/devbox/devel/quic_research/ngtcp2/lib ./configure
echo "make..."
make -j$(nproc)
echo "make install..."
sudo make install
echo
echo "Compiled successfully!!!"
echo
