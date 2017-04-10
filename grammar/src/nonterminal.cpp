#include "../include/nonterminal.hpp"

using namespace asparserations;
using namespace grammar;

Nonterminal::Nonterminal(const std::vector<Production>& productions)
  : _productions(productions)
{
  for(Production& production : _productions) {
    auto first = production.symbols().front()->first_set();
    for(const Token* token : first) {
      _first_set.insert(token);
    }
  }
}

const std::vector<Production>& Nonterminal::productions() const
{
  return _productions;
}

const std::set<const Token*>& Nonterminal::first_set() const
{
  return _first_set;
}
