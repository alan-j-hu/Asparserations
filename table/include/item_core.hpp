#ifndef ASPARSERATIONS_TABLE_ITEM_CORE_H_
#define ASPARSERATIONS_TABLE_ITEM_CORE_H_

namespace asparserations {
  namespace grammar {
    class Production;
  }
  namespace table {
    struct Item_Core
    {
      Item_Core(const grammar::Production&, unsigned short int);
      const grammar::Production& production;
      const unsigned short int marker;
    };
    bool operator<(const Item_Core&, const Item_Core&);
  }
}

#endif
