#include "../include/grammar.hpp"
#include <utility>

using namespace asparserations;
using namespace grammar;

Grammar::NonterminalImp::NonterminalImp(Grammar& g, const std::string& id)
  : m_grammar(&g), m_id(id), m_derives_empty_string(false)
{}

const std::map<std::string,Production>&
Grammar::NonterminalImp::productions() const
{
  return m_productions;
}

Production& Grammar::NonterminalImp::add_production
                     (const std::string& id, std::vector<const Symbol*> symbols)
{
  return m_productions.emplace(std::piecewise_construct,
      	                       std::forward_as_tuple(id),
                               std::forward_as_tuple(*this, id, symbols)).first
    ->second;
}

Production& Grammar::NonterminalImp::production_at(const std::string& id)
{
  return m_productions.at(id);
}

const Production&
Grammar::NonterminalImp::production_at(const std::string& id) const
{
  return m_productions.at(id);
}

const std::string& Grammar::NonterminalImp::id() const
{
  return m_id;
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
