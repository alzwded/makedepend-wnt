makedepend-wnt
==============

a hacked together console app that does what gcc -M does, only without the annoying path stripping

how to run
=========
makedepend /IincludeDir /OoutputPath.d input.cpp Path /IadditionalIncludeDir /Ooutput2.d input2.cpp
You can have as many /I's as you want, only the ones given before inputPath will be taken into account
Only the last /O will be taken into account for any one inputPath

The generated file will be a makefile, not unlike that created with gcc -M

What's next
==========
a better, non-annoying make for both windows and linux.