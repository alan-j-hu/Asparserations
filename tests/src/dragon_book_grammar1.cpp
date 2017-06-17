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
