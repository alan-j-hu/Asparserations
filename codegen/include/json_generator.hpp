#ifndef ASPARSERATIONS_CODEGEN_JSON_GENERATOR_H_
#define ASPARSERATIONS_CODEGEN_JSON_GENERATOR_H_

#include "../../table/include/item_set.hpp"
#include "code_generator.hpp"
#include <map>
#include <set>
#include <string>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Nonterminal;
    class Token;
    class Production;
  }
  namespace table {class State;class Table;}
  namespace codegen {
    class JSON_Generator : public Code_Generator
    {
    public:
      JSON_Generator(const table::Table&, bool, const std::string& tab = "  ");
      const std::string& code() const;
    private:
      std::string m_code;
      const table::Table& m_table;
      const grammar::Grammar& m_grammar;
      bool m_pretty_print;
      std::string m_tab;
      unsigned int m_indent_depth;
      void m_break_and_indent();
      void m_generate();
      void m_generate_token(const grammar::Token&);
      void m_generate_nonterminal(const grammar::Nonterminal&);
      void m_generate_grammar(const grammar::Grammar&);
      void m_generate_actions(
        const std::map<const grammar::Token*,
        std::pair<const table::State*,std::set<const grammar::Production*>>>&
                             );
      void m_generate_gotos(const std::map<const grammar::Nonterminal*,
                                           const table::State*>&);
      void m_generate_item_set(const table::Item_Set&);
      void m_generate_state(const table::State&, const table::Item_Set*);
      void m_generate_table(const table::Table&);
    };
  }
}

#endif
