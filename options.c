#include "options.h"

long long get_nbytes(int argc, char **argv) {
    bool valid = false;
    long long nbytes;

    if (argc == 2) {
        char *endptr;
        errno = 0;
        nbytes = strtoll(argv[1], &endptr, 10);
        if (errno)
            perror(argv[1]);
        else
            valid = !*endptr && 0 <= nbytes;
    }
    if (!valid) {
        return -1;
    }
    return nbytes;
}