#include "../include/grammar.hpp"
#include "../include/production.hpp"

using namespace asparserations;
using namespace grammar;

Grammar::TokenImp::TokenImp(Grammar& g, const std::string& id)
  : Token(g, id) {}

Grammar::NonterminalImp::NonterminalImp(Grammar& g, const std::string& id)
  : Nonterminal(g, id) {}

bool Grammar::NonterminalImp::has_empty_string_in_first_set() const
{
  return _has_empty_string_in_first_set;
}

const std::set<const Token*>& Grammar::NonterminalImp::first_set() const
{
  return _first_set;
}

Grammar::Grammar()
  : _start_symbol(nullptr) {}

Token& Grammar::add_token(const std::string& id)
{
  _tokens.emplace_back(*this, id);
  return _tokens.back();
}

Nonterminal& Grammar::add_nonterminal(const std::string& id)
{
  _nonterminals.emplace_back(*this, id);
  return _nonterminals.back();
}

Nonterminal* Grammar::start_symbol()
{
  return _start_symbol;
}

const Nonterminal* Grammar::start_symbol() const
{
  return _start_symbol;
}

void Grammar::set_start_symbol(Nonterminal* start)
{
  if(start != nullptr && &start->grammar() == this) {
    _start_symbol = start;
  }
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
