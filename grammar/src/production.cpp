#include "../include/nonterminal.hpp"
#include "../include/production.hpp"
#include <stdexcept>

using namespace asparserations;
using namespace grammar;

Production::Production(Nonterminal& nt, const std::vector<const Symbol*>& syms)
  : _nonterminal(nt), _symbols(syms)
{
  for(const Symbol* symbol : _symbols) {
    if(&symbol->grammar() != &_nonterminal.grammar()) {
      throw std::invalid_argument("Symbol does not belong to same grammar");
    }
  }
}

const std::vector<const Symbol*>& Production::symbols() const
{
  return _symbols;
}
