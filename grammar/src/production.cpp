#include "../include/nonterminal.hpp"
#include "../include/production.hpp"
#include <stdexcept>

using namespace asparserations;
using namespace grammar;

Production::Production(Nonterminal& nt, const std::string& id,
		       const std::vector<const Symbol*>& syms)
  : _nonterminal(nt), _id(id), _symbols(syms)
{
  for(const Symbol* symbol : _symbols) {
    if(symbol == nullptr) {
      throw std::invalid_argument("Symbol is null");
    }
    if(&symbol->grammar() != &_nonterminal.grammar()) {
      throw std::invalid_argument("Symbol does not belong to same grammar");
    }
  }
}

const std::vector<const Symbol*>& Production::symbols() const
{
  return _symbols;
}

const std::string& Production::id() const
{
  return _id;
}

Nonterminal& Production::nonterminal()
{
  return _nonterminal;
}

const Nonterminal& Production::nonterminal() const
{
  return _nonterminal;
}

void Production::set_symbol(unsigned int idx, const Symbol* symbol)
{
  if(symbol == nullptr) {
    throw std::invalid_argument("Symbol is null");
  }
  if(&symbol->grammar() != &_nonterminal.grammar()) {
    throw std::invalid_argument("Symbol does not belong to the same grammar");
  }
  _symbols[idx] = symbol;
}

void Production::insert_symbol(unsigned int idx, const Symbol* symbol)
{
  if(&symbol->grammar() != &_nonterminal.grammar()) {
    throw std::invalid_argument("Symbol does not belong to the same grammar");
  }
  _symbols.insert(_symbols.begin() + idx, symbol);
}

void Production::erase_symbol(unsigned int idx)
{
  _symbols.erase(_symbols.begin() + idx);
}
