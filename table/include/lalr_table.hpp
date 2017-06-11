#include "state.hpp"
#include "table.hpp"
#include <list>

namespace asparserations {
  namespace grammar {
    class Grammar;
  }
  namespace table {
    class LALR_Table : public Table
    {
    public:
      LALR_Table(grammar::Grammar&);
      const std::list<State>& states() const;
    private:
      std::list<State> _states;
    };
  }
}
