#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static char* file_read(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buf = malloc(size + 1);
    fread(buf, size, 1, file);
    buf[size] = 0;
    fclose(file);
    return buf;
}

#define ARRLEN 0xfff
#define STRLEN 0xff

static long puzzle(const char strs[ARRLEN][STRLEN], const long size)
{
    long arr[0xfff], arrcount = 0, max = 0, count = 0, i, j, tmp, s = 1;
    for (i = 0; i < size; ++i) {
        if (strs[i][0] >= '0' && strs[i][0] <= '9') {
            count += atoi(strs[i]);
        } else {
            arr[arrcount++] = count;
            if (max < count) {
                max = count;
            }
            count = 0;
        }
    }

    for (i = 0; i < arrcount - 1 && s; ++i) {
        s = 0;
        for (j = 0; j < arrcount - i - 1; ++j) {
            if (arr[j] < arr[j + 1]) {
                tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                ++s;
            }
        }
      
    }

    for (i = 0; i < arrcount; ++i) {
        printf("Count: %ld\n", arr[i]);
    }

    printf("Puzzle 2: %ld\n", arr[0] + arr[1] + arr[2]);
    return max;
}

int main(const int argc, const char** argv)
{
    if (argc < 2) {
        printf("Missing input puzzle.\n");
        return EXIT_FAILURE;
    }

    char* buf = file_read(argv[1]);
    if (!buf) {
        printf("Could not open file '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    long j = 0, arrcount = 0;
    char strs[ARRLEN][STRLEN];

    for (long i = 0; buf[i]; ++i) {
        if (buf[i] == '\n') {
            strs[arrcount++][j] = 0;
            j = 0;
            if (arrcount > ARRLEN) {
                printf("Error: array buffer is too small!\n");
                return EXIT_FAILURE;
            }
        } else {
            strs[arrcount][j++] = buf[i];
            if (j > STRLEN) {
                printf("Error: string buffer is too small!\n");
                return EXIT_FAILURE;
            }
        }
    }

    if (j) {
        strs[arrcount++][j] = 0;
    }

    printf("Puzzle 1: %ld\n", puzzle(strs, arrcount));

    free(buf);
    return EXIT_SUCCESS;
}
