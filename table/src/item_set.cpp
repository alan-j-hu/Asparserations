#include "../include/item_set.hpp"

using namespace asparserations;
using namespace table;

Item_Set::Item_Set(const std::set<Item>& items)
  : _items(items) {}

bool table::operator<(const Item_Set& lhs, const Item_Set& rhs)
{
  if(lhs._items < rhs._items) {
    return true;
  }
  return false;
}
