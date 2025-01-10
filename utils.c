#include "utils.h"

bool not_either_internal(char a, char b1, char b2) {
    return a != b1 && a != b2;
}

const char* jump_to_pos(const char* str, char target) {
    const char* pos = str;
    while (not_either_internal(*(++pos), target, '\0'));
    return pos;
}

long string_to_long_ended(const char* str, char ender) {
    long val = 0;
    long mult = 1;
    if (*str == ender || *str == '\0') return -1;
    const char* ptr = jump_to_pos(str, ender);
    --ptr;
    while (ptr >= str) {
        val += code_to_int(*(ptr--)) * mult;
        mult *= 10;
    }
    return val;
}

long string_to_long(const char* str) {
    return string_to_long_ended(str, '\0');
}

size_t pow_int(size_t base, size_t exponent) {
    size_t val = 1;
    for (size_t i = 0; i < exponent; ++i)
        val *= base;
    return val;
}

char code_buff_to_byte(const char* buff, size_t len) {
    char val = 0;
    size_t mult = 1;
    for (size_t i = len - 1; i > -1; --i) {
        val += code_to_int(buff[i]) * mult;
        mult*=10;
    }
    return val;
}

char hex_buff_to_byte(const char* buff, size_t len) {
    char val = 0;
    size_t shift = 0;
    for (size_t i = len - 1; i > -1; --i) {
        val += hex_to_int(buff[i]) << shift;
        shift+=4;
    }
    return val;
}

size_t byte_to_code_buff(char byte, char** buff) {
    unsigned char byteS = (unsigned char) byte;
    size_t sz = (byteS > 99) ? 3 : ((byteS > 9) ? 2 : 1);
    *buff = malloc(sizeof(char)*sz);
    size_t mult = pow_int(10, sz - 1);
    for (size_t i = 0; i < sz; ++i) {
        (*buff)[i] = ((size_t) byteS / mult) + 48;
        byteS = (size_t) byteS % mult;
        mult /= 10;
    }
    return sz;
}

size_t byte_to_hex_buff(char byte, char** buff, bool upper) {
    *buff = malloc(sizeof(char)*2);
    (*buff)[0] = (upper ? int_to_hex_upper : int_to_hex)((byte>>4)&15);
    (*buff)[1] = (upper ? int_to_hex_upper : int_to_hex)(byte&15);
    return 2;
}

char code_to_int(char code) {
    if (code < 48 || code > 57) {
        return 0;
    }
    return code - 48;
}

char hex_to_int(char hex) {
    if (hex < 48 || (hex > 57 && hex < 65) || (hex > 70 && hex < 97) || hex > 102) {
        return 0;
    }
    if (hex > 47 && hex < 58) {
        return hex - 48;
    }
    if (hex > 64 && hex < 71) {
        return hex - 55;
    }
    return hex - 87;
}

char int_to_hex(char i) {
    if ((unsigned char) i < 11) {
        return (unsigned char) i + 48;
    }
    return (unsigned char) i + 55;
}

char int_to_hex_upper(char i) {
    if ((unsigned char) i < 11) {
        return (unsigned char) i + 48;
    }
    return (unsigned char) i + 87;
}
