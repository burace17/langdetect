langdetect
==========

This is a fairly simple program that can detect the language of a given text. It uses "stop words" to perform the detection. Stop words are generally used by search engines to filter out words that won't improve the search. These end up being common words that are usually specific to one language. In English, words like "the" are considered stop words. I have included stop word lists for many different languages. More can be added easily. Each stop word file must be in the UTF8 encoding. 

The main purpose of this project is to help me learn more about C and to learn about writing and maintaining cross platform applications.

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
