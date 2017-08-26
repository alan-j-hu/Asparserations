#include "../include/item.hpp"
#include "../../grammar/include/production.hpp"

using namespace asparserations;
using namespace grammar;
using namespace table;

Item::Item(const Production& p, unsigned short int m, const Token& l)
  : production(p), marker(m), lookahead(l) {}

const Symbol* Item::next() const
{
  return production.symbols().at(marker);
}

const Symbol* Item::peek() const
{
  return production.symbols().at(marker + 1);
}

bool Item::compare_cores(const Item& rhs) const
{
  if(marker < rhs.marker) {
    return true;
  }
  if(rhs.marker < marker) {
    return false;
  }
  if(&production < &rhs.production) {
    return true;
  }
  if(&rhs.production < &production) {
    return false;
  }
  return false;
}

bool table::operator<(const Item& lhs, const Item& rhs)
{
  if(lhs.marker < rhs.marker) {
    return true;
  }
  if(rhs.marker < lhs.marker) {
    return false;
  }
  if(&lhs.production < &rhs.production) {
    return true;
  }
  if(&rhs.production < &lhs.production) {
    return false;
  }
  if(&lhs.lookahead < &rhs.lookahead) {
    return true;
  }
  if(&rhs.lookahead < &lhs.lookahead) {
    return false;
  }
  return false;
}
