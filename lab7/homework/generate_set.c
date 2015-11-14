// Generates a string of all ascii characters
// that are frobincated for use in tr 
#include <stdio.h>

int main() {
	
	int c;

	// print out unfrobnicated octals
	for(c = 0;c < 256; c++) {
		fprintf(stdout, "\\%o", c);
	}

	fprintf(stdout, "\n");

	// print out frobnicated octals
	for(c = 0;c < 256; c++) {
		fprintf(stdout, "\\%o", (c^42) );
	}

	return 0;
}