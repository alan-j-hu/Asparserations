#include "../include/grammar.hpp"

using namespace asparserations;
using namespace grammar;

Grammar::TokenImp::TokenImp(Grammar& g, const std::string& name,
                            unsigned int index)
  : m_grammar(&g), m_name(name), m_first_set{this}, m_index(index)
{}

const std::string& Grammar::TokenImp::name() const
{
  return m_name;
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
