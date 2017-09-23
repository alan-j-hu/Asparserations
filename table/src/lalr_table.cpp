#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/token.hpp"
#include "../include/lalr_item_set.hpp"
#include "../include/lalr_table.hpp"
#include "../include/item_set.hpp"
#include <map>
#include <utility>

using namespace asparserations;
using namespace grammar;
using namespace table;

LALR_Table::LALR_Table(Grammar& grammar)
  : _grammar(grammar)
{
  _grammar.compute_first_sets();
  Item_Set start_set({Item(grammar.accept().production_at("__root__"),
			   0, grammar.dummy_lookahead())});
  _states.emplace_back(0);
  std::map<Item_Set,LALR_Item_Set> item_sets {{start_set,
	                                       LALR_Item_Set(_states.back())}};
  std::list<std::pair<const Item_Set,LALR_Item_Set>*> queue;
  unsigned int state_index = 1;
  for(auto& pair : queue) {
    for(auto& abs_item : pair->second.abstract_items()) {
      // Compute closure of each individual item seperately
      auto foo = gotos(closure(Item_Set({abs_item.item()})));
      std::map<const Symbol*,std::set<Item>>& gotos = foo.first;
      std::map<const Token*,std::set<const Production*>>& reduces = foo.second;
      for(auto& pair2 : gotos) {
	// Add a new state to the list of states, try to add a new item set,
	// if item set exists, use its own corresponding state and pop newly
	// added state
	_states.emplace_back(state_index);
        auto result = item_sets.emplace(Item_Set(pair2.second),
					LALR_Item_Set(_states.back()));
	if(!result.second) {
	  _states.pop_back();
	}

        for(const Item& item : pair2.second) {
          if(&item.lookahead == &grammar.dummy_lookahead()) {
	    // Propagated lookahead from item to new item
	    abs_item.add_child(result.first->second.add_abstract_item(item));
          } else {
	    // Spontaneous lookahead from item
	    result.first->second.add_concrete_item(item);
          }
        }
      }
      for(auto& pair2 : reduces) {
        if(pair2.first == &grammar.dummy_lookahead()) {
	  // Propagated lookahead in reduction
	  abs_item.add_reduction(pair2.second);
        } else {
 	  // Spontaneous lookahead in reduction
	  pair->second.add_reduction(pair2);
        }
      }
    }
  }
  queue.front()->second.abstract_items().front().propagate(_grammar.end());
}

const std::list<State>& LALR_Table::states() const
{
  return _states;
}

const Grammar& LALR_Table::grammar() const
{
  return _grammar;
}
