# Asparserations
Asparserations is an LR(1) parser generator frontend written in C++. It outputs JSON representing the state machine table. A C++ backend, used to generate the grammar parser, is included in the bootstrap folder.

## Prerequisites
This project requires G++, GNU Make, and Python 3.

## Installing
    make
    make install
The binary file is saved to `~/local/bin` cand can be invoked by typing `asparserations [flags...] grammar_file_name`.

## Options
* `--out`, `-o` - Specifies the output file (a.out.json by default)
* `--root`, `-r` - Specifies the root node (Root by default)
* `--lalr`, `-l` - Use LALR(1) (not yet supported)
