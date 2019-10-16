The assignment was to take the given srt implementation and make it multi-
threaded, using pthread.h and the functions pthread_create and pthread_join.

One of the main challenges was implementing pthread_create, since it takes an
argument that is a function, so I needed to figure out what part(s) of the 
original main function to extract and put into a separate function (that would
then be called via the call to pthread_create. Another issue that came up was
having to use global variables in certain cases, because they would need to be
accessed by both the main function and the new doRayTrace() function.

Other issues in implementing the multithreaded program were smaller things,
such as compilation errors and simple human error typos. Many of my 
compilation errors were simply forgetting that in C you must initialize all 
variables before anything else (cannot put "int i = 0" in a for loop).

Timing:
Here is my timing output, for nthreads=1,2,4,8 respectively.

real	0m41.615s
user	0m41.401s
sys	0m0.127s

real	0m21.081s
user	0m41.278s
sys	0m0.006s

real	0m14.891s
user	0m43.227s
sys	0m0.005s

real	0m9.263s
user	0m44.047s
sys	0m0.047s

Clearly, using a multithreaded implementation greatly increases speed. As seen
above, using 8 threads is faster than 4 threads, which is faster than 2, which
is faster than 1 (as expected).
