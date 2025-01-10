#ifndef UTILS_H_INCLUDED

#define UTILS_H_INCLUDED

#include "bools.h"
#include <stdlib.h>
#include <stdio.h>

#define eof -1
#define null 0

int string_to_int(const char* str);

size_t pow_int(size_t base, size_t exponent);

char code_buff_to_byte(const char* buff, size_t len);
char hex_buff_to_byte(const char* buff, size_t len);
size_t byte_to_code_buff(char byte, char** buff);
size_t byte_to_hex_buff(char byte, char** buff, bool upper);

char code_to_int(char code);
char hex_to_int(char hex);
char int_to_hex(char i);
char int_to_hex_upper(char i);

#endif
