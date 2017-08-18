#include "../include/lexer.hpp"
#include <cctype>
#include <stdexcept>

using namespace asparserations;
using namespace bootstrap;

std::pair<const char*,bool> Lexer::expect(_generated::Token token,
					  const char*& c)
{
  if(c == nullptr) throw std::runtime_error("Null pointer");
  skip_whitespace(c);
  bool result = false;
  switch(token) {
  case _generated::Token::Bar:
    result = match("|", c);
    return std::make_pair(c, result);

  case _generated::Token::Identifier:
    result = match_range('A', 'z', c);
    if(!result) {
      return std::make_pair(c, false);
    }
    do {
      if(*c == '\0') return std::make_pair(c, true);
      result = match_range('A', 'z', c);
      if(!result) {
	result = match_range('0','9', c);
      }
    } while(result);
    return std::make_pair(c, true);

  case _generated::Token::Colon:
    result = match(":", c);
    return std::make_pair(c, result);

  case _generated::Token::Comma:
    result = match(",", c);
    return std::make_pair(c, result);

  case _generated::Token::Semicolon:
    result = match(";", c);
    return std::make_pair(c, result);

  case _generated::Token::Tokens_Keyword:
    result = match("tokens", c);
    return std::make_pair(c, result);

  case _generated::Token::Open_Bracket:
    result = match("{", c);
    return std::make_pair(c, result);

  case _generated::Token::Close_Bracket:
    result = match("}", c);
    return std::make_pair(c, result);

  case _generated::Token::Hash:
    result = match("#", c);
    return std::make_pair(c, result);

  case _generated::Token::__end__:
    result = (*c == '\0');
    return std::make_pair(c, result);
  }
  throw std::runtime_error("Unknown variant");
}

void Lexer::skip_whitespace(const char*& c)
{
  for(; *c != '\0' && std::isspace(*c); ++c);
}

bool Lexer::match(const std::string& match, const char*& c)
{
  const char* iter = c;
  for(auto ch : match) {
    if(*iter == '\0' || *iter != ch) {
      return false;
    }
    ++iter;
  }
  c = iter;
  return true;
}

bool Lexer::match_range(char lo, char hi, const char*& c)
{
  if(*c >= lo && *c <= hi) {
    ++c;
    return true;
  }
  return false;
}
