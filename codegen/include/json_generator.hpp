#ifndef _ASPARSERATIONS_CODEGEN_JSON_GENERATOR_
#define _ASPARSERATIONS_CODEGEN_JSON_GENERATOR_

#include "code_generator.hpp"
#include <string>

namespace asparserations {
  namespace grammar {class Grammar;class Nonterminal;class Token;}
  namespace table {class State;class Table;}
  namespace codegen {
    class JSON_Generator : public Code_Generator
    {
    public:
      JSON_Generator(const table::Table&, bool, const std::string& tab = "  ");
      const std::string& code() const;
    private:
      std::string _code;
      const table::Table& _table;
      const grammar::Grammar& _grammar;
      bool _pretty_print;
      std::string _tab;
      unsigned int _indent_depth;
      void _break_and_indent();
      void _generate();
      void _generate_token(const grammar::Token&);
      void _generate_nonterminal(const grammar::Nonterminal&);
      void _generate_grammar(const grammar::Grammar&);
      void _generate_state(const table::State&);
      void _generate_table(const table::Table&);
    };
  }
}

#endif
