#!/bin/bash
BASE="`pwd`/.."
WD="$BASE/bootstrap"
PREFIX="$BASE/redist"
##############################################################################

LIBJPEG_TGZ=jpegsrc.v7.tar.gz

tar -xzf "$LIBJPEG_TGZ" && \
pushd jpeg-7/ && \
./configure --prefix="$PREFIX" && \
make && \
make install && \
popd && \
rm -rf jpeg-7/
