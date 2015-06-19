langdetect
==========

This is just a simple program that can detect whether the given text is in English, Spanish, German, or French.
It uses a dictionary of the top 1000 most used words in each language to show the user the probility of the text
being in each language. In my experience, this is very accurate. 

The main purpose of this, however, is to help me learn more about C and to learn about writing and maintaining cross platform applications.

This was originally a programming challnge from the Reddit board /r/dailyprogrammer. However, I've extended it
by creating a cross platform GUI application that can be built for Windows, Mac OS X, Linux and many other
operating systems from a single codebase.

It uses CMake to generate the build files. This works well on any platform that CMake is supported on. For Unix-like
operating systems apart from OS X, GTK+ 2 and pkg-config are required. I may add Qt support in the future. 

How to build
------------
from the root of the repository:

	mkdir build && cd build
	cmake ..
	cmake --build .

however, for Mac OS X you must do:

	mkdir build && cd build
	cmake -GXcode ..
	cmake --build .

otherwise, it will use a normal Unix Makefile which doesn't seem to work with the Objective-C code.
