#include "../include/grammar.hpp"
#include <utility>

using namespace asparserations;
using namespace grammar;

Grammar::NonterminalImp::NonterminalImp(Grammar& g, const std::string& name,
                                        unsigned int index)
  : m_grammar(&g), m_name(name), m_derives_empty_string(false), m_index(index)
{}

const std::list<Production>& Grammar::NonterminalImp::productions() const
{
  return m_productions;
}

Production& Grammar::NonterminalImp::add_production
(const std::string& name, std::vector<const Symbol*> symbols)
{
  m_productions.emplace_back(*this, name, symbols, m_productions.size());
  auto& production = m_productions.back();
  m_production_map.emplace(name, &production);
  return production;
}

Production& Grammar::NonterminalImp::production_at(const std::string& name)
{
  return *m_production_map.at(name);
}

const Production&
Grammar::NonterminalImp::production_at(const std::string& name) const
{
  return *m_production_map.at(name);
}

const std::string& Grammar::NonterminalImp::name() const
{
  return m_name;
}

unsigned int Grammar::NonterminalImp::index() const
{
  return m_index;
}

Grammar& Grammar::NonterminalImp::grammar()
{
  return *m_grammar;
}

const Grammar& Grammar::NonterminalImp::grammar() const
{
  return *m_grammar;
}

bool Grammar::NonterminalImp::is_token() const
{
  return false;
}

bool Grammar::NonterminalImp::derives_empty_string() const
{
  return m_derives_empty_string;
}

const std::set<std::reference_wrapper<const Token>>&
Grammar::NonterminalImp::first_set() const
{
  return m_first_set;
}

bool grammar::operator<(const Nonterminal& l, const Nonterminal& r)
{
  if(&l.grammar() < &r.grammar()) {
    return true;
  }
  if(&r.grammar() < &l.grammar()) {
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
