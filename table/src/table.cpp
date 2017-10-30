#include "../include/item_set.hpp"
#include "../include/table.hpp"
#include "../../grammar/include/symbol.hpp"
#include "../../grammar/include/token.hpp"
#include <list>

using namespace asparserations;
using namespace grammar;
using namespace table;

std::set<Item> table::closure(const Item_Set& item_set)
{
  std::set<Item> items(item_set.items());
  std::list<const Item*> queue;

  for(const Item& item : items) {
    queue.push_back(&item);
  }

  for(const Item* item : queue) {
    if(item->marker < item->production.symbols().size()) {
      // Handle the fact that first sets can have the empty string
      for(auto& pair : item->next()->productions()) {
        const Production& production = pair.second;
        bool inherits_lookahead = true;
        for(int i=item->marker+1; i < item->production.symbols().size(); ++i) {
          for(auto& lookahead : item->peek()->first_set()) {
            auto result = items.insert(Item(production, 0, lookahead.get()));
	    if(result.second) {
	      queue.push_back(&*result.first);
	    }
          }
          if(!item->peek()->derives_empty_string()) {
            inherits_lookahead = false;
	    break;
          }
        }
        if(inherits_lookahead) {
          auto result = items.insert(Item(production, 0, item->lookahead));
	  if(result.second) {
	    queue.push_back(&*result.first);
	  }
        }
      }
    }
  }
  return items;
}

std::pair<std::map<std::reference_wrapper<const Symbol>,std::set<Item>>,
	  std::map<std::reference_wrapper<const Token>,
                   std::set<std::reference_wrapper<const Production>>>>
table::gotos(const std::set<Item>& items)
{
  std::pair<std::map<std::reference_wrapper<const Symbol>,std::set<Item>>,
            std::map<std::reference_wrapper<const Token>,
                     std::set<std::reference_wrapper<const Production>>>> ret;
  for(const Item& item : items) {
    if(item.marker < item.production.symbols().size()) {
      ret.first[std::cref(*item.next())].emplace(item.production,
                                     item.marker + 1,
                                     item.lookahead);
    } else {
      ret.second[std::cref(item.lookahead)].insert(std::cref(item.production));
    }
  }

  return ret;
}
