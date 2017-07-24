#include "../include/scheme_code_generator.hpp"
#include "../../table/include/table.hpp"

using namespace asparserations;
using namespace table;
using namespace codegen;

Scheme_Code_Generator::Scheme_Code_Generator(const table::Table& table)
  : _table(table) {_generate();}

const std::string& Scheme_Code_Generator::code() const {return _code;}

void Scheme_Code_Generator::_generate()
{
  _code =
    "(define (make-parse-table states)\n"
    "\n"
    "  (lambda (message)\n"
    "    (cond ((eq? message 'states) states)\n"
    "          (else '())\n"
    "    )\n"
    "  )\n"
    ")\n"
    "\n"
    "(define (make-state transitions reductions)\n"
    "  (lambda (message)\n"
    "    (cond ((eq? message 'transitions) transitions)\n"
    "          ((eq? message 'reductions) reductions)\n"
    "          (else '())\n"
    "    )\n"
    "  )\n"
    ")\n"
    "\n"
    "(define parse-table (make-parse-table (list \n";
  for(auto& state : _table.states()) {
    _code += "  (make-state (list \n";
    for(auto& transition : _state.transitions()) {
      _code += "    (cons ";
      _code += "";
      _code += " ";
      _code += "";
      _code += ")\n";
    }
    _code += "    ) (list \n";
    for(auto& reduction : _state.reductions()) {
      _code += "    (cons ";
      _code += ")\n";
    }
    _code += "    )\n";
    _code += "  )\n";
  }
}
