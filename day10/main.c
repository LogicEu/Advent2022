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
    long i, cycles = 0, n, p = 0, x = 1, sum = 0;
    char buf[0xff]; 
    for (i = 0; i < size; ++i) {
        ++cycles;
        if (cycles == 20 || cycles == 60 || cycles == 100 || 
            cycles == 140 || cycles == 180 || cycles == 220) {
            sum += cycles * x;
        }
       
        if (p) {
            p = 0;
            x += n;
        } else if (strcmp(strs[i], "noop")) {
            sscanf(strs[i], "%s %ld", buf, &n);
            ++p;
            --i;
        }
         
    }
    return sum;
}

static void puzzle2(const char strs[ARRLEN][STRLEN], const long size)
{
    long i, crt, cycles = 0, n, p = 0, x = 1;
    char buf[0xff]; 
    for (i = 0; i < size; ++i) {
        ++cycles;
        if (cycles == 20 || cycles == 60 || cycles == 100 || 
            cycles == 140 || cycles == 180 || cycles == 220) {
        }

        crt = (cycles - 1) % 40;
        if (!crt) {
            printf("\n");
        }
       
        printf("%c", (x >= crt - 1 && x <= crt + 1) ? '#' : ' ');
        if (p) {
            p = 0;
            x += n;
        } else if (strcmp(strs[i], "noop")) {
            sscanf(strs[i], "%s %ld", buf, &n);
            ++p;
            --i;
        }
         
    }
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

    printf("Puzzle 1: %ld\n", puzzle(strs, arrcount));
    printf("Puzzle 2:\n"); 
    puzzle2(strs, arrcount);

    free(buf);
    return EXIT_SUCCESS;
}
