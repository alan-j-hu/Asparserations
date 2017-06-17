#include "../include/print_states.hpp"
#include "../../grammar/include/grammar.hpp"
#include "../../table/include/lr_table.hpp"

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

  asparserations::table::LR_Table table(dragon_book);
  asparserations::print_states(table);
  return 0;
}
