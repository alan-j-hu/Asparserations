# Asparserations
Asparserations is an LR(1) parser generator frontend written in C++. It outputs
JSON representing the state machine table. A C++ backend, used to generate the
grammar parser, is included in the bootstrap folder.

## Features
- Outputs JSON for consumption by backends, without needing to implement the
  [LA]LR(1) table construction algorithm
- Productions are named instead of indexed and names are local to the
  nonterminal
- Used to generate the parser for grammar files
- Can be integrated with lexers easily (the grammar file lexer is lazy/a
  generator/monadic!)

## Options
- `--out`, `-o` - Specifies the output file (a.out.json by default)
- `--root`, `-r` - Specifies the root node (Root by default)
- `--lalr`, `-l` - Use LALR(1) (experimental)

## Building From Source

### Prerequisites
This project requires G++, GNU Make, and Python 3 in order to build from source.

### Installing
    $ git clone https://github.com/TheAspiringHacker/Asparserations.git
    $ cd Asparserations
    $ make install
The binary file is saved to `~/local/bin` and can be invoked by typing
`asparserations [flags...] grammar_file_name`.
