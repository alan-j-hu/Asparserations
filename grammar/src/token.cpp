#include "../include/token.hpp"

using namespace asparserations;
using namespace grammar;

Token::Token(Grammar& g, const std::string& id)
  : _grammar(g), _id(id), _first_set{this}
{}

const std::string& Token::id() const
{
  return _id;
}

Grammar& Token::grammar()
{
  return _grammar;
}

const Grammar& Token::grammar() const
{
  return _grammar;
}

const std::set<const Token*>& Token::first_set() const
{
  return _first_set;
}

const std::list<Production>& Token::productions() const
{
  return _productions;
}

bool Token::derives_empty_string() const
{
  return false;
}

bool Token::has_empty_string_in_first_set() const
{
  return false;
}
