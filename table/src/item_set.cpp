#include "../include/item_set.hpp"

using namespace asparserations;
using namespace table;

Item_Set::Item_Set(const std::set<Item>& items)
  : _items(items) {}

const std::set<Item>& Item_Set::items() const
{
  return _items;
}

void Item_Set::insert(const Item& item)
{
  _items.insert(item);
}

bool Item_Set::merge(const Item_Set& item_set)
{
  bool was_merged = false;
  bool foo;

  for(const Item& item : item_set._items) {
    foo = _items.insert(item).second;
    if(!was_merged) {
      was_merged = foo;
    }
  }
  return was_merged;
}

bool table::operator<(const Item_Set& lhs, const Item_Set& rhs)
{
  if(lhs._items < rhs._items) {
    return true;
  }
  return false;
}
