#include "../include/token.hpp"

using namespace asparserations;
using namespace grammar;

Token::Token()
  : _first_set{this}
{}

const std::set<const Token*>& Token::first_set() const
{
  return _first_set;
}

const std::vector<Production>& Token::productions() const
{
  return _productions;
}
