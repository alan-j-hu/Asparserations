#ifndef _ASPARSERATIONS_GRAMMAR_SYMBOL_H_
#define _ASPARSERATIONS_GRAMMAR_SYMBOL_H_

#include "production.hpp"
#include <list>
#include <set>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Token;
    class Symbol
    {
    public:
      virtual Grammar& grammar()=0;
      virtual const Grammar& grammar() const=0;
      virtual const std::set<const Token*>& first_set() const=0;
      virtual const std::list<Production>& productions() const=0;
      virtual bool derives_empty_string() const=0;
      virtual bool has_empty_string_in_first_set() const=0;
    };
  }
}

#endif
