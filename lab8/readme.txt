Name: Ryan Peterman
UID: 704269982
Section: Lab 3

Makefile changes:

	1. Added -lpthread to the LDLIBS target so we can use
	the pthreads library

main.c changes:

	1. Grabbed the nested for loop that does computation
	for every pixel and hoisted it into my thread function
	multithread_ray_trace

	2. Declared the number of threads, scene, and scaled_color
	array as global so that the threads can access them, and add
	the result of the pixel calculations to the global scaled_color 
	array
		- Figuring out how to give the threads each access to these
		parameters without passing in wasteful structs was one of
		the harder problems for me

	3. Allocate memory for the thread arguments and the thread ids
	to pass into pthread_create by address

	4. Wait for all the threads to finish then printf all the resultant
	pixel colors to disk 

	5. Free malloced memory for thread args and thread ids

Results:

1. First I ran make clean check to get an initial benchmark:
	Each test ran at around the same time:
		real	0m48.048s
		user	0m48.053s
		sys		0m0.002s

2. Then after spliting up the work in the main computation loop:
	This time make check clean output faster and faster
	results for more and more threads:
	
	time ./srt 1-test.ppm >1-test.ppm.tmp

		real	0m48.018s
		user	0m48.019s
		sys		0m0.005s

	time ./srt 2-test.ppm >2-test.ppm.tmp

		real	0m24.304s
		user	0m48.494s
		sys		0m0.008s

	time ./srt 4-test.ppm >4-test.ppm.tmp

		real	0m12.121s
		user	0m48.144s
		sys		0m0.001s

	time ./srt 8-test.ppm >8-test.ppm.tmp

		real	0m6.528s
		user	0m50.566s
		sys		0m0.003s

	In conclusion, because this problem is inherently embarrasingly
	parallel, multithread massively improves performance since the
	heavy computation can be split among threads and run in parallel
	without having to worry about synchronization.