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
  : Symbol_List Comma Identifier # recursive_case
  | Identifier # base_case
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
