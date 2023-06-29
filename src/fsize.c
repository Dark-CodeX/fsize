#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum FILE_SIZE_TYPE
{
    BIT = 8,          // multiply
    BYTES = 1,        // multiply
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
    enum FILE_SIZE_TYPE type = BYTES; // default type is BYTES
    if (strncmp(argv[i], "--size=", 7) == 0)
    {
        argv[i] += 7; // skip `--size=` which is total of 7 characters/bytes excluding null-terminating character
        if (strcmp(argv[i], "bit") == 0 || strcmp(argv[i], "bits") == 0)
        {
            type = BIT;
        }
        else if (strcmp(argv[i], "byte") == 0 || strcmp(argv[i], "bytes") == 0)
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
            printf("%s: %0.2f kibibytes\n", argv[i], (float)len / KIB);
            break;
        case MIB:
            printf("%s: %0.2f mebibytes\n", argv[i], (float)len / MIB);
            break;
        case GIB:
            printf("%s: %0.2f gibibytes\n", argv[i], (float)len / GIB);
            break;
        case KB:
            printf("%s: %0.2f kilobytes\n", argv[i], (float)len / KB);
            break;
        case MB:
            printf("%s: %0.2f megabytes\n", argv[i], (float)len / MB);
            break;
        case GB:
            printf("%s: %0.2f gigabytes\n", argv[i], (float)len / GB);
            break;
        }

        fclose(fptr);
    }
    return EXIT_SUCCESS;
}