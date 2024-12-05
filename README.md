# randall.c

Tuned and refactored random number generator in C from CS 35L with additional options (original by Paul Eggert)

## Usage

./randall [OPTIONS] BYTES

### BYTES

number of random bytes to output

### -i _input_

Input is one of the following:

_rdrand_ – use the hardware random-number generation supported by x86-64 processors if available, used by default

_lrand48_r_ - use the lrand48_r function of the GNU C library

_/F_ (any argument beginning with /) – use the file /F as a source of random data, instead of using /dev/random

### -o _output_

Output is one of the following:

_stdio_ – Use stdio output, used by default

_N (positive decimal integer)_ – Output N bytes at a time, using the write system call
