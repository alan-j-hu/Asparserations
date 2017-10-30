#include "../include/nonterminal.hpp"
#include "../include/production.hpp"
#include <stdexcept>

using namespace asparserations;
using namespace grammar;

Production::Production(Nonterminal& nt, const std::string& name,
		       const std::vector<const Symbol*>& syms, unsigned int idx)
  : m_nonterminal(nt), m_name(name), m_symbols(syms), m_index(idx)
{
  for(const Symbol* symbol : m_symbols) {
    if(symbol == nullptr) {
      throw std::invalid_argument("Symbol is null");
    }
    if(&symbol->grammar() != &m_nonterminal.grammar()) {
      throw std::invalid_argument("Symbol does not belong to same grammar");
    }
  }
}

const std::vector<const Symbol*>& Production::symbols() const
{
  return m_symbols;
}

const std::string& Production::name() const
{
  return m_name;
}

unsigned int Production::index() const
{
  return m_index;
}

Nonterminal& Production::nonterminal()
{
  return m_nonterminal;
}

const Nonterminal& Production::nonterminal() const
{
  return m_nonterminal;
}

void Production::set_symbol(unsigned int idx, const Symbol& symbol)
{
  if(&symbol.grammar() != &m_nonterminal.grammar()) {
    throw std::invalid_argument("Symbol does not belong to the same grammar");
  }
  m_symbols[idx] = &symbol;
}

void Production::insert_symbol(unsigned int idx, const Symbol& symbol)
{
  if(&symbol.grammar() != &m_nonterminal.grammar()) {
    throw std::invalid_argument("Symbol does not belong to the same grammar");
  }
  m_symbols.insert(m_symbols.begin() + idx, &symbol);
}

void Production::erase_symbol(unsigned int idx)
{
  m_symbols.erase(m_symbols.begin() + idx);
}

bool grammar::operator<(const Production& l, const Production& r)
{
  if(l.nonterminal() < r.nonterminal()) {
    return true;
  }
  if(r.nonterminal() < l.nonterminal()) {
    return false;
  }
  if(l.index() < r.index()) {
    return true;
  }
  if(r.index() < l.index()) {
    return false;
  }
  return false;
}
