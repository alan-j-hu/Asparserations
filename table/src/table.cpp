#include "../include/table.hpp"
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
    std::set<const Token*> lookaheads =
      (item->marker < item->production.symbols().size() - 1
        ? item->peek()->first_set()
       : std::set<const Token*> {&item->lookahead}
      );

    for(const Production& production : item->next()->productions()) {
      for(const Token* lookahead : lookaheads) {
	auto result = items.insert(Item(production, 0, *lookahead));
	if(result.second) {
	  queue.push_back(&*result.first);
	}
      }
    }
  }

  return items;
}

std::map<const Symbol*,std::pair<std::set<Item>,std::set<const Production*>>>
Table::_goto(const std::set<Item>& items) const
{
  std::map<const Symbol*,
	   std::pair<std::set<Item>,std::set<const Production*>>> ret;
  for(const Item& item : items) {
    if(item.marker < item.production.symbols().size() - 1) {
      ret[item.next()].first.emplace(item.production,
			             item.marker + 1,
			             item.lookahead);
    } else {
      ret[&item.lookahead].second.insert(&item.production);
    }
  }

  return ret;
}
