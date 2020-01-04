#!/bin/sh -e
echo "libtoolize..."
libtoolize --copy --ltdl --force
echo "aclocal..."
aclocal -I aclocal
echo "autoconf..."
autoconf
echo "autoheader..."
autoheader
echo "automake..."
automake --add-missing --copy
echo
echo "Now run: ./configure --prefix="
echo
