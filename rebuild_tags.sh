#!/bin/sh
TMP=`mktemp -t tags`
LST=`mktemp -t filelist`

find -E /System/Library/Frameworks/OpenGL.framework -regex "^.*\.h$" >> "$LST"
find -E /System/Library/Frameworks/System.framework -regex "^.*\.h$" >> "$LST"
find -E /usr/include >> "$LST"
find -E /opt/local/include >> "$LST"
find -E ./redist/include >> "$LST"
find -E ./src -regex "^.*\.(cpp|h)$" >> "$LST"

ctags --links=no -f "$TMP" "--extra=+f" -L "$LST"
mv "$TMP" ./tags
rm "$LST"

cscope -b -R

