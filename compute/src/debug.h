#ifndef WORDBRUSH_DEBUG_H
#define WORDBRUSH_DEBUG_H

#ifndef NDEBUG

#include <stdio.h>

#define debug(...) \
    fprintf(stderr, __VA_ARGS__)

#define debug_only(x) do { \
    x; \
} while(0)

#else

#define debug(x, ...)

#define debug_only(x)

#endif //NDEBUG

#endif //WORDBRUSH_DEBUG_H
