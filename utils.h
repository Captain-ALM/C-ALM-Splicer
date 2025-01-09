#ifndef UTILS_H_INCLUDED

#define UTILS_H_INCLUDED

#include "bools.h"
#include <stdlib.h>
#include <stdio.h>

#define eof -1
#define null 0

int pow_int(int base, int exponent);

char code_buff_to_byte(char* buff, int len);
char hex_buff_to_byte(char* buff, int len);
int byte_to_code_buff(char byte, char** buff);
int byte_to_hex_buff(char byte, char** buff, bool upper);

char code_to_int(char code);
char hex_to_int(char hex);
char int_to_hex(char i);
char int_to_hex_upper(char i);

#endif
