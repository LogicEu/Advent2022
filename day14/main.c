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

#define ARRLEN 0xff
#define STRLEN 0xfff

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define W 54
#define H 167

#define MX 458
#define MY 0

#define WIDTH 1024
#define HEIGHT 1024

//#define TY (MY + H - 2)
#define TY (11)

typedef struct ivec2 {
    int x, y;
} ivec2;

static long getpath(const char* str, ivec2* arr)
{
    static const char* div = " ->,";

    long size = 0;
    char buf[STRLEN], *tok;
    int *iarr = (int*)arr;

    strcpy(buf, str);
    tok = strtok(buf, div);
    while (tok) {
        iarr[size++] = atoi(tok);
        tok = strtok(NULL, div);
    }
    
    return size / 2;
}

static void drawline(char grid[H][W], const ivec2 p, const ivec2 q)
{
    int i, min, max;
    if (p.x == q.x) {
        min = MIN(p.y, q.y), max = MAX(p.y, q.y);
        for (i = min; i <= max; ++i) {
            grid[i - MY][p.x - MX] = '#';
        }
    } else {
        min = MIN(p.x, q.x), max = MAX(p.x, q.x);
        for (i = min; i <= max; ++i) {
            grid[p.y - MY][i - MX] = '#';
        }
    }
}

static void drawline2(char grid[HEIGHT][WIDTH], const ivec2 p, const ivec2 q)
{
    int i, min, max;
    if (p.x == q.x) {
        min = MIN(p.y, q.y), max = MAX(p.y, q.y);
        for (i = min; i <= max; ++i) {
            grid[i - MY][p.x - MX] = '#';
        }
    } else {
        min = MIN(p.x, q.x), max = MAX(p.x, q.x);
        for (i = min; i <= max; ++i) {
            grid[p.y - MY][i - MX] = '#';
        }
    }
}

static void printgrid2(const char grid[HEIGHT][WIDTH])
{
    long i, j;
    for (i = 0; i < H + 3; ++i) {
        for (j = 0; j < W; ++j) {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
}

static void printgrid(const char grid[H][W])
{
    long i, j;
    for (i = 0; i < H; ++i) {
        for (j = 0; j < W; ++j) {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
}

static long sandfall(char grid[H][W], ivec2 p)
{
    long n = 0;
    grid[p.y][p.x] = 'o';
    while (!n) {
        if (p.y == H - 1) {
            --n;
        } else if (grid[p.y + 1][p.x] == '.') {
            grid[p.y][p.x] = '.';
            grid[++p.y][p.x] = 'o';
        } else if (grid[p.y + 1][p.x - 1] == '.') {
            grid[p.y][p.x] = '.';
            grid[++p.y][--p.x] = 'o';
        } else if (grid[p.y + 1][p.x + 1] == '.') {
            grid[p.y][p.x] = '.';
            grid[++p.y][++p.x] = 'o';
        } else {
            ++n;
        }
    }
    return n;
}

static long sandfall2(char grid[HEIGHT][WIDTH], const ivec2 q)
{
    long n = 0;
    ivec2 p = q;
    grid[p.y][p.x] = 'o';
    while (!n) {
        if (grid[p.y + 1][p.x] == '.') {
            grid[p.y][p.x] = '.';
            grid[++p.y][p.x] = 'o';
        } else if (grid[p.y + 1][p.x - 1] == '.') {
            grid[p.y][p.x] = '.';
            grid[++p.y][--p.x] = 'o';
        } else if (grid[p.y + 1][p.x + 1] == '.') {
            grid[p.y][p.x] = '.';
            grid[++p.y][++p.x] = 'o';
        } else if (p.x == q.x && p.y == q.y) {
            --n;
        } else {
            ++n;
        }
    }
    return n;
}

static long puzzle(const char strs[ARRLEN][STRLEN], const long size)
{
    const ivec2 spawn = {500 - MX, 0};

    long i, j, n;
    ivec2 arr[ARRLEN];
    char grid[H][W];
    memset(grid, '.', W * H);
 
    for (i = 0; i < size; ++i) {
        n = getpath(strs[i], arr);
        for (j = 0; j < n - 1; ++j) {
            drawline(grid, arr[j], arr[j + 1]);
        }
    }

    n = 0, i = 0;
    while (n != -1) {
        i += n;
        n = sandfall(grid, spawn);
    }

    //printgrid(grid);
    return i;
}


static long puzzle2(const char strs[ARRLEN][STRLEN], const long size)
{
    const ivec2 spawn = {500 - MX, 0};

    long i, j, n;
    ivec2 arr[ARRLEN];
    char grid[HEIGHT][WIDTH];
    memset(grid, '.', WIDTH * HEIGHT);
 
    for (i = 0; i < size; ++i) {
        n = getpath(strs[i], arr);
        for (j = 0; j < n - 1; ++j) {
            drawline2(grid, arr[j], arr[j + 1]);
        }
    }
    drawline2(grid, (ivec2){0, H + 1}, (ivec2){WIDTH - 1, H + 1});

    n = 0, i = 0;
    while (n != -1) {
        i += n;
        n = sandfall2(grid, spawn);
    }

    //printgrid2(grid);
    return i + 1;
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
