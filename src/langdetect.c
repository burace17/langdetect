#ifdef UWP
#include "pch.h"
using namespace Windows::Storage;
#endif
#include "langdetect.h"

// this array stores the number of words found for each language we process.
static LANG_T* lang_occurances;
static size_t lang_occurances_size = LANG_INITIAL_SIZE;
static size_t numberOfLanguages;
static int initialized = 0;
static LIST_CELL_T** word_dictionary;

// slightly modified string hashing function (sdbm) from: 
// http://www.cse.yorku.ca/~oz/hash.html
unsigned long hash(char *str) {
	unsigned long hash = 0;
	int c;
	while ((c = *str++))
		hash = c + (hash << 6) + (hash << 16) - hash;
	return hash;
}

// gets the index in the hash table of the given key
static inline unsigned int hash_get_index(char* key) {
	return hash(key) % HT_SIZE;
}

// returns the list cell that contains the target key
LIST_CELL_T* hash_get_cell(char* key, LIST_CELL_T* list) {
	LIST_CELL_T* p;
	for (p = list; p != NULL; p = p->next) {
		if (strcmp(p->pair->key, key) == 0) {
			return p;
		}
	}
	return NULL;
}

// puts the given key value pair in the given hash table
void hash_put_kv(LIST_CELL_T** hash_table, KV_PAIR_T* kv) {
	// find the index where this KV pair should go
	unsigned int index = hash_get_index(kv->key);
	// then find the cell
	LIST_CELL_T* cell = hash_get_cell(kv->key, hash_table[index]);
	if (cell == NULL) {
		// if there's nothing in this cell, allocate some memory for it
		LIST_CELL_T* cell = (LIST_CELL_T*)calloc(1, sizeof(LIST_CELL_T));
		cell->pair = kv;
		cell->next = hash_table[index];
		hash_table[index] = cell;
	}
}

// frees all of the objects associated with a given hash table
void hash_free(LIST_CELL_T** hash_table) {
	size_t i;
	for (i = 0; i < HT_SIZE; i++) {
		LIST_CELL_T* ph = hash_table[i];
		LIST_CELL_T* p = hash_table[i];
		while (p != NULL) {
			LANG_LIST_T* qh = p->pair->value;
			LANG_LIST_T* q = p->pair->value;
			while (q != NULL) { // also deallocate the language linked list
				q = q->next;
				free(qh);
				qh = q;
			}

			// deallocate key value pair
			free(p->pair);

			// finally, deallocate this linked list cell
			p = p->next;
			free(ph);
			ph = p;
		}

	}
	free(hash_table);
}

// finds the given word in the given hash table
// returns the key value pair with the key equal to the given word.
KV_PAIR_T* find_word(LIST_CELL_T** hash_table, char* word) {
	// get the index in the hash table where this word should be located
	unsigned int index = hash_get_index(word);
	// get the list cell that contains the target word and return it if found
	LIST_CELL_T* cell = hash_get_cell(word, hash_table[index]);

	if (cell != NULL) {
		return cell->pair;
	}
	return NULL;
}

// adds all of the stop words in the given file to the hash table
// sets errno if a problem occurs
void process_language(LIST_CELL_T** hash_table, STOP_FILE file_name, char* language_name) {
	FILE* fp;
	char line[BUFSIZ];
	LANG_T lang;
	KV_PAIR_T* kv;
	LANG_LIST_T* cell;
	LANG_T* tmp;

	if ((fp = fopen(file_name, "r")) != NULL) {	
		// add this language to the language occurance array
		strncpy(lang.lang_name, language_name, sizeof(lang.lang_name));
		lang.occurances = 0;

		if (numberOfLanguages == lang_occurances_size) {
			// if the array is full, double its size
			tmp = (LANG_T*)realloc(lang_occurances, lang_occurances_size * 2 * sizeof(LANG_T));
			if (tmp != NULL) {
				lang_occurances = tmp;
				lang_occurances_size *= 2;
			}
			else {
				fclose(fp);
				errno = ENOMEM;
				return;
			}
		}
		// add this language to the occurance array
		lang_occurances[numberOfLanguages++] = lang;

		// read each line of the file
		while (fgets(line, sizeof(line), fp)) {
			// strip newline char from input line
			line[strcspn(line, "\n")] = '\0';

			// locate this word in the hash table (it may or may not be there already)
			kv = find_word(hash_table, line);

			// allocate some memory to store this language in the language list	for this word
			cell = (LANG_LIST_T*)calloc(1, sizeof(LANG_LIST_T));
			strncpy(cell->name, language_name, sizeof(cell->name));

			if (kv != NULL) {
				// word is in hash table
				// add this language to the front of the linked list
				cell->next = kv->value;
				kv->value = cell;
			}
			else {
				// this word isn't in the hash table, so we need to allocate some memory for its
				// KV pair. 
				kv = (KV_PAIR_T*)calloc(1, sizeof(KV_PAIR_T));
				// copy the language name to the key 
				strncpy(kv->key, line, sizeof(kv->key));
				// value is the linked list of languages
				kv->value = cell;
				hash_put_kv(hash_table, kv);
			}

		}
	}
	else {
		errno = ENOENT;
		return;
	}
	fclose(fp);
	errno = 0;
}

