#ifndef ASPARSERATIONS_GRAMMAR_SYMBOL_H_
#define ASPARSERATIONS_GRAMMAR_SYMBOL_H_

#include "production.hpp"
#include <list>
#include <map>
#include <set>
#include <string>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Token;
    class Symbol
    {
    public:
      virtual const std::string& name() const=0;
      virtual Grammar& grammar()=0;
      virtual const Grammar& grammar() const=0;
      virtual const std::set<const Token*>& first_set() const=0;
      virtual const std::map<std::string,Production>& productions() const=0;
      virtual bool is_token() const=0;
      virtual bool derives_empty_string() const=0;
    protected:
      Symbol()=default;
    };
  }
}

#endif
