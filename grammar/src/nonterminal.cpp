#include "../include/nonterminal.hpp"

using namespace asparserations;
using namespace grammar;

Nonterminal::Nonterminal(Grammar& g)
  : _grammar(g)
{}

Grammar& Nonterminal::grammar()
{
  return _grammar;
}

const Grammar& Nonterminal::grammar() const
{
  return _grammar;
}

bool Nonterminal::derives_empty_string() const
{
  for(const Production& p : _productions) {
    if(p.symbols().empty()) {
      return true;
    }
  }
  return false;
}

const std::list<Production>& Nonterminal::productions() const
{
  return _productions;
}

Production& Nonterminal::add_production(std::vector<const Symbol*> symbols)
{
  _productions.emplace_back(*this, symbols);
  return _productions.back();
}
