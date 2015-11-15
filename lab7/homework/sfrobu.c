#include <stdio.h>	  // for print functions for errors
#include <stdlib.h>   // for malloc, realloc
#include <stdbool.h>  // for bool type
#include <error.h>    // for exit
#include <unistd.h>   // for system calls
#include <sys/stat.h> // for fstat
#include <sys/types.h>// for struct members

// Name: Ryan Peterman
// UID: 704269982
// Section: Lab 3

// struct to contain char* while maintaining length information
// length information us useful when allocating memory
typedef struct {
	int length;
	char* string;
} string;

// for use in system calls
const int m_stdin = 0;
const int m_stdout = 1;

// to print comparisons done in qsort
int comparisons;

// creates array of strings from stdin
string* read_stdin_into_strings(int* num_strings);
// prints char** to stdout
void print_word_array(char** words, int num_strings);
// sfrob comparison function for two strings
int sfrob(const char* a, const char* b);
// wrapper function to make it work for qsort with the correct prototype
int frob_cmp_wrapper(const void* a, const void* b);


int main() {

  comparisons = 0;
	int* num_strings = (int*) malloc(sizeof(int));

  // return an array of strings with their accompanying lengths
  // and alters num_strings by reference
	string* strings = read_stdin_into_strings(num_strings);

  // using char* array now since cmp function doesnt need to know length of strings
  // i.e. strings are space terminated and therefore inherently give length
  int i = 0;
  char** str_arr =  (char**) malloc(sizeof(char**) * (*num_strings));

  // intitialize char* array to the strings in our structs
  for(; i < (*num_strings); i++) {
    str_arr[i] = strings[i].string;
  }

  // test
  // print_word_array(str_arr,(*num_strings));

  // sort the strings
  qsort(str_arr, (size_t) *num_strings, sizeof(char*), frob_cmp_wrapper);

  // print sorted array to stdout
  print_word_array(str_arr,(*num_strings));

  // free memory
  free(num_strings);
  free(str_arr);
  free(strings);

	return 0;
}

// comparison logic function
int sfrob(const char* a, const char* b) {
	
  int index = 0;

  int a_int, b_int;
  // loop through both strings
  while(index >= 0) {

    a_int = a[index];
    b_int = b[index];

    // char in a is less than b
    if( (a_int ^ 42) < (b_int ^ 42) )
      return -1;
    // a ended before b
    if( (a_int == ' ') && (b_int != ' '))
      return -1;
    // char in b is less than a
    if ( (a_int ^ 42) > (b_int ^ 42) ) 
      return 1;
    // b ended before a
    if ( (a_int != ' ') && (b_int == ' '))
      return 1;
    // ended at the same time
    if(a_int == ' ' && b_int == ' ')
      return 0;

    // increment
    index++;
  }
}

// wrapper function for compare
int frob_cmp_wrapper(const void* a, const void* b) {

  // comparison function was called by qsort
  comparisons++;

  // cast pointers to be useable by sfrob
  const char * first = *(const char **) a;
  const char * second = *(const char **) b;

  return sfrob(first, second); 
}

// reads stdin into a buffer
string* read_stdin_into_strings(int* num_strings) 
{
  size_t capacity = 1024; // arbitrary size
  size_t iter = 0;
  (*num_strings) = 0;

  char* buffer = (char*) malloc(capacity);
  int* next_byte = (int*) malloc(sizeof(int));

  // flags
  bool eat_whitespace = true;
  bool last_byte_was_space = false;
  bool not_only_whitespace = true;

  struct stat file_stat;
  fstat(m_stdin, &file_stat);

  // if its a regular file
  if(S_ISREG(file_stat.st_mode)) {
    // update capacity variable so we don't realloc later
    capacity = file_stat.st_size;

    // allocate all the memory for the buffer up front
    buffer = (char*) realloc(buffer, capacity);
  }

  // loop through entire file
  while (read(m_stdin, next_byte, 1)) {
  	
  	// eats all spaces prior to first nonspace character in stdin
  	if(*next_byte == ' ' && eat_whitespace) {

  		while((not_only_whitespace = read(m_stdin, next_byte, 1)) && *next_byte == ' ') {

  			if(!not_only_whitespace) {
  				fprintf(stderr, "Error: stdin only contained spaces");
  				exit(1);
  			}
  		}

  		// done eating whitespace for this stdin
  		eat_whitespace = false;	
  	}

    if(*next_byte == ' ') {
    	if(!last_byte_was_space) {
	    	(*num_strings)++;
	    	last_byte_was_space = true;    		
    	}
    	else // last byte was also a space
    		continue; // dont store multiple spaces
    }
    else { //*next_byte is a nonspace character 
    	last_byte_was_space = false;
    	eat_whitespace = false;    	
    }


    // store in buffer
    buffer[iter++] = *next_byte;

    // if buffer full
    if (capacity == iter)
      capacity *= 2;
      buffer = (char*) realloc(buffer, capacity);
  }

  // empty file case
  if(iter == 0) {
    fprintf(stderr, "Error: Input was an empty file.\n");
    exit(1);
  }

  // space terminate buffer if not already
  if(buffer[iter - 1] != ' ') {
  	// if buffer full add one byte
    if (capacity == iter)
      buffer = (char*) realloc(buffer, capacity + 1);

  	buffer[iter] = ' ';
  	(*num_strings)++;
  }


  // Now we parse the buffer into an array of strings to be sorted
  string* str_arr = (string*) malloc(sizeof(string) * (*num_strings));

  int word_len = 0;
  int string_iter = 0;
  int i;
  // loop through entire buffer and store the string lengths
  for(i = 0; i <= iter; i++) {
  	if(buffer[i] != ' ')
  		word_len++;
  	else {
  		// store length
  		str_arr[string_iter].length = word_len + 1;
  		// reset length counter
  		word_len = 0;
  		// increment string counter
  		string_iter++;
  	}
  }

  string_iter = 0;
  int word_iter = 0;
  // allocate space for string
  str_arr[string_iter].string = (char*) malloc(sizeof(char) * str_arr[string_iter].length);

  // now that we know length we loop storing the strings
  for(i = 0; i <= iter; i++) {
  	

  	// current string in str_arr is full and there are more strings left to fill
  	if( ((word_iter) == str_arr[string_iter].length) && (string_iter + 1 != (*num_strings)) ) {
  		
  		string_iter++;
  		word_iter = 0;
  		// allocate space for next string
  		str_arr[string_iter].string = (char*) malloc(sizeof(char) * str_arr[string_iter].length);
  	}

	  str_arr[string_iter].string[word_iter] = buffer[i];
  	word_iter++;
  }

  // free buffer
  free(buffer);
  free(next_byte);

  return str_arr;
}

// for debugging and printing answer
void print_word_array(char** words, int num_strings) {
  char to_print;
  int str_index = 0;
  int char_index = 0;

  // for every string
  for(; str_index < num_strings; str_index++) {

    char_index = 0;

    while((to_print = words[str_index][char_index]) != ' ') {
      write(m_stdout, &to_print, 1);
      char_index++;
    }

    to_print = ' ';
    // print out space we skipped over
    write(m_stdout, &to_print, 1);
  }

  // print comparisons as per spec
  fprintf(stderr, "Comparisons: %d\n", comparisons);
}