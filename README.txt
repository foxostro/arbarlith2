Arbarlith 2 
============

Homepage: http://www.gamecreation.org/~arbarlith/

You may find that Arbarlith 2 compiles with warnings in CodeBlocks.
This is due to a bug in versions of GCC before 4.0.0 as demonstrated
here: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=22207

To compile Arbarlith 2 from source, you will need the development libraries
and headers for DevIL (and DevILU and DevILUT), glew, SDL, and boost.  The
CodeBlocks project assumes these files can be found in the compiler and linker
search paths, so make sure to update them accordingly.

DevIL needed to be rebuilt from source to work properly with MinGW. So for
your convenience, I will eventually post a tarball of the libraries I was
linking against.
