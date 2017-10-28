#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/token.hpp"
#include "../include/lalr_table.hpp"
#include "../include/lalr_state.hpp"
#include "../include/item_core.hpp"
#include "../include/item_set.hpp"
#include <map>
#include <set>
#include <utility>

using namespace asparserations;
using namespace grammar;
using namespace table;

LALR_Table::LALR_Table(Grammar& grammar)
  : m_grammar(grammar)
{
  m_grammar.compute_first_sets();
  m_states.emplace_back(0);
  m_item_sets = {
    {
      {
        Item_Core { grammar.accept().production_at("_root_"), 0 }
      },
        LALR_State(m_states.back(),
                   Item_Set({ Item(grammar.accept().production_at("_root_"),
                                   0, grammar.end())}))
    }
  };
  std::list<std::pair<const std::set<Item_Core>,LALR_State>*> queue;
  m_item_set_state_pairs.emplace_back(&m_item_sets.begin()->second.item_set(),
                                      &m_states.back());
  queue.push_back(&*m_item_sets.begin());
  unsigned int index = 1;
  for(auto& pair : queue) {
    auto foo = gotos(closure(pair->second.item_set()));
    auto& transitions = foo.first;
    auto& reductions = foo.second;
    for(auto& transition : transitions) {
      m_states.emplace_back(index);
      std::set<Item_Core> s;
      for(const Item& i : transition.second) {
        s.emplace(i.production, i.marker);
      }

      /*
        -Attempt to emplace new item set and state in the set of item sets
        -If the emplacement was successful, then LALR_State's item set is empty
        -If an item set with the same item cores already exists, then the
         LALR_State's item set already has items
        -transition.second is the set of items of the new item set
        -try to merge them with LALR_State's item set
       */
      auto result = m_item_sets.emplace(s, LALR_State(m_states.back()));
      auto result2 = result.first->second.merge(transition.second);
      if(result.second) {
        ++index;
      } else {
        m_states.pop_back();
      }
      if(result2) {
        queue.push_back(&*result.first);
	m_item_set_state_pairs.emplace_back(&result.first->second.item_set(),
                                            &result.first->second.state());
      }
      pair->second.state().add_transition(transition.first,
       	                                  &result.first->second.state());
    }
    pair->second.state().add_reductions(reductions);
  }
}

const std::list<State>& LALR_Table::states() const
{
  return m_states;
}

const Grammar& LALR_Table::grammar() const
{
  return m_grammar;
}

const std::vector<std::pair<const Item_Set*,const State*>>&
LALR_Table::item_set_state_pairs() const
{
  return m_item_set_state_pairs;
}
