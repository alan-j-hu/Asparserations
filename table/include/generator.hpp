#ifndef _ASPARSERATIONS_TABLE_GENERATOR_H_
#define _ASPARSERATIONS_TABLE_GENERATOR_H_

#include "../../grammar/include/symbol.hpp"
#include "item_set.hpp"
#include <map>
#include <set>

namespace asparserations {
  namespace table {
    class Generator
    {
    public:
    protected:
      std::set<Item> _closure(const Item_Set&) const;
      std::map<const asparserations::grammar::Symbol*,
	       std::set<Item>> _goto(const std::set<Item>&) const;
    };
  }
}

#endif
