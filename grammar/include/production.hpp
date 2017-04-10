#ifndef _ASPARSERATIONS_GRAMMAR_PRODUCTION_H_
#define _ASPARSERATIONS_GRAMMAR_PRODUCTION_H_

#include <initializer_list>
#include <vector>

namespace asparserations {
  namespace grammar {
    class Symbol;
    class Production
    {
    public:
      Production(const std::vector<Symbol*>&);
      const std::vector<const Symbol*>& symbols() const;
    private:
      std::vector<const Symbol*> _symbols;
    };
  }
}

#endif
