#include "../include/token.hpp"

using namespace asparserations;
using namespace grammar;

Token::Token(Grammar& g)
  : _grammar(g), _first_set{this}
{}

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
