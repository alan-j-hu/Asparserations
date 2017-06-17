CXX = g++
CXXFLAGS = -std=c++11
GRAMMAR_OBJS = build/grammar.o build/nonterminal.o build/token.o \
build/production.o
TABLE_OBJS = build/table.o build/lr_table.o build/lalr_table.o \
build/item_set.o build/item.o build/state.o

clean :
	rm build/*

grammar/include/grammar.hpp : grammar/include/nonterminal.hpp \
                              grammar/include/token.hpp
grammar/include/grammar.cpp : grammar/include/grammar.hpp \
                              grammar/include/production.hpp

grammar/include/symbol.hpp : grammar/include/production.hpp
grammar/src/symbol.cpp : grammar/include/symbol.hpp

grammar/include/nonterminal.hpp : grammar/include/production.hpp \
				  grammar/include/symbol.hpp
grammar/src/nonterminal.cpp : grammar/include/nonterminal.hpp

grammar/include/token.hpp : grammar/include/production.hpp \
                            grammar/include/symbol.hpp

grammar/src/token.cpp : grammar/include/token.hpp

table/include/table.hpp: table/include/state.hpp table/include/item.hpp
table/src/table.cpp : table/include/table.hpp table/include/item_set.hpp \
                      grammar/include/token.hpp

table/include/lr_table.hpp : table/include/state.hpp table/include/table.hpp
table/src/lr_table.cpp : grammar/include/grammar.hpp \
grammar/include/nonterminal.hpp grammar/include/token.hpp \
table/include/lr_table.hpp table/include/state.hpp

table/include/lalr_table.hpp : table/include/state.hpp table/include/table.hpp
table/src/lalr_table.cpp : table/include/lalr_table.hpp \
table/include/item_set.hpp

table/include/item_set.hpp : table/include/item.hpp
table/src/item_set.cpp : table/include/item_set.hpp

table/include/state.hpp :
table/src/state.cpp : grammar/include/production.hpp grammar/include/symbol.hpp

bootstrap/src/grammar_syntax.cpp : bootstrap/include/grammar_syntax.hpp

tests/src/first_set1.cpp : bootstrap/include/grammar_syntax.hpp \
grammar/include/grammar.hpp

tests/include/print_states.hpp :
tests/src/print_states.cpp : tests/include/print_states.hpp \
table/include/state.hpp table/include/table.hpp

#Grammar
build/grammar.o : grammar/src/grammar.cpp
	$(CXX) $(CXXFLAGS) -c -o build/grammar.o grammar/src/grammar.cpp

build/nonterminal.o : grammar/src/nonterminal.cpp
	$(CXX) $(CXXFLAGS) -c -o build/nonterminal.o grammar/src/nonterminal.cpp

build/token.o : grammar/src/token.cpp
	$(CXX) $(CXXFLAGS) -c -o build/token.o grammar/src/token.cpp

build/production.o : grammar/src/production.cpp
	$(CXX) $(CXXFLAGS) -c -o build/production.o grammar/src/production.cpp

#Table
build/table.o : table/src/table.cpp
	$(CXX) $(CXXFLAGS) -c -o build/table.o table/src/table.cpp

build/lr_table.o : table/src/lr_table.cpp
	$(CXX) $(CXXFLAGS) -c -o build/lr_table.o table/src/lr_table.cpp

build/lalr_table.o : table/src/lalr_table.cpp
	$(CXX) $(CXXFLAGS) -c -o build/lalr_table.o table/src/lalr_table.cpp

build/item.o : table/src/item.cpp
	$(CXX) $(CXXFLAGS) -c -o build/item.o table/src/item.cpp

build/item_set.o : table/src/item_set.cpp
	$(CXX) $(CXXFLAGS) -c -o build/item_set.o table/src/item_set.cpp

build/state.o : table/src/state.cpp
	$(CXX) $(CXXFLAGS) -c -o build/state.o table/src/state.cpp

#Miscellaneous
build/grammar_syntax.o : bootstrap/src/grammar_syntax.cpp
	$(CXX) $(CXXFLAGS) -c -o build/grammar_syntax.o \
bootstrap/src/grammar_syntax.cpp

build/first_set1.o : tests/src/first_set1.cpp
	$(CXX) $(CXXFLAGS) -c -o build/first_set1.o tests/src/first_set1.cpp

build/print_states.o : tests/src/print_states.cpp
	$(CXX) $(CXXFLAGS) -c -o build/print_states.o tests/src/print_states.cpp

build/lr_table1.o : tests/src/lr_table1.cpp
	$(CXX) $(CXXFLAGS) -c -o build/lr_table1.o tests/src/lr_table1.cpp

build/lalr_table1.o : tests/src/lalr_table1.cpp
	$(CXX) $(CXXFLAGS) -c -o build/lalr_table1.o tests/src/lalr_table1.cpp

#Executables
bin/first_set1.out : build/first_set1.o build/grammar_syntax.o $(GRAMMAR_OBJS)
	$(CXX) -std=c++11 -o bin/first_set1.out build/first_set1.o \
build/grammar_syntax.o $(GRAMMAR_OBJS)

bin/lr_table1.out : build/lr_table1.o build/print_states.o $(GRAMMAR_OBJS) \
$(TABLE_OBJS)
	$(CXX) -std=c++11 -o bin/lr_table1.out build/lr_table1.o \
build/print_states.o $(GRAMMAR_OBJS) $(TABLE_OBJS)

bin/lalr_table1.out : build/lalr_table1.o build/print_states.o $(GRAMMAR_OBJS) \
$(TABLE_OBJS)
	$(CXX) -std=c++11 -o bin/lalr_table1.out build/lalr_table1.o \
build/print_states.o $(GRAMMAR_OBJS) $(TABLE_OBJS)
