#include "../include/grammar.hpp"

using namespace asparserations;
using namespace grammar;

Grammar::TokenImp::TokenImp(Grammar& g, const std::string& name,
                            unsigned int index)
  : m_grammar(&g), m_name(name), m_index(index)
{
  const Token& t = *this;
  m_first_set = {std::cref(t)};
}

const std::string& Grammar::TokenImp::name() const
{
  return m_name;
}

unsigned int Grammar::TokenImp::index() const
{
  return m_index;
}

Grammar& Grammar::TokenImp::grammar()
{
  return *m_grammar;
}

const Grammar& Grammar::TokenImp::grammar() const
{
  return *m_grammar;
}

const std::set<std::reference_wrapper<const Token>>&
Grammar::TokenImp::first_set() const
{
  return m_first_set;
}

const std::list<Production>& Grammar::TokenImp::productions() const
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

bool grammar::operator<(const Token& l, const Token& r)
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
