CXX = g++
CXXFLAGS = -std=c++11 -c

grammar/include/symbol.hpp : grammar/include/production.hpp
grammar/include/nonterminal.hpp : grammar/include/production.hpp \
				  grammar/include/symbol.hpp \
				  grammar/include/token.hpp
grammar/include/token.hpp : grammar/include/symbol.hpp

build/nonterminal.o : grammar/include/nonterminal.hpp
	$(CXX) $(CXXFLAGS) -o build/nonterminal.o grammar/src/nonterminal.cpp

build/token.o : grammar/include/token.hpp
	$(CXX) $(CXXFLAGS) -o build/token.o grammar/src/token.cpp

build/generator.o : table/src/generator.cpp
	$(CXX) $(CXXFLAGS) -o build/generator.o table/src/generator.cpp

build/item.o : table/src/item.cpp
	$(CXX) $(CXXFLAGS) -o build/item.o table/src/generator.cpp
