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

#define DIR UAP_DIR
#define FILE UAP_FILE
#define dirent uap_dirent
#define opendir uap_opendir
#define readdir uap_readdir
#define chdir uap_chdir
#define closedir uap_closedir
#define fopen uap_fopen
#define fclose uap_fclose
#define fgets uap_fgets
#define DT_REG 0x8000

using namespace Windows::Storage;

typedef struct {
	int file_ptr;
	struct uap_dirent** files;
	StorageFolder^ folder;
	int numberOfFiles;
} UAP_DIR;

struct uap_dirent {
	int d_type;
	StorageFile^ d_name;
};

typedef struct {
	Windows::Storage::StorageFile^ uapfile;
	Windows::Foundation::Collections::IVector<Platform::String^>^ lines;
	int line_ptr;
} UAP_FILE;


UAP_DIR* uap_opendir(StorageFolder^ folder);
struct uap_dirent* uap_readdir(UAP_DIR* dir);
void uap_chdir(StorageFolder^ folder);
void uap_closedir(UAP_DIR* dir);

UAP_FILE* uap_fopen(StorageFile^ name, char* flags);
int uap_fgets(char* buffer, int bufsize, UAP_FILE* fp);
void uap_fclose(UAP_FILE* fp);

void uap_display_dialog(char* text);

