#ifndef _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_
#define _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_

#include "nonterminal.hpp"
#include "token.hpp"
#include <list>
#include <string>

namespace asparserations {
  namespace grammar {
    class Grammar
    {
    public:
      Grammar();
      Token& add_token(const std::string&);
      Nonterminal& add_nonterminal(const std::string&);
      Nonterminal* start_symbol();
      const Nonterminal* start_symbol() const;
      void set_start_symbol(Nonterminal*);
      void compute_first_sets();
    private:
      struct TokenImp : public Token
      {
	TokenImp(Grammar& g, const std::string& id);
      };
      struct NonterminalImp : public Nonterminal
      {
	NonterminalImp(Grammar& g, const std::string& id);
	bool _has_empty_string_in_first_set;
        bool has_empty_string_in_first_set() const;
	std::set<const Token*> _first_set;
        const std::set<const Token*>& first_set() const;
      };

      std::list<TokenImp> _tokens;
      std::list<NonterminalImp> _nonterminals;
      Nonterminal* _start_symbol;
    };
  }
}

#endif
