#ifndef _ASPARSERATIONS_TABLE_LR_GENERATOR_H_
#define _ASPARSERATIONS_TABLE_LR_GENERATOR_H_

#include "table.hpp"
#include "state.hpp"
#include <list>

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
    private:
      std::list<State> m_states;
      grammar::Grammar& m_grammar;
    };
  }
}

#endif
