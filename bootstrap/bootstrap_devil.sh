#!/bin/bash
BASE="`pwd`/.."
WD="$BASE/bootstrap"
PREFIX="$BASE/redist"
##############################################################################

tar -xzf "DevIL-1.7.8.tar.gz" && \
pushd devil-1.7.8/ && \
patch src-ILU/ilur/ilur.c ../devil_ilur.patch && \
export CPPFLAGS="-I$PREFIX/include" && \
export LDFLAGS="-L$PREFIX/lib"  && \
configure --disable-release --prefix="$PREFIX" --enable-ILU --enable-ILUT && \
make && \
make install && \
popd && \
rm -rf devil-1.7.8/
