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
        if (arr[j].x < pivot.x) {                                           \
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
#define STRLEN 0xff

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(a) ((a) < 0 ? -(a) : (a))

typedef struct ivec2 {
    long x, y;
} ivec2;

sort_type(ivec2);

static long dist(const ivec2 p, const ivec2 q)
{
    return ABS(p.x - q.x) + ABS(p.y - q.y);
}

static long puzzle(const char strs[ARRLEN][STRLEN], const long size)
{
    long i, j, d, closer, dists[ARRLEN], maxdist = 0, sum = 0;
    ivec2 s, b, min = {INT_MAX, INT_MAX}, max = {INT_MIN, INT_MIN};
    ivec2 p = {0, 2000000}, sensors[ARRLEN], beacons[ARRLEN];
    //ivec2 p = {0, 10}, sensors[ARRLEN], beacons[ARRLEN];
    
    for (i = 0; i < size; ++i) {
        
        sscanf(
            strs[i], 
            "Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld",
            &s.x, &s.y, &b.x, &b.y
        );

        sensors[i] = s;
        beacons[i] = b;
        dists[i] = dist(s, b);
        
        max.x = MAX(max.x, s.x);
        max.y = MAX(max.y, s.y);
        min.x = MIN(min.x, s.x);
        min.y = MIN(min.y, s.y);
        maxdist = MAX(maxdist, dists[i]);
    }

    for (p.x = min.x - maxdist - 1; p.x < max.x + maxdist + 1; ++p.x) {
        closer = 0;
        for (i = 0; i < size; ++i) {
             d = dist(p, sensors[i]);
             if (d <= dists[i]) {
                for (j = 0; j < size; ++j) {
                    if (p.x == beacons[j].x && p.y == beacons[j].y) {
                        --closer;
                        break;
                    }
                }
                ++closer;
                break;
             }
        }
        sum += !!closer;
    }

    return sum;
}

static ivec2 getrange(const ivec2 s, const long d, const long y)
{
    const long dy = ABS(s.y - y);
    const long dx = d - dy;
    return dx < 0 ? (ivec2){0, 0} : (ivec2){s.x - dx, s.x + dx};
}

static void printranges(const ivec2* ranges, const long size)
{
    long i;
    for (i = 0; i < size; ++i) {
        printf("%ld, %ld\n", ranges[i].x, ranges[i].y);
    }
    printf("\n");
}

static long clean(ivec2* arr, long size)
{
    long i;
    for (i = 0; i < size; ++i) {
        if (arr[i].x == 0 && arr[i].y == 0) {
            memmove(arr + i, arr + i + 1, (size - i) * sizeof(ivec2));
            --size;
            --i;
        }
    }
    return size;
}

static long puzzle2(const char strs[ARRLEN][STRLEN], const long size)
{
    long i, j, closer = 0, dists[ARRLEN], maxdist = 0;
    ivec2 s, b, min = {INT_MAX, INT_MAX}, max = {INT_MIN, INT_MIN};
    ivec2 sensors[ARRLEN], beacons[ARRLEN];
    
    for (i = 0; i < size; ++i) {
        
        sscanf(
            strs[i], 
            "Sensor at x=%ld, y=%ld: closest beacon is at x=%ld, y=%ld",
            &s.x, &s.y, &b.x, &b.y
        );

        sensors[i] = s;
        beacons[i] = b;
        dists[i] = dist(s, b);
        
        max.x = MAX(max.x, s.x);
        max.y = MAX(max.y, s.y);
        min.x = MIN(min.x, s.x);
        min.y = MIN(min.y, s.y);
        maxdist = MAX(maxdist, dists[i]);
    }

#define LEN 4000000L
//#define LEN 20L
    
    ivec2 p, ranges[ARRLEN];
    for (p.y = 0; p.y <= LEN; ++p.y) {
        for (i = 0; i < size; ++i) {
            ranges[i] = getrange(sensors[i], dists[i], p.y);
        }

        sort(ranges, size, ivec2);
        j = clean(ranges, size);
        
        p.x = 0;
        for (i = 0; i < j && p.x <= LEN; ++i) {
            if (p.x >= ranges[i].x && p.x <= ranges[i].y) {
                p.x = ranges[i].y + 1;
            } else if (p.x < ranges[i].x) {
                //printf("Winner!: %ld, %ld\n", p.x, p.y);
                return p.x * 4000000L + p.y;
            }
        }
    }

    return 0;
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
