#include "utils.h"

int pow_int(int base, int exponent) {
    int val = 1;
    for (int i = 0; i < exponent; ++i)
        val *= base;
    return val;
}

char code_buff_to_byte(char* buff, int len) {
    char val = 0;
    int mult = 1;
    for (int i = len - 1; i > -1; --i) {
        val += code_to_int(buff[i]) * mult;
        mult*=10;
    }
    return val;
}

char hex_buff_to_byte(char* buff, int len) {
    char val = 0;
    int shift = 0;
    for (int i = len - 1; i > -1; --i) {
        val += hex_to_int(buff[i]) << shift;
        shift+=4;
    }
    return val;
}

int byte_to_code_buff(char byte, char** buff) {
    unsigned char byteS = (unsigned char) byte;
    int sz = (byteS > 99) ? 3 : ((byteS > 9) ? 2 : 1);
    *buff = malloc(sizeof(char)*sz);
    int mult = pow_int(10, sz - 1);
    for (int i = 0; i < sz; ++i) {
        (*buff)[i] = ((int) byteS / mult) + 48;
        byteS = (int) byteS % mult;
        mult /= 10;
    }
    return sz;
}

int byte_to_hex_buff(char byte, char** buff, bool upper) {
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
