#ifndef RAND64_SW_H
#define RAND64_SW_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Software implementation.  */

/* Initialize the software rand64 implementation.  */
void software_rand64_init(void);

void set_file(char *input);

/* Initialize the software rand64 implementation with specific file as input */
void software_rand64_init_file(void);

/* Return a random value, using software operations.  */
unsigned long long software_rand64(void);

/* Finalize the software rand64 implementation.  */
void software_rand64_fini(void);



/* lrand48_r implementation.  */

void lrand_init(void);

unsigned long long lrand_rand64(void);

void lrand_fini(void);

#endif /* RAND64_SW_H */