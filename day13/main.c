#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

typedef char* charptr_t;
static int cmp(char*, char*);

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
        if (cmp(arr[j], pivot) > 0) {                                       \
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

static long getlist(const char* str)
{
    long stack = 0, i;
    for (i = 0; str[i]; ++i) {
        stack += str[i] == '[';
        stack -= str[i] == ']';
        if (!stack) {
            return i;
        }
    }
    return -1;
}

static long getnum(const char* str)
{
    long i;
    for (i = 0; isdigit(str[i]); ++i);
    return i;
}

static char* getarg(const char* str, const long n)
{
    char* s = malloc(STRLEN);
    memcpy(s, str, n);
    s[n] = 0;
    return s;
}   

static char** listargs(const char* str)
{
    long i, n = 0;
    char** args = malloc(sizeof(char*) * 0xff);
    for (i = 0; str[i]; ++i) {
        if (i && str[i] == '[') {
            args[n++] = getarg(str + i, getlist(str + i) + 1);
        } else if (isdigit(str[i])) {
            args[n++] = getarg(str + i, getnum(str + i));
        }
    }
    args[n] = NULL;
    return args;
}

static void tolist(char* num)
{
    size_t len = strlen(num);
    num[len++] = ']';
    memmove(num + 1, num, len + 1);
    num[0] = '[';
}

static void freeargs(char** args)
{
    long i;
    for (i = 0; args[i]; ++i) {
        free(args[i]);
    }
    free(args);
}

static int cmp(char* left, char* right)
{
    if (!left && !right) {
        return 0;
    }

    if (!left) {
        return 1;
    }

    if (!right) {
        return -1;
    }

    int dleft = isdigit(*left), dright = isdigit(*right);
    if (dleft && dright) {
        return atoi(right) - atoi(left);
    }
    
    if (dleft) {
        tolist(left);
        return cmp(left, right);
    }
    
    if (dright) {
        tolist(right);
        return cmp(left, right);
    }

    int i, c = 0;
    char** largs = listargs(left);
    char** rargs = listargs(right);

    for (i = 0; !c; ++i) {
        c = cmp(largs[i], rargs[i]);
        if (!largs[i] && !rargs[i]) {
            break;
        }
    }

    freeargs(largs);
    freeargs(rargs);

    return c;
}

static long puzzle(char strs[ARRLEN][STRLEN], const long size)
{
    long i, sum = 0;
    for (i = 0; i * 3 < size; ++i) {
        if (cmp(strs[i * 3], strs[i * 3 + 1]) > 0) {
            sum += i + 1;
        }
    }
    return sum;
}

sort_type(charptr_t);

static long findarg(char** args, const char* s)
{
    long i;
    for (i = 0; args[i]; ++i) {
        if (!strcmp(args[i], s)) {
            return i + 1;
        }
    }
    return -1;
}

static long puzzle2(char strs[ARRLEN][STRLEN], const long size)
{
    static char* d[] = {"[[2]]", "[[6]]"};

    long i, j = 0, sum = 0;
    char* args[ARRLEN];
    args[j++] = d[0];
    args[j++] = d[1];
    for (i = 0; i * 3 < size; ++i) {
        args[j++] = strs[i * 3];
        args[j++] = strs[i * 3 + 1];
    }
    args[j] = NULL;

    sort(args, j, charptr_t);
    return findarg(args, d[0]) * findarg(args, d[1]);
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
