/**
 * @file fsize.c
 * @license This file is licensed under the GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007. You may obtain a copy of this license at https://www.gnu.org/licenses/gpl-3.0.en.html.
 * @author Tushar Chaurasia (Dark-CodeX)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#include <Windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

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

bool is_directory(const char *loc)
{
    if (!loc)
        return false;
#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    DWORD fileAttributes = GetFileAttributesA(loc);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES)
        return false;
    else if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        return true;
    else
        return false;
#else
    struct stat buffer;
    if (stat(loc, &buffer) == 0 && S_ISDIR(buffer.st_mode))
        return true;
#endif
    return false;
}

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
        if (is_directory(argv[i]) == true)
        {
            fprintf(stderr, "err: '%s': Is a directory\n", argv[i]);
            continue;
        }
        FILE *fptr = fopen(argv[i], "rb");
        if (!fptr)
        {
            fprintf(stderr, "err: fopen(): '%s': %s\n", argv[i], strerror(errno));
            continue;
        }
        fseek(fptr, 0, SEEK_END);
        size_t len = ftell(fptr); // size of file in bytes
        fseek(fptr, 0, SEEK_SET);

        switch (type)
        {
        case BIT:
            printf("%zu bits : %s\n", len * BIT, argv[i]);
            break;
        case BYTES:
            printf("%zu bytes : %s\n", len, argv[i]);
            break;
        case KIB:
            printf("%0.3f kibibytes : %s \n", (float)len / KIB, argv[i]);
            break;
        case MIB:
            printf("%0.3f mebibytes : %s \n", (float)len / MIB, argv[i]);
            break;
        case GIB:
            printf("%0.3f gibibytes : %s \n", (float)len / GIB, argv[i]);
            break;
        case KB:
            printf("%0.3f kilobytes : %s \n", (float)len / KB, argv[i]);
            break;
        case MB:
            printf("%0.3f megabytes : %s \n", (float)len / MB, argv[i]);
            break;
        case GB:
            printf("%0.3f gigabytes : %s \n", (float)len / GB, argv[i]);
            break;
        }

        fclose(fptr);
    }
    return EXIT_SUCCESS;
}