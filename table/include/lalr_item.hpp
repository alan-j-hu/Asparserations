#ifndef _ASPARSERATIONS_TABLE_LALR_ITEM_
#define _ASPARSERATIONS_TABLE_LALR_ITEM_

namespace asparserations {
  namespace grammar {
    class Production;
  }
  namespace table {
    struct LALR_Item
    {
      const grammar::Production& production;
      const unsigned int marker;
    };
  }
}

#endif
