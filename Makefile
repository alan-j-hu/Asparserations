CXX = g++
CXXFLAGS = -std=c++11 -c

grammar/include/symbol.hpp : grammar/include/production.hpp
grammar/src/symbol.cpp : grammar/include/symbol.hpp

grammar/include/nonterminal.hpp : grammar/include/production.hpp \
				  grammar/include/symbol.hpp \
				  grammar/include/token.hpp
grammar/src/nonterminal.cpp : grammar/include/nonterminal.hpp

grammar/include/token.hpp : grammar/include/symbol.hpp
grammar/src/token.cpp : grammar/include/token.hpp

table/include/generator.hpp: grammar/include/symbol.hpp \
                             table/include/item_set.hpp
table/src/generator.cpp : table/include/generator.hpp

table/include/item.hpp : grammar/include/production.hpp \
                         grammar/include/symbol.hpp \
                         grammar/include/token.hpp
table/src/item.cpp : table/include/item.hpp

table/include/item_set.hpp : table/include/item.hpp
table/src/item_set.cpp : table/include/item_set.hpp

build/nonterminal.o : grammar/src/nonterminal.cpp
	$(CXX) $(CXXFLAGS) -o build/nonterminal.o grammar/src/nonterminal.cpp

build/token.o : grammar/include/token.hpp
	$(CXX) $(CXXFLAGS) -o build/token.o grammar/src/token.cpp

build/table.o : table/src/table.cpp
	$(CXX) $(CXXFLAGS) -o build/table.o table/src/table.cpp

build/lr_generator.o : table/src/lr_table.cpp
	$(CXX) $(CXXFLAGS) -o build/lr_table.o table/src/lr_table.cpp

build/item.o : table/src/item.cpp
	$(CXX) $(CXXFLAGS) -o build/item.o table/src/item.cpp

build/item_set.o : table/src/item_set.cpp
	$(CXX) $(CXXFLAGS) -o build/item_set.o table/src/item_set.cpp

build/state.o : table/src/state.cpp
	$(CXX) $(CXXFLAGS) -o build/state.o table/src/state.cpp
