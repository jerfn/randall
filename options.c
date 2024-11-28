#include "options.h"

long long get_nbytes(int argc, char **argv) {
    bool valid = false;
    long long nbytes;

    if (optind >= argc) {
        fprintf(stderr, "Error: Missing required argument\n");
        return -1;
    }

    char *endptr;
    errno = 0;
    nbytes = strtoll(argv[optind], &endptr, 10);
    if (errno)
        perror(argv[optind]);
    else
        valid = !*endptr && 0 <= nbytes;
        
    if (!valid) {
        return -1;
    }

    return nbytes;
}