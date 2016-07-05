cmake_minimum_required (VERSION 2.6)
project (langdetect)

# Add precompiled header for MSVC
MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

# Windows 10 Universal App
if (${CMAKE_SYSTEM_NAME} MATCHES "WindowsStore")
	include_directories (./include/windows-uwp)
	add_definitions (-DUWP -D_CRT_SECURE_NO_WARNINGS)
	set (UWP 1)
	ADD_MSVC_PRECOMPILED_HEADER ("${CMAKE_CURRENT_SOURCE_DIR}/include/windows-uwp/pch.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/windows-uwp/pch.cpp" SRC_FILES)

	set (SRC_FILES src/windows-uwp/App.xaml.cpp src/langdetect.c src/gui.c src/windows-uwp/MainPage.xaml.cpp 
		src/windows-uwp/langdetect-uwp.cpp src/windows-uwp/str-utils.cpp
		src/windows-uwp/App.xaml
		src/windows-uwp/MainPage.xaml)

	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /ZW /EHsc")
	set_source_files_properties (src/langdetect.c PROPERTIES LANGUAGE CXX)
	set_source_files_properties (src/gui.c PROPERTIES LANGUAGE CXX)

	set_source_files_properties (src/windows-uwp/App.xaml PROPERTIES VS_XAML_TYPE ApplicationDefinition)
# Win32
elseif (WIN32)
	set (SRC_FILES src/main.c src/langdetect.c src/gui.c src/windows/windows_gui.c)

# OS X
elseif (APPLE)
	set (SRC_FILES src/main.c src/langdetect.c src/gui.c src/osx/osx_init.m
		src/osx/osx_gui.m)
	set (IB_FILES osx/MainMenu.nib)
	set (OSX_BUNDLE MACOSX_BUNDLE)

	set (CMAKE_C_FLAGS "-x objective-c")
	set (CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework AppKit -framework Foundation")

# Haiku
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
	add_definitions (-DHAIKU_OS)
	set (SRC_FILES src/main.c src/langdetect.c src/gui.c src/beos/beos_gui.cpp src/beos/beos_init.cpp)
	set_source_files_properties (src/main.c PROPERTIES LANGUAGE CXX)
	set_source_files_properties (src/gui.c PROPERTIES LANGUAGE CXX)
	set_source_files_properties (src/langdetect.c PROPERTIES LANGUAGE CXX)

# All other operating systems that support GTK
else()
	include (FindPkgConfig)
	pkg_check_modules (GTK3 QUIET gtk+-3.0)
	if (${GTK3_FOUND}) # use gtk 3 if we can
		pkg_search_module (GTK REQUIRED gtk+-3.0)
	else() # or fallback to gtk 2
		pkg_search_module (GTK REQUIRED gtk+-2.0)
	endif()
	add_definitions (-DGTK_UI)
	include_directories (${GTK_INCLUDE_DIRS})
	set (SRC_FILES src/main.c src/langdetect.c src/gui.c src/gtk/gtk_gui.c)
endif()

include_directories(./include)
add_executable (langdetect ${OSX_BUNDLE} ${SRC_FILES} ${IB_FILES})

if (UWP)
	set_target_properties (langdetect PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
	set_target_properties (langdetect PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
	set_target_properties (langdetect PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
	set_target_properties (langdetect PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")
	message ("NOTE: You must run 'fix_vs_proj.ps1' in PowerShell in order for the project to compile.")
elseif (APPLE)
	set_source_files_properties (${IB_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	set_target_properties (langdetect PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/osx/Info.plist)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Haiku")
	# This is kind of a hack to fix issues with adding Haiku's C++ headers to our existing C code
	# Compiling everything as C++ makes things easier. 
	target_link_libraries (langdetect be tracker)
else()
	target_link_libraries (langdetect ${GTK_LDFLAGS})
endif()
