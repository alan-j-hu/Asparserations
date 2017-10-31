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
      throw std::runtime_error("Unknown production "
        + Parser::production_to_string(production) + " in _accept_");
    }
  case generated::Nonterminal::Root:
    switch(production) {
    case generated::Production::main:
      return Payload();
    default:
      throw std::runtime_error("Unknown production "
        + Parser::production_to_string(production) + "in root");
    }
  case generated::Nonterminal::Identifier_List:
    switch(production) {
    case generated::Production::recursive_case:
    case generated::Production::base_case:
      return Payload();
    default:
      throw std::runtime_error("Unknown production "
        + Parser::production_to_string(production) + " in Identifier_List");
    }
  case generated::Nonterminal::Nonterminal_List:
    switch(production) {
    case generated::Production::main:
    case generated::Production::empty:
      return Payload();
    default:
      throw std::runtime_error("Unknown production "
        + Parser::production_to_string(production) + " in Nonterminal_List");
    }
  case generated::Nonterminal::Nonterminal:
    switch(production) {
    case generated::Production::main:
      m_nonterminal
        = &m_grammar.add_nonterminal(std::string(children[0]->state().begin,
                  	                         children[0]->state().end));
      return Payload();
    default:
      throw std::runtime_error("Unknown production "
        + Parser::production_to_string(production) + " in Nonterminal");
    }
  case generated::Nonterminal::Production_List:
    switch(production) {
    case generated::Production::recursive_case:
    case generated::Production::base_case:
      return Payload();
    default:
      throw std::runtime_error("Unknown production in "
        + Parser::production_to_string(production) + " Production_List");
    }
  case generated::Nonterminal::Production:
    switch(production) {
    case generated::Production::main:
      if(m_nonterminal == nullptr) {
	throw std::runtime_error("Null pointer");
      }
      m_nonterminal->add_production(std::string(children[2]->state().begin,
                                                children[2]->state().end),
		                    m_symbols
      );
      m_symbols = std::vector<const Symbol*>();
      return Payload();
    default:
      throw std::runtime_error("Unknown production "
        + Parser::production_to_string(production) + " in Production");
    }
  case generated::Nonterminal::Symbol_List:
    switch(production) {
    case generated::Production::main:
    case generated::Production::explicit_tok:
    case generated::Production::empty:
      return Payload();
    default:
      throw std::runtime_error("Unknown production "
        + Parser::production_to_string(production) + " in Symbol_List");
    }
  default:
    throw std::runtime_error("Unknown nonterminal");
  }
}

Callback::Payload Callback::call(generated::Token token,
				 const std::string& string)
{
  std::string tok_name;
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
      m_token_names.insert(string);
      return Payload();
    case Mode::Nonterminal:
      if(m_token_names.find(string) == m_token_names.end()) {
        m_nonterminal = &m_grammar.add_nonterminal(string);
        return Payload();
      }
      throw std::runtime_error("Error: " + string + " is a token name");
    case Mode::Append:
      if(m_token_names.find(string) == m_token_names.end()) {
          m_symbols.push_back(&m_grammar.add_nonterminal(string));
      } else {
        m_symbols.push_back(&m_grammar.token_at(string));
      }
      return Payload();
    }
    return Payload();
  case generated::Token::Prime_Identifier:
    tok_name = std::string(string, 1, std::string::npos);
    if(m_token_names.find(tok_name) == m_token_names.end()) {
      throw std::runtime_error("Undefined token: " + tok_name);
    } else {
      m_symbols.push_back(&m_grammar.token_at(tok_name));
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
