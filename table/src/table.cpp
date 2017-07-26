#include "../include/item_set.hpp"
#include "../include/table.hpp"
#include "../../grammar/include/symbol.hpp"
#include "../../grammar/include/token.hpp"
#include <list>

using namespace asparserations;
using namespace grammar;
using namespace table;

std::set<Item> Table::_closure(const Item_Set& item_set) const
{
  std::set<Item> items(item_set.items());
  std::list<const Item*> queue;

  for(const Item& item : items) {
    queue.push_back(&item);
  }

  for(const Item* item : queue) {
    if(item->marker < item->production.symbols().size()) {
      std::set<const Token*> lookaheads =
        (item->marker < item->production.symbols().size() - 1
          ? item->peek()->first_set()
         : std::set<const Token*> {&item->lookahead}
        );

      for(const auto& elem : item->next()->productions()) {
        const Production& production = elem.second;
        for(const Token* lookahead : lookaheads) {
	  auto result = items.insert(Item(production, 0, *lookahead));
	  if(result.second) {
	    queue.push_back(&*result.first);
	  }
        }
      }
    }
  }

  return items;
}

//std::map<const Symbol*,std::pair<std::set<Item>,std::set<const Production*>>>
std::pair<std::map<const Symbol*,std::set<Item>>,
	  std::map<const Token*,std::set<const Production*>>>
Table::_goto(const std::set<Item>& items) const
{
  std::pair<std::map<const Symbol*,std::set<Item>>,
            std::map<const Token*,std::set<const Production*>>> ret;
  /*
  std::map<const Symbol*,std::pair<std::set<Item>,std::set<const Production*>>>
    ret;
  */
  for(const Item& item : items) {
    if(item.marker < item.production.symbols().size()) {
      ret.first[item.next()].emplace(item.production,
                                     item.marker + 1,
                                     item.lookahead);
      /*
      ret[item.next()].first.emplace(item.production,
			             item.marker + 1,
			             item.lookahead);
      */
    } else {
      ret.second[&item.lookahead].insert(&item.production);
      //ret[&item.lookahead].second.insert(&item.production);
    }
  }

  return ret;
}
