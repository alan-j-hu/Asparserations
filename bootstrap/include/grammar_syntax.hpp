/**
This is the grammar for the grammar file, expressed in the C++ API.

tokens {
  Bar,
  Identifier,
  Colon,
  Comma,
  Semicolon,
  Tokens_Keyword,
  Open_Bracket,
  Close_Bracket,
  Hash
}

Symbol_List
  : Identifier Symbol_List # main
  | # empty
  ;

Production
  : Symbol_List Hash Identifier # main
  ;

Production_List
  : Production Bar Production_List # recursive_case
  | Production Semicolon # base_case
  ;

Nonterminal
  : Identifier Colon Production_List # main
  ;

Nonterminal_List
  : Nonterminal Nonterminal_List # main
  | # empty
  ;

Identifier_List
  : Identifier Comma Identifier_List # recursive
  | Identifier # base
  ;

Root
  : Tokens_Keyword Open_Bracket Identifier_List Close_Bracket Nonterminal_List
      # main
  ;
 */

#include "../../grammar/include/grammar.hpp"

namespace asparserations {
  namespace bootstrap {
    asparserations::grammar::Grammar grammar_syntax();
  }
}
