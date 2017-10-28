#ifndef ASPARSERATIONS_GRAMMAR_PRODUCTION_H_
#define ASPARSERATIONS_GRAMMAR_PRODUCTION_H_

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
      Nonterminal& nonterminal();
      const Nonterminal& nonterminal() const;
      void set_symbol(unsigned int, const Symbol*);
      void insert_symbol(unsigned int, const Symbol*);
      void erase_symbol(unsigned int);
    private:
      Nonterminal& m_nonterminal;
      std::string m_id;
      std::vector<const Symbol*> m_symbols;
    };
  }
}

#endif
