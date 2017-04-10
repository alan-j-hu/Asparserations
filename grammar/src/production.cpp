#include "../include/production.hpp"

using namespace asparserations;
using namespace symbol;

Production::Production(const std::vector<Symbol*>& symbols)
  : _symbols(symbols) {}

const std::vector<Symbol*>& Production::symbols() const
{
  return _symbols;
}
