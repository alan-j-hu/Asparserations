#include "../include/grammar.hpp"
#include "../include/production.hpp"
#include <utility>

#include<iostream>

using namespace asparserations;
using namespace grammar;

Grammar::Grammar(const std::string& start)
  : m_end(*this, "_end_"), m_accept(*this, "_accept_")
{
  m_start_symbol = &add_nonterminal(start);
  m_accept.add_production("_root_", {m_start_symbol});
}

Grammar::Grammar(Grammar&& old)
  : m_tokens(std::move(old.m_tokens)),
    m_nonterminals(std::move(old.m_nonterminals)),
    m_start_symbol(std::move(old.m_start_symbol)),
    m_end(std::move(old.m_end)),
    m_accept(std::move(old.m_accept))
{
  for(auto& pair : m_tokens) {
    pair.second.m_grammar = this;
  }
  for(auto& pair : m_nonterminals) {
    pair.second.m_grammar = this;
  }
}

Token& Grammar::add_token(const std::string& name)
{
  auto result = m_tokens.emplace(std::piecewise_construct,
		                 std::forward_as_tuple(name),
			         std::forward_as_tuple(*this, name));
  Token& tok = result.first->second;
  if(result.second) {
    m_token_vec.push_back(&tok);
  }
  return tok;
}

Nonterminal& Grammar::add_nonterminal(const std::string& name)
{
  auto result = m_nonterminals
    .emplace(std::piecewise_construct, std::forward_as_tuple(name),
	     std::forward_as_tuple(*this, name));
  Nonterminal& nt = result.first->second;
  if(result.second) {
    m_nonterminal_vec.push_back(&nt);
  }
  return nt;
}

Token& Grammar::token_at(const std::string& name)
{
  return m_tokens.at(name);
}

const Token& Grammar::token_at(const std::string& name) const
{
  return m_tokens.at(name);
}

Nonterminal& Grammar::nonterminal_at(const std::string& name)
{
  return m_nonterminals.at(name);
}

const Nonterminal& Grammar::nonterminal_at(const std::string& name) const
{
  return m_nonterminals.at(name);
}

const std::vector<Nonterminal*>& Grammar::nonterminals() const
{
  return m_nonterminal_vec;
}

const std::vector<Token*>& Grammar::tokens() const
{
  return m_token_vec;
}

const Nonterminal& Grammar::accept() const
{
  return m_accept;
}

const Token& Grammar::end() const
{
  return m_end;
}

Nonterminal& Grammar::start_symbol()
{
  return *m_start_symbol;
}

const Nonterminal& Grammar::start_symbol() const
{
  return *m_start_symbol;
}

void Grammar::set_start_symbol(Nonterminal* start)
{
  if(start != nullptr && &start->grammar() == this) {
    m_start_symbol = start;
    m_accept.production_at("_root_").set_symbol(0, start);
  }
}

void Grammar::compute_first_sets()
{
  bool needs_update = true;
  while(needs_update) {
    needs_update = false;
    for(auto& pair : m_nonterminals) {
      Grammar::NonterminalImp& nonterminal = pair.second;
      for(const auto& elem : nonterminal.productions()) {
	const Production& production = elem.second;
	bool production_derives_empty_string = true;
	//Repeat until first set does not have empty string or end is reached
	for(const Symbol* symbol : production.symbols()) {
	  for(const Token* first : symbol->first_set()) {
	    auto result = nonterminal.m_first_set.insert(first);
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
	  nonterminal.m_derives_empty_string = true;
	}
      }
    }
  }
  //Handle accept symbol, which is not in nonterminal map
  if(m_start_symbol != nullptr) {
    m_accept.m_first_set = m_start_symbol->first_set();
    if(m_start_symbol->derives_empty_string()) {
      m_accept.m_first_set.insert(&m_end);
    }
  }
}
