#ifndef _ASPARSERATIONS_GRAMMAR_NONTERMINAL_H_
#define _ASPARSERATIONS_GRAMMAR_NONTERMINAL_H_

#include "production.hpp"
#include "symbol.hpp"
#include "token.hpp"
#include <list>
#include <set>
#include <vector>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Nonterminal : public Symbol
    {
      friend class Grammar;
    public:
      Nonterminal(Grammar&);
      const std::list<Production>& productions() const;
      const std::set<const Token*>& first_set() const;
      bool derives_empty_string() const;
      bool has_empty_string_in_first_set() const;
      Production& add_production(std::vector<const Symbol*>);
    private:
      void _compute_first_set(std::set<Symbol*>);
      bool _has_empty_string_in_first_set;
      std::set<Symbol*> _visited;
      std::set<Symbol*> _unfulfilled_promises;

      Grammar& _grammar;
      std::list<Production> _productions;
      std::set<const Token*> _first_set;
    };
  }
}

#endif
