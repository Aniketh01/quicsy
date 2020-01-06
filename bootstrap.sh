#!/bin/sh -e
echo "autoreconf..."
autoreconf -i --force
echo "automake..."
automake --foreign --add-missing
echo "autoconf..."
autoconf
echo
echo "Now run: ./configure --prefix="
echo
