#include "../../grammar/include/grammar.hpp"
#include <iostream>

void print_first_set(const asparserations::grammar::Symbol& s)
{
  std::cout << s.id() << ":" << std::endl;
  for(const asparserations::grammar::Token* token : s.first_set()) {
    std::cout << token->id() << std::endl;
  }
}

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
  dragon_book.compute_first_sets();

  print_first_set(S);
  print_first_set(C);
  print_first_set(c);
  print_first_set(d);

  //Grammar with empty string in a first set
  asparserations::grammar::Grammar e_str("foo");
  asparserations::grammar::Token& foo = e_str.add_token("foo");
  asparserations::grammar::Token& bar = e_str.add_token("bar");
  asparserations::grammar::Nonterminal& e = e_str.add_nonterminal("e");
  asparserations::grammar::Nonterminal& f = e_str.add_nonterminal("f");
  asparserations::grammar::Nonterminal& h = e_str.add_nonterminal("h");
  asparserations::grammar::Nonterminal& i = e_str.add_nonterminal("i");
  e.add_production("empty", {});
  e.add_production("2nd", {&e});
  f.add_production("carry", {&e, &foo});
  f.add_production("empty", {});
  h.add_production("h's", {&h});
  i.add_production("carry2", {&f});
  e_str.compute_first_sets();
  print_first_set(e);
  print_first_set(f);
  print_first_set(h);
  print_first_set(i);
  return 0;
}
