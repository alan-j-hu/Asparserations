#include "../include/json_generator.hpp"
#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/production.hpp"
#include "../../grammar/include/token.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../table/include/state.hpp"
#include "../../table/include/table.hpp"

#include <iostream>
#include <stdexcept>

using namespace asparserations;
using namespace grammar;
using namespace table;
using namespace codegen;

JSON_Generator::JSON_Generator(const Table& table, bool pretty_print,
                	       bool debug, const std::string& tab)
  : m_table(table),
    m_grammar(table.grammar()),
    m_pretty_print(pretty_print),
    m_indent_depth(0),
    m_debug(debug),
    m_tab(tab)
{
  m_generate();
}

const std::string& JSON_Generator::code() const {return m_code;}

void JSON_Generator::m_break_and_indent()
{
  if(m_indent_depth > 10) {
    std::cout << m_code << std::endl;
    throw std::runtime_error("Int underflow");
  }
  if(m_pretty_print) {
    m_code += "\n";
    for(unsigned int i = 0; i < m_indent_depth; ++i) {
      m_code += m_tab;
    }
  }
}

void JSON_Generator::m_generate()
{
  m_code += "{";
  ++m_indent_depth;
  m_break_and_indent();
  m_code += "\"grammar\" : ";
  m_generate_grammar(m_grammar);
  m_code += ",";
  m_break_and_indent();
  m_code += "\"table\" : ";
  m_generate_table(m_table);
  --m_indent_depth;
  m_break_and_indent();
  m_code += "}";
  
}

void JSON_Generator::m_generate_token(const Token& token)
{
  m_break_and_indent();
  m_code += "\"" + token.name() + "\"";
}

void JSON_Generator::m_generate_nonterminal(const Nonterminal& n)
{
  m_code += "\"" + n.name() + "\" : {";
  ++m_indent_depth;
  bool needs_comma = false;
  for(auto& production : n.productions()) {
    if(needs_comma) {
      m_code += ",";
    } else {
      needs_comma = true;
    }
    m_break_and_indent();
    m_code += "\"" + production.name() + "\" : [";
    ++m_indent_depth;
    bool needs_comma2 = false;
    for(const Symbol* const symbol : production.symbols()) {
      if(needs_comma2) {
        m_code += ",";
      } else {
	needs_comma2 = true;
      }
      m_break_and_indent();
      m_code += "{";
      ++m_indent_depth;
      m_break_and_indent();
      m_code += "\"name\" : \"" + symbol->name() + "\",";
      m_break_and_indent();
      m_code += "\"isToken\" : "
        + std::string((symbol->is_token() ? "true" : "false"));
      --m_indent_depth;
      m_break_and_indent();
      m_code += "}";
    }
    --m_indent_depth;
    m_break_and_indent();
    m_code += "]";
  }
  --m_indent_depth;
  m_break_and_indent();
  m_code += "}";
}

void JSON_Generator::m_generate_grammar(const Grammar& grammar)
{
  m_code += "{";
  ++m_indent_depth;
  m_break_and_indent();
  // Tokens
  m_code += "\"tokens\" : [";
  ++m_indent_depth;
  m_generate_token(grammar.end());
  for(const Token* token : grammar.tokens()) {
    m_code += ",";
    m_generate_token(*token);
  }
  --m_indent_depth;
  m_break_and_indent();
  m_code += "],";
  m_break_and_indent();
  // Nonterminals
  m_code += "\"nonterminals\" : {";
  ++m_indent_depth;
  m_break_and_indent();
  m_generate_nonterminal(grammar.accept());
  for(const Nonterminal* nonterminal : grammar.nonterminals()) {
    m_code += ",";
    m_break_and_indent();
    m_generate_nonterminal(*nonterminal);
  }
  --m_indent_depth;
  m_break_and_indent();
  m_code += "}";
  --m_indent_depth;
  m_break_and_indent();
  m_code += "}";
}

