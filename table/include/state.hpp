#ifndef _ASPARSERATIONS_TABLE_STATE_H_
#define _ASPARSERATIONS_TABLE_STATE_H_

#include <map>
#include <set>

namespace asparserations {
  namespace grammar {
    class Production;
    class Symbol;
  }
  namespace table {
    class State
    {
    public:
      State(unsigned int);
      unsigned int index() const;
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
      unsigned int _index;
      std::map<const asparserations::grammar::Symbol*,const State*>
      _transitions;
      std::map<const asparserations::grammar::Symbol*,
	       std::set<const asparserations::grammar::Production*>>
      _reductions;
    };
  }
}

#endif
