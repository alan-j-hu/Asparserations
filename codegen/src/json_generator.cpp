#include "../include/json_generator.hpp"
#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/production.hpp"
#include "../../grammar/include/token.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../table/include/state.hpp"
#include "../../table/include/table.hpp"

using namespace asparserations;
using namespace grammar;
using namespace table;
using namespace codegen;

JSON_Generator::JSON_Generator(const Table& table, bool pretty_print,
			       const std::string& tab = "  ")
  : _table(table),
    _grammar(table.grammar()),
    _pretty_print(pretty_print),
    _indent_depth(0),
    _tab(tab)
{
  _generate();
}

const std::string& JSON_Generator::code() const {return _code;}

void JSON_Generator::_break_and_indent()
{
  if(_pretty_print) {
    _code += "\n";
    for(unsigned int i = 0; i < _indent_depth; ++i) {
      _code += _tab;
    }
  }
}

void JSON_Generator::_generate()
{
  _code += "{";
  ++_indent_depth;
  _break_and_indent();
  _code += "\"grammar\" : ";
  _generate_grammar(_grammar);
  _code += ",";
  _break_and_indent();
  _code += "\"table\" : ";
  _generate_table(_table);
  --_indent_depth;
  _break_and_indent();
  _code += "}";
  
}

void JSON_Generator::_generate_token(const Token& token)
{
  _break_and_indent();
  _code += "\"" + token.id() + "\"";
}

void JSON_Generator::_generate_nonterminal(const Nonterminal& n)
{
  _code += "\"" + n.id() + "\" : {";
  ++_indent_depth;
  bool needs_comma = false;
  for(auto& pair : n.productions()) {
    const Production& production = pair.second;
    _break_and_indent();
    if(needs_comma) {
      _code += ",";
    } else {
      needs_comma = true;
    }
    _code += "\"" + production.id() + "\" : [";
    ++_indent_depth;
    bool needs_comma2 = false;
    for(const Symbol* const symbol : production.symbols()) {
      if(needs_comma2) {
	_code += ",";
      } else {
	needs_comma2 = true;
      }
      _break_and_indent();
      _code += "{";
      ++_indent_depth;
      _break_and_indent();
      _code += "\"id\" : \"" + symbol->id() + "\",";
      _break_and_indent();
      _code += "\"isToken\" : " + std::to_string(symbol->is_token());
      --_indent_depth;
      _break_and_indent();
      _code += "}";
    }
    --_indent_depth;
    _break_and_indent();
    _code += "]";
  }
  --_indent_depth;
  _break_and_indent();
  _code += "}";
}

void JSON_Generator::_generate_grammar(const Grammar& grammar)
{
  _code = "{";
  ++_indent_depth;
  _break_and_indent();
  //Tokens
  _code += "\"tokens\" : [";
  ++_indent_depth;
  _generate_token(grammar.end());
  for(const Token* token : grammar.tokens()) {
    _generate_token(*token);
  }
  --_indent_depth;
  _break_and_indent();
  _code += "],";
  _break_and_indent();
  //Nonterminals
  _code += "\"nonterminals\" : [";
  ++_indent_depth;
  _break_and_indent();
  _generate_nonterminal(grammar.start_symbol());
  for(const Nonterminal* nonterminal : grammar.nonterminals()) {
    _code += ",";
    _break_and_indent();
    _generate_nonterminal(*nonterminal);
  }
  --_indent_depth;
  _break_and_indent();
  _code += "]";
  --_indent_depth;
  _break_and_indent();
  _code += "}";
}

void JSON_Generator::_generate_state(const State& state)
{
  _code += "{";
  ++_indent_depth;
  _break_and_indent();
  _code += "\"shifts\" : {";
  ++_indent_depth;
  bool needs_comma = false;
  for(auto& transition : state.transitions()) {
    const Symbol& symbol = *transition.first;
    const State& state = *transition.second;
    if(symbol.is_token()) {
      if(needs_comma) {
        _code += ",";
      } else {
        needs_comma = true;
      }
      _break_and_indent();
      _code += "\"" + symbol.id() + "\" : " + std::to_string(state.index());
    }
  }
  --_indent_depth;
  _break_and_indent();
  _code += ",";
  _break_and_indent();
  _code += "\"reductions\" : {";
  ++_indent_depth;
  needs_comma = false;
  for(auto& reduction : state.reductions()) {
    const Symbol& symbol = *reduction.first;
    if(needs_comma) {
      _code += ",";
    } else {
      needs_comma = true;
    }
    _break_and_indent();
    _code += "\"" + symbol.id() + "\" : [";
    ++_indent_depth;
    for(auto& production : reduction.second) {
      _break_and_indent();
      _code += "{";
      ++_indent_depth;
      _break_and_indent();
      _code += "\"nonterminal\" : \"" + production->nonterminal().id() + "\",";
      _break_and_indent();
      _code += "\"production\" : \"" + production->id() + "\"";
      --_indent_depth;
      _break_and_indent();
      _code += "}";
    }
    --_indent_depth;
    _break_and_indent();
    _code += "]";
    --_indent_depth;
    _break_and_indent();
    _code += "},";
    _break_and_indent();
    _code += "\"gotos\" : {";
    ++_indent_depth;
    needs_comma = false;
    for(auto& transition : state.transitions()) {
      const Symbol& symbol = *transition.first;
      const State& state = *transition.second;
      if(!symbol.is_token()) {
        if(needs_comma) {
	  _code += ",";
        } else {
	  needs_comma = true;
        }
        _break_and_indent();
	_code += "\"" + symbol.id() + "\" : " + std::to_string(state.index());
      }
    }
    --_indent_depth;
    _break_and_indent();
    _code += "}";
  }
  --_indent_depth;
  _break_and_indent();
  _code += "}";
};

void JSON_Generator::_generate_table(const Table& table)
{
  _code += "[";
  ++_indent_depth;
  bool insert_comma = false;
  for(const State& state : table.states()) {
    if(insert_comma) {
      _code += ",";
    } else {
      insert_comma = true;
    }
    _break_and_indent();
    _generate_state(state);
  }
  --_indent_depth;
  _break_and_indent();
  _code += "]";
}
