#include "../include/lr_table.hpp"
#include "../include/state.hpp"
#include <list>
#include <map>

using namespace asparserations;
using namespace grammar;
using namespace table;

LR_Table::LR_Table()
{
  std::list<std::pair<const Item_Set,State>*> queue;

  //For each element in the queue of item sets...
  for(auto elem : queue) {
    //Get the kernel items of its transitions
    auto transitions =  _goto(_closure(elem->first));
    //For each transition...
    for(auto transition : transitions) {
      //Try to add a new item set to the state machine
      if(!transition.second.first.empty()) {
        auto result =
	  _states.insert(std::make_pair(Item_Set(transition.second.first),
				        State()));

        //If the item set doesn't already exist, queue it for processing
        if(result.second) {
	  queue.push_back(&*result.first);
        }
        elem->second.add_transition(transition.first, &result.first->second);
      }

      //Register the reductions
      elem->second.add_reduction(transition.first, transition.second.second);
    }
  }
}

const std::map<Item_Set,State>& LR_Table::states() const
{
  return _states;
}
