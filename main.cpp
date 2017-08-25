#include "autogen/include/Parser.hpp"
#include "bootstrap/include/callback.hpp"
#include "bootstrap/include/lexer.hpp"
#include "codegen/include/json_generator.hpp"
#include "grammar/include/grammar.hpp"
#include "table/include/lr_table.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
  if(argc < 4) {
    std::cout << "Not enough args" << std::endl;
    return -1;
  }
  std::ifstream grammar_file(argv[1], std::ios_base::in | std::ios_base::ate);
  auto size = grammar_file.tellg();

  // std::string::data() doesn't return mutable char* until C++17, so I need my
  // own char* pointer
  char* temp_heap_data = new char[size];
  grammar_file.seekg(std::ios_base::beg);
  grammar_file.read(temp_heap_data, size);
  std::string grammar_str(temp_heap_data, temp_heap_data + size);
  // Can I use move semantics so that grammar takes ownership of heap pointer,
  // rather than copying it, then making me delete it?
  delete temp_heap_data;
  
  asparserations::grammar::Grammar grammar(argv[2]);
  asparserations::bootstrap::Lexer lexer;
  asparserations::bootstrap::Callback callback(grammar);
  asparserations::_generated::Parser parser(lexer, callback);

  // Discard the Node* return value; all necessary info is in the callback obj
  delete parser.parse(grammar_str);

  asparserations::table::LR_Table table(grammar);
  asparserations::codegen::JSON_Generator code(table, true);
  std::ofstream output_file(argv[3]);
  output_file.write(code.code().data(), code.code().size());
  return 0;
}
