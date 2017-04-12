#include "../include/nonterminal.hpp"

using namespace asparserations;
using namespace grammar;

Nonterminal::Nonterminal(Grammar& g)
  : _grammar(g)
{}

bool Nonterminal::derives_empty_string() const
{
  for(const Production& p : _productions) {
    if(p.symbols().empty()) {
      return true;
    }
  }
  return false;
}

bool Nonterminal::has_empty_string_in_first_set() const
{
  return _has_empty_string_in_first_set;
}

const std::list<Production>& Nonterminal::productions() const
{
  return _productions;
}

const std::set<const Token*>& Nonterminal::first_set() const
{
  return _first_set;
}

Production& Nonterminal::add_production(std::vector<const Symbol*> symbols)
{
  _productions.emplace_back(*this, symbols);
  return _productions.back();
}
