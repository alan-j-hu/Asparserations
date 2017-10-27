#ifndef _ASPARSERATIONS_BOOTSTRAP_CALLBACK_
#define _ASPARSERATIONS_BOOTSTRAP_CALLBACK_

#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/production.hpp"
#include <string>
#include <vector>

namespace asparserations {
  namespace generated {
    enum class Nonterminal;
    enum class Production;
    enum class Token;
    class Node;
  }
  namespace bootstrap {
    class Callback
    {
    public:
      class Payload
      {
      };
      Callback(grammar::Grammar&);
      Payload call(generated::Nonterminal, generated::Production,
		   const std::vector<generated::Node*>&);
      Payload call(generated::Token, const std::string&);
      const grammar::Grammar& grammar() const;
    private:
      enum class Mode {Token, Nonterminal, Production, Append};
      Mode m_mode;
      grammar::Grammar& m_grammar;
      grammar::Nonterminal* m_nonterminal;
      grammar::Production* m_production;
      std::vector<const grammar::Symbol*> m_symbols;
    };
  }
}

#endif
