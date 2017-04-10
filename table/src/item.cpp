#include "../include/item.hpp"

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

bool table::operator<(const Item& lhs, const Item& rhs)
{
  if(lhs.marker < rhs.marker) {
    return true;
  }
  if(&lhs.lookahead < &rhs.lookahead) {
    return true;
  }
  if(&lhs.production < &rhs.production) {
    return true;
  }
  return false;
}
