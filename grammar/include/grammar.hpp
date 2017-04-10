#ifndef _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_
#define _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_

#include "nonterminal.hpp"
#include "symbol.hpp"
#include "token.hpp"

namespace asparserations {
  namespace grammar {
    class Grammar
    {
    public:
      Token& add_token();
      Nonterminal& add_nonterminal();
    private:
      
    };
  }
}

#endif
