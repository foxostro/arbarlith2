#!/bin/bash

BASE="`pwd`/.."
WD="$BASE/bootstrap"
PREFIX="$BASE/redist"

##############################################################################

function check_md5
{
	PKG_FILE="$1"
	MD5_FILE="$2"

	if [ -f "$MD5_FILE" ] ; then
		echo "MD5 file seems to be present"
	else
		echo "MD5 file seems to be missing: $MD5_FILE"
		exit -1
	fi

	if [ -f "$PKG_FILE" ] ; then
		echo "Going to check '$PKG_FILE'"
	else
		echo "File seems to be missing: $PKG_FILE"
		return 1
	fi

	if [ `uname` == "Darwin" ] ; then
		CALCULATED=`md5 -q "$PKG_FILE"`
		EXPECTED=`sed -E 's/^([0-9a-f]+) .*$/\1/' "$MD5_FILE"`
		
		if [ "$CALCULATED" == "$EXPECTED" ] ; then
			return 0
		else
			return 1
		fi
	else
		md5sum --check "$MD5_FILE" --status
		return $?
	fi
}

##############################################################################

function lazy_fetch
{
	URL="$1"
	PKG_FILE="$2"
	MD5_FILE="$3"
	
	# Have we already downloaded it already?
	check_md5 "$PKG_FILE" "$MD5_FILE"

	if [ $? -ne 0 ] ; then
		echo "Downloading $URL..."
		curl -O "$URL"
		if [ $? -ne 0 ] ; then
			echo "Failed to download $URL"
			exit -1
		fi

		# Check the hash on the finished download
		check_md5 "$PKG_FILE" "$MD5_FILE"
		if [ $? -ne 0 ] ; then
			echo "Downloaded '$PKG_FILE', but the MD5 hash looks WRONG."
			exit -1
		fi
	else
		echo "Already downloaded '$PKG_FILE' and the MD5 hash looks OK."
	fi
}
	
##############################################################################

function get_boost
{
	BOOST_URL=http://softlayer.dl.sourceforge.net/project/boost/boost/1.39.0/boost_1_39_0.tar.bz2
	BOOST_PKG=boost_1_39_0.tar.bz2
	BOOST_MD5=md5s/boost_1_39_0.tar.bz2.md5
	
	echo "Getting Boost 1.39.0"
	lazy_fetch $BOOST_URL $BOOST_PKG $BOOST_MD5
	tar -xjvf $BOOST_PKG
	
	# We only need the Boost  headers for Arbarlith 2
	rm -f $PREFIX/include/boost
	mkdir -p $PREFIX/include
	ln -s `pwd`/boost_1_39_0/boost $PREFIX/include/boost
}

##############################################################################

function get_sdl
{
	SDL_URL=http://www.libsdl.org/release/SDL-1.2.14.tar.gz
	SDL_TGZ=SDL-1.2.14.tar.gz
	SDL_MD5=md5s/SDL-1.2.14.tar.gz.md5
	
	echo "Getting SDL 1.2.14"
	lazy_fetch $SDL_URL $SDL_TGZ $SDL_MD5
	tar -xzvf $SDL_TGZ
	pushd SDL-1.2.14/
	./configure --prefix=$PREFIX && make && make install
	popd # SDL-1.2.14
}

##############################################################################

function get_sdl_mixer
{
	SDL_MIXER_URL=http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-1.2.11.tar.gz
	SDL_MIXER_TGZ=SDL_mixer-1.2.11.tar.gz
	SDL_MIXER_MD5=md5s/SDL_mixer-1.2.11.tar.gz.md5
	
	echo "Getting SDL_mixer 1.2.11"
	lazy_fetch $SDL_MIXER_URL $SDL_MIXER_TGZ $SDL_MIXER_MD5
	tar -xzvf $SDL_MIXER_TGZ
	pushd SDL_mixer-1.2.11/
	./configure --prefix=$PREFIX && make && make install
	popd # SDL_mixer-1.2.11
}

##############################################################################

function get_glew
{
# Building GLEW on Linux requires libx11-dev, libxi-dev, libxext-dev, libxmu-dev

	echo "Getting GLEW 1.5.0"
	GLEW_URL="http://downloads.sourceforge.net/project/glew/glew/1.5.0/glew-1.5.0-src.tgz?download&amp;failedmirror=softlayer.dl.sourceforge.net"
	GLEW_TGZ=glew-1.5.0-src.tgz
	GLEW_MD5=md5s/glew-1.5.0-src.tgz.md5
	lazy_fetch "$GLEW_URL" "$GLEW_TGZ" "$GLEW_MD5"
	tar -xzvf "$GLEW_TGZ"
	mkdir -p "$PREFIX/include/GL"
	pushd glew/
		export GLEW_DEST="$PREFIX"
		make
		make install
	popd # glew/
}

##############################################################################

function get_libjpeg
{
	LIBJPEG_URL=http://www.ijg.org/files/jpegsrc.v7.tar.gz
	LIBJPEG_TGZ=jpegsrc.v7.tar.gz
	LIBJPEG_MD5=md5s/jpegsrc.v7.tar.gz.md5
	
	echo "Getting libjpeg 7"
	lazy_fetch "$LIBJPEG_URL" "$LIBJPEG_TGZ" "$LIBJPEG_MD5"
	tar -xzvf "$LIBJPEG_TGZ"
	pushd jpeg-7/
		./configure --prefix="$PREFIX" \
		  && make \
		  && make install
	popd # jpeg-7/
}

##############################################################################

function get_libpng
{
	LIBPNG_URL=http://softlayer.dl.sourceforge.net/project/libpng/00-libpng-stable/1.2.39/libpng-1.2.39.tar.gz
	LIBPNG_TGZ=libpng-1.2.39.tar.gz
	LIBPNG_MD5=md5s/libpng-1.2.39.tar.gz.md5
	
	echo "Getting libpng 1.2.39"
	lazy_fetch "$LIBPNG_URL" "$LIBPNG_TGZ" "$LIBPNG_MD5"
	tar -xzvf "$LIBPNG_TGZ"
	pushd libpng-1.2.39/
		./configure --prefix="$PREFIX" \
		  && make \
		  && make install
	popd # libpng-1.2.39/
}

##############################################################################

function get_devil_178
{
	DEVIL_URL=http://softlayer.dl.sourceforge.net/project/openil/DevIL/1.7.8/DevIL-1.7.8.tar.gz
	DEVIL_TGZ=DevIL-1.7.8.tar.gz
	DEVIL_MD5=md5s/DevIL-1.7.8.tar.gz.md5
	
	echo "Getting DevIL 1.7.8"
	lazy_fetch "$DEVIL_URL" "$DEVIL_TGZ" "$DEVIL_MD5"
	
	# XXX: On Mac, this may require a small patch after extracting...

	tar -xzvf "$DEVIL_TGZ"
	pushd devil-1.7.8/
		export CPPFLAGS="-I$PREFIX/include"
		export LDFLAGS="-L$PREFIX/lib" 
		configure --disable-release --prefix="$PREFIX" --enable-ILU --enable-ILUT
		make
		make install
	popd # devil-1.7.8/
}

##############################################################################
##############################################################################
##############################################################################

# We'll put prereq libraries in here
mkdir -p $PREFIX

# We'll do all all work here so as to not clutter the project.
cd $WD

get_glew
get_boost
get_sdl
get_sdl_mixer
get_libjpeg
get_libpng
get_devil_178

echo "Done. Prereqs installed in '$PREFIX'"

