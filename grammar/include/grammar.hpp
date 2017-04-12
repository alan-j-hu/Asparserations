#ifndef _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_
#define _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_

#include "nonterminal.hpp"
#include "production.hpp"
#include "symbol.hpp"
#include "token.hpp"
#include <map>
#include <string>

namespace asparserations {
  namespace grammar {
    class Grammar
    {
    public:
      Token& add_token();
      Nonterminal& add_nonterminal();
      void compute_first_set();
    private:
      std::list<Token> _tokens;
      std::list<Nonterminal> _nonterminals;
    };
  }
}

#endif
