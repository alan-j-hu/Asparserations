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
  | Prime_Identifier Symbol_List # explicit_tok
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

#include "../include/grammar_syntax.hpp"
#include "../../grammar/include/production.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/token.hpp"
#include <iostream>

using namespace asparserations;
using namespace grammar;

std::unique_ptr<Grammar> asparserations::bootstrap::grammar_syntax()
{
  std::unique_ptr<Grammar> grammar(new Grammar("Root"));
  //Tokens
  Token& bar = grammar->add_token("Bar"); //|
  Token& identifier = grammar->add_token("Identifier"); //[A-Za-z_][A-Za-z0-9_]*
  Token& prime_identifier = grammar->add_token("Prime_Identifier");
  Token& colon = grammar->add_token("Colon"); //:
  Token& comma = grammar->add_token("Comma");
  Token& semicolon = grammar->add_token("Semicolon"); //;
  Token& tokens_keyword = grammar->add_token("Tokens_Keyword"); //@tokens
  Token& open_bracket = grammar->add_token("Open_Bracket"); //{
  Token& close_bracket = grammar->add_token("Close_Bracket"); //}
  Token& hash = grammar->add_token("Hash"); //#

  //Nonterminals
  Nonterminal& symbol_list = grammar->add_nonterminal("Symbol_List");
  symbol_list.add_production("main", {&identifier, &symbol_list});
  symbol_list.add_production("explicit_tok", {&prime_identifier, &symbol_list});
  symbol_list.add_production("empty", {});

  Nonterminal& production = grammar->add_nonterminal("Production");
  production.add_production("main", {&symbol_list, &hash, &identifier});

  Nonterminal& production_list = grammar->add_nonterminal("Production_List");
  production_list.add_production("recursive_case",
				 {&production, &bar, &production_list});
  production_list.add_production("base_case", {&production, &semicolon});

  Nonterminal& nonterminal = grammar->add_nonterminal("Nonterminal");
  nonterminal.add_production("main", {&identifier, &colon, &production_list});

  Nonterminal& nonterminal_list = grammar->add_nonterminal("Nonterminal_List");
  nonterminal_list.add_production("main", {&nonterminal, &nonterminal_list});
  nonterminal_list.add_production("empty", {});

  Nonterminal& identifier_list = grammar->add_nonterminal("Identifier_List");
  identifier_list.add_production("recursive_case",
				 {&identifier, &comma, &identifier_list});
  identifier_list.add_production("base_case", {&identifier});

  Nonterminal& root = grammar->start_symbol();
  root.add_production("main",
                      {&tokens_keyword, &open_bracket, &identifier_list,
 	                 &close_bracket, &nonterminal_list});

  return grammar;
}
