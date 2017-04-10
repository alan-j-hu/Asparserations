#ifndef _ASPARSERATIONS_TABLE_STATE_H_
#define _ASPARSERATIONS_TABLE_STATE_H_

#include <map>

namespace asparserations {
  namespace table {
    class State
    {
    public:
      void add_transition(const Symbol*, const State*);
    private:
      std::map<Symbol*,const State*> _transitions;
    };
  }
}

#endif
