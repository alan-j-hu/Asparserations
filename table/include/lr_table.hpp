#ifndef _ASPARSERATIONS_TABLE_LR_GENERATOR_H_
#define _ASPARSERATIONS_TABLE_LR_GENERATOR_H_

#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "item_set.hpp"
#include "table.hpp"
#include "state.hpp"
#include <list>

namespace asparserations {
  namespace table {
    class LR_Table : public Table
    {
    public:
      LR_Table(grammar::Grammar&, const grammar::Nonterminal&);
      const std::list<State>& states() const;
    private:
      std::list<State> _states;
    };
  }
}

#endif
