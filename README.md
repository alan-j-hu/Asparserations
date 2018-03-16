# Asparserations
Asparserations is an LR(1) parser generator frontend written in C++. It outputs
JSON representing the state machine table. A C++ backend, used to generate the
grammar parser, is included in the bootstrap folder.

## Features
- Outputs JSON for consumption by backends, without needing to implement the
  [LA]LR(1) table construction algorithm
- Productions are named instead of indexed and names are local to the
  nonterminal
- Preserves shift/reduce and reduce/reduce conflicts in JSON
- Used to generate the parser for grammar files
- Can be integrated with lexers easily (the grammar file lexer is lazy/a
  generator/monadic!)

## Options
- `--version`, `-v` - Prints the software version
- `--help`, `-h` - Prints the help
- `--out`, `-o` - Specifies the output file (a.out.json by default)
- `--root`, `-r` - Specifies the root node (Root by default)
- `--lalr`, `-l` - Use LALR(1) (experimental)
- `--debug`, `-d` - Output the item sets as well as the states

## Building From Source

### Prerequisites
This project requires G++, GNU Make, and Python 3 in order to build from source.

### Installing
    $ git clone https://github.com/TheAspiringHacker/Asparserations.git
    $ cd Asparserations
    $ make install
The binary file is saved to `~/local/bin` and can be invoked by typing
`asparserations [flags...] grammar_file_name`.

### Installing with CMake
I've decided to learn CMake by using it for this project. Use it by running:

    $ git clone https://github.com/TheAspiringHacker/Asparserations.git
    $ cd Asparserations
    $ mkdir build
    $ cd build
    $ cmake ../
    $ make
The resulting binary is `build/bin/asparserations`.

You can then run CPack:

    $ cpack --config CPackConfig.cmake

(You will have to move the binary to a location in your `PATH` yourself.)

## Trying it out
Create a file called `grammar` in your favorite text editor:

    tokens {
      number,
      plus,
      minus,
      asterisk,
      slash
    }
    
    Term : Term plus Factor # add
         | Term minus Factor # sub
         | Factor # base_case
         ;
    
    Factor : Factor asterisk number # mult
           | Factor slash number # div
           | number # base_case
           ;

Run:

    $ asparserations -r Term -o table.json grammar
    $ cat table.json
