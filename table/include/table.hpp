#ifndef _ASPARSERATIONS_TABLE_GENERATOR_H_
#define _ASPARSERATIONS_TABLE_GENERATOR_H_

#include "state.hpp"
#include "item.hpp"
#include <list>
#include <map>
#include <set>

namespace asparserations {
  namespace grammar {
    class Grammar;
    class Production;
    class Symbol;
    class Token;
  }
  namespace table {
    class Item_Set;
    class Table
    {
    public:
      virtual const grammar::Grammar& grammar() const=0;
      virtual const std::list<State>& states() const=0;
    };
    /**
       Computes the closed set of items
       @param  item_set the kernel item set
       @return the closed set of items
     */
    std::set<Item> closure(const Item_Set&);

    /**
       Shifts the marker over by one for each item, or reduces it
       if the marker has reached the end
       @param  items the set of items to process
       @return a map of pointers to Symbols to pairs consisting of shifted
       items and productions for reductions
     */
    std::pair<std::map<const grammar::Symbol*,std::set<Item>>,
              std::map<const grammar::Token*,
		       std::set<const grammar::Production*>>>
    gotos(const std::set<Item>&);
  }
}

#endif
