#ifndef _ASPARSERATIONS_TABLE_ITEM_H_
#define _ASPARSERATIONS_TABLE_ITEM_H_

#include "../../grammar/include/production.hpp"
#include "../../grammar/include/symbol.hpp"
#include "../../grammar/include/token.hpp"

namespace asparserations {
  namespace table {
    struct Item
    {
      Item(const asparserations::grammar::Production&, unsigned short int,
	   const asparserations::grammar::Token&);
      const asparserations::grammar::Symbol* next() const;
      const asparserations::grammar::Symbol* peek() const;
      const asparserations::grammar::Production& production;
      const unsigned short int marker;
      const asparserations::grammar::Token& lookahead;
    };

    bool operator<(const Item&, const Item&);
  }
}

#endif
