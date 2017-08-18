#ifndef _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_
#define _ASPARSERATIONS_GRAMMAR_GRAMMAR_H_

#include "nonterminal.hpp"
#include "token.hpp"
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace asparserations {
  namespace grammar {
    class Grammar
    {
    public:
      //Constructor
      Grammar(const std::string&);
      //Move constructor
      Grammar(Grammar&&);

      /**
	 Adds a token to the grammar
	 @param id the name of the token
	 @return a reference to the newly added token
       */
      Token& add_token(const std::string&);

      /**
	 Adds a nonterminal to the grammar
	 @param id the name of the nonterminal
	 @return a reference to the newly added nonterminal
       */
      Nonterminal& add_nonterminal(const std::string&);

      /**
	 Gets token by id
	 @param the key of the token
	 @return a reference to the corresponding token
       */
      Token& token_at(const std::string&);
      const Token& token_at(const std::string&) const;

      /**
	 Gets nonterminal by id
	 @param the key of the nonterminal
	 @return a reference to the nonterminal
       */
      Nonterminal& nonterminal_at(const std::string&);
      const Nonterminal& nonterminal_at(const std::string&) const;

      const std::vector<Nonterminal*>& nonterminals() const;
      const std::vector<Token*>& tokens() const;
      const Nonterminal& accept() const;
      const Token& end() const;

      /**
	 Returns the start symbol
       */
      Nonterminal& start_symbol();
      const Nonterminal& start_symbol() const;

      /**
	 Sets the start symbol
       */
      void set_start_symbol(Nonterminal*);

      /**
	 Computes the FIRST sets of the symbols - the set of tokens that the
	 symbol can start with

	 Only call after grammar has been completely constructed!
       */
      void compute_first_sets();
    private:
      Grammar(const Grammar&) = delete;

      struct TokenImp : public Token
      {
	//Methods
	TokenImp(Grammar& g, const std::string& id);
	const std::string& id() const;
        Grammar& grammar();
        const Grammar& grammar() const;
        const std::set<const Token*>& first_set() const;
        const std::map<std::string,Production>& productions() const;
	bool is_token() const;
        bool derives_empty_string() const;
	//Members
	const std::string _id;
        Grammar* _grammar;
        std::set<const Token*> _first_set;
        const std::map<std::string,Production> _productions;
      };

      struct NonterminalImp : public Nonterminal
      {
	//Methods
        NonterminalImp(Grammar& g, const std::string& id);	
        const std::map<std::string,Production>& productions() const;
        Production& production_at(const std::string&);
        const Production& production_at(const std::string&) const;
        const std::string& id() const;
        Grammar& grammar();
        const Grammar& grammar() const;
        const std::set<const Token*>& first_set() const;
	bool is_token() const;
        bool derives_empty_string() const;
        Production& add_production(const std::string&,
				   std::vector<const Symbol*>);
	//Members
	const std::string _id;
        Grammar* _grammar;
        std::map<std::string,Production> _productions;
	std::set<const Token*> _first_set;
	bool _derives_empty_string;
      };

      std::vector<Token*> _token_vec;
      std::vector<Nonterminal*> _nonterminal_vec;
      std::map<std::string,TokenImp> _tokens;
      std::map<std::string,NonterminalImp> _nonterminals;
      Nonterminal* _start_symbol;
      TokenImp _end;
      NonterminalImp _accept;
    };
  }
}

#endif
