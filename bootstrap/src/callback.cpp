#include "../include/callback.hpp"
#include "../../autogen/include/Parser.hpp"
#include <stdexcept>

#include <iostream>

using namespace asparserations;
using namespace bootstrap;
using namespace grammar;
using namespace _generated;

Callback::Callback(Grammar& grammar)
  : _mode(Mode::Token),
    _grammar(grammar),
    _nonterminal(nullptr),
    _production(nullptr) {}

Callback::Payload Callback::call(_generated::Nonterminal nonterminal,
				 _generated::Production production,
		                 const std::vector<Node*>& children)
{
  std::string id;
  switch(nonterminal) {
  case _generated::Nonterminal::__accept__:
    switch(production) {
    case _generated::Production::__root__:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in __accept__");
    }
  case _generated::Nonterminal::root:
    switch(production) {
    case _generated::Production::main:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + "in root");
    }
  case _generated::Nonterminal::Identifier_List:
    switch(production) {
    case _generated::Production::recursive_case:
      return Payload();
    case _generated::Production::base_case:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Identifier_List");
    }
  case _generated::Nonterminal::Nonterminal_List:
    switch(production) {
    case _generated::Production::main:
      return Payload();
    case _generated::Production::empty:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Nonterminal_List");
    }
  case _generated::Nonterminal::Nonterminal:
    switch(production) {
    case _generated::Production::main:
      _nonterminal = &_grammar.add_nonterminal(std::string(children[0]->begin(),
		  					   children[0]->end()));
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Nonterminal");
    }
  case _generated::Nonterminal::Production_List:
    switch(production) {
    case _generated::Production::recursive_case:
      return Payload();
    case _generated::Production::base_case:
      return Payload();
    default:
      throw std::runtime_error("Unknown production in " + Parser::production_to_string(production) + " Production_List");
    }
  case _generated::Nonterminal::Production:
    switch(production) {
    case _generated::Production::main:
      if(_nonterminal == nullptr) {
	throw std::runtime_error("Null pointer");
      }
      _nonterminal->add_production(
		      std::string(children[2]->begin(), children[2]->end()),
		      _symbols
      );
      _symbols = std::vector<const Symbol*>();
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Production");
    }
  case _generated::Nonterminal::Symbol_List:
    switch(production) {
    case _generated::Production::main:
      return Payload();
    case _generated::Production::empty:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Symbol_List");
    }
  default:
    throw std::runtime_error("Unknown nonterminal");
  }
}

Callback::Payload Callback::call(_generated::Token token,
				 const std::string& string)
{
  switch(token) {
  case _generated::Token::__end__:
    return Payload();
  case _generated::Token::Bar:
    return Payload();
  case _generated::Token::Identifier:
    switch(_mode) {
    case Mode::Token:
      _grammar.add_token(string);
      return Payload();
    case Mode::Nonterminal:
      _nonterminal = &_grammar.add_nonterminal(string);
      return Payload();
    case Mode::Append:
      try {
        _symbols.push_back(&_grammar.nonterminal_at(string));
      } catch(const std::out_of_range& e) {
	try {
	  _symbols.push_back(&_grammar.token_at(string));
	} catch(const std::out_of_range& e) {
	  throw std::runtime_error("Undefined symbol " + string);
	}
      }
      return Payload();
    }
    return Payload();
  case _generated::Token::Colon:
    _mode = Mode::Append;
    return Payload();
  case _generated::Token::Comma:
    return Payload();
  case _generated::Token::Semicolon:
    _mode = Mode::Nonterminal;
    return Payload();
  case _generated::Token::Tokens_Keyword:
    return Payload();
  case _generated::Token::Open_Bracket:
    return Payload();
  case _generated::Token::Close_Bracket:
    _mode = Mode::Nonterminal;
    return Payload();
  case _generated::Token::Hash:
    _mode = Mode::Production;
    return Payload();
  default:
    throw std::runtime_error("Unknown token");
  }
}

const Grammar& Callback::grammar() const
{
  return _grammar;
}
