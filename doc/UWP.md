UWP
==========================

When I first started researching UWP, I discovered that Microsoft had required all applications to be sandboxed. This means that normal file IO functions do not work, and the new UWP-specific functions must be used. This concerned me because a large amount of the business logic for langdetect depends on C standard file IO functions. I did not want to modify the cross platform business logic very much, so I decided to make sort of an adapter between UWP and the business logic. 

When the UWP application tries to call a C standard function, it instead calls one of my adapter functions. I use the preprocessor to replace the original function name with the new adapter function. My new functions do not mimic their C standard counterparts exactly. They are designed to work only with this application. I have implemented adapters for several data structures and functions. FILE is replaced with UWP_FILE, which is designed to hold a vector of the lines in the requested file. It is used by the uwp_fgets() function. 

All of these functions and structures are defined in langdetect-uwp.h and implemented in langdetect-uwp.cpp. 

Unfortunately, I still had to make changes to the business logic. They were very minor compared to the changes I would have had to make if I had not chosen this design. I was able to keep the business logic code mostly clean of platform specific code. I will be trying to think of ways to remove that platform specific code, however. Until now, the business logic did not require platform specific code.

Overall, adding UWP support to an existing codebase definitely challenged me. It probably took 2 weeks for me to get everything working. I encountered many problems getting CMake to generate the correct build files. Learning about Microsoft's new C++ extensions was also interesting. 