void JSON_Generator::m_generate_actions(
  const std::map<std::reference_wrapper<const Token>,
                 std::pair<const State*,
                           std::set<std::reference_wrapper<const Production>>>>&
  actions)
{
  m_code += "{";
  ++m_indent_depth;
  bool needs_comma = false;
  for(auto& action : actions) {
    if(needs_comma) {
      m_code += ",";
    } else {
      needs_comma = true;
    }
    m_break_and_indent();
    m_code += "\"" + action.first.get().name() + "\" : {";
    ++m_indent_depth;
    m_break_and_indent();
    m_code += "\"shift\" : "
      + (action.second.first == nullptr
	 ? "null"
	 : std::to_string(action.second.first->index()))
      + ",";
    m_break_and_indent();
    m_code += "\"reduces\" : [";
    ++m_indent_depth;
    bool needs_comma2 = false;
    for(auto& production : action.second.second) {
      if(needs_comma2) {
	m_code += ",";
      } else {
	needs_comma2 = true;
      }
      m_break_and_indent();
      m_code += "{";
      ++m_indent_depth;
      m_break_and_indent();
      m_code += "\"nonterminal\" : \""
        + production.get().nonterminal().name() + "\",";
      m_break_and_indent();
      m_code += "\"production\" : \"" + production.get().name() + "\"";
      --m_indent_depth;
      m_break_and_indent();
      m_code += "}";
    }
    --m_indent_depth;
    m_break_and_indent();
    m_code += "]";
    --m_indent_depth;
    m_break_and_indent();
    m_code += "}";
  }
  --m_indent_depth;
  m_break_and_indent();
  m_code += "}";
}

void JSON_Generator::m_generate_gotos(
  const std::map<std::reference_wrapper<const Nonterminal>,const State*>& gotos
                                      )
{
  m_code += "{";
  ++m_indent_depth;
  bool needs_comma = false;
  for(auto& go_to : gotos) {
    if(needs_comma) {
      m_code += ",";
    } else {
      needs_comma = true;
    }
    m_break_and_indent();
    m_code += "\"" + go_to.first.get().name() + "\" : "
      + std::to_string(go_to.second->index());
  }
  --m_indent_depth;
  m_break_and_indent();
  m_code += "}";
}

void JSON_Generator::m_generate_item_set(const Item_Set& item_set)
{
  m_code += "[";
  ++m_indent_depth;
  bool needs_comma = false;
  for(const Item& item : item_set.items()) {
    if(needs_comma) {
      m_code += ",";
    } else {
      needs_comma = true;
    }
    m_break_and_indent();
    m_code += "{";
    ++m_indent_depth;
    m_break_and_indent();
    m_code += "\"production\" : {";
    ++m_indent_depth;
    m_break_and_indent();
    m_code += "\"nonterminal\" : \""
      + item.production.nonterminal().name() + "\",";
    m_break_and_indent();
    m_code += "\"production\" : \"" + item.production.name() + "\"";
    --m_indent_depth;
    m_break_and_indent();
    m_code += "},";
    m_break_and_indent();
    m_code += "\"marker\" : " + std::to_string(item.marker) + ",";
    m_break_and_indent();
    m_code += "\"lookahead\" : \"" + item.lookahead.name() + "\"";
    --m_indent_depth;
    m_break_and_indent();
    m_code += "}";
  }
  --m_indent_depth;
  m_break_and_indent();
  m_code += "]";
}

void JSON_Generator::m_generate_state(const State& state,
                                      const Item_Set* item_set)
{
  m_code += "{";
  ++m_indent_depth;
  m_break_and_indent();
  m_code += "\"index\" : " + std::to_string(state.index()) + ",";
  m_break_and_indent();
  m_code += "\"actions\" : ";
  m_generate_actions(state.actions());
  m_code += ",";
  m_break_and_indent();
  m_code += "\"gotos\" : ";
  m_generate_gotos(state.gotos());
  m_code += ",";
  m_break_and_indent();
  m_code += "\"itemSet\" : ";
  if(item_set == nullptr) {
    m_code += "null";
  } else {
    m_generate_item_set(*item_set);
  }
  --m_indent_depth;
  m_break_and_indent();
  m_code += "}";
};

void JSON_Generator::m_generate_table(const Table& table)
{
  m_code += "[";
  ++m_indent_depth;
  bool insert_comma = false;
  for(auto& pair : table.item_set_state_pairs()) {
    if(insert_comma) {
      m_code += ",";
    } else {
      insert_comma = true;
    }
    m_break_and_indent();
    if(m_debug) {
      m_generate_state(*pair.second, pair.first);
    } else {
      m_generate_state(*pair.second, nullptr);
    }
  }
  --m_indent_depth;
  m_break_and_indent();
  m_code += "]";
}
