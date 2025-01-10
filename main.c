#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "actionmeta.h"

void help(void);
bool begin(ActionMeta** sources, size_t sLen, ActionMeta* destination);
bool copyData(ActionMeta* source, ActionMeta* destination, FILE* output);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        help();
    }
    else
    {
        if (strcmp("-h", argv[1]) == 0)
        {
            help();
        }
        else
        {
            ActionMeta* c = null;
            ActionMeta* out = null;
            ActionMeta** ins = malloc(sizeof(ActionMeta*));
            bool settingsNext = false;
            bool isOut = false;
            bool fileNext = false;
            bool expectRange = false;
            bool expectSeperator = false;
            bool expectBSz = false;
            bool expectTTNL = false;
            size_t insL = 1;
            size_t insC = 0;
            for (size_t i = 1; i < argc; ++i)
            {
                if (c == null)
                {
                    if (strcmp(argv[i], "-i") == 0)
                    {
                        isOut = false;
                    }
                    else if (strcmp(argv[i], "-o") == 0)
                    {
                        isOut = true;
                    }
                    else
                    {
                        continue;
                    }
                    settingsNext = true;
                }
                else if (settingsNext)
                {
                    settingsNext = false;
                    expectRange = false;
                    expectSeperator = false;
                    expectBSz = false;
                    expectTTNL = false;
                    c = newActionMeta();
                    if (isOut)
                    {
                        if (out != null)
                            free(out);
                        out = c;
                    }
                    else
                    {
                        if (insC >= insL)
                        {
                            ins = realloc(ins, sizeof(ActionMeta*)*insL*2);
                            insL *= 2;
                        }
                        ins[insC++] = c;
                    }
                    char* ptr = argv[i];
                    while (*ptr != '\0')
                    {
                        char cc = *(ptr++);
                        if (cc == binary_mode || cc == code_mode || cc == hex_mode || cc == hex_upper_mode)
                        {
                            c->type = cc;
                        }
                        else if (cc == 'r' && !isOut)
                        {
                            expectRange = true;
                        }
                        else if (cc == 's')
                        {
                            expectSeperator = true;
                        }
                        else if (cc == 'z' && !isOut)
                        {
                            expectBSz = true;
                        }
                        else if (cc == 'n' && isOut)
                        {
                            expectTTNL = true;
                        }
                    }
                    fileNext = true;
                }
                else if (fileNext)
                {
                    fileNext = false;
                    c->filePath = argv[i];
                }
                else if (expectRange)
                {
                    expectRange = false;
                    if (*argv[i] == ':')
                    {
                        long u = string_to_long(argv[i]+1);
                        if (u > 0)
                        {
                            c->unbounded = false;
                            c->last = (size_t) u;
                        }
                    }
                    else
                    {
                        long l = string_to_long_ended(argv[i], '-');
                        if (l < 0)
                            continue;
                        c->first = (size_t) l;
                        const char* pargv = jump_to_pos(argv[i], '-');
                        if (*(pargv++) != '-')
                            continue;
                        long u = string_to_long(pargv);
                        if (u > l)
                        {
                            c->unbounded = false;
                            c->last = (size_t) u;
                        }
                    }
                }
                else if (expectSeperator)
                {
                    expectSeperator = false;
                    c->seperator = *argv[i];
                }
                else if (expectBSz)
                {
                    expectBSz = false;
                    long sz = string_to_long(argv[i]);
                    if (sz > 0)
                        c->bufferSize = (size_t) sz;
                }
                else if (expectTTNL)
                {
                    expectTTNL = false;
                    long tc = string_to_long(argv[i]);
                    if (tc > -1)
                        c->tokensToNewLine = (size_t) tc;
                }
                else
                {
                    c = null;
                }
            }

            if (out == null || insC == 0) {
                for (size_t i = 0; i < insC; ++i)
                    free(ins[i]);
                free(ins);
                if (out == null)
                    return 3;
                else
                    free(out);
                return 2;
            }

            bool ok = begin(ins, insC, out);

            for (size_t i = 0; i < insC; ++i)
                free(ins[i]);
            free(ins);
            free(out);

            if (!ok)
                return 1;
        }
    }
    return 0;
}

