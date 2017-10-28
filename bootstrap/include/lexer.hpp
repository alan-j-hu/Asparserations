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
      std::pair<std::pair<const char*,const char*>,bool>
        expect(generated::Token, const char*);
    private:
      bool match(const std::string&, const char*&);
      bool match_range(char, char, const char*&);
      void skip_whitespace(const char*&);
    };
  }
}

#endif
