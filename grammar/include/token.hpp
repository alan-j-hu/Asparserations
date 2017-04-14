#ifndef _ASPARSERATIONS_GRAMMAR_TOKEN_H_
#define _ASPARSERATIONS_GRAMMAR_TOKEN_H_

#include "production.hpp"
#include "symbol.hpp"
#include <list>
#include <set>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Token : public Symbol
    {
    public:
      Grammar& grammar();
      const Grammar& grammar() const;
      const std::set<const Token*>& first_set() const;
      const std::list<Production>& productions() const;
      bool derives_empty_string() const;
      bool has_empty_string_in_first_set() const;
    protected:
      Token(Grammar&);
    private:
      Grammar& _grammar;
      std::set<const Token*> _first_set;
      const std::list<Production> _productions;
    };
  }
}

#endif
