# C-ALM File Splicer

Splicing all those files!

This program is designed for binary manipulation and can convert between binary, denary, octal and hexadecimal!
It also supports ranges from inputs with support for using stdout and stdin!

## Usage:
    
    C-ALM-Splicer <args...>
    
    (C) Captain ALM 2025 - BSD 3-Clause License
    
    args:
    -h : This help message
    -i [bchHorsz] <file-path|-> [range] [separator] [buffer-size] : Reads a file in, - to read from stdin
    -o [bchHosn] <file-path|-> [separator] [tokens-to-new-line] : Writes a file out, - to write to stdout
    
    Modes:
    b : Binary (Default)
    c : ASCII Integer Code (Denary)
    h : Hexadecimal (Lower case)
    H : Hexadecimal (Upper case)
    o : Octal
    
    range:
    start-end : r : Specify a range from start (inclusive) to end (exclusive)
    
    separator:
    " " : s : Specify the separator between tokens for c, h and H modes
    
    buffer-size:
    8192 : z : Size of the copy buffer
    
    tokens-to-new-line:
    0 : n : Number of tokens per line, 0 for no new lines

    Exit Codes:
    0 : Completed
    1 : Aborted
    2 : Input Missing
    3 : Output Missing
    
