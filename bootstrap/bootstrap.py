#!/usr/bin/env python
# vim: set expandtab sw=4 ts=4 :
# 
# This script fetches and builds libraries on which this project depends.
#

import os
import sys
import hashlib
import urllib
import subprocess



file_urls = {
"DevIL-1.7.8.tar.gz"      : "http://softlayer.dl.sourceforge.net/project/openil/DevIL/1.7.8/DevIL-1.7.8.tar.gz",
"SDL-1.2.14.tar.gz"       : "http://www.libsdl.org/release/SDL-1.2.14.tar.gz",
"SDL_mixer-1.2.11.tar.gz" : "http://www.libsdl.org/projects/SDL_mixer/release/SDL_mixer-1.2.11.tar.gz",
"boost_1_39_0.tar.bz2"    : "http://softlayer.dl.sourceforge.net/project/boost/boost/1.39.0/boost_1_39_0.tar.bz2",
"glew-1.5.0-src.tgz"      : "http://downloads.sourceforge.net/project/glew/glew/1.5.0/glew-1.5.0-src.tgz?download&amp;failedmirror=softlayer.dl.sourceforge.net",
"jpegsrc.v7.tar.gz"       : "http://www.ijg.org/files/jpegsrc.v7.tar.gz",
"libpng-1.4.3.tar.gz"     : "ftp://ftp.simplesystems.org/pub/libpng/png/src/libpng-1.4.3.tar.gz"
}

file_hashes = {
"DevIL-1.7.8.tar.gz"      : "7918f215524589435e5ec2e8736d5e1d",
"SDL-1.2.14.tar.gz"       : "e52086d1b508fa0b76c52ee30b55bec4",
"SDL_mixer-1.2.11.tar.gz" : "65ada3d997fe85109191a5fb083f248c",
"boost_1_39_0.tar.bz2"    : "a17281fd88c48e0d866e1a12deecbcc0",
"glew-1.5.0-src.tgz"      : "3fececda0151b060c08ffd8a12892741",
"jpegsrc.v7.tar.gz"       : "382ef33b339c299b56baf1296cda9785",
"libpng-1.4.3.tar.gz"     : "df3521f61a1b8b69489d297c0ca8c1f8"
}

scripts = [
("SDL"       , "/bin/sh ./bootstrap_sdl.sh"),
("SDL_mixer" , "/bin/sh ./bootstrap_sdlmixer.sh"),
("glew"      , "/bin/sh ./bootstrap_glew.sh"),
("jpegsrc"   , "/bin/sh ./bootstrap_jpeg.sh"),
("libpng"    , "/bin/sh ./bootstrap_libpng.sh"),
("DevIL"     , "/bin/sh ./bootstrap_devil.sh"),
("boost"     , "/bin/sh ./bootstrap_boost.sh")
]



def check_md5(filename, expected_md5):
    m = hashlib.md5()
    f = open(filename)
    m.update(f.read())
    f.close()
    actual_md5 = m.hexdigest()
    if expected_md5 == actual_md5:
        return True
    else:
        print >>sys.stderr, "Expected\t" + expected_md5
        print >>sys.stderr, "Actual  \t" + actual_md5



def verify(filename):
    return check_md5(filename, file_hashes[filename])



def download(url, filename):
    def reporthook(blocks_read, block_size, total_size):
        if not blocks_read:
            print ("(0%%)\tDownloading '%s'\r" % filename),
            return

        if total_size < 0:
            # Unknown size
            print ("(??%%)\tDownloading '%s': %d blocks\r" % (blocks_read, filename)),
        else:
            amount_read = blocks_read * block_size
            print ("(%.1f%%)\tDownloading '%s'\r" % (100 * float(amount_read) / float(total_size), filename)),
    
    (filename, headers) = urllib.urlretrieve(url, filename, reporthook)
    print "Download complete:", filename



def fetch_all():
    for filename, url in file_urls.iteritems():
        if not os.path.exists(filename):
            download(url, filename)
        else:
            print "Skipping", ("\""+filename+"\""), "as it is already here."

    for filename, url in file_urls.iteritems():
        if not verify(filename):
            print >>sys.stderr, "File fails checksum verification:", filename
            sys.exit(-1)



def clean():
    for filename in file_hashes.keys():
        try:
            os.remove(filename)
        except OSError, e:
            pass



def build_all():
    for (what, script) in scripts:
        try:
            os.remove(what + ".log")
        except OSError, e:
            pass

    for (what, script) in scripts:
        print "Building", what, "...",
        sys.stdout.flush()
        f = open(what + ".log", "w")
        try:
            subprocess.check_call(script, shell=True, stdout=f, stderr=f)
        except subprocess.CalledProcessError, e:
            print "failed.\nRefer to", ("\""+what+".log\"")
            sys.exit(-1)
        f.close()
        print "done."



def bootstrap_all():
    fetch_all()
    build_all()



def get_verbs():
    return { "help"  : usage,
             "clean" : clean,
             "fetch" : fetch_all,
             "build" : build_all,
             "all"   : bootstrap_all }



def usage():
    print "%s <VERB>" % sys.argv[0]
    print "Available verbs:"
    for verb in get_verbs().keys():
        print "\t%s" % verb



def main():
    if len(sys.argv) < 2:
        print >>sys.stderr, "Must specify a verb on the command-line."
        usage()
        sys.exit(-1)

    verbs = get_verbs()
    verb = sys.argv[1]
    verbs[verb]()



if __name__ == "__main__":
    main()

