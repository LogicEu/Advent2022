#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define swap(a, b, type) do { type tmp = a; a = b; b = tmp; } while (0)

#define sort_type(type)                                                     \
                                                                            \
static long partition_ ## type(type* arr, const long low, const long high)  \
{                                                                           \
    type pivot = arr[high];                                                 \
    long i = (low - 1), j;                                                  \
    for (j = low; j < high; ++j) {                                          \
        if (arr[j] < pivot) {                                               \
            ++i;                                                            \
            swap(arr[i], arr[j], type);                                     \
        }                                                                   \
    }                                                                       \
    swap(arr[i + 1], arr[high], type);                                      \
    return i + 1;                                                           \
}                                                                           \
                                                                            \
static void sortq_ ## type(type* arr, const long low, const long high)      \
{                                                                           \
    if (low < high) {                                                       \
        long p = partition_ ## type(arr, low, high);                        \
        sortq_ ## type(arr, low, p - 1);                                    \
        sortq_ ## type(arr, p + 1, high);                                   \
    }                                                                       \
}                                                                           \
                                                                            \
static inline void sort_ ## type(type* arr, const long size)                \
{                                                                           \
    return sortq_ ## type(arr, 0, size - 1);                                \
}

#define sort(arr, size, type) sort_ ## type(arr, size)

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
#define LEN 9

static void puzzle(const char strs[ARRLEN][STRLEN], const long size)
{
    long i, j, n, pos[LEN], stackcounts[LEN] = {0};
    char stacks[LEN][ARRLEN], c;
    for (i = 0; strs[i][0]; ++i);

    const long mark = i--;
    for (j = 0; strs[i][j]; ++j) {
        if (strs[i][j] != ' ') {
            n = strs[i][j] - '0' - 1;
            pos[n] = j;
        }
    }

    for (--i; i >= 0; --i) {
        for (j = 0; j < LEN; ++j) {
            c = strs[i][pos[j]];
            if (c != ' ') {
                stacks[j][stackcounts[j]++] = c;
            }
        }
    }

    long times, from, to;
    for (i = mark + 1; i < size; ++i) {
        sscanf(strs[i], "move %ld from %ld to %ld", &times, &from, &to);
        --to, --from;
        for (j = 0; j < times; ++j) {
            stacks[to][stackcounts[to]++] = stacks[from][--stackcounts[from]];
        }
    }
    
    printf("Puzzle 1: ");
    for (i = 0; i < LEN; ++i) {
        printf("%c", stacks[i][stackcounts[i] - 1]);
    }
    printf("\n");
}

static void puzzle2(const char strs[ARRLEN][STRLEN], const long size)
{
    long i, j, n, pos[LEN], stackcounts[LEN] = {0};
    char stacks[LEN][ARRLEN], c;
    for (i = 0; strs[i][0]; ++i);

    const long mark = i--;
    for (j = 0; strs[i][j]; ++j) {
        if (strs[i][j] != ' ') {
            n = strs[i][j] - '0' - 1;
            pos[n] = j;
        }
    }

    for (--i; i >= 0; --i) {
        for (j = 0; j < LEN; ++j) {
            c = strs[i][pos[j]];
            if (c != ' ') {
                stacks[j][stackcounts[j]++] = c;
            }
        }
    }

    long times, from, to;
    for (i = mark + 1; i < size; ++i) {
        sscanf(strs[i], "move %ld from %ld to %ld", &times, &from, &to);
        --to, --from;
        
        stackcounts[from] -= times;
        memcpy(stacks[to] + stackcounts[to], stacks[from] + stackcounts[from], times);
        stackcounts[to] += times;
    }
    
    printf("Puzzle 2: ");
    for (i = 0; i < LEN; ++i) {
        printf("%c", stacks[i][stackcounts[i] - 1]);
    }
    printf("\n");
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

    long i, j = 0, arrcount = 0;
    char strs[ARRLEN][STRLEN];

    for (i = 0; buf[i]; ++i) {
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

    puzzle(strs, arrcount);
    puzzle2(strs, arrcount);

    free(buf);
    return EXIT_SUCCESS;
}
