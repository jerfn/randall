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
