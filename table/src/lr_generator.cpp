#include "../include/lr_generator.hpp"
#include "../include/state.hpp"
#include <list>
#include <map>

using namespace asparserations;
using namespace grammar;
using namespace table;

LR_Generator::LR_Generator()
{
  std::map<Item_Set,State> ret;
  std::list<std::pair<Item_Set,State>*> queue;

  for(auto elem : queue) {
    auto transitions =  _goto(_closure(elem->first));
    for(auto transition : transitions) {
      auto result = ret.insert(std::make_pair(Item_Set(transition.second),
					      State()));
      if(result.second) {
	queue.push_back(&*result.first);
      }
      queue->second.add_transition(transition.first, &*result.first);
    }
  }
}
