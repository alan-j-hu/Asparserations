#include "../../nonterminal.hpp"

namespace asparserations {
  namespace grammar {
    class Nonterminal;
  }
  namespace codegen {
    class Template;
    class Generator
    {
    public:
      enum class Algorithm
      {
	LR,
	LALR
      };

      Generator(Algorithm, const Nonterminal&, const Template&);
      const std::string& parser() const;
    private:
      std::string& _parser;
    };
  }
}
