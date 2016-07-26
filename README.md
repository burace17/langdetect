langdetect
==========

This is a fairly simple program that can detect the language of a given text. It uses "stop words" to perform the detection. Stop words are generally used by search engines to filter out words that won't improve the search. These end up being common words that are usually specific to one language. In English, words like "the" are considered stop words. I have included stop word lists for many different languages. More can be added easily. Each stop word file must be in the UTF8 encoding. 

The main purpose of this project is to help me learn more about C and to learn about writing and maintaining cross platform applications.

This was originally a programming challnge from the Reddit board /r/dailyprogrammer. However, I've extended it
by creating a cross platform GUI application that can be built for Windows, Mac OS X, Linux and many other
operating systems from a single codebase. It can even run as a Universal Windows 10 application with a native XAML-based interface.

It uses CMake to generate the build files. This works well on any platform that CMake is supported on. For Unix-like
operating systems apart from OS X, GTK+ 3 and pkg-config are required. GTK+ 2 can be used as fallback if GTK+ 3 is not present. Qt 5 can also be used. It may run on Qt 4 if CMakeLists is modified slightly.  

You can find binary releases of langdetect [here](http://burace17.net/langdetect). These may not be up to date with the Git version. I mainly have this for older platforms that require fixes that I don't want to commit to Git. I may add instructions for these platforms eventually.

Screenshots
---------------
![linux-screenshot](http://i.imgur.com/syTbicB.png)
![win32-screenshot](http://i.imgur.com/FiHzkLu.png)
![uwp-screenshot](http://i.imgur.com/1sKwV41.png)
![osx-screenshot](http://i.imgur.com/GVSX4Jo.png)

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

if you would like to build using Qt, you can do:

	mkdir build && cd build
	cmake -DQT_UI=ON ..
	cmake --build .

using Qt on Windows is a bit more complicated. I ran the following commands in Git bash to make it work:

	mkdir build && cd build
	CMAKE_PREFIX_PATH=/C/Qt/5.7/msvc2015_64/ cmake -G"Visual Studio 14 2015 Win64" -DQT_UI=ON ..
	cmake --build .

You will have to replace the prefix path with the location of your Qt installation. If you install a 64 bit version of
Qt, you will have to build a 64 bit version of langdetect as I did.

Universal Windows Platform
--------------------------------

I just recently added support for UWP in Windows 10. It is still experimental. To build for UWP, use the following command:

    cmake -G "Visual Studio 14 2015" -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0

After running this command, you MUST run the included PowerShell script or the application will not compile. CMake does not generate the project file properly and I can't find a better way of fixing it. 

I recommend you open the generated Visual Studio solution and build and deploy the application from there. Debugging does not work from Visual Studio yet. I'm working on improving the user interface (mainly correcting the scaling).

I have documented how UWP support was implemented in the doc folder.  



