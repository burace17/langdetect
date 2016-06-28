/*
	The Universal Windows Platform requires one to use specific WinRT functions to handle file IO. 
	This program originally used C standard library functions which do not work with UWP.

	I decided to override the original C functions with my own which use the UWP API so that I don't have to make
	major changes to the cross platform language detection code. These functions and structures are only designed to work with this project and
	do not function exactly like their C counterparts. 

*/

#pragma once
#include "langdetect.h"
#include <ppltasks.h>
#include "pch.h"
#include "str-utils.h"

#define DIR UWP_DIR
#define FILE UWP_FILE
#define dirent uwp_dirent
#define opendir uwp_opendir
#define readdir uwp_readdir
#define chdir uwp_chdir
#define closedir uwp_closedir
#define fopen uwp_fopen
#define fclose uwp_fclose
#define fgets uwp_fgets
#define DT_REG 0x8000

using namespace Windows::Storage;

typedef struct {
	int file_ptr;
	struct uwp_dirent** files;
	StorageFolder^ folder;
	int numberOfFiles;
} UWP_DIR;

struct uwp_dirent {
	int d_type;
	StorageFile^ d_name;
};

typedef struct {
	Windows::Storage::StorageFile^ uwpfile;
	Windows::Foundation::Collections::IVector<Platform::String^>^ lines;
	int line_ptr;
} UWP_FILE;


UWP_DIR* uwp_opendir(StorageFolder^ folder);
struct uwp_dirent* uwp_readdir(UWP_DIR* dir);
void uwp_chdir(StorageFolder^ folder);
void uwp_closedir(UWP_DIR* dir);

UWP_FILE* uwp_fopen(StorageFile^ name, char* flags);
int uwp_fgets(char* buffer, int bufsize, UWP_FILE* fp);
void uwp_fclose(UWP_FILE* fp);

void uwp_display_dialog(char* text);

