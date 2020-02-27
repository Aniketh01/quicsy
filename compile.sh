#!/bin/sh -e
rootdir="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

echo "Run configure..."
PKG_CONFIG_PATH=$rootdir/deps/build_openssl/lib/pkgconfig:$rootdir/deps/nghttp3_build/lib/pkgconfig:$rootdir/deps/ngtcp2/lib ./configure
echo "make..."
make -j$(nproc)
echo "make install..."
sudo make install
echo
echo "Compiled successfully!!!"
echo