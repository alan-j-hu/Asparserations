#include "../include/grammar.hpp"
#include "../include/production.hpp"
#include <utility>

#include<iostream>

using namespace asparserations;
using namespace grammar;

Grammar::Grammar(const std::string& start)
  : _end(*this, "__end__"), _accept(*this, "__accept__")
{
  _start_symbol = &add_nonterminal(start);
  _accept.add_production("__root__", {_start_symbol});
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
  auto result = _tokens.emplace(std::piecewise_construct,
			       std::forward_as_tuple(id),
			       std::forward_as_tuple(*this, id));
  Token& tok = result.first->second;
  if(result.second) {
    _token_vec.push_back(&tok);
  }
  return tok;
}

Nonterminal& Grammar::add_nonterminal(const std::string& id)
{
  auto result = _nonterminals
    .emplace(std::piecewise_construct, std::forward_as_tuple(id),
	     std::forward_as_tuple(*this, id));
  Nonterminal& nt = result.first->second;
  if(result.second) {
    _nonterminal_vec.push_back(&nt);
  }
  return nt;
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

const std::vector<Nonterminal*>& Grammar::nonterminals() const
{
  return _nonterminal_vec;
}

const std::vector<Token*>& Grammar::tokens() const
{
  return _token_vec;
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
	bool production_derives_empty_string = true;
	//Repeat until first set does not have empty string or end is reached
	for(const Symbol* symbol : production.symbols()) {
	  for(const Token* first : symbol->first_set()) {
	    auto result = nonterminal._first_set.insert(first);
	    if(result.second) {
	      needs_update = true;
	    }
	  }
	  if(!symbol->derives_empty_string()) {
	    production_derives_empty_string = false;
	    break;
	  }
	}
	if(production_derives_empty_string) {
	  nonterminal._derives_empty_string = true;
	}
      }
    }
  }
  //Handle accept symbol, which is not in nonterminal map
  if(_start_symbol != nullptr) {
    _accept._first_set = _start_symbol->first_set();
    if(_start_symbol->derives_empty_string()) {
      _accept._first_set.insert(&_end);
    }
  }
}
