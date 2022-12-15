#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

#include <gmp.h>

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
        if (arr[j] > pivot) {                                               \
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

typedef unsigned long ulong;
sort_type(ulong);

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
#define MONSIZE 8

typedef struct monkey_t {
    unsigned long arr[0xff];
    unsigned long arrcount;
    unsigned long mod;
    const char* op;
    unsigned long mtrue;
    unsigned long mfalse;
} monkey_t;

static unsigned long getarr(const char* str, unsigned long* arr)
{
    unsigned long count = 0;
    char* c = strchr(str, ':') + 2, *s;
    s = c;
    c = strchr(c, ',');
    while (c) {
        *c = 0;
        arr[count] = atol(s);
        ++count;
        *c = ',';
        s = c + 2;
        c = strchr(s, ',');
    }
    arr[count] = atol(s);
    ++count;
    return count;
}

static inline const char* getop(const char* str)
{
    static const char oldstr[] = "old";
    return strstr(str, oldstr) + sizeof(oldstr);
}

static inline unsigned long getmod(const char* str)
{
    static const char bystr[] = "by";
    return atol(strstr(str, "by") + sizeof(bystr));
}

static inline unsigned long getbool(const char* str)
{
    static const char monkeystr[] = "monkey";
    return atol(strstr(str, monkeystr) + sizeof(monkeystr));
}

static void printmonkeys(const monkey_t* monkeys, const int monkeysize)
{
    unsigned long m, i;
    for (m = 0; m < monkeysize; ++m) {
        printf("Monkey %lu: ", m);
        for (i = 0; i < monkeys[m].arrcount; ++i) {
            if (i) {
                printf(", ");
            }
            printf("%lu\n", monkeys[m].arr[i]);
        }
        printf("\n");
    }
}

static void printtimes(const unsigned long* times, const unsigned long size,
        unsigned long round)
{
    unsigned long i;
    printf("Round %lu:\n", round);
    for (i = 0; i < size; ++i) {
        printf("Monkey %lu inspected items %lu times.\n", i, times[i]);
    }
    printf("\n");
}

static unsigned long doop(const char* op, const unsigned long n)
{
    char c, num[16];
    sscanf(op, "%c %s", &c, num);
    unsigned long m = isdigit(num[0]) ? atol(num) : n;
    switch (c) {
        case '+': return n + m;
        case '*': return n * m;
        default: return printf("Failed Op: %c\n", c);
    }
}

static unsigned long puzzle(const char strs[ARRLEN][STRLEN], const unsigned long size)
{
    monkey_t monkeys[MONSIZE];
    unsigned long i = 1, n, m, x, k, times[MONSIZE] = {0};
    for (m = 0; m < MONSIZE; ++m) {
        monkeys[m].arrcount = getarr(strs[i++], monkeys[m].arr);
        monkeys[m].op = getop(strs[i++]);
        monkeys[m].mod = getmod(strs[i++]);
        monkeys[m].mtrue = getbool(strs[i++]);
        monkeys[m].mfalse = getbool(strs[i++]);
        i += 2;
    }
    
    for (n = 0; n < 20; ++n) {
        for (m = 0; m < MONSIZE; ++m) {
            for (i = 0; i < monkeys[m].arrcount; ++i) {
                x = doop(monkeys[m].op, monkeys[m].arr[i]) / 3;
                k = x % monkeys[m].mod ? monkeys[m].mfalse : monkeys[m].mtrue;
                monkeys[k].arr[monkeys[k].arrcount++] = x;
            }
            times[m] += monkeys[m].arrcount;
            monkeys[m].arrcount = 0;
        }
    }

    sort(times, MONSIZE, ulong);
    return times[0] * times[1];
}

static unsigned long puzzle2(const char strs[ARRLEN][STRLEN], const unsigned long size)
{
    monkey_t monkeys[MONSIZE];
    unsigned long i = 1, n, m, x, k, bigmod = 1, times[MONSIZE] = {0};
    for (m = 0; m < MONSIZE; ++m) {
        monkeys[m].arrcount = getarr(strs[i++], monkeys[m].arr);
        monkeys[m].op = getop(strs[i++]);
        monkeys[m].mod = getmod(strs[i++]);
        monkeys[m].mtrue = getbool(strs[i++]);
        monkeys[m].mfalse = getbool(strs[i++]);
        bigmod *= monkeys[m].mod;
        i += 2;
    }

    for (n = 0; n < 10000; ++n) {
        for (m = 0; m < MONSIZE; ++m) {
            for (i = 0; i < monkeys[m].arrcount; ++i) {
                x = doop(monkeys[m].op, monkeys[m].arr[i]) % bigmod;
                k = x % monkeys[m].mod ? monkeys[m].mfalse : monkeys[m].mtrue;
                monkeys[k].arr[monkeys[k].arrcount++] = x;
            }
            times[m] += monkeys[m].arrcount;
            monkeys[m].arrcount = 0;
        }
        /*if (n == 0 || n == 19 || n == 999 || n == 1999) {
            printmonkeys(monkeys, MONSIZE);
            printtimes(times, MONSIZE, n + 1);
        }*/
    }

    sort(times, MONSIZE, ulong);
    return times[0] * times[1];
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

    unsigned long i, j = 0, arrcount = 0;
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

    printf("Puzzle 1: %lu\n", puzzle(strs, arrcount));
    printf("Puzzle 2: %lu\n", puzzle2(strs, arrcount));

    free(buf);
    return EXIT_SUCCESS;
}
