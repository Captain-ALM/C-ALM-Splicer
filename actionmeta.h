#ifndef ACTIONMETA_H_INCLUDED

#define ACTIONMETA_H_INCLUDED

#include "bools.h"

#define binary_mode 'b'
#define code_mode 'c'
#define hex_mode 'h'
#define hex_upper_mode 'H'
#define oct_mode 'o'
#define any_mode 'a'

typedef struct {
    char* filePath;
    char type;
    char seperator;
    size_t tokensToNewLine;
    size_t bufferSize;
    size_t first;
    size_t last;
    bool unbounded;
    char base;
    char max;
} ActionMeta;

ActionMeta* newActionMeta(void);

#endif
