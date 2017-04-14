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
    public:
      const std::list<Production>& productions() const;
      Grammar& grammar();
      const Grammar& grammar() const;
      virtual const std::set<const Token*>& first_set() const=0;
      bool derives_empty_string() const;
      virtual bool has_empty_string_in_first_set() const=0;
      Production& add_production(std::vector<const Symbol*>);
    protected:
      Nonterminal(Grammar&);
    private:
      Grammar& _grammar;
      std::list<Production> _productions;
    };
  }
}

#endif
