#include "../include/production.hpp"

using namespace asparserations;
using namespace grammar;

Production::Production(Nonterminal& nt,
		       const std::vector<const Symbol*>& symbols)
  : _nonterminal(nt), _symbols(symbols) {}

const std::vector<const Symbol*>& Production::symbols() const
{
  return _symbols;
}
