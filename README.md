langdetect
==========

This is a fairly simple program that can detect the language of a given text. It uses "stop words" to perform the detection. Stop words are generally used by search engines to filter out words that won't improve the search. These end up being common words that are usually specific to one language. In English, words like "the" are considered stop words. I have included stop word lists for many different languages. More can be added easily. Each stop word file must be in the UTF8 encoding. 

The main purpose of this project is to help me learn more about C and to learn about writing and maintaining cross platform applications.

This was originally a programming challnge from the Reddit board /r/dailyprogrammer. However, I've extended it
by creating a cross platform GUI application that can be built for Windows, Mac OS X, Linux and many other
operating systems from a single codebase. It can even run as a Universal Windows 10 application with a native XAML-based interface.

It uses CMake to generate the build files. This works well on any platform that CMake is supported on. For Unix-like
operating systems apart from OS X, GTK+ 3 and pkg-config are required. GTK+ 2 can be used as fallback if GTK+ 3 is not present. I may add Qt support in the future. 

Screenshots
---------------
![linux-screenshot](http://i.imgur.com/0c0y3py.png)
![win32-screenshot](http://i.imgur.com/FiHzkLu.png)
![uwp-screenshot](http://i.imgur.com/1sKwV41.png)
![haiku-screenshot](http://i.imgur.com/R6pKzh5.png)

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

Universal Windows Platform
--------------------------------

I just recently added support for UWP in Windows 10. It is still experimental. To build for UWP, use the following command:

    cmake -G "Visual Studio 14 2015" -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0

After running this command, you MUST run the included PowerShell script or the application will not compile. CMake does not generate the project file properly and I can't find a better way of fixing it. 

I recommend you open the generated Visual Studio solution and build and deploy the application from there. Debugging does not work from Visual Studio yet. I'm working on improving the user interface (mainly correcting the scaling).

I have documented how UWP support was implemented in the doc folder.  



