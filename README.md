# C-ALM File Splicer

Splicing all those files!

This program is designed for binary manipulation and can convert between binary, denary, octal and hexadecimal!
It also supports ranges from inputs with support for using stdout and stdin!

## Usage:
    
    C-ALM-Splicer <args...>
    
    (C) Captain ALM 2026 - BSD 3-Clause License
    
    args:
    -h : This help message
    -i <[abchHorsz]> <file-path|-> [range] [separator] [buffer-size] : Reads a file in, - to read from stdin
    -o <[abchHorsn]> <file-path|-> [range] [separator] [tokens-to-new-line] : Writes a file out, - to write to stdout
    -b <base> : Sets the base used for the any mode
    
    Modes:
    b : Binary (Default)
    c : ASCII Integer Code (Denary)
    h : Hexadecimal (Lower case)
    H : Hexadecimal (Upper case)
    o : Octal
    a : Any base (2-63), base set from -b option
    
    Base options are read in order and are paired with each used base mode
    
    range:
    start-end : r : Specify a range from start (inclusive) to end (exclusive)
    
    separator:
    " " : s : Specify the separator between tokens for o, c, h and H modes
    
    buffer-size:
    8192 : z : Size of the copy buffer
    
    tokens-to-new-line:
    0 : n : Number of tokens per line, 0 for no new lines

    Exit Codes:
    0  : Completed
    1  : Aborted
    2  : Input Missing
    3  : Output Missing
    63 : Memory Error at init
    
