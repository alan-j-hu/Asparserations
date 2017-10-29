#ifndef ASPARSERATIONS_BOOTSTRAP_LEXER_H_
#define ASPARSERATIONS_BOOTSTRAP_LEXER_H_

#include "../../autogen/include/Parser.hpp"
#include <string>
#include <utility>

namespace asparserations {
  namespace bootstrap {
    class Lexer
    {
    public:
      std::pair<generated::Lexer_State,bool>
      expect(generated::Token, const generated::Lexer_State&);
    private:
      bool match(const std::string&, generated::Lexer_State&);
      bool match_range(char, char, generated::Lexer_State&);
      void skip_whitespace(generated::Lexer_State&);
    };
  }
}

#endif
