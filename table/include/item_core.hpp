#ifndef _ASPARSERATIONS_TABLE_ITEM_CORE_H_
#define _ASPARSERATIONS_TABLE_ITEM_CORE_H_

namespace asparserations {
  namespace table {
    class Item_Core
    {
      Item_Core(const asparserations::grammar::Production&, unsigned short int);

      const asparserations::grammar::Symbol* next() const;
      const asparserations::grammar::Symbol* peek() const;
      const asparserations::grammar::Production& production;
      const unsigned short int marker;
    };
  }
}

#endif
