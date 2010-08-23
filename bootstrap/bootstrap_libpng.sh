#!/bin/bash
BASE="`pwd`/.."
WD="$BASE/bootstrap"
PREFIX="$BASE/redist"
##############################################################################

tar -xzf "libpng-1.4.3.tar.gz" && \
pushd libpng-1.4.3/ && \
./configure --prefix="$PREFIX" && \
make && \
make install && \
popd && \
rm -rf libpng-1.4.3/
