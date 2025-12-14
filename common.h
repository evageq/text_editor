#ifndef __COMMON__
#define __COMMON__

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include "debug.h"

#define _concat(a, b) a##b
#define concat(a, b) _concat(a, b)
#define macrovar(name) concat(name, __LINE__)

#define defer(start, end) for ( \
        int macrovar(_i_) = (start, 0); \
        !macrovar(_i_); \
        (macrovar(_i_) += 1), end)

#define ncurse defer(initscr(), endwin())

typedef int Errno;
typedef bool valid_t;

typedef struct {
    int x;
    int y;
} vec2;

size_t get_n_by_perc(size_t n, uint perc);

#endif // __COMMON__
