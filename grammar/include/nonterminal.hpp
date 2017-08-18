#ifndef _ASPARSERATIONS_GRAMMAR_NONTERMINAL_H_
#define _ASPARSERATIONS_GRAMMAR_NONTERMINAL_H_

#include "production.hpp"
#include "symbol.hpp"
#include <map>
#include <set>
#include <string>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Token;
    class Nonterminal : public Symbol
    {
    public:
      virtual const std::map<std::string,Production>& productions() const=0;
      virtual Production& production_at(const std::string&)=0;
      virtual const Production& production_at(const std::string&) const=0;
      virtual const std::string& id() const=0;
      virtual Grammar& grammar()=0;
      virtual const Grammar& grammar() const=0;
      virtual const std::set<const Token*>& first_set() const=0;
      virtual bool is_token() const=0;
      virtual bool derives_empty_string() const=0;
      virtual Production& add_production(const std::string&,
		               		 std::vector<const Symbol*>)=0;
    protected:
      Nonterminal()=default;
      Nonterminal(Nonterminal&&)=default;
    };
  }
}

#endif