// this function is used by the analyze() function to 
// walk through the language linked list associated with each
// encountered word. this handles cases where a word is associated
// with multiple languages
void update_occurances(LANG_LIST_T* list) {
	LANG_LIST_T* p;
	size_t i;
	for (p = list; p != NULL; p = p->next) {
		for (i = 0; i < numberOfLanguages; i++) {
			if (strcmp(p->name, lang_occurances[i].lang_name) == 0) {
				lang_occurances[i].occurances++;
				break;
			}
		}
	}
}

// this function takes in the word->language hash table and
// a file to analyze. it updates the lang_occurances array
// with the number of time each encountered word appears in the
// languages that we can track. 
void analyze(LIST_CELL_T** hash_table, char* text) {
	char* word;
	KV_PAIR_T* kv;
	word = strtok(text, " ");
	while (word != NULL) {
		kv = find_word(hash_table, word);
		if (kv != NULL)
			update_occurances(kv->value);
		word = strtok(NULL, " ");
	}
}

void cleanup() {
	numberOfLanguages = 0;
	lang_occurances_size = LANG_INITIAL_SIZE;
	if (word_dictionary != NULL) {
		hash_free(word_dictionary);
	}
	if (lang_occurances != NULL) {
		free(lang_occurances);
	}
}

int initialize(STOP_FILES_DIR stop_files_dir) {
	DIR* stop_files;
	struct dirent* dir;
	char* language_name;

	// cleanup existing resources
	if (initialized) {
		cleanup();
	}

	// Create hash table 
	word_dictionary = (LIST_CELL_T**)calloc(HT_SIZE, sizeof(LIST_CELL_T*));

	// Create language occurance array
	lang_occurances = (LANG_T*)calloc(LANG_INITIAL_SIZE, sizeof(LANG_T));

	// open the stop files directory and then try to process each file
	if ((stop_files = opendir(stop_files_dir)) != NULL) {
		chdir(stop_files_dir);
		while ((dir = readdir(stop_files)) != NULL) {
			if (dir->d_type == DT_REG) {
				// get the language name from whatever object we are dealing with. char* or StorageFile^?
				language_name = get_language_name(dir->d_name);

				// process
				process_language(word_dictionary, dir->d_name, language_name);
				
				// free up memory allocated when getting the language name
				free(language_name);

				if (errno == ENOMEM) {
					// terminate if language processing failed due to lack of memory
					display_dialog("Out of memory. Please restart application");
					cleanup();
					closedir(stop_files);
					return 1;
				}
			}
		}
	}
	else {
		display_dialog("could not open stop file directory");
		return 1;
	}
	initialized = 1;
	closedir(stop_files);
	return 0;
}

void detect_language(char* text) {
	size_t i;
	unsigned int max = 0;
	size_t max_index = 0;
	char output[BUFSIZ];

	if (!initialized) {
		display_dialog("Please select a folder containing the stop words to use");
		return;
	}

	analyze(word_dictionary, text);

	// find out which language has the most occurances
	for (i = 0; i < numberOfLanguages; i++) {
		if (lang_occurances[i].occurances > max) {
			max = lang_occurances[i].occurances;
			max_index = i;
		}
	}

	// finally, print out the language with the most occurances
	if (max == 0) {
		display_dialog("Could not identify language");
	}
	else {
		sprintf(output, "%s", lang_occurances[max_index].lang_name);
		display_dialog(output);
		for (i = 0; i < numberOfLanguages; i++) {
			lang_occurances[i].occurances = 0;
		}
	}
}

