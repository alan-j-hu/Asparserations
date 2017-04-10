#include "../include/state.hpp"
#include <utility>

using namespace asparserations;
using namespace grammar;
using namespace table;

void State::add_transition(const Symbol* const input, const State* state)
{
  _transitions.insert(std::make_pair(input, state));
}

void State::add_reduction(const Symbol* const input,
			  const std::set<const Production*>& productions)
{
  std::set<const Production*>& ref = _reductions[input];
  for(const Production* p : productions) {
    ref.insert(p);
  }
}
