#include "../include/grammar.hpp"

using namespace asparserations;
using namespace grammar;

Grammar::TokenImp::TokenImp(Grammar& g, const std::string& id)
  : _grammar(&g), _id(id), _first_set{this}
{}

const std::string& Grammar::TokenImp::id() const
{
  return _id;
}

Grammar& Grammar::TokenImp::grammar()
{
  return *_grammar;
}

const Grammar& Grammar::TokenImp::grammar() const
{
  return *_grammar;
}

const std::set<const Token*>& Grammar::TokenImp::first_set() const
{
  return _first_set;
}

const std::map<std::string,Production>& Grammar::TokenImp::productions() const
{
  return _productions;
}

bool Grammar::TokenImp::is_token() const
{
  return true;
}

bool Grammar::TokenImp::derives_empty_string() const
{
  return false;
}
