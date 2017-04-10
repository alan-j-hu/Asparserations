#include "../include/item_set.hpp"

using namespace asparserations;
using namespace table;

Item_Set::Item_Set(const std::set<Item>& items)
  : _items(items) {}
