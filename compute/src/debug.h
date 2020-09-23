#ifndef WORDBRUSH_DEBUG_H
#define WORDBRUSH_DEBUG_H

#ifndef NDEBUG

#include <stdio.h>

#define debug(...) \
    fprintf(stderr, __VA_ARGS__)

#else

#define debug(x, ...)

#endif //NDEBUG

#endif //WORDBRUSH_DEBUG_H
