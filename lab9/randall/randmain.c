#include "randcpuid.h"	// for rand_supported interface
#include <dlfcn.h>		// for dll functions
#include <stdbool.h>	// for bool type
#include <stdlib.h>		// for exit and strtoll
#include <stdio.h>		// for fprintf
#include <errno.h>		// for errno

// Name: Ryan Peterman
// UID: 704269982
// Section: Lab 3

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
	/* Check arguments.  */
	bool valid = false;
	long long nbytes;
	if (argc == 2) {

      	char *endptr;
      	errno = 0;
      	nbytes = strtoll (argv[1], &endptr, 10);
	    if (errno)
			perror (argv[1]);
	    else
			valid = !*endptr && 0 <= nbytes;
	}
	if (!valid)
	{
	    fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
	    return 1;
	}

	/* If there's no work to do, don't worry about which library to use.  */
	if (nbytes == 0)
	    return 0;

	/* Now that we know we have work to do, arrange to use the
	 appropriate library.  */
	unsigned long long (*rand64) (void);
	// commented to silence compiler of unused functions
	// void (*initialize) (void);
	// void (*finalize) (void);

	// string to hold error messages
	char* error_str; 
	// handle for opening dll
	void* dll_handle;		

	// if the hardware generation is supported 
  	if (rdrand_supported ())
    {
    	// for testing
    	// printf("Using Hardware Implementation");

    	// open the library and perform relocations now
    	dll_handle = dlopen("randlibhw.so", RTLD_NOW);
    	if(dll_handle == NULL) {
    		fprintf(stderr, "%s\n", dlerror());
    		exit(1);
    	}

    	// grab handle for function
      	rand64 = dlsym(dll_handle, "hardware_rand64");
      	// see if there was an error grabbing function
      	error_str = dlerror();
      	if(error_str) {
    		fprintf(stderr, "%s\n", error_str);
    		exit(1);
    	}
    }
    // hardware not supported, use software libraries
  	else {
  		// for testing
  		// printf("Using Software Implementation");

  		// open library and perform relocations now
  		dll_handle = dlopen("randlibsw.so", RTLD_NOW);
  		if(dll_handle == NULL) {
  			fprintf(stderr, "%s\n", dlerror());
    		exit(1);
  		}

    	// grab handle for function
      	rand64 = dlsym(dll_handle, "software_rand64");
      	// see if there was an error grabbing function
      	error_str = dlerror();
      	if(error_str) {
    		fprintf(stderr, "%s\n", error_str);
    		exit(1);
    	}
    }

    // removed this since sw_lib now uses attribute to call init before main
    // hardware doesn't need to call init since its an empty function anyway
  	// initialize (); 

  	int wordsize = sizeof rand64 ();
  	int output_errno = 0;

  	do {
      	unsigned long long x = rand64 ();
      	size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      	if (fwrite (&x, 1, outbytes, stdout) != outbytes) {
	  		output_errno = errno;
	  		break;
	  	}
      	nbytes -= outbytes;
    } while (0 < nbytes);

  	if (fclose (stdout) != 0)
    	output_errno = errno;

  	if (output_errno) {
      	errno = output_errno;
      	perror ("output");
      	// finalize ();
      	return 1;
    }

    // remove this to prevent double free since we call destructor after main using gcc attribute
    // hardware doesn't need to call finalize since its an empty function anyway
  	// finalize ();

    // clean up dll handle
  	dlclose(dll_handle);

  	return 0;
}