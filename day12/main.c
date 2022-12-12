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

#define W 67
#define H 41

typedef struct ivec2 {
    long x, y;
} ivec2;

//sort_type(long);

static ivec2 find(const char strs[ARRLEN][STRLEN], const int c)
{
    long x, y;
    for (y = 0; y < H; ++y) {
        for (x = 0; x < W; ++x) {
            if (strs[y][x] == c) {
                return (ivec2){x, y};
            }
        }
    }
    printf("Failed to find %c!\n", c);
    return (ivec2){-1, -1};
}

#define strat(strs, x, y, c) (strs[y][x] <= (c) + 1)

static long path[H][W];

static long rec(const char strs[ARRLEN][STRLEN], const ivec2 targ, ivec2 p, long steps)
{
    static long min = LONG_MAX;

    if ((steps && steps >= min) || (path[p.y][p.x] && path[p.y][p.x] <= steps)) {
        return 0;
    }

    if (p.x == targ.x && p.y == targ.y) {
        min = steps;
        //printf("Min!: %ld\n", min);
        return steps;
    }

    const char c = strs[p.y][p.x];
    const long prev = path[p.y][p.x];
    path[p.y][p.x] = steps;

    if (p.x + 1 < W && strat(strs, p.x + 1, p.y, c)) {
        ++p.x;
        rec(strs, targ, p, steps + 1);
        --p.x;
    }

    if (p.y + 1 < H && strat(strs, p.x, p.y + 1, c)) {
        ++p.y;
        rec(strs, targ, p, steps + 1);
        --p.y;
    }


    if (p.x > 0 && strat(strs, p.x - 1, p.y, c)) {
        --p.x;
        rec(strs, targ, p, steps + 1);
        ++p.x;
    }

    if (p.y > 0 && strat(strs, p.x, p.y - 1, c)) {
        --p.y;
        rec(strs, targ, p, steps + 1);
        ++p.y;
    }

    return min;
}

static long puzzle(char strs[ARRLEN][STRLEN], const long size)
{
    ivec2 p = find(strs, 'S');
    const ivec2 target = find(strs, 'E');
    strs[p.y][p.x] = 'a';
    strs[target.y][target.x] = 'z';
    memset(path, 0, W * H * sizeof(long));
    long n = rec(strs, target, p, 0);
    strs[target.y][target.x] = 'E';
    return n;
}

static long puzzle2(char strs[ARRLEN][STRLEN], const long size)
{
    long x, y, n, min = LONG_MAX;
    ivec2 p;
    const ivec2 target = find(strs, 'E');
    strs[target.y][target.x] = 'z';
    for (y = 0; y < H; ++y) {
        for (x = 0; x < W; ++x) {
            if (strs[y][x] == 'a') {
                memset(path, 0, W * H * sizeof(long));
                p.x = x;
                p.y = y;
                n = rec(strs, target, p, 0);
                if (n < min) {
                    min = n;
                }
            }
        }
    }
    return min;
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
    printf("Puzzle 2: %ld\n", puzzle2(strs, arrcount));

    free(buf);
    return EXIT_SUCCESS;
}
