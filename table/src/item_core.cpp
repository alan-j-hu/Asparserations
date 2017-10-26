#include "../include/item_core.hpp"
#include "../../grammar/include/production.hpp"

using namespace asparserations;
using namespace grammar;
using namespace table;

Item_Core::Item_Core(const Production& p, unsigned short int m)
  : production(p), marker(m) {}

bool table::operator<(const Item_Core& lhs, const Item_Core& rhs)
{
  if(&lhs.production < &rhs.production) {
    return true;
  }
  if(&rhs.production < &lhs.production) {
    return false;
  }
  if(lhs.marker < rhs.marker) {
    return true;
  }
  if(rhs.marker < lhs.marker) {
    return false;
  }
  return false;
}
