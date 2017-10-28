#ifndef ASPARSERATIONS_TABLE_ITEM_H_
#define ASPARSERATIONS_TABLE_ITEM_H_

namespace asparserations {
  namespace grammar {
    class Production;
    class Symbol;
    class Token;
  }
  namespace table {
    struct Item
    {
      Item(const asparserations::grammar::Production&, unsigned short int,
	   const asparserations::grammar::Token&);

      const asparserations::grammar::Symbol* next() const;
      const asparserations::grammar::Symbol* peek() const;
      bool compare_cores(const Item&) const;

      const asparserations::grammar::Production& production;
      const unsigned short int marker;
      const asparserations::grammar::Token& lookahead;
    };

    bool operator<(const Item&, const Item&);
  }
}

#endif
