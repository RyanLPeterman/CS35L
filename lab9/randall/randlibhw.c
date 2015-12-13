#include "randlib.h"
#include <immintrin.h>	// for hardware rand

// Module for hardware implementation of the random number generator

/* Initialize the hardware rand64 implementation.  */
// removed static to silence compiler (defined not used)
void hardware_rand64_init (void)
{
}

/* Return a random value, using hardware operations.  */
// changed to extern
extern unsigned long long
hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}

/* Finalize the hardware rand64 implementation.  */
// removed static to silence compiler (defined not used)
void hardware_rand64_fini (void)
{
}