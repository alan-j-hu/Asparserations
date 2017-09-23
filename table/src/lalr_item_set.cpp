#include "../include/lalr_item_set.hpp"
#include "../table/include/table.hpp"

namespace asparserations;
namespace grammar;
namespace table;

const Item& LALR_Item_Set::Abstract_Item::item()
{
  return _item;
}

void LALR_Item_Set::Abstract_Item::add_child(Abstract_Item& item)
{
  _children.insert(item);
}

void LALR_Item_Set::Abstract_Item::add_reduction(const Production& production)
{
  _reductions.push_back(&production);
}

void LALR_Item_Set::Abstract_Item::propagate(const Token& token)
{
  for(const Abstract_Item& abs_item : _children) {
  }
}

LALR_Item_Set::Abstract_Item::Abstract_Item(const Item& item,
					    LALR_Item_Set& set)
  : _item(item), _owner(set) {}

LALR_Item_Set::LALR_Item_Set(State& state)
  : _state(state)
{
}

Abstract_Item& LALR_Item_Set::add_abstract_item(const Item& item)
{
  return *_abstract_items.emplace().first;
}

void LALR_Item_Set::add_concrete_item(const Item& item)
{
  _item_set.insert(item);
}
