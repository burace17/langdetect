#ifndef INCLUDE_LANGDETECT
#define INCLUDE_LANGDETECT
#include "gui.h"
#include <stdio.h>
#include <string.h>
#ifndef UAP
#include <dirent.h>
#include <unistd.h>
#endif

#include <stdlib.h>
#include <errno.h>

#ifdef UAP
#include "langdetect-uap.h"
using namespace Windows::Storage;
#endif

// size of hash table
#define HT_SIZE 1000

// initial size of the language occurance array
#define LANG_INITIAL_SIZE 50


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

#ifdef UAP
int initialize(StorageFolder^ stop_files_dir);
#else
int initialize(char* stop_files_dir);
#endif

void detect_language(char* text);
void cleanup();
#endif
