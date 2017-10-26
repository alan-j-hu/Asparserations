#include "../include/lalr_state.hpp"
#include <set>

using namespace asparserations;
using namespace grammar;
using namespace table;

LALR_State::LALR_State(State& state)
  : m_item_set({}), m_state(state)
{
}

LALR_State::LALR_State(State& state, const Item_Set& item_set)
  : m_item_set(item_set), m_state(state)
{
}

const Item_Set& LALR_State::item_set() const
{
  return m_item_set;
}

State& LALR_State::state()
{
  return m_state;
}

bool LALR_State::merge(const std::set<Item>& items)
{
  return m_item_set.merge(Item_Set(items));
}
