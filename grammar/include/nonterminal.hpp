#ifndef _ASPARSERATIONS_GRAMMAR_NONTERMINAL_H_
#define _ASPARSERATIONS_GRAMMAR_NONTERMINAL_H_

#include "production.hpp"
#include "symbol.hpp"
#include "token.hpp"

namespace asparserations {
  namespace grammar {
    class Nonterminal : Symbol
    {
    public:
      Nonterminal(const std::vector<Production>&);
      const std::vector<Production>& productions() const;
      const std::set<const Token*>& first_set() const;
    private:
      std::vector<Production> _productions;
      std::set<const Token*> _first_set;
    };
  }
}

#endif
