#include "../include/state.hpp"
#include "../../grammar/include/production.hpp"
#include "../../grammar/include/symbol.hpp"
#include "../../grammar/include/token.hpp"
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

void State::add_reductions(
  const std::map<const Token*,std::set<const Production*>>& reductions
			   )
{
  for(auto& pair : reductions) {
    std::set<const Production*>& ref = _reductions[pair.first];
    for(const Production* p : pair.second) {
      ref.insert(p);
    }
  }
}

const std::map<const asparserations::grammar::Symbol*,const State*>&
State::transitions() const
{
  return _transitions;
}

const std::map<const asparserations::grammar::Token*,
               std::set<const asparserations::grammar::Production*>>&
State::reductions() const
{
  return _reductions;
}
