#include "output.h"

bool writebytes(unsigned long long x, int nbytes) {
    do {
        if (putchar(x) < 0)
            return false;
        x >>= CHAR_BIT;
        nbytes--;
    } while (0 < nbytes);

    return true;
}

int handleoutput(char *input, char *output, int nbytes) {

    // printf("Input: %s\nOutput: %s\n", input, output);

    /* Now that we know we have work to do, arrange to use the
       appropriate library.  */
    // Function pointers
    void (*initialize)(void);
    unsigned long long (*rand64)(void);
    void (*finalize)(void);

    // Error handling for missing input or output
    if (!input || !output) {
        fprintf(stderr, "Missing input or output!\n");
        return 1;
    }

    // Handle input options
    if (strcmp(input, "rdrand") == 0) {
        // Handles hardware case

        // Set function pointers
        if (rdrand_supported()) {
            initialize = hardware_rand64_init;
            rand64 = hardware_rand64;
            finalize = hardware_rand64_fini;
        } else {
            fprintf(stderr, "hardware random-number generation not supported\n");
            return 1;
            // initialize = software_rand64_init;
            // rand64 = software_rand64;
            // finalize = software_rand64_fini;
        }

    } else if (strcmp(input, "lrand48_r") == 0) {
        // Handles lrand48_r case
    } else {
        // Handle /F case.
    }

    

    initialize();
    int wordsize = sizeof rand64();
    int output_errno = 0;

    do {
        unsigned long long x = rand64();
        int outbytes = nbytes < wordsize ? nbytes : wordsize;
        if (!writebytes(x, outbytes)) {
            output_errno = errno;
            break;
        }
        nbytes -= outbytes;
    } while (0 < nbytes);

    if (fclose(stdout) != 0)
        output_errno = errno;

    if (output_errno) {
        errno = output_errno;
        perror("output");
    }

    finalize();
    return !!output_errno;
}