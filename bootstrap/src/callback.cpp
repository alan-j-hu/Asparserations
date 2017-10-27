#include "../include/callback.hpp"
#include "../../autogen/include/Parser.hpp"
#include <stdexcept>

#include <iostream>

using namespace asparserations;
using namespace bootstrap;
using namespace grammar;
using namespace generated;

Callback::Callback(Grammar& grammar)
  : m_mode(Mode::Token),
    m_grammar(grammar),
    m_nonterminal(nullptr),
    m_production(nullptr) {}

Callback::Payload Callback::call(generated::Nonterminal nonterminal,
				 generated::Production production,
		                 const std::vector<Node*>& children)
{
  std::string id;
  switch(nonterminal) {
  case generated::Nonterminal::_accept_:
    switch(production) {
    case generated::Production::_root_:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in __accept__");
    }
  case generated::Nonterminal::Root:
    switch(production) {
    case generated::Production::main:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + "in root");
    }
  case generated::Nonterminal::Identifier_List:
    switch(production) {
    case generated::Production::recursive_case:
      return Payload();
    case generated::Production::base_case:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Identifier_List");
    }
  case generated::Nonterminal::Nonterminal_List:
    switch(production) {
    case generated::Production::main:
      return Payload();
    case generated::Production::empty:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Nonterminal_List");
    }
  case generated::Nonterminal::Nonterminal:
    switch(production) {
    case generated::Production::main:
      m_nonterminal = &m_grammar.add_nonterminal(std::string(children[0]->begin(),
                  	                                     children[0]->end()));
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Nonterminal");
    }
  case generated::Nonterminal::Production_List:
    switch(production) {
    case generated::Production::recursive_case:
      return Payload();
    case generated::Production::base_case:
      return Payload();
    default:
      throw std::runtime_error("Unknown production in " + Parser::production_to_string(production) + " Production_List");
    }
  case generated::Nonterminal::Production:
    switch(production) {
    case generated::Production::main:
      if(m_nonterminal == nullptr) {
	throw std::runtime_error("Null pointer");
      }
      m_nonterminal->add_production(
		      std::string(children[2]->begin(), children[2]->end()),
		      m_symbols
      );
      m_symbols = std::vector<const Symbol*>();
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Production");
    }
  case generated::Nonterminal::Symbol_List:
    switch(production) {
    case generated::Production::main:
      return Payload();
    case generated::Production::empty:
      return Payload();
    default:
      throw std::runtime_error("Unknown production " + Parser::production_to_string(production) + " in Symbol_List");
    }
  default:
    throw std::runtime_error("Unknown nonterminal");
  }
}

Callback::Payload Callback::call(generated::Token token,
				 const std::string& string)
{
  switch(token) {
  case generated::Token::_end_:
    return Payload();
  case generated::Token::Bar:
    m_mode = Mode::Append;
    return Payload();
  case generated::Token::Identifier:
    switch(m_mode) {
    case Mode::Token:
      m_grammar.add_token(string);
      return Payload();
    case Mode::Nonterminal:
      m_nonterminal = &m_grammar.add_nonterminal(string);
      return Payload();
    case Mode::Append:
      try {
        m_symbols.push_back(&m_grammar.nonterminal_at(string));
      } catch(const std::out_of_range& e) {
	try {
	  m_symbols.push_back(&m_grammar.token_at(string));
	} catch(const std::out_of_range& e) {
	  throw std::runtime_error("Undefined symbol " + string);
	}
      }
      return Payload();
    }
    return Payload();
  case generated::Token::Colon:
    m_mode = Mode::Append;
    return Payload();
  case generated::Token::Comma:
    return Payload();
  case generated::Token::Semicolon:
    m_mode = Mode::Nonterminal;
    return Payload();
  case generated::Token::Tokens_Keyword:
    return Payload();
  case generated::Token::Open_Bracket:
    return Payload();
  case generated::Token::Close_Bracket:
    m_mode = Mode::Nonterminal;
    return Payload();
  case generated::Token::Hash:
    m_mode = Mode::Production;
    return Payload();
  default:
    throw std::runtime_error("Unknown token");
  }
}

const Grammar& Callback::grammar() const
{
  return m_grammar;
}
