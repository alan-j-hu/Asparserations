#include "../include/print_states.hpp"
#include "../../grammar/include/production.hpp"
#include "../../grammar/include/symbol.hpp"
#include "../../table/include/state.hpp"
#include "../../table/include/table.hpp"
#include <iostream>
#include <list>

using namespace asparserations;
using namespace table;

void asparserations::print_states(const Table& table)
{
  const std::list<State>& states = table.states();
  for(const State& state : states) {
    std::cout << "State " << state.index() << ":\nTransitions:\n";
    //Print state transitions
    const auto& transitions = state.transitions();
    for(const auto& transition : transitions) {
      std::cout << transition.first->id() << ":"
		<< transition.second->index() << "\n";
    }
    //Print state reductions
    std::cout << "Reductions:\n";
    const auto& reductions = state.reductions();
    for(auto& reduction : reductions) {
      std::cout << reduction.first->id() << ": {\n";
      const auto& productions = reduction.second;
      for(const auto& production : productions) {
	std::cout << production->id() << "\n";
      }
      std::cout << "}\n";
    }
  }
  std::cout << std::flush;
}
