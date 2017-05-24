#ifndef _ASPARSERATIONS_GRAMMAR_PRODUCTION_H_
#define _ASPARSERATIONS_GRAMMAR_PRODUCTION_H_

#include <string>
#include <vector>

namespace asparserations {
  namespace grammar {
    class Nonterminal;
    class Symbol;
    class Production
    {
    public:
      Production(Nonterminal&, const std::string&,
		 const std::vector<const Symbol*>&);
      const std::vector<const Symbol*>& symbols() const;
      const std::string& id() const;
      void set_symbol(unsigned int, const Symbol*);
      void insert_symbol(unsigned int, const Symbol*);
      void erase_symbol(unsigned int);
    private:
      Nonterminal& _nonterminal;
      std::string _id;
      std::vector<const Symbol*> _symbols;
    };
  }
}

#endif
