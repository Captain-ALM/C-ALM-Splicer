#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main()
{
    printf("Hello world!\n");
    char* buff;
    int l = byte_to_code_buff((char)-76, &buff);
    ++l;
    buff = realloc(buff, sizeof(char)*l);
    printf("%i\n",l);
    buff[l-1] = 0;
    for (int i = 0; i < l - 1; ++i) {
        printf("%i : %i\n", i, (unsigned char) buff[i]);
    }
    printf("%s\n",buff);
    printf("%i\n", code_buff_to_byte(buff, l-1));
    free(buff);
    printf("###\n");
    l = byte_to_hex_buff((char)-76, &buff, true);
    ++l;
    buff = realloc(buff, sizeof(char)*l);
    printf("%i\n",l);
    buff[l-1] = 0;
    for (int i = 0; i < l - 1; ++i) {
        printf("%i : %i\n", i, (unsigned char) buff[i]);
    }
    printf("%s\n",buff);
    printf("%i\n", hex_buff_to_byte(buff, l-1));
    free(buff);
    return 0;
}
