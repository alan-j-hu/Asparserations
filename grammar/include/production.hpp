#ifndef _ASPARSERATIONS_GRAMMAR_PRODUCTION_H_
#define _ASPARSERATIONS_GRAMMAR_PRODUCTION_H_

#include <vector>

namespace asparserations {
  namespace grammar {
    class Nonterminal;
    class Symbol;
    class Production
    {
    public:
      Production(Nonterminal&, const std::vector<const Symbol*>&);
      const std::vector<const Symbol*>& symbols() const;
    private:
      Nonterminal& _nonterminal;
      std::vector<const Symbol*> _symbols;
    };
  }
}

#endif
