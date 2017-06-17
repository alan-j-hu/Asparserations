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

#include "../include/grammar_syntax.hpp"
#include "../../grammar/include/production.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/token.hpp"

using namespace asparserations;
using namespace grammar;

Grammar asparserations::bootstrap::grammar_syntax()
{
  Grammar grammar("root");
  //Tokens
  Token& bar = grammar.add_token("Bar"); //|
  Token& identifier = grammar.add_token("Identifier"); //[A-Za-z_][A-Za-z0-9_]*
  Token& colon = grammar.add_token("Colon"); //:
  Token& semicolon = grammar.add_token("Semicolon"); //;
  Token& tokens_keyword = grammar.add_token("Tokens_Keyword"); //@tokens
  Token& rules_keyword = grammar.add_token("Rules_Keyword"); //@rules
  Token& hash = grammar.add_token("Hash"); //#

  //Nonterminals
  Nonterminal& symbol_list = grammar.add_nonterminal("Symbol_List");
  symbol_list.add_production("main", {&symbol_list, &identifier});
  symbol_list.add_production("empty", {});

  Nonterminal& production = grammar.add_nonterminal("Production");
  production.add_production("main", {&symbol_list, &hash, &identifier});

  Nonterminal& production_list = grammar.add_nonterminal("Production_List");
  production_list.add_production("recursive_case",
				 {&production, &bar, &production_list});
  production_list.add_production("base_case", {&production});

  Nonterminal& nonterminal = grammar.add_nonterminal("Nonterminal");
  nonterminal
  .add_production("main", {&identifier, &colon, &production_list, &semicolon});

  Nonterminal& nonterminal_list = grammar.add_nonterminal("Nonterminal_List");
  nonterminal_list.add_production("main", {&nonterminal_list, &nonterminal});
  nonterminal_list.add_production("empty", {});

  Nonterminal& identifier_list = grammar.add_nonterminal("Identifier_List");
  identifier_list.add_production("main", {&identifier, &identifier_list});
  identifier_list.add_production("empty", {});

  Nonterminal& root = grammar.start_symbol();
  root.add_production("main",
    {&tokens_keyword, &identifier_list, &rules_keyword, &nonterminal_list});

  //Use move semantics so productions' pointers to symbols are valid
  return grammar;
}
