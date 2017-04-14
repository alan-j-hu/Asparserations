#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/production.hpp"
#include "../../grammar/include/token.hpp"
#include "../include/lr_table.hpp"
#include "../include/state.hpp"
#include <map>

using namespace asparserations;
using namespace grammar;
using namespace table;

LR_Table::LR_Table(Grammar& grammar, const Nonterminal& root)
{
  std::map<Item_Set,State*> item_sets;
  std::list<std::pair<const Item_Set,State*>*> queue;

  Token& empty_string = grammar.add_token();
  Nonterminal& s = grammar.add_nonterminal();
  s.add_production({&root, &empty_string});
  grammar.compute_first_sets();

  Item_Set start_set({Item(s.productions().front(), 0, empty_string)});
  _states.emplace_back();
  item_sets.insert(std::make_pair(start_set, &_states.back()));
  queue.push_back(&*item_sets.begin());

  //For each element in the queue of item sets...
  for(auto elem : queue) {
    //Get the kernel items of its transitions
    auto transitions =  _goto(_closure(elem->first));
    //For each transition...
    for(auto transition : transitions) {
      //Try to add a new item set to the state machine
      if(!transition.second.first.empty()) {
	_states.emplace_back();
        auto result = item_sets.insert(
	  std::make_pair(Item_Set(transition.second.first), &_states.back()));

        //If the item set doesn't already exist, queue it for processing
        if(result.second) {
	  queue.push_back(&*result.first);
        } else {
	  _states.pop_back();
	}
        elem->second->add_transition(transition.first, result.first->second);
      }

      //Register the reductions
      elem->second->add_reduction(transition.first, transition.second.second);
    }
  }
}

const std::list<State>& LR_Table::states() const
{
  return _states;
}
