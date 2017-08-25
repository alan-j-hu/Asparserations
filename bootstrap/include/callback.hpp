#ifndef _ASPARSERATIONS_BOOTSTRAP_CALLBACK_
#define _ASPARSERATIONS_BOOTSTRAP_CALLBACK_

#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/production.hpp"
#include <string>
#include <vector>

namespace asparserations {
  namespace _generated {
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
      Payload call(_generated::Nonterminal, _generated::Production,
		   const std::vector<_generated::Node*>&);
      Payload call(_generated::Token, const std::string&);
      const grammar::Grammar& grammar() const;
    private:
      enum class Mode {Token, Nonterminal, Production, Append};
      Mode _mode;
      grammar::Grammar& _grammar;
      grammar::Nonterminal* _nonterminal;
      grammar::Production* _production;
      std::vector<const grammar::Symbol*> _symbols;
    };
  }
}

#endif
