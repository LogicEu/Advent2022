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
#define LEN 0xffff

typedef struct ivec2 {
    int x, y;
} ivec2;

static ivec2 dirtable[] = {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1}  
};

static inline int getdir(const char c)
{
    switch (c) {
        case 'R': return 0;
        case 'D': return 1;
        case 'L': return 2;
        case 'U': return 3;
    }
    printf("Error parsing char\n");
    return -1;
}

static long puzzle(const char strs[ARRLEN][STRLEN], const long size)
{
    char c;
    long i, j, k, n, f, arrcount = 0;
    ivec2 h = {0, 0}, t = {0, 0}, arr[LEN];
    arr[arrcount++] = t;
    for (i = 0; i < size; ++i) {
        sscanf(strs[i], "%c %ld", &c, &n);
        j = getdir(c);
        const ivec2 v = {dirtable[j].x, dirtable[j].y};
        for (j = 0; j < n; ++j) {
            h.x += v.x;
            h.y += v.y;

            int difx = h.x - t.x, dify = h.y - t.y, moved = 0;
            if (difx > 1 || difx < -1) {
                ++moved;
                difx = difx > 0 ? 1 : -1;
            }
            else if (dify > 1 || dify < -1) {
                ++moved;
                dify = dify > 0 ? 1 : -1;
            }

            if (moved) {
                t.x += difx;
                t.y += dify;
                for (k = 0, f = 0; k < arrcount; ++k) {
                    if (arr[k].x == t.x && arr[k].y == t.y) {
                        ++f;
                        break;
                    }
                }

                if (!f) {
                    arr[arrcount++] = t;
                    if (arrcount > LEN) {
                        printf("Missing memmory!!\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }
    return arrcount;
}

static long puzzle2(const char strs[ARRLEN][STRLEN], const long size)
{
    char c;
    long i, j, k, n, m, f, x, y, arrcount = 0;
    ivec2 rope[10], arr[LEN];
    memset(rope, 0, sizeof(ivec2) * 10);
    arr[arrcount++] = rope[9];
    for (i = 0; i < size; ++i) {
        sscanf(strs[i], "%c %ld", &c, &n);
        j = getdir(c);
        const ivec2 v = {dirtable[j].x, dirtable[j].y};
        for (j = 0; j < n; ++j) {
            
            rope[0].x += v.x;
            rope[0].y += v.y;

            for (k = 1; k < 10; ++k) {
                
                m = 0;
                x = rope[k - 1].x - rope[k].x;
                y = rope[k - 1].y - rope[k].y;
                
                if (x > 1 || x < -1) {
                    ++m;
                    x = x > 0 ? 1 : -1;
                }
                
                if (y > 1 || y < -1) {
                    ++m;
                    y = y > 0 ? 1 : -1;
                }

                if (m) {
                    rope[k].x += x;
                    rope[k].y += y;
                }
            }

            if (m) {
                for (m = 0, f = 0; m < arrcount; ++m) {
                    if (arr[m].x == rope[9].x && arr[m].y == rope[9].y) {
                        ++f;
                        break;
                    }
                }

                if (!f) {
                    arr[arrcount++] = rope[9];
                    if (arrcount >= LEN) {
                        printf("Missing memmory!!\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }
    return arrcount;
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
