#ifndef ASPARSERATIONS_TABLE_LR_GENERATOR_H_
#define ASPARSERATIONS_TABLE_LR_GENERATOR_H_

#include "table.hpp"
#include "state.hpp"
#include <list>
#include <utility>
#include <vector>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Nonterminal;
  }
  namespace table {
    class LR_Table : public Table
    {
    public:
      LR_Table(grammar::Grammar&);
      const std::list<State>& states() const;
      const grammar::Grammar& grammar() const;
      const std::vector<std::pair<const Item_Set*,const State*>>&
      item_set_state_pairs() const;
    private:
      std::list<State> m_states;
      grammar::Grammar& m_grammar;
      std::map<Item_Set,State*> m_item_sets;
      std::vector<std::pair<const Item_Set*,const State*>>
        m_item_set_state_pairs;
    };
  }
}

#endif
