#ifndef _ASPARSERATIONS_GRAMMAR_TOKEN_H_
#define _ASPARSERATIONS_GRAMMAR_TOKEN_H_

#include "symbol.hpp"
#include <set>
#include <vector>

namespace asparserations {
  namespace grammar {
    class Token : public Symbol
    {
    public:
      Token();
      const std::set<const Token*>& first_set() const;
      const std::vector<Production>& productions() const;
    private:
      std::set<const Token*> _first_set;
      const std::vector<Production> _productions;
    };
  }
}

#endif
