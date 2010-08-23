#!/bin/bash
BASE="`pwd`/.."
WD="$BASE/bootstrap"
PREFIX="$BASE/redist"
##############################################################################

echo "BASE=$BASE"
echo "WD=$WD"
echo "PREFIX=$PREFIX"

tar -xzf "SDL-1.2.14.tar.gz" && \
pushd SDL-1.2.14/ && \
./configure --prefix="$PREFIX" && \
make && \
make install && \
popd && \
rm -rf SDL-1.2.14/
