#include "../include/lalr_table.hpp"
#include "../include/item_set.hpp"
#include <map>
#include <utility>

using namespace asparserations;
using namespace grammar;
using namespace table;

LALR_Table::LALR_Table()
{
  std::map<Item_Set,State*,Item_Set::Compare_Cores> item_sets;  
  std::list<std::pair<const Item_Set,State*>*> queue;

  unsigned int index;
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
    }
  }
}

const std::list<State>& LALR_Table::states() const
{
  return _states;
}
