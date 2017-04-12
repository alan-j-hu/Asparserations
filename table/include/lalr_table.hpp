#include "item_set.hpp"
#include "state.hpp"
#include "table.hpp"
#include <list>

namespace asparserations {
  namespace table {
    class LALR_Table : public Table
    {
    public:
      LALR_Table();
      const std::list<State>& states() const;
    private:
      std::list<State> _states;
    };
  }
}
