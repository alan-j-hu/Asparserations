#ifndef _ASPARSERATIONS_GRAMMAR_SYMBOL_H_
#define _ASPARSERATIONS_GRAMMAR_SYMBOL_H_

#include "production.hpp"
#include <set>
#include <vector>

namespace asparserations {
  namespace grammar {
    class Token;
    class Symbol
    {
    public:
      virtual const std::set<const Token*>& first_set() const=0;
      virtual const std::vector<Production>& productions() const=0;
    };
  }
}

#endif