void help(void)
{
    printf("c-alm-splicer <args...>\n"
           "\n"
           "(C) Captain ALM 2025 - BSD 3-Clause License\n"
           "\n"
           "args:\n"
           "-h : This help message\n"
           "-i [bchHrsz] <file-path|-> [range] [separator] [buffer-size] : Reads a file in, - to read from stdin\n"
           "-o [bchHsn] <file-path|-> [separator] [tokens-to-new-line] : Writes a file out, - to wite to stdout\n"
           "\n"
           "Modes:\n"
           "b : Binary (Default)\n"
           "c : ASCII Integer Code\n"
           "h : Hexadecimal (Lower case)\n"
           "H : Hexadecimal (Upper case)\n"
           "\n"
           "range:\n"
           "start-end : r : Specify a range from start (inclusive) to end (exclusive)\n"
           "\n"
           "separator:\n"
           "\" \" : s : Specify the separator between tokens for c, h and H modes\n"
           "\n"
           "buffer-size:\n"
           "8192 : z : Size of the copy buffer\n"
           "\n"
           "tokens-to-new-line:\n"
           "0 : n : Number of tokens per line, 0 for no new lines\n"
           "\n");
}

bool begin(ActionMeta** sources, size_t sLen, ActionMeta* destination)
{
    FILE* output = null;
    if (strcmp(destination->filePath, "-") == 0)
    {
        output = stdout;
        setbuf(output, null);
    }
    else
    {
        output = fopen(destination->filePath, "w");
        if (output == null)
            return false;
    }

    for (size_t i = 0; i < sLen; ++i)
    {
        if (!copyData(sources[i], destination, output))
        {
            fclose(output);
            return false;
        }
    }

    if (strcmp(destination->filePath, "-") == 0)
        return true;
    return fclose(output) != eof;
}

