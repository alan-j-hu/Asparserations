#include "../../codegen/include/json_generator.hpp"
#include "../../grammar/include/grammar.hpp"
#include "../../table/include/lr_table.hpp"
#include "../include/grammar_syntax.hpp"
#include <iostream>
#include <memory>

int main(int argc, char** argv)
{
  using namespace asparserations;
  std::unique_ptr<grammar::Grammar> g = bootstrap::grammar_syntax();
  table::LR_Table t(*g);
  codegen::JSON_Generator j(t, true);
  std::cout << j.code() << std::endl;
  return 0;
}
