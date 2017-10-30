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
                 const std::vector<const Symbol*>&, unsigned int);
      const std::vector<const Symbol*>& symbols() const;
      const std::string& name() const;
      unsigned int index() const;
      Nonterminal& nonterminal();
      const Nonterminal& nonterminal() const;
      void set_symbol(unsigned int, const Symbol*);
      void insert_symbol(unsigned int, const Symbol*);
      void erase_symbol(unsigned int);
    private:
      Nonterminal& m_nonterminal;
      unsigned int m_index;
      std::string m_name;
      std::vector<const Symbol*> m_symbols;
    };
    bool operator<(const Production&, const Production&);
  }
}

#endif
