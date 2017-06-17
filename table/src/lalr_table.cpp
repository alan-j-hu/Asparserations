#include "../../grammar/include/grammar.hpp"
#include "../include/lalr_table.hpp"
#include "../include/item_set.hpp"
#include <map>
#include <utility>

using namespace asparserations;
using namespace grammar;
using namespace table;

LALR_Table::LALR_Table(Grammar& grammar)
{
  std::map<Item_Set,State*,Item_Set::Compare_Cores> item_sets;  
  std::list<std::pair<const Item_Set,State*>*> queue;

  grammar.compute_first_sets();

  Item_Set start_set({Item(grammar.accept().production_at("$root"),
			   0, grammar.end())});
  _states.emplace_back(0);
  item_sets.insert(std::make_pair(start_set, &_states.back()));
  queue.push_back(&*item_sets.begin());

  unsigned int index = 1;
  //For each element in the queue of item sets...
  for(auto& elem : queue) {
    auto transitions = _goto(_closure(elem->first));
    //For each transition...
    for(auto& transition : transitions) {
      if(!transition.second.first.empty()) {
	//Try to add the new item set and state pair to the map
	Item_Set new_item_set(transition.second.first);
	_states.emplace_back(index);
        auto result = item_sets.insert(std::make_pair(new_item_set,
						      &_states.back()));

        //If the item set doesn't already exist, queue it for processing
        if(result.second) {
	  queue.push_back(&*result.first);
	  ++index;
        } else {
	  bool was_merged = new_item_set.merge(result.first->first);
	  //If the cores were the same, but the items sets were not:
	  if(was_merged) {
	    State* state = result.first->second;
	    item_sets.erase(result.first);
	    queue.push_back(&*item_sets.insert
			    (std::make_pair(new_item_set, state)).first);
	  }
	  _states.pop_back();
	}
        elem->second->add_transition(transition.first, result.first->second);
      }
      elem->second->add_reductions(transition.first, transition.second.second);
    }
  }
}

const std::list<State>& LALR_Table::states() const
{
  return _states;
}
