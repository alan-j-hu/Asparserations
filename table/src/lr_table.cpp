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
  : _grammar(grammar)
{
  std::map<Item_Set,State*> item_sets;
  std::list<std::pair<const Item_Set,State*>*> queue;

  grammar.compute_first_sets();

  Item_Set start_set({Item(grammar.accept().production_at("__root__"),
			   0, grammar.end())});
  _states.emplace_back(0);
  item_sets.insert(std::make_pair(start_set, &_states.back()));
  queue.push_back(&*item_sets.begin());

  unsigned int index = 1;
  //For each element in the queue of item sets...
  for(auto elem : queue) {
    //Get the kernel items of its transitions
    auto transitions =  _goto(_closure(elem->first));
    //For each transition...
    for(auto transition : transitions) {
      //Try to add a new item set to the state machine
      if(!transition.second.first.empty()) {
	_states.emplace_back(index);
        auto result = item_sets.insert(
	  std::make_pair(Item_Set(transition.second.first), &_states.back()));
        //If the item set doesn't already exist, queue it for processing
        if(result.second) {
	  queue.push_back(&*result.first);
	  ++index;
        } else {
	  _states.pop_back();
	}
        elem->second->add_transition(transition.first, result.first->second);
      }
      //Register the reductions
      elem->second->add_reductions(transition.first, transition.second.second);
    }
  }
}

const std::list<State>& LR_Table::states() const
{
  return _states;
}

const Grammar& LR_Table::grammar() const
{
  return _grammar;
}
