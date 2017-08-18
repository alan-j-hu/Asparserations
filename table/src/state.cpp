#include "../include/state.hpp"
#include "../../grammar/include/nonterminal.hpp"
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
  if(input->is_token()) {
    auto token = dynamic_cast<const Token*>(input);
    if(token == nullptr) {
      throw std::runtime_error("Bad cast from const Symbol* to const Token*");
    }
    _actions[token].first = state;
  } else {
    auto nt = dynamic_cast<const Nonterminal*>(input);
    if(nt == nullptr) {
      throw std::runtime_error(
	"Bad cast from const Symbol* to const Nonterminal*"
      );
    }
    _gotos[nt] = state;
  }
}

void State::add_reductions(
  const std::map<const Token*,std::set<const Production*>>& reductions
			   )
{
  for(auto& pair : reductions) {
    auto result = _actions.insert(
      std::make_pair(pair.first,std::make_pair(nullptr,pair.second)));
    //std::set<const Production*>& ref = _actions[pair.first].second;
    if(!result.second) {
      for(const Production* p : pair.second) {
        result.first->second.second.insert(p);
      }
    }
  }
}

const std::map<const Token*,
	       std::pair<const State*,std::set<const Production*>>>&
State::actions() const
{
  return _actions;
}

const std::map<const Nonterminal*,const State*>& State::gotos() const
{
  return _gotos;
}

/*
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
*/
