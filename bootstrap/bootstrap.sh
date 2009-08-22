#!/bin/bash

BASE="`pwd`/../"
WD="$BASE/bootstrap"
PREFIX="$BASE/prereqs/"

##############################################################################

function lazy_fetch
{
	PKG_URL="$1"
	PKG_TGZ="$2"
	PKG_MD5="$3"
	
	# Have we already downloaded it already?
	md5sum --check "$PKG_MD5" --status
	
	if [ $? -ne 0 ] ; then
		echo "Downloading $PKG_URL..."
		curl -O $PKG_URL
		if [ $? -ne 0 ] ; then
			echo "Failed to download $PKG_URL"
			exit -1
		fi
	else
		echo "Already downloaded $PKG_TGZ and the MD5 hash looks OK."
	fi
}

function get_scons
{
	echo "Getting SCons 1.2.0"
	SCONS_URL=http://softlayer.dl.sourceforge.net/project/scons/scons/1.2.0/scons-1.2.0.tar.gz
	SCONS_TGZ=scons-1.2.0.tar.gz
	SCONS_MD5=md5s/scons-1.2.0.tar.gz.md5
	lazy_fetch $SCONS_URL $SCONS_TGZ $SCONS_MD5
	tar -xzvf $SCONS_TGZ
	pushd scons-1.2.0
	python setup.py install --prefix=$PREFIX
	popd # scons-1.2.0
}
	
function get_boost
{
	echo "Getting Boost 1.39.0"
	BOOST_URL=http://softlayer.dl.sourceforge.net/project/boost/boost/1.39.0/boost_1_39_0.tar.bz2
	BOOST_PKG=boost_1_39_0.tar.bz2
	BOOST_MD5=md5s/boost_1_39_0.tar.bz2.md5
	lazy_fetch $BOOST_URL $BOOST_PKG $BOOST_MD5
	tar -xjvf $BOOST_PKG
	
	# We only need the Boost  headers for Arbarlith 2
	rm -f $PREFIX/include/boost
	mkdir -p $PREFIX/include
	ln -s `pwd`/boost_1_39_0/boost $PREFIX/include/boost
}

function get_sdl
{
	echo "Getting SDL 1.2.13"
	SDL_URL=http://www.libsdl.org/release/SDL-1.2.13.tar.gz
	SDL_TGZ=SDL-1.2.13.tar.gz
	SDL_MD5=md5s/SDL-1.2.13.tar.gz.md5
	lazy_fetch $SDL_URL $SDL_TGZ $SDL_MD5
	tar -xzvf $SDL_TGZ
	pushd SDL-1.2.13/
	./configure --prefix=$PREFIX && make && make install
	popd # SDL-1.2.13
}

function get_sdl_mixer
{
	echo "Getting SDL_mixer 1.2.8"
	SDL_MIXER_URL=http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-1.2.8.tar.gz
	SDL_MIXER_TGZ=SDL_mixer-1.2.8.tar.gz
	SDL_MIXER_MD5=md5s/SDL_mixer-1.2.8.tar.gz.md5
	lazy_fetch $SDL_MIXER_URL $SDL_MIXER_TGZ $SDL_MIXER_MD5
	tar -xzvf $SDL_MIXER_TGZ
	pushd SDL_mixer-1.2.8/
	./configure --prefix=$PREFIX && make && make install
	popd # SDL_mixer-1.2.8
}

function get_glee
{
	echo "Getting GLEE 5.4.0"
	GLEE_URL=http://elf-stone.com/downloads/GLee/GLee-5.4.0-src.tar.gz
	GLEE_TGZ=GLee-5.4.0-src.tar.gz
	GLEE_MD5=md5s/GLee-5.4.0-src.tar.gz.md5
	lazy_fetch $GLEE_URL $GLEE_TGZ $GLEE_MD5
	mkdir GLee-5.4.0-src
	pushd GLee-5.4.0-src
		tar -xzvf ../$GLEE_TGZ
		./configure --prefix=$PREFIX
		make
		
		mkdir -p $PREFIX/include/GL/
		
		# We will get an error:
		# "/sbin/ldconfig.real: Can't create temporary cache file /etc/ld.so.cache~: Permission denied"
		# This is OK.
		make install
	popd # GLee-5.4.0-src
}

function get_devil
{
# DevIL can build without them, but you want to install libpng and libjpeg too

#TODO: Fetch and build zlib, libpng and libjpeg too
#http://www.zlib.net/zlib-1.2.3.tar.gz
#http://www.ijg.org/files/jpegsrc.v7.tar.gz
#http://softlayer.dl.sourceforge.net/project/libpng/00-libpng-stable/1.2.39/libpng-1.2.39.tar.gz
	
	echo "Getting DevIL 1.7.8"
	DEVIL_URL=http://softlayer.dl.sourceforge.net/project/openil/DevIL/1.7.8/DevIL-1.7.8.tar.gz
	DEVIL_TGZ=DevIL-1.7.8.tar.gz
	DEVIL_MD5=md5s/DevIL-1.7.8.tar.gz.md5
	lazy_fetch $DEVIL_URL $DEVIL_TGZ $DEVIL_MD5
	tar -xzvf $DEVIL_TGZ
	pushd devil-1.7.8/
		autoreconf -i
		./configure --prefix=$PREFIX --enable-ILU --enable-ILUT
		make
		make install
	popd # devil-1.7.8/
}

##############################################################################

# We'll put prereq libraries in here
mkdir -p $PREFIX

# We'll do all all work here so as to not clutter the project.
cd $WD

get_scons
get_boost
get_sdl
get_sdl_mixer
get_glee
get_devil

echo "Done."
