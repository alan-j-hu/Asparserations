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
  : _grammar(grammar)
{
  _grammar.compute_first_sets();
  _states.emplace_back(0);
  std::map<std::set<Item_Core>,LALR_State> _item_sets {
    {
      {
        Item_Core { grammar.accept().production_at("__root__"), 0 }
      },
        LALR_State(_states.back(),
		   Item_Set({ Item(grammar.accept().production_at("__root__"),
				   0, grammar.end())}))
    }
  };
  std::list<std::pair<const std::set<Item_Core>,LALR_State>*> queue;
  queue.push_back(&*_item_sets.begin());
  unsigned int index = 1;
  for(auto& pair : queue) {
    auto foo = gotos(closure(pair->second.item_set()));
    auto& transitions = foo.first;
    auto& reductions = foo.second;
    for(auto& transition : transitions) {
      _states.emplace_back(index);
      std::set<Item_Core> s;
      for(const Item& i : transition.second) {
        s.emplace(i.production, i.marker);
      }

      /*
        -Attempt to emplace new item set and state in the set of item sets
        -If the emplacement was successful, then LALR_State's item set is empty
        -If an item set with the sane item cores already exists, then the
         LALR_State's item set already has items
        -transition.second is the set of items of the new item set
        -try to merge them with LALR_State's item set
       */
      auto result = _item_sets.emplace(s, LALR_State(_states.back()));
      auto result2 = result.first->second.merge(transition.second);
      if(result.second) {
        ++index;
      } else {
        _states.pop_back();
      }
      if(result2) {
        queue.push_back(&*result.first);
      }
      pair->second.state().add_transition(transition.first,
       	                                  &result.first->second.state());
    }
    pair->second.state().add_reductions(reductions);
    //queue.pop_front();
  }
}

const std::list<State>& LALR_Table::states() const
{
  return _states;
}

const Grammar& LALR_Table::grammar() const
{
  return _grammar;
}
