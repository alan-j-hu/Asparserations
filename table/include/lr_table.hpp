#ifndef _ASPARSERATIONS_TABLE_LR_GENERATOR_H_
#define _ASPARSERATIONS_TABLE_LR_GENERATOR_H_
#include "table.hpp"
#include "state.hpp"
#include <map>

namespace asparserations {
  namespace table {
    class LR_Table : public Table
    {
    public:
      LR_Table();
      const std::map<Item_Set,State>& states() const;
    private:
      std::map<Item_Set,State> _states;
    };
  }
}

#endif
