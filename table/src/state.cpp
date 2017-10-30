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
  : m_index(index) {}

unsigned int State::index() const
{
  return m_index;
}

void State::add_transition(const Symbol* const input, const State* state)
{
  if(input->is_token()) {
    auto token = dynamic_cast<const Token*>(input);
    if(token == nullptr) {
      throw std::runtime_error("Bad cast from const Symbol* to const Token*");
    }
    m_actions[std::cref(*token)].first = state;
  } else {
    auto nt = dynamic_cast<const Nonterminal*>(input);
    if(nt == nullptr) {
      throw std::runtime_error(
	"Bad cast from const Symbol* to const Nonterminal*"
      );
    }
    m_gotos[std::cref(*nt)] = state;
  }
}

void State::add_reductions(
  const std::map<std::reference_wrapper<const Token>,
  std::set<std::reference_wrapper<const Production>>>& reductions
                           )
{
  for(auto& pair : reductions) {
    auto result
      = m_actions.insert(std::make_pair(pair.first,
                                        std::make_pair(nullptr, pair.second)));
    if(!result.second) {
      for(auto& p : pair.second) {
        result.first->second.second.insert(p);
      }
    }
  }
}

const std::map<std::reference_wrapper<const Token>,
               std::pair<const State*,
                         std::set<std::reference_wrapper<const Production>>>>&
State::actions() const
{
  return m_actions;
}

const std::map<std::reference_wrapper<const Nonterminal>,const State*>&
State::gotos() const
{
  return m_gotos;
}
