#!/bin/bash
BASE="`pwd`/.."
WD="$BASE/bootstrap"
PREFIX="$BASE/redist"
##############################################################################

tar -xjf boost_1_39_0.tar.bz2 && \
rm -f $PREFIX/include/boost && \
mkdir -p $PREFIX/include && \
ln -s `pwd`/boost_1_39_0/boost $PREFIX/include/boost

