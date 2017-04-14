#ifndef _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_
#define _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_

#include "nonterminal.hpp"
#include "production.hpp"
#include "symbol.hpp"
#include "token.hpp"
#include <list>

namespace asparserations {
  namespace grammar {
    class Grammar
    {
    public:
      Token& add_token();
      Nonterminal& add_nonterminal();
      void compute_first_sets();
    private:
      class TokenImp;
      class NonterminalImp;
      std::list<TokenImp> _tokens;
      std::list<NonterminalImp> _nonterminals;
    };
  }
}

#endif
