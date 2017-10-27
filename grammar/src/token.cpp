#include "../include/grammar.hpp"

using namespace asparserations;
using namespace grammar;

Grammar::TokenImp::TokenImp(Grammar& g, const std::string& id)
  : m_grammar(&g), m_id(id), m_first_set{this}
{}

const std::string& Grammar::TokenImp::id() const
{
  return m_id;
}

Grammar& Grammar::TokenImp::grammar()
{
  return *m_grammar;
}

const Grammar& Grammar::TokenImp::grammar() const
{
  return *m_grammar;
}

const std::set<const Token*>& Grammar::TokenImp::first_set() const
{
  return m_first_set;
}

const std::map<std::string,Production>& Grammar::TokenImp::productions() const
{
  return m_productions;
}

bool Grammar::TokenImp::is_token() const
{
  return true;
}

bool Grammar::TokenImp::derives_empty_string() const
{
  return false;
}
