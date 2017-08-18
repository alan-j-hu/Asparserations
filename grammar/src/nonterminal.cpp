#include "../include/grammar.hpp"
#include <utility>

using namespace asparserations;
using namespace grammar;

Grammar::NonterminalImp::NonterminalImp(Grammar& g, const std::string& id)
  : _grammar(&g), _id(id), _derives_empty_string(false)
{}

const std::map<std::string,Production>&
Grammar::NonterminalImp::productions() const
{
  return _productions;
}

Production& Grammar::NonterminalImp::add_production
                     (const std::string& id, std::vector<const Symbol*> symbols)
{
  return _productions.emplace(std::piecewise_construct,
			      std::forward_as_tuple(id),
		              std::forward_as_tuple(*this, id, symbols)).first
    ->second;
}

Production& Grammar::NonterminalImp::production_at(const std::string& id)
{
  return _productions.at(id);
}

const Production&
Grammar::NonterminalImp::production_at(const std::string& id) const
{
  return _productions.at(id);
}

const std::string& Grammar::NonterminalImp::id() const
{
  return _id;
}

Grammar& Grammar::NonterminalImp::grammar()
{
  return *_grammar;
}

const Grammar& Grammar::NonterminalImp::grammar() const
{
  return *_grammar;
}

bool Grammar::NonterminalImp::is_token() const
{
  return false;
}

bool Grammar::NonterminalImp::derives_empty_string() const
{
  return _derives_empty_string;
}

const std::set<const Token*>& Grammar::NonterminalImp::first_set() const
{
  return _first_set;
}
