#ifndef _ASPARSERATIONS_TABLE_STATE_H_
#define _ASPARSERATIONS_TABLE_STATE_H_

#include "../../grammar/include/production.hpp"
#include "../../grammar/include/symbol.hpp"
#include <map>
#include <set>

namespace asparserations {
  namespace table {
    class State
    {
    public:
      void add_transition(const asparserations::grammar::Symbol* const,
			  const State*);
      void add_reduction(const asparserations::grammar::Symbol* const,
	     const std::set<const asparserations::grammar::Production*>&);
      const std::map<const asparserations::grammar::Symbol*,const State*>&
      transitions();
      const std::map<const asparserations::grammar::Symbol*,
	       std::set<const asparserations::grammar::Production*>>&
      reductions();
    private:
      std::map<const asparserations::grammar::Symbol*,const State*>
      _transitions;
      std::map<const asparserations::grammar::Symbol*,
	       std::set<const asparserations::grammar::Production*>>
      _reductions;
    };
  }
}

#endif
