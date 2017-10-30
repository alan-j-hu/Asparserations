#include "../include/grammar.hpp"
#include <utility>

using namespace asparserations;
using namespace grammar;

Grammar::NonterminalImp::NonterminalImp(Grammar& g, const std::string& name)
  : m_grammar(&g), m_name(name), m_derives_empty_string(false)
{}

const std::map<std::string,Production>&
Grammar::NonterminalImp::productions() const
{
  return m_productions;
}

Production& Grammar::NonterminalImp::add_production
(const std::string& name, std::vector<const Symbol*> symbols)
{
  return m_productions.emplace(std::piecewise_construct,
      	                       std::forward_as_tuple(name),
                               std::forward_as_tuple(*this, name, symbols))
    .first->second;
}

Production& Grammar::NonterminalImp::production_at(const std::string& name)
{
  return m_productions.at(name);
}

const Production&
Grammar::NonterminalImp::production_at(const std::string& name) const
{
  return m_productions.at(name);
}

const std::string& Grammar::NonterminalImp::name() const
{
  return m_name;
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

const std::set<const Token*>& Grammar::NonterminalImp::first_set() const
{
  return m_first_set;
}
