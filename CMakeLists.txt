# I'm still learning how to do this...
project(Asparserations)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(GRAMMAR_SRC
  grammar/src/grammar.cpp
  grammar/src/nonterminal.cpp
  grammar/src/production.cpp
  grammar/src/token.cpp
  )

set(TABLE_SRC table/src/item.cpp
  table/src/item_core.cpp
  table/src/item_set.cpp
  table/src/lalr_state.cpp
  table/src/lalr_table.cpp
  table/src/lr_table.cpp
  table/src/state.cpp
  table/src/table.cpp
  )

set(CODEGEN_SRC codegen/src/json_generator.cpp)

add_executable(gen_json.out
  bootstrap/src/gen_json.cpp
  ${GRAMMAR_SRC}
  ${TABLE_SRC}
  ${CODEGEN_SRC}
  bootstrap/src/grammar_syntax.cpp
  )

add_custom_command(
  OUTPUT bootstrap/output_json.json
  COMMAND touch ../bootstrap/output_json.json
  COMMAND bin/gen_json.out > ../bootstrap/output_json.json
  DEPENDS gen_json.out
  )

add_custom_command(
  OUTPUT bootstrap/include/Parser.hpp
  OUTPUT bootstrap/src/Parser.cpp
  WORKING_DIRECTORY ../bootstrap
  COMMAND python3 parser_gen.py output_json.json config.json ../autogen
  DEPENDS bootstrap/parser_gen.py
          bootstrap/output_json.json
          bootstrap/config.json
  )

set(BOOTSTRAP_SRC autogen/src/Parser.cpp
                  bootstrap/src/callback.cpp
                  bootstrap/src/lexer.cpp
		  )

set(SRC_FILES ${GRAMMAR_SRC} ${TABLE_SRC} ${CODEGEN_SRC} ${BOOTSTRAP_SRC})
add_executable(asparserations main.cpp ${SRC_FILES})

include(InstallRequiredSystemLibraries)
include(CPack)
