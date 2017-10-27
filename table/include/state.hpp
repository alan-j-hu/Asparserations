#ifndef _ASPARSERATIONS_TABLE_STATE_H_
#define _ASPARSERATIONS_TABLE_STATE_H_

#include <map>
#include <set>

namespace asparserations {
  namespace grammar {
    class Nonterminal;
    class Production;
    class Symbol;
    class Token;
  }
  namespace table {
    class State
    {
    public:
      State(unsigned int);
      unsigned int index() const;
      void add_transition(const grammar::Symbol* const, const State*);
      void add_reductions(const std::map<const grammar::Token*,
			  std::set<const grammar::Production*>>&);
      const std::map<const grammar::Token*,
		     std::pair<const State*,
			       std::set<const grammar::Production*>>>&
      actions() const;
      const std::map<const grammar::Nonterminal*,const State*>& gotos() const;
      /*
      const std::map<const grammar::Symbol*,const State*>& transitions() const;
      const std::map<const asparserations::grammar::Token*,
	             std::set<const grammar::Production*>>& reductions() const;
      */
    private:
      unsigned int m_index;
      std::map<const grammar::Token*,
               std::pair<const State*,std::set<const grammar::Production*>>>
      m_actions;
      std::map<const grammar::Nonterminal*,const State*> m_gotos;
      /*
      std::map<const grammar::Symbol*,const State*> _transitions;
      std::map<const asparserations::grammar::Token*,
	       std::set<const grammar::Production*>> _reductions;
      */
    };
  }
}

#endif
