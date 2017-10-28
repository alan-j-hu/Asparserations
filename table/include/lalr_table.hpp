#ifndef ASPARSERATIONS_TABLE_LALR_TABLE_H_
#define ASPARSERATIONS_TABLE_LALR_TABLE_H_

#include "item_core.hpp"
#include "lalr_state.hpp"
#include "state.hpp"
#include "table.hpp"
#include <list>
#include <utility>
#include <vector>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Production;
  }
  namespace table {
    class LALR_Table : public Table
    {
    public:
      LALR_Table(grammar::Grammar&);
      const std::list<State>& states() const;
      const grammar::Grammar& grammar() const;
      const std::vector<std::pair<const Item_Set*,const State*>>&
        item_set_state_pairs() const;
    private:
      std::list<State> m_states;
      grammar::Grammar& m_grammar;
      std::map<std::set<Item_Core>,LALR_State> m_item_sets;
      std::vector<std::pair<const Item_Set*,const State*>>
	m_item_set_state_pairs;
    };
  }
}

#endif
