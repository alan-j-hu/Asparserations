#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/token.hpp"
#include "../include/item_set.hpp"
#include "../include/lr_table.hpp"
#include "../include/state.hpp"
#include <map>

using namespace asparserations;
using namespace grammar;
using namespace table;

LR_Table::LR_Table(Grammar& grammar)
  : m_grammar(grammar)
{
  std::list<std::pair<const Item_Set,State*>*> queue;

  grammar.compute_first_sets();

  Item_Set start_set({Item(grammar.accept().production_at("_root_"),
			   0, grammar.end())});
  m_states.emplace_back(0);
  m_item_sets.insert(std::make_pair(start_set, &m_states.back()));
  m_item_set_state_pairs.emplace_back(&m_item_sets.begin()->first,
                                      &m_states.back());
  queue.push_back(&*m_item_sets.begin());

  unsigned int index = 1;
  //For each element in the queue of item sets...
  for(auto& elem : queue) {
    //Get the kernel items of its transitions
    auto foo = gotos(closure(elem->first));
    auto& transitions = foo.first;
    auto& reductions = foo.second;
    //For each transition...
    for(auto& transition : transitions) {
      //Try to add a new item set to the state machine
      m_states.emplace_back(index);
      auto result = m_item_sets.insert(
        std::make_pair(Item_Set(transition.second), &m_states.back()));
      // If the item set doesn't already exist, queue it for processing
      if(result.second) {
        queue.push_back(&*result.first);
	m_item_set_state_pairs.emplace_back(&result.first->first,
                                            &m_states.back());
        ++index;
      } else {
        m_states.pop_back();
      }
      elem->second->add_transition(transition.first, result.first->second);
    }
    elem->second->add_reductions(reductions);
  }
}

const std::list<State>& LR_Table::states() const
{
  return m_states;
}

const Grammar& LR_Table::grammar() const
{
  return m_grammar;
}

const std::vector<std::pair<const Item_Set*,const State*>>&
LR_Table::item_set_state_pairs() const
{
  return m_item_set_state_pairs;
}
