#include "../include/item_core.hpp"

using namespace asparserations;
using namespace grammar;
using namespace table;

Item_Core::Item_Core(const Production& p, unsigned short int m)
  : production(p), marker(m) {}

const Symbol* Item_Core::next() const
{
  return production.symbols().at(marker);
}

const Symbol* Item_Core::peek() const
{
  return production.symbols().at(marker + 1);
}
