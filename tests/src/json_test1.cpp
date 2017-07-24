#include "../../codegen/include/json_generator.hpp"
#include "../../grammar/include/grammar.hpp"
#include "../../grammar/include/nonterminal.hpp"
#include "../../grammar/include/token.hpp"
#include "../../table/include/lr_table.hpp"
#include <iostream>

int main()
{
  //Grammar in the dragon book
  asparserations::grammar::Grammar dragon_book("S");
  asparserations::grammar::Nonterminal& S = dragon_book.add_nonterminal("S");
  asparserations::grammar::Nonterminal& C = dragon_book.add_nonterminal("C");
  asparserations::grammar::Token& c = dragon_book.add_token("c");
  asparserations::grammar::Token& d = dragon_book.add_token("d");

  S.add_production("1", {&C, &C});
  C.add_production("1", {&c, &C});
  C.add_production("2", {&d});

  std::cout << "Foo" << std::endl;
  asparserations::table::LR_Table table(dragon_book);
  std::cout << "Foo" << std::endl;
  asparserations::codegen::JSON_Generator gen(table, true);
  std::cout << "Foo" << std::endl;
  std::cout << gen.code() << std::endl;
  return 0;
}
