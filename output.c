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
        }

    } else if (strcmp(input, "lrand48_r") == 0) {
        // Handles lrand48_r case
        initialize = lrand_init;
        rand64 = lrand_rand64;
        finalize = lrand_fini;
    } else {
        // Handle /F case.
        if (strncmp(input, "/", 1) == 0) {
            set_file(input);
            initialize = software_rand64_init_file;
            rand64 = software_rand64;
            finalize = software_rand64_fini;
        } else {
            fprintf(stderr, "file name does not start with /\n");
            return 1;
        }
    }

    initialize();
    int wordsize = sizeof rand64();
    int output_errno = 0;

    if (strcmp(output, "stdio") == 0) {
        // Handles stdio case
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
    } else {
        // Handle -o N option
        if (!output || *output == '\0')
            return 1; // NULL or empty string is not a valid positive integer

        char *endptr;
        errno = 0;
        long long N = strtoll(output, &endptr, 10); // Read N

        // Check for errors
        if (errno != 0 || *endptr != '\0' || N <= 0) {
            perror("strtoll read N");
            return 1; // Conversion failed or there are leftover characters or N is not positive
        }



        // Create buffer
        char *buf = (char *)malloc(nbytes);

        if (!buf) {
            perror("malloc");
            return 1;
        }

        char *current = buf; // Pointer to the current position in the buffer
        int bytes_left_to_populate = nbytes;

        // Loop to populate buffer
        do {
            unsigned long long x = rand64();
            int bytes_populate = bytes_left_to_populate < 8 ? bytes_left_to_populate : 8;

            memcpy(current, &x, bytes_populate); // Copy bytes (binary representation)
            current += bytes_populate;           // Move the pointer forward
            
            bytes_left_to_populate -= bytes_populate;

            // printf("rand num: %x, buffer: %x\n", x, buf);

        } while (0 < bytes_left_to_populate);



        // Write bytes, N at a time
        do {
            int bytes_to_write = nbytes < N ? nbytes : N;
            long bytes_written = write(1, buf, bytes_to_write);

            if (bytes_written == -1) {
                perror("write");
                free(buf);
                return 1;
            }
            nbytes -= bytes_written;
        } while (0 < nbytes);

        free(buf);
    }

    finalize();
    return !!output_errno;
}
