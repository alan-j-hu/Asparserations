/**
This is the grammar for the grammar file, expressed in the C++ API.

@tokens

Bar
Identifier
Colon
Semicolon
Tokens_Keyword
Rules_Keyword
Hash

@rules

Symbol_List
  : Symbol_List Identifier # main
  | # empty
  ;

Production
  : Symbol_List Hash Identifier # main
  ;

Production_List
  : Production Bar Production_List # recursive_case
  | Production # base_case
  ;

Nonterminal
  : Identifier Colon Production_List Semicolon # main
  ;

Nonterminal_List
  : Nonterminal_List Nonterminal # main
  | # empty
  ;

Identifier_List
  : Identifier Identifier_List # main
  | # empty
  ;

Root
  : Tokens_Keyword Identifier_List Rules_Keyword Nonterminal_List # main
  ;
 */

#include "../../grammar/include/grammar.hpp"

namespace asparserations {
  namespace bootstrap {
    asparserations::grammar::Grammar grammar_syntax();
  }
}
