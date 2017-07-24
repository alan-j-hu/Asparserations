#ifndef _ASPARSERATIONS_GRAMMAR_TOKEN_H_
#define _ASPARSERATIONS_GRAMMAR_TOKEN_H_

#include "production.hpp"
#include "symbol.hpp"
#include <string>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Token : public Symbol
    {
    public:
      virtual const std::string& id() const=0;
      virtual Grammar& grammar()=0;
      virtual const Grammar& grammar() const=0;
      virtual const std::set<const Token*>& first_set() const=0;
      virtual const std::map<std::string,Production>& productions() const=0;
      virtual bool is_token() const=0;
      virtual bool derives_empty_string() const=0;
      virtual bool has_empty_string_in_first_set() const=0;
    };
  }
}

#endif
