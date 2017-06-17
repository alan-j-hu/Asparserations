#include "../include/state.hpp"
#include "../../grammar/include/production.hpp"
#include "../../grammar/include/symbol.hpp"
#include <utility>

using namespace asparserations;
using namespace grammar;
using namespace table;

State::State(unsigned int index)
  : _index(index) {}

unsigned int State::index() const
{
  return _index;
}

void State::add_transition(const Symbol* const input, const State* state)
{
  _transitions.insert(std::make_pair(input, state));
}

void State::add_reductions(const Symbol* const input,
			  const std::set<const Production*>& productions)
{
  std::set<const Production*>& ref = _reductions[input];
  for(const Production* p : productions) {
    ref.insert(p);
  }
}

const std::map<const asparserations::grammar::Symbol*,const State*>&
State::transitions() const
{
  return _transitions;
}

const std::map<const asparserations::grammar::Symbol*,
               std::set<const asparserations::grammar::Production*>>&
State::reductions() const
{
  return _reductions;
}
