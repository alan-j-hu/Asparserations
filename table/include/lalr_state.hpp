#ifndef ASPARSERATIONS_TABLE_LALR_STATE_H_
#define ASPARSERATIONS_TABLE_LALR_STATE_H_

#include "item_set.hpp"

namespace asparserations {
  namespace grammar {
    class Token;
  }
  namespace table {
    class State;
    class LALR_Item_Set;
    class LALR_State
    {
    public:
      LALR_State(State&);
      LALR_State(State&, const Item_Set&);
      const Item_Set& item_set() const;
      State& state();
      bool merge(const std::set<Item>&);
    private:
      Item_Set m_item_set;
      State& m_state;
    };
  }
}

#endif
