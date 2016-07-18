#ifndef INCLUDE_LANGDETECT
#define INCLUDE_LANGDETECT
#include "gui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifndef UWP
// These obviously won't work on UWP
#include <dirent.h>
#include <unistd.h>
#endif

// size of hash table
#define HT_SIZE 1000

// initial size of the language occurance array
#define LANG_INITIAL_SIZE 50

// Below we either define StorageFolder^ or char* to be STOP_FILES_DIR
// This is so we can make our business logic work more seamlessly across 
// platforms. Everything but UWP will simply be char* 
#ifdef UWP
#include "langdetect-uwp.h"
using namespace Windows::Storage;
typedef StorageFolder^ STOP_FILES_DIR;
#else
typedef char* STOP_FILES_DIR;
typedef char* STOP_FILE;
#endif

// used to keep track of the number of words encountered for each language
typedef struct {
	char lang_name[64];
	unsigned int occurances;
} LANG_T;


// linked list structure to store a list of languages in key value pair 
typedef struct lang_list {
	char name[64];
	struct lang_list* next;
} LANG_LIST_T;

// this structure associates a word (the key) with a (linked) list
// of the languages it appears in
typedef struct {
	char key[64];
	LANG_LIST_T* value;
} KV_PAIR_T;

// hash table to store everything 
typedef struct list_cell {
	KV_PAIR_T* pair;
	struct list_cell* next;
} LIST_CELL_T;

int initialize(STOP_FILES_DIR stop_files_dir);

void detect_language(char* text);
void cleanup();
#endif
