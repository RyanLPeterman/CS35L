#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// determines length of a null terminated string
int str_len(const char* str);

int main(int argc, char** argv) {

	// number of arguments error check
	if(argc != 3) {
		fprintf(stderr, "Incorrect number of arguments passed in\n");
		exit(1);
	}

	// contains argument sets
	char* from = argv[1];
	char* to = argv[2];

	int len = str_len(from);

	// set length error check
	if(len != str_len(to)) {
		fprintf(stderr, "There is a length mismatch between the two sets\n");
		exit(2);
	}

	// check that from does not have any duplicate bytes
	int i,j;
	char curr;

	for(i = 0; i < len; i++) {

		// current character we are checking
		curr = from[i];

		for(j = i + 1; j < len; j++) {
			if(curr == from[j]) {
				fprintf(stderr, "From set contains duplicate : %c\n", curr);
				exit(3);
			}
		}
	}

	// Now that arguments are valid we populate array of chars with the proper conversion
	char* lookup_table = malloc(sizeof(char) * 256);
	bool* contains_translation = malloc(sizeof(bool) * 256);

	// initialize 
	for(i = 0; i < 256 ;i++) {
		contains_translation[i] = false;
	}

	// populating lookup table
	for(i = 0; i < len; i++) {
		lookup_table[from[i]] = to[i];
		contains_translation[from[i]] = true;
	}

	char next_byte;

	// while there are characters in stdin 
	while((next_byte = getchar()) != EOF) {
		// if we have a translation
		if(contains_translation[next_byte])
			putchar(lookup_table[next_byte]);
		// otherwise just printout the untranslated byte
		else
			putchar(next_byte);
	}

	// for readability
	fprintf(stdout, "\n");

	return 0;
}


int str_len(const char* str) {
	int len = 0;

	while(str[len] != '\0') {
		len++;
	}

	return len + 1;
}