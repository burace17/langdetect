#include "langdetect.h"
#include <errno.h>
void detect_language(char text[]) {
	char* pch;
	char* inputWords[BUFSIZ];
	char output[1024];
	int numWords = 0;
	int matchedEn, matchedEs, matchedFr, matchedDe;

	pch = strtok(text, " ");
	while (pch != NULL) {
		inputWords[numWords] = pch;
		numWords++;
		pch = strtok(NULL, " ");	
	}

	matchedEn = get_occurances(inputWords, numWords, "../dict/en.dic");
	matchedEs = get_occurances(inputWords, numWords, "../dict/es.dic");
	matchedFr = get_occurances(inputWords, numWords, "../dict/fr.dic");
	matchedDe = get_occurances(inputWords, numWords, "../dict/de.dic");

	sprintf(output, "English: %d/%d (%g%%)\n Spanish: %d/%d (%g%%)\n French: %d/%d (%g%%)\n German: %d/%d (%g%%)\n",
			matchedEn, numWords, floor((matchedEn/(double)numWords)* 100),
			matchedEs, numWords, floor((matchedEs/(double)numWords)* 100),
			matchedFr, numWords, floor((matchedFr/(double)numWords)* 100),
			matchedDe, numWords, floor((matchedDe/(double)numWords)* 100));

	display_dialog(output);
}

int get_occurances(char* input[], int numWords, const char* filename) {
	FILE* file = fopen(filename, "r");
	int occurances = 0;
	int i;
	char buffer[BUFSIZ];

	if (file == NULL) {
		char error[1024];
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		sprintf(error, "Could not open file: %d\nCWD: %s", errno, cwd);
		display_dialog(error);		
		return 0;
	}

	while (fgets(buffer, sizeof(buffer), file)) {
		size_t ln = strlen(buffer) - 1;
		if (buffer[ln] == '\n')
			buffer[ln] = '\0';
		for (i = 0; i < numWords; i++) {
			if (input[i] == NULL) {
				break;
			}

			ln = strlen(input[i]) - 1;
			
			if (input[i][ln] == '\n') {
				input[i][ln] = '\0';
			}
			
#ifdef _WIN32
			if (stricmp(input[i], buffer) == 0) {
#else
			if (strcasecmp(input[i], buffer) == 0) {
#endif
				occurances++;
			}
		}
	}

	fclose(file);
	return occurances;
}

