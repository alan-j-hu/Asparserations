#include "../include/grammar.hpp"
#include "../include/token.hpp"

using namespace asparserations;
using namespace grammar;

struct Grammar::TokenImp : public Token
{
  TokenImp(Grammar& g)
    : Token(g) {}
};

struct Grammar::NonterminalImp : public Nonterminal
{
  NonterminalImp(Grammar& g)
    : Nonterminal(g) {}

  bool _has_empty_string_in_first_set;
  bool has_empty_string_in_first_set() const
  {
    return _has_empty_string_in_first_set;
  }

  std::set<const Token*> _first_set;
  const std::set<const Token*>& first_set() const
  {
    return _first_set;
  }
};

Token& Grammar::add_token()
{
  _tokens.emplace_back(*this);
  return _tokens.back();
}

Nonterminal& Grammar::add_nonterminal()
{
  _nonterminals.emplace_back(*this);
  return _nonterminals.back();
}

void Grammar::compute_first_sets()
{
  bool needs_update = true;
  while(needs_update) {
    needs_update = false;
    for(Grammar::NonterminalImp& nonterminal : _nonterminals) {
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
