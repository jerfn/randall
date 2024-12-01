#include "rand64-sw.h"

/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;

static char *file;

/* Initialize the software rand64 implementation.  */
void software_rand64_init(void) {
    urandstream = fopen("/dev/random", "r");
    if (!urandstream)
        abort();
}

void set_file(char *input) {
    file = input;
}

/* Initialize the software rand64 implementation with specific file as input */
void software_rand64_init_file(void) {
    urandstream = fopen(file, "r");
    if (!urandstream)
        abort();
}

/* Return a random value, using software operations.  */
unsigned long long software_rand64(void) {
    unsigned long long int x;
    if (fread(&x, sizeof x, 1, urandstream) != 1)
        abort();
    return x;
}

/* Finalize the software rand64 implementation.  */
void software_rand64_fini(void) { fclose(urandstream); }


/* lrand48_r implementation.  */

static struct drand48_data buffer;

void lrand_init(void) {
    srand48_r(time(NULL), &buffer);
}

unsigned long long lrand_rand64(void) {
    long result1, result2;

    mrand48_r(&buffer, &result1);
    mrand48_r(&buffer, &result2);

    return ((unsigned long long) result1 << 32) + (unsigned long long) result2;
}

void lrand_fini(void) {}