bool copyData(ActionMeta* source, ActionMeta* destination, FILE* output)
{
    FILE* input = null;
    if (strcmp(source->filePath, "-") == 0)
    {
        input = stdin;
        setbuf(input, null);
    }
    else
    {
        input = fopen(source->filePath, "r");
        if (input == null)
            return false;
    }

    size_t pos = 0;
    char* buff = malloc(sizeof(char)*source->bufferSize);
    while (pos < source->first)
    {
        size_t r_left = source->first - pos;
        size_t rd = fread(buff, sizeof(char), (size_t) ((r_left < source->bufferSize) ? r_left : source->bufferSize), input);
        if (rd == 0)
        {
            free(buff);
            if (strcmp(source->filePath, "-") != 0)
                fclose(input);
            return false;
        }
        pos += rd;
    }

    char* ptr = buff;
    char* cbuff = malloc(sizeof(char));
    size_t cpos = 0;
    size_t clen = 1;
    char cb = 0;
    bool saanviSingla = false;
    size_t tokensLeft = destination->tokensToNewLine;
    char const newLineChar = '\n';

    while (source->unbounded || pos < source->last)
    {
        size_t r_left = source->last - pos;
        size_t rd = fread(buff, sizeof(char), (size_t) ((r_left < source->bufferSize) ? r_left : source->bufferSize), input);
        if (rd == 0 && !feof(input))
        {
            free(cbuff);
            free(buff);
            if (strcmp(source->filePath, "-") != 0)
                fclose(input);
            return false;
        }
        pos += rd;
        if (feof(input))
            break;
        char* lbuffEnd = buff + rd;
        while (ptr < lbuffEnd)
        {
            if (source->type == binary_mode && destination->type == binary_mode)
            {
                size_t wt = fwrite(buff, sizeof(char), lbuffEnd - ptr, output);
                if (wt == 0)
                {
                    free(cbuff);
                    free(buff);
                    if (strcmp(source->filePath, "-") != 0)
                        fclose(input);
                    return false;
                }
                ptr += wt;
            }
            else if (source->type == binary_mode)
            {
                char* tw = null;
                size_t twl = 0;
                if (destination->type == code_mode)
                {
                    twl = byte_to_code_buff(*(ptr++), &tw);
                }
                else
                {
                    twl = byte_to_hex_buff(*(ptr++), &tw, destination->type == hex_upper_mode);
                }
                size_t wt = fwrite(tw, sizeof(char), twl, output);
                if (wt == 0)
                {
                    free(cbuff);
                    free(buff);
                    if (strcmp(source->filePath, "-") != 0)
                        fclose(input);
                    return false;
                }
                bool paulaSuarezRodriguez = true;
                if (destination->tokensToNewLine > 0)
                {
                    --tokensLeft;
                    if (tokensLeft == 0)
                    {
                        tokensLeft = destination->tokensToNewLine;
                        wt = fwrite(&newLineChar, sizeof(char), 1, output);
                        if (wt == 0)
                        {
                            free(cbuff);
                            free(buff);
                            if (strcmp(source->filePath, "-") != 0)
                                fclose(input);
                            return false;
                        }
                        paulaSuarezRodriguez = false;
                    }
                }
                if (paulaSuarezRodriguez)
                {
                    wt = fwrite(&(destination->seperator), sizeof(char), 1, output);
                    if (wt == 0)
                    {
                        free(cbuff);
                        free(buff);
                        if (strcmp(source->filePath, "-") != 0)
                            fclose(input);
                        return false;
                    }
                }
            }
            else
            {
                cb = *(ptr++);
                if (cb == source->seperator || cb == '\r' || cb == '\n')
                {
                    if (saanviSingla)
                    {
                        saanviSingla = false;
                        if (cb == '\n')
                            continue;
                    }
                    if (cb == '\r')
                        saanviSingla = true;
                    char cpb = ((source->type == code_mode) ? code_buff_to_byte : hex_buff_to_byte)(cbuff, cpos);
                    if (destination->type == binary_mode)
                    {
                        size_t wt = fwrite(&cpb, sizeof(char), 1, output);
                        if (wt == 0)
                        {
                            free(cbuff);
                            free(buff);
                            if (strcmp(source->filePath, "-") != 0)
                                fclose(input);
                            return false;
                        }
                    }
                    else
                    {
                        char* tw = null;
                        size_t twl = 0;
                        if (destination->type == code_mode)
                        {
                            twl = byte_to_code_buff(cpb, &tw);
                        }
                        else
                        {
                            twl = byte_to_hex_buff(cpb, &tw, destination->type == hex_upper_mode);
                        }
                        size_t wt = fwrite(tw, sizeof(char), twl, output);
                        if (wt == 0)
                        {
                            free(cbuff);
                            free(buff);
                            if (strcmp(source->filePath, "-") != 0)
                                fclose(input);
                            return false;
                        }
                        bool paulaSuarezRodriguez = true;
                        if (destination->tokensToNewLine > 0)
                        {
                            --tokensLeft;
                            if (tokensLeft == 0)
                            {
                                tokensLeft = destination->tokensToNewLine;
                                wt = fwrite(&newLineChar, sizeof(char), 1, output);
                                if (wt == 0)
                                {
                                    free(cbuff);
                                    free(buff);
                                    if (strcmp(source->filePath, "-") != 0)
                                        fclose(input);
                                    return false;
                                }
                                paulaSuarezRodriguez = false;
                            }
                        }
                        if (paulaSuarezRodriguez)
                        {
                            wt = fwrite(&(destination->seperator), sizeof(char), 1, output);
                            if (wt == 0)
                            {
                                free(cbuff);
                                free(buff);
                                if (strcmp(source->filePath, "-") != 0)
                                    fclose(input);
                                return false;
                            }
                        }
                    }
                }
                else
                {
                    if (saanviSingla)
                        saanviSingla = false;
                    if (cpos >= clen)
                    {
                        cbuff = realloc(cbuff, sizeof(char)*clen*2);
                        clen *= 2;
                    }
                    cbuff[cpos++] = cb;
                }
            }
        }
        ptr = buff;
    }

    free(cbuff);
    free(buff);
    if (strcmp(source->filePath, "-") == 0)
        return true;
    return fclose(input) != eof;
}
