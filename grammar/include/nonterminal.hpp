#ifndef ASPARSERATIONS_GRAMMAR_NONTERMINAL_H_
#define ASPARSERATIONS_GRAMMAR_NONTERMINAL_H_

#include "production.hpp"
#include "symbol.hpp"
#include <list>
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
      virtual const std::list<Production>& productions() const=0;
      virtual Production& production_at(const std::string&)=0;
      virtual const Production& production_at(const std::string&) const=0;
      virtual const std::string& name() const=0;
      virtual Grammar& grammar()=0;
      virtual const Grammar& grammar() const=0;
      virtual const std::set<std::reference_wrapper<const Token>>&
        first_set() const=0;
      virtual bool is_token() const=0;
      virtual bool derives_empty_string() const=0;
      virtual Production& add_production(const std::string&,
		               		 std::vector<const Symbol*>)=0;
    protected:
      Nonterminal()=default;
      Nonterminal(Nonterminal&&)=default;
    };
    bool operator<(const Nonterminal&, const Nonterminal&);
  }
}

#endif
