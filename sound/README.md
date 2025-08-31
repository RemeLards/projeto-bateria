# Wave to C converter

## Introduction

This is a converter that transcripts the information encoded in a wave file (.WAV suffix) into a C file (.c suffix).

`Only PCM files are processed`


## Wave file format


## Usage

Wave2C [-vsfc] files....

-v :  verbose output
-s : generate statistics
-f : write output to a file
-c : generate C code

The name of the outfile is the basename of the input file with the suffix wav change to c.
and non ascii characters, including whitespaces and dash, changed to _.

The output file has the following format for a file named claves

    #include <stdint.h>
    /* Even: channel 1 | Odd: channel 2 */

    #define CLAVES_SIZE (101387)
    uint16_t CLAVES[] = {
       -50,    -61,
       -61,    -75,
       -45,    -61,
        22,    -39,
        284,    110,
        797,    524,
        1468,   1114,
        . . .

        -1,       1,
    }; // CLAVES


## References

1. [WAV at Wikipedia](https://en.wikipedia.org/wiki/WAV)
2. [Audio File Format Specifications](https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html)
3. [WAVE Audio File Format](https://www.loc.gov/preservation/digital/formats/fdd/fdd000001.shtml)
4. [What is a WAV file?](https://docs.fileformat.com/audio/wav/)
5. [WAVE PCM soundfile format](http://soundfile.sapp.org/doc/WaveFormat/)
