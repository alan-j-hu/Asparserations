#ifndef ASPARSERATIONS_GRAMMAR_TOKEN_H_
#define ASPARSERATIONS_GRAMMAR_TOKEN_H_

#include "production.hpp"
#include "symbol.hpp"
#include <string>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Token : public Symbol
    {
    public:
      virtual const std::string& name() const=0;
      virtual Grammar& grammar()=0;
      virtual const Grammar& grammar() const=0;
      virtual unsigned int index() const=0;
      virtual const std::set<std::reference_wrapper<const Token>>&
        first_set() const=0;
      virtual const std::list<Production>& productions() const=0;
      virtual bool is_token() const=0;
      virtual bool derives_empty_string() const=0;
    };
    bool operator<(const Token&, const Token&);
  }
}

#endif
