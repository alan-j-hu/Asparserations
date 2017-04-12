#include "../include/item_set.hpp"

using namespace asparserations;
using namespace table;

Item_Set::Item_Set(const std::set<Item>& items)
  : _items(items) {}

const std::set<Item>& Item_Set::items() const
{
  return _items;
}

bool Item_Set::compare_cores(const Item_Set& item_set) const
{
  auto iter1 = _items.begin();
  auto iter2 = item_set._items.begin();
  while(iter1 != _items.end() && iter2 != item_set._items.end()) {
    if(iter1 == _items.end()) {
      return false;
    }
    if(iter2 == item_set._items.end()) {
      return true;
    }
    if(iter1->compare_cores(*iter2)) {
      return true;
    }

    auto iter3 = iter1;
    //Skip all of the items with identical cores
    while(iter1 != _items.end() && !iter3->compare_cores(*iter1)) {
      ++iter1;
    }

    //Skip all of the items with identical cores
    auto iter4 = iter2;
    while(iter2 != item_set._items.end() && !iter4->compare_cores(*iter2)) {
      ++iter2;
    }
  }
  return false;
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

bool Item_Set::Compare_Cores::operator()(const Item_Set& lhs,
					 const Item_Set& rhs) const
{
  return lhs.compare_cores(rhs);
}
