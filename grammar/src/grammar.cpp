#include "../include/grammar.hpp"

using namespace asparserations;
using namespace grammar;

Token& Grammar::add_token()
{
  _tokens.emplace_back();
  return _tokens.back();
}

Nonterminal& Grammar::add_nonterminal()
{
  _nonterminals.emplace_back(*this);
  return _nonterminals.back();
}

void Grammar::compute_first_set()
{
  bool needs_update = true;
  while(needs_update) {
    needs_update = false;
    for(Nonterminal& nonterminal : _nonterminals) {
      for(const Production& production : nonterminal.productions()) {
        nonterminal._has_empty_string_in_first_set = true;
	//Repeat until first set does not have empty string or end is reached
	for(const Symbol* symbol : production.symbols()) {
	  for(const Token* first : symbol->first_set()) {
	    auto result = nonterminal._first_set.insert(first);
	    if(result.second) {
	      needs_update = true;
	    }
	  }
	  if(!symbol->has_empty_string_in_first_set()) {
	    nonterminal._has_empty_string_in_first_set = false;
	    break;
	  }
	}
      }
    }
  }
}
