#!/bin/sh -e
rootdir="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

echo "Building dependencies for quicsy ..."

echo "Building patched version of openssl ..."
cd $rootdir/deps/openssl
./config enable-tls1_3 --prefix=$rootdir/deps/openssl_build
make -j$(nproc)
make install_sw

echo "Building HTTP/3 with nghttp3 ..."
cd $rootdir/deps/nghttp3
autoreconf -i
./configure --prefix=$rootdir/deps/nghttp3_build --enable-lib-only
make -j$(nproc) 
make install

echo "Building QUIC with ngtcp2 ..."
cd $rootdir/deps/ngtcp2
autoreconf -i
./configure PKG_CONFIG_PATH=$rootdir/deps/openssl_build/lib/pkgconfig:$rootdir/deps/nghttp3_build/lib/pkgconfig LDFLAGS="-Wl,-rpath,$rootdir/deps/openssl_build/lib" --prefix=$rootdir/deps/ngtcp2_build
make -j$(nproc) check

cd $rootdir/
echo "building dependencies complete!"