#include "item.hpp"
#include "item_set.hpp"
#include "state.hpp"
#include <map>
#include <set>

namespace asparserations {
  namespace grammar {
    class Production;
    class Token;
  }
  namespace table {
    class LALR_Item_Set
    {
    public:
      class Abstract_Item
      {
	friend class LALR_Item_Set;
      public:
	const Item& item() const;
	void add_child(const Abstract_Item&);
	void add_reduction(const std::set<const grammar::Production*>&);
	void propagate(const grammar::Token&);
      private:
	const Item _item;
	LALR_Item_Set& _owner;
	std::set<Abstract_Item> _children;
	std::set<const grammar::Production*> _reductions;
	Abstract_Item(const Item&, LALR_Item_Set&);
      };
      LALR_Item_Set(State&);
      std::list<Abstract_Item>& abstract_items();
      Abstract_Item& add_abstract_item(const Item&);
      void add_concrete_item(const Item&);
      void add_reduction(const std::pair<const grammar::Token*,
			                 std::set<const grammar::Production*>>&);
    private:
      std::list<Abstract_Item> _abstract_items;
      Item_Set _concrete_item_set;
      State& _state;
      std::map<const grammar::Token* const,std::set<const grammar::Production*>>
        _concrete_reductions;
      friend class Abstract_Item;
    };
  }
}
