#!/bin/bash
BASE="`pwd`/.."
WD="$BASE/bootstrap"
PREFIX="$BASE/redist"
##############################################################################

# Building GLEW on Linux requires libx11-dev, libxi-dev, libxext-dev, libxmu-dev

GLEW_TGZ=glew-1.5.0-src.tgz

tar -xzf "$GLEW_TGZ" && \
mkdir -p "$PREFIX/include/GL" && \
pushd glew/ && \
export GLEW_DEST="$PREFIX" && \
make && \
make install && \
popd && \
rm -rf glew/
