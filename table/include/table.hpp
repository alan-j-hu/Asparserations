#ifndef _ASPARSERATIONS_TABLE_GENERATOR_H_
#define _ASPARSERATIONS_TABLE_GENERATOR_H_

#include "../../grammar/include/production.hpp"
#include "../../grammar/include/symbol.hpp"
#include "../../grammar/include/token.hpp"
#include "state.hpp"
#include "item_set.hpp"
#include <map>
#include <set>

namespace asparserations {
  namespace table {
    class Table
    {
    public:
      virtual const std::list<State>& states() const=0;
    protected:
      /**
	 Computes the closed set of items
	 @param  item_set the kernel item set
	 @return the closed set of items
       */
      std::set<Item> _closure(const Item_Set&) const;

      /**
	 Shifts the marker over by one for each item, or reduces it
	 if the marker has reached the end
	 @param  items the set of items to process
	 @return a map of pointers to Symbols to pairs consisting of shifted
         items and productions for reductions
       */
      std::map<const asparserations::grammar::Symbol*,
	       std::pair<std::set<Item>,
			 std::set<const asparserations::grammar::Production*>>>
      _goto(const std::set<Item>&) const;
    };
  }
}

#endif
