#include "autogen/include/Parser.hpp"
#include "bootstrap/include/callback.hpp"
#include "bootstrap/include/lexer.hpp"
#include "codegen/include/json_generator.hpp"
#include "grammar/include/grammar.hpp"
#include "table/include/lalr_table.hpp"
#include "table/include/lr_table.hpp"
#include "table/include/table.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace {
  enum class Argument
  {
    Argument,
    Optional_Argument,
    No_Argument
  };
}

int main(int argc, char** argv)
{

  bool use_lalr = false;
  std::string output_filename = "a.out.json";
  std::string root = "Root";
  std::vector<std::string> positional_args;
  std::map<char,Argument> flags {
    {'o', Argument::Argument},
    {'r', Argument::Argument},
    {'l', Argument::No_Argument}
  };
  std::map<std::string,const std::pair<const char,Argument>*>
    option_abbreviations {
      {"out", &*flags.find('o')},
      {"root", &*flags.find('r')},
      {"lalr", &*flags.find('l')} 
  };
  std::map<char,std::string> flag_values;

  Argument expects_argument = Argument::No_Argument;
  char char_flag = '\0';
  for(int i = 1; i < argc; ++i) {
    if(argv[i][0] == '-') { // Flag
      if(expects_argument == Argument::Argument) {
	std::cout << "Error: Expected argument, got flag" << std::endl;
	return -1;
      }
      expects_argument = Argument::No_Argument;
      if(argv[i][1] == '\0') {
	std::cout << "Error: single dash without flag" << std::endl;
	return -1;
      } else if(argv[i][1] == '-') { // Long flag name
	if(argv[i][2] == '\0') {
	  std::cout << "Error: double dash without flag" << std::endl;
	  return -1;
	}
	int j = 2;
	// Increment until null terminator or =
	for(j = 2; argv[i][j] != '\0' && argv[i][j] != '='; ++j);
	std::string name(argv[i] + 2, argv[i] + j);

        auto iter = option_abbreviations.find(name);
	if(iter == option_abbreviations.end()) {
	  std::cout << "Unknown option --" << name << std::endl;
	  return -1;
	}
	if(argv[i][j] == '=') {
	  std::string arg(argv[i] + j + 1);
	  if(flags[iter->second->first] == Argument::No_Argument) {
	    std::cout << "Error: No argument needed for "
		      << name << std::endl;
	    return -1;
	  }
	  flag_values[iter->second->first] = arg;
	  expects_argument = Argument::No_Argument;
	} else {
	  char_flag = iter->second->first;
	  expects_argument = iter->second->second;
	}
      } else if(argv[i][2] == '\0') { // One character after dash
	char_flag = argv[i][1];
	expects_argument = flags[char_flag];
      } else { // Chain of flags (e.g. -abcd)
	for(int j = 1; argv[i][j] != '\0'; ++j) {
	  flag_values[argv[i][j]] = "";
	}
      }
    } else if(expects_argument == Argument::No_Argument) {
      positional_args.emplace_back(argv[i]);
    } else {
      flag_values[char_flag] = argv[i];
      expects_argument = Argument::No_Argument;
    }
  }

  if(positional_args.size() < 1) {
    std::cout << "No file provided" << std::endl;
    return -1;
  }

  for(auto pair : flag_values) {
    switch(pair.first) {
    case 'l':
      use_lalr = true;
      break;
    case 'o':
      if(pair.second == "") {
	std::cout << "No output filename provided" << std::endl;
	return -1;
      }
      output_filename = pair.second;
      break;
    case 'r':
      if(pair.second == "") {
	std::cout << "No root nonterminal provided" << std::endl;
	return -1;
      }
      root = pair.second;
      break;
    }
  }

  std::ifstream grammar_file(positional_args[0],
			     std::ifstream::in | std::ifstream::ate);
  auto size = grammar_file.tellg();
  if(!grammar_file || size == -1) {
    std::cerr << "Unable to open file: " << positional_args[0] << std::endl;
    return -1;
  }
  // std::string::data() doesn't return mutable char* until C++17, so I need my
  // own char* pointer
  char* temp_heap_data = nullptr;
  try {
    temp_heap_data = new char[size];
  } catch(std::bad_alloc& ex) {
    grammar_file.close();
    std::cerr << "Failed to allocate memory." << std::endl;
    return -1;
  }
  grammar_file.seekg(std::ios_base::beg);
  grammar_file.read(temp_heap_data, size);
  std::string grammar_str(temp_heap_data, temp_heap_data + size);
  // Can I use move semantics so that grammar takes ownership of heap pointer,
  // rather than copying it, then making me delete it?
  delete temp_heap_data;
    
  asparserations::grammar::Grammar grammar(root);
  asparserations::bootstrap::Lexer lexer;
  asparserations::bootstrap::Callback callback(grammar);
  asparserations::_generated::Parser parser(lexer, callback);

  // Discard the Node* return value; all necessary info is in the callback obj
  delete parser.parse(grammar_str);

  asparserations::table::Table* table;
  if(use_lalr) {
    table = new asparserations::table::LALR_Table(grammar);
  } else {
    table = new asparserations::table::LR_Table(grammar);
  }
  asparserations::codegen::JSON_Generator code(*table, true);
  std::ofstream output_file(output_filename);
  output_file.write(code.code().data(), code.code().size());
  delete table;
  return 0;
}
