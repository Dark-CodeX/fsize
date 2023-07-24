/**
 * @file fsize.c
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define FSIZE_VER "1.0.0"

enum FILE_SIZE_TYPE
{
    BIT = 8,          // multiply
    BYTES = 1,        // as-it-is BASE
    KIB = 1024,       // divide
    MIB = 1048576,    // divide
    GIB = 1073741824, // divide
    KB = 1000,        // divide
    MB = 1000000,     // divide
    GB = 1000000000   // divide
};

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fprintf(stderr, "err: no file was given\n");
        return EXIT_FAILURE;
    }
    int i = 1;
    if (strcmp(argv[i], "--help") == 0)
    {
        puts("Usage: [option] [files]");
        puts("Options:");
        puts("    --help                             Display this help message.");
        puts("    --version                          Display the version of the app.");
        puts("    --size=<unit_of_byte>              Converts bytes into other units.");
        puts("          =bit, bits, BIT, BITS        Converts bytes into bits.");
        puts("          =byte, bytes, BYTE, BYTES    Default size.");
        puts("          =kib, KIB                    Converts bytes into kibibytes.");
        puts("          =mib, MIB                    Converts bytes into mebibytes.");
        puts("          =gib, GIB                    Converts bytes into gibibytes.");
        puts("          =kb, KB                      Converts bytes into kilobytes.");
        puts("          =mb, MB                      Converts bytes into megabytes.");
        puts("          =gb, GB                      Converts bytes into gigabytes.");

        return EXIT_SUCCESS;
    }
    if (strcmp(argv[i], "--version") == 0)
    {
        printf("%s: %s\n", argv[0], FSIZE_VER);
        return EXIT_SUCCESS;
    }
    enum FILE_SIZE_TYPE type = BYTES; // default type is BYTES
    if (strncmp(argv[i], "--size=", 7) == 0)
    {
        argv[i] += 7; // skip `--size=` which is total of 7 characters/bytes excluding null-terminating character
        if (strcmp(argv[i], "bit") == 0 || strcmp(argv[i], "bits") == 0 ||
            strcmp(argv[i], "BIT") == 0 || strcmp(argv[i], "BITS") == 0)
        {
            type = BIT;
        }
        else if (strcmp(argv[i], "byte") == 0 || strcmp(argv[i], "bytes") == 0 ||
                 strcmp(argv[i], "BYTE") == 0 || strcmp(argv[i], "BYTES") == 0)
        {
            type = BYTES;
        }
        else if (strcmp(argv[i], "kib") == 0 || strcmp(argv[i], "KIB") == 0)
        {
            type = KIB;
        }
        else if (strcmp(argv[i], "mib") == 0 || strcmp(argv[i], "MIB") == 0)
        {
            type = MIB;
        }
        else if (strcmp(argv[i], "gib") == 0 || strcmp(argv[i], "GIB") == 0)
        {
            type = GIB;
        }
        else if (strcmp(argv[i], "kb") == 0 || strcmp(argv[i], "KB") == 0)
        {
            type = KB;
        }
        else if (strcmp(argv[i], "mb") == 0 || strcmp(argv[i], "MB") == 0)
        {
            type = MB;
        }
        else if (strcmp(argv[i], "gb") == 0 || strcmp(argv[i], "GB") == 0)
        {
            type = GB;
        }
        else
        {
            fprintf(stderr, "err: '%s' is not a valid unit of byte, try using '--help' flag\n", argv[i]);
            return EXIT_FAILURE;
        }
        i++;
        if (i == argc)
        {
            fprintf(stderr, "err: no file was given\n");
            return EXIT_FAILURE;
        }
    }
    for (; i < argc; i++)
    {
        FILE *fptr = fopen(argv[i], "rb");
        if (!fptr)
        {
            fprintf(stderr, "err: fopen(): '%s': %s\n", argv[i], strerror(errno));
            return EXIT_FAILURE;
        }
        fseek(fptr, 0, SEEK_END);
        size_t len = ftell(fptr); // size of file in bytes
        fseek(fptr, 0, SEEK_SET);

        switch (type)
        {
        case BIT:
            printf("%s: %zu bits\n", argv[i], len * BIT);
            break;
        case BYTES:
            printf("%s: %zu bytes\n", argv[i], len);
            break;
        case KIB:
            printf("%s: %0.3f kibibytes\n", argv[i], (float)len / KIB);
            break;
        case MIB:
            printf("%s: %0.3f mebibytes\n", argv[i], (float)len / MIB);
            break;
        case GIB:
            printf("%s: %0.3f gibibytes\n", argv[i], (float)len / GIB);
            break;
        case KB:
            printf("%s: %0.3f kilobytes\n", argv[i], (float)len / KB);
            break;
        case MB:
            printf("%s: %0.3f megabytes\n", argv[i], (float)len / MB);
            break;
        case GB:
            printf("%s: %0.3f gigabytes\n", argv[i], (float)len / GB);
            break;
        }

        fclose(fptr);
    }
    return EXIT_SUCCESS;
}