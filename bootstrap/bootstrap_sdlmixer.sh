#!/bin/bash
BASE="`pwd`/.."
WD="$BASE/bootstrap"
PREFIX="$BASE/redist"
##############################################################################

SDL_MIXER_TGZ=SDL_mixer-1.2.11.tar.gz

tar -xzf "$SDL_MIXER_TGZ" && \
pushd SDL_mixer-1.2.11/ && \
./configure --prefix=$PREFIX && \
make && \
make install && \
popd && \
rm -rf SDL_mixer-1.2.11/

