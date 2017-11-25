#include "../include/lexer.hpp"
#include <cctype>
#include <stdexcept>

using namespace asparserations;
using namespace bootstrap;

std::pair<generated::Lexer_State,bool>
Lexer::expect(generated::Token token, const generated::Lexer_State& state)
{
  generated::Lexer_State s = state;
  skip_whitespace(s);
  bool result = false;
  switch(token) {
  case generated::Token::Bar:
    result = match("|", s);
    return std::make_pair(s, result);

  case generated::Token::Identifier:
    result = match_range('A', 'Z', s);
    if(!result) {
      result = match_range('a', 'z', s);
    }
    if(!result) {
      return std::make_pair(s, false);
    }
    do {
      result = match_range('A', 'Z', s);
      if(!result) {
        result = match_range('a', 'z', s);
      }
      if(!result) {
        result = match_range('0','9', s);
      }
      if(!result) {
        result = match("_", s);
      }
    } while(result);
    return std::make_pair(s, true);

  case generated::Token::Prime_Identifier:
    result = match("'", s);
    if(!result) {
      return std::make_pair(s, false);
    }
    result = match_range('A', 'Z', s);
    if(!result) {
      result = match_range('a', 'z', s);
    }
    if(!result) {
      return std::make_pair(s, false);
    }
    do {
      result = match_range('A', 'Z', s);
      if(!result) {
        result = match_range('a', 'z', s);
      }
      if(!result) {
        result = match_range('0','9', s);
      }
      if(!result) {
        result = match("_", s);
      }
    } while(result);
    return std::make_pair(s, true);

  case generated::Token::Colon:
    result = match(":", s);
    return std::make_pair(s, result);

  case generated::Token::Comma:
    result = match(",", s);
    return std::make_pair(s, result);

  case generated::Token::Semicolon:
    result = match(";", s);
    return std::make_pair(s, result);

  case generated::Token::Tokens_Keyword:
    result = match("tokens", s);
    return std::make_pair(s, result);

  case generated::Token::Open_Bracket:
    result = match("{", s);
    return std::make_pair(s, result);

  case generated::Token::Close_Bracket:
    result = match("}", s);
    return std::make_pair(s, result);

  case generated::Token::Hash:
    result = match("#", s);
    return std::make_pair(s, result);

  case generated::Token::end_:
    result = (*s.end == '\0');
    return std::make_pair(s, result);
  }
  throw std::runtime_error("Unknown variant");
}

void Lexer::skip_whitespace(generated::Lexer_State& state)
{
  for(; *state.begin != '\0' && std::isspace(*state.begin); ++state.begin) {
    if(*state.begin == '\n') {
      state.last_newline = state.begin;
      ++state.lines;
    }
  }
  state.end = state.begin;
}

bool Lexer::match(const std::string& match, generated::Lexer_State& state)
{
  generated::Lexer_State s = state;
  for(auto ch : match) {
    if(*s.end == '\n') {
      s.last_newline = s.end;
      ++s.lines;
    }
    if(*s.end == '\0' || *s.end != ch) {
      return false;
    }
    ++s.end;
  }
  state = s;
  return true;
}

bool Lexer::match_range(char lo, char hi, generated::Lexer_State& state)
{
  if(*state.end >= lo && *state.end <= hi) {
    if(*state.end == '\n') {
      state.last_newline = state.end;
      ++state.lines;
    }
    ++state.end;
    return true;
  }
  return false;
}
