#include "../include/grammar.hpp"
#include "../include/production.hpp"
#include <utility>

#include<iostream>

using namespace asparserations;
using namespace grammar;

Grammar::Grammar(const std::string& start)
  : _end(*this,"$end"), _accept(*this,"$accept")
{
  _start_symbol = &add_nonterminal(start);
  _accept.add_production("$root", {_start_symbol, &_end});
}

Grammar::Grammar(Grammar&& old)
  : _tokens(std::move(old._tokens)),
    _nonterminals(std::move(old._nonterminals)),
    _start_symbol(std::move(old._start_symbol)),
    _end(std::move(old._end)),
    _accept(std::move(old._accept))
{
  for(auto& pair : _tokens) {
    pair.second._grammar = this;
  }
  for(auto& pair : _nonterminals) {
    pair.second._grammar = this;
  }
}

Token& Grammar::add_token(const std::string& id)
{
  return _tokens.emplace(std::piecewise_construct,
			 std::forward_as_tuple(id),
			 std::forward_as_tuple(*this, id)).first->second;
}

Nonterminal& Grammar::add_nonterminal(const std::string& id)
{
  return _nonterminals.emplace(std::piecewise_construct,
			       std::forward_as_tuple(id),
			       std::forward_as_tuple(*this, id)).first->second;
}

Token& Grammar::token_at(const std::string& id)
{
  return _tokens.at(id);
}

const Token& Grammar::token_at(const std::string& id) const
{
  return _tokens.at(id);
}

Nonterminal& Grammar::nonterminal_at(const std::string& id)
{
  return _nonterminals.at(id);
}

const Nonterminal& Grammar::nonterminal_at(const std::string& id) const
{
  return _nonterminals.at(id);
}

const Nonterminal& Grammar::accept() const
{
  return _accept;
}

const Token& Grammar::end() const
{
  return _end;
}

Nonterminal& Grammar::start_symbol()
{
  return *_start_symbol;
}

const Nonterminal& Grammar::start_symbol() const
{
  return *_start_symbol;
}

void Grammar::set_start_symbol(Nonterminal* start)
{
  if(start != nullptr && &start->grammar() == this) {
    _start_symbol = start;
    _accept.production_at("$root").set_symbol(0, start);
  }
}

void Grammar::compute_first_sets()
{
  bool needs_update = true;
  while(needs_update) {
    needs_update = false;
    for(auto& pair : _nonterminals) {
      Grammar::NonterminalImp& nonterminal = pair.second;
      for(const auto& elem : nonterminal.productions()) {
	const Production& production = elem.second;
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
  //Handle accept symbol, which is not in nonterminal map
  if(_start_symbol != nullptr) {
    _accept._first_set = _start_symbol->first_set();
    if(_start_symbol->has_empty_string_in_first_set()) {
      _accept._first_set.insert(&_end);
    }
  }
}
