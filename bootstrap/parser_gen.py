#!/usr/bin/python3
import argparse
import json
import string
import sys

header_template = """
#ifndef ASPARSERATIONS_GENERATED_${class_name}_H_
#define ASPARSERATIONS_GENERATED_${class_name}_H_

#include <array>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>
$header_front

$begin_namespace
enum class Token
{
  $tokens
};

enum class Nonterminal
{
  $nonterminals
};

enum class Production
{
  $productions
};

struct Lexer_State
{
  const char* begin;
  const char* end;
  unsigned int lines;
  const char* last_newline;
};

Lexer_State next(const Lexer_State&);

/**
 */
class Node
{
  public:
    Node(const $payload&, const Lexer_State&);
    Node(const $payload&, const std::vector<Node*>&);
    const $payload& payload() const;
    const std::vector<Node*>& children() const;
    const Lexer_State& state() const;
    ~Node();
  private:
    $payload m_payload;
    std::vector<Node*> m_children;
    Lexer_State m_state;
};

class $class_name
{
public:

  $class_name($lexer&, $callback&);
  Node* parse(const std::string&);
  static std::string nonterminal_to_string(Nonterminal);
  static std::string production_to_string(Production);
  ~$class_name();
private:

  struct Mangled_Production
  {
    const Nonterminal nonterminal;
    const Production production;
    unsigned int child_count;
  };

  struct Productions
  {
    Productions();
    $mangled_productions_header
  };

  struct State
  {
    std::map<Token,std::pair<const State*,std::set<const Mangled_Production*>>>
      actions;
    std::map<Nonterminal,const State*> gotos;
  };

  std::vector<State> m_states;
  $callback& m_callback;
  $lexer& m_lexer;
  std::vector<std::pair<Node*,const State*>> m_stack;
  std::unique_ptr<Productions> m_productions;
  Node* m_root;

  void m_process(const State&, const Lexer_State&);
  void m_reduce(const Mangled_Production&);
};
$end_namespace

#endif
"""

src_template = """
#include <stdexcept>
#include "../include/$class_name.hpp"
$src_front

$namespace::Lexer_State $namespace::next(const $namespace::Lexer_State& ls)
{
  $namespace::Lexer_State ls_prime = {
    ls.end,
    ls.end,
    ls.lines,
    ls.last_newline
  };
  return ls_prime;
}

$namespace::Node::Node(const $payload& payload,
                       const $namespace::Lexer_State& state)
  : m_payload(payload), m_state(state) {}

$namespace::Node::Node(const $payload& payload,
                       const std::vector<Node*>& children)
{
  if(children.empty())
    throw std::runtime_error("Zero children,"
                             "call Node(const char*, const char*) instead");
  m_payload = payload;
  m_children = children;
  m_state = $namespace::Lexer_State {
    children.front()->state().begin,
    children.back()->state().end,
    children.back()->state().lines,
    children.back()->state().last_newline
  };
}

const $payload& $namespace::Node::payload() const
{
  return m_payload;
}

const std::vector<$namespace::Node*>& $namespace::Node::children() const
{
  return m_children;
}

const $namespace::Lexer_State& $namespace::Node::state() const
{
  return m_state;
}

$namespace::Node::~Node()
{
  for(Node* node : m_children) delete node;
}

$namespace::$class_name::Productions::Productions()
  : $mangled_productions_src
{
}

$namespace::$class_name::$class_name($lexer& lexer, $callback& callback)
  : m_lexer(lexer), m_callback(callback), m_productions(new Productions()),
    m_root(nullptr), m_states($state_count)
{
  $states
}

$namespace::Node* $namespace::$class_name::parse(const std::string& input)
{
  m_process(m_states.front(),
            $namespace::Lexer_State{input.data(), input.data(), 
                                    1, input.data() - 1});
  while(!m_stack.empty()) {
    m_process(*m_stack.back().second,
              $namespace::next(m_stack.back().first->state()));
  }
  return m_root;
}

std::string
$namespace::$class_name::nonterminal_to_string($namespace::Nonterminal nt)
{
  switch(nt) {
    $nonterminals_to_strings
  }
  throw std::runtime_error("Unknown nonterminal");
}

std::string
$namespace::$class_name::production_to_string($namespace::Production p)
{
  switch(p) {
    $productions_to_strings
  }
  throw std::runtime_error("Unknown production");
}

$namespace::$class_name::~$class_name()
{
  for(auto pair : m_stack) {
    delete pair.first;
  }
}

void $namespace::$class_name::m_process
(const State& state, const $namespace::Lexer_State& lex_state)
{
  $namespace::Lexer_State err;
  for(auto& action : state.actions) {
    auto result = m_lexer.expect(action.first, lex_state);
    err = result.first;
    if(result.second) {
      if(action.second.first != nullptr) {
        m_stack.emplace_back(
          new Node(m_callback.call(action.first,
                                   std::string(result.first.begin,
                                               result.first.end)),
                   result.first),
          action.second.first
        );
        return;
      }
      if(!action.second.second.empty()) {
        m_reduce(**action.second.second.begin());
        return;
      }
    }
  }
  throw std::runtime_error("Failed parse: " + std::to_string(err.lines)
    + ":" + std::to_string(err.begin - err.last_newline));
}

void $namespace::$class_name::m_reduce
(const $namespace::$class_name::Mangled_Production& production)
{
  if(m_stack.empty()) throw std::runtime_error("Can't reduce empty stack");
  Node* node = nullptr;
  if(production.child_count == 0) {
    node = new Node(m_callback.call(production.nonterminal,
                                    production.production,
                                    {}),
                    $namespace::next(m_stack.back().first->state()));
  } else {
    std::vector<Node*> popped;
    for(int i = 0; i < production.child_count; ++i) {
      if(m_stack.empty()) throw std::runtime_error("Stack underflow");
      popped.push_back(m_stack.back().first);
      m_stack.pop_back();
    }
    auto vec = std::vector<Node*>(popped.rbegin(), popped.rend());
    node = new Node(m_callback.call(production.nonterminal,
                                    production.production,
                                    vec),
                    vec);
  }
  if(production.nonterminal == Nonterminal::_accept_) {
    m_root = node;
    return;
  }
  const State* state;
  if(m_stack.empty()) {
    state = &m_states[0];
  } else {
    state = m_stack.back().second;
  }
  auto iter = state->gotos.find(production.nonterminal);
  if(iter == m_stack.back().second->gotos.end()) {
    throw std::runtime_error("Unknown nonterminal");
  }
  m_stack.emplace_back(node, iter->second);
}

"""

def gen_namespace_decls(namespaces):
    begin = ""
    end = ""
    for namespace in namespaces:
        begin += "namespace " + namespace + " {\n"
        end = "} // " + namespace + "\n" + end
    return {"begin_namespace" : begin, "end_namespace" : end}

def gen_production_list(grammar):
    names = set()
    for name,productions in grammar["nonterminals"].items():
        for prodname,wildcard in productions.items():
            names.add(prodname)
    lines = ",\n  ".join(names)
    return lines

def gen_mangled_production_list_header(grammar):
    lines = ""
    for name,productions in grammar["nonterminals"].items():
        for prodname,symbols in productions.items():
            lines += "Mangled_Production " + name + "_" + prodname + ";\n    "
    return lines

def gen_header(template, table, config):
    tokens = ",\n  ".join(table["grammar"]["tokens"])
    nonterminal_list = []
    for name, wildcard in table["grammar"]["nonterminals"].items():
        nonterminal_list.append(name)
    nonterminals = ",\n  ".join(nonterminal_list)
    mangled_productions = gen_mangled_production_list_header(table["grammar"])
    productions = gen_production_list(table["grammar"])
    # Lost in stupid parentheses
    return string.Template( \
             string.Template( \
               string.Template(template) \
               .safe_substitute(config)) \
             .safe_substitute(tokens=tokens, nonterminals=nonterminals, \
                              mangled_productions_header=mangled_productions, \
                              productions=productions,
                              state_count=str(len(table["table"])))) \
           .substitute(gen_namespace_decls(config["namespace"]))

def gen_namespace_prefix(namespaces):
    return "::".join(namespaces)

def gen_mangled_productions_src(grammar):
    lines = []
    for name,productions in grammar["nonterminals"].items():
        for prodname,symbols in productions.items():
            lines.append(name + "_" + prodname + " {Nonterminal::"\
                         + name + ", " + "Production::" + prodname + ", " \
                         + str(len(symbols)) + "}")
    return ",\n  ".join(lines)

def gen_state(template, state, config):
    actions = []
    gotos = []
    for token, action in state["actions"].items():
        action_str = "{\n        Token::" + token + ", {"
        if action["shift"] is None:
            action_str += "nullptr, {\n          "
        else:
            action_str += "&m_states["+str(action["shift"])+"], {\n          "
        reduce_strs = map(lambda x :
                          "&m_productions->" + x["nonterminal"]
                          + "_" + x["production"],\
                          action["reduces"])
        reduce_str = ",\n          ".join(reduce_strs)
        action_str += reduce_str + "\n        }}\n      }"
        actions.append(action_str)
    for nonterminal, index in state["gotos"].items():
        goto_str = "{Nonterminal::" + nonterminal \
                   + ", &m_states[" + str(index) + "]}"
        gotos.append(goto_str)

    actions_str = ",\n      ".join(actions)
    gotos_str = ",\n        ".join(gotos)
    return "m_states[" + str(state["index"]) \
        + "] = State {\n    { // actions\n      " + actions_str + "\n    }" \
        + ",\n    { // gotos \n        " + gotos_str + "\n    }\n  };"

def gen_nonterminal_to_strings(nonterminal):
    name, wildcard = nonterminal
    return "case Nonterminal::" + name + ": return \"" + name + "\";"

def gen_productions_to_strings(grammar):
    names = set()
    for name,productions in grammar["nonterminals"].items():
        for prodname,wildcard in productions.items():
            names.add(prodname)
    lines = map(lambda p: "case Production::" + p + ": return \"" + p \
                + "\";",
                names)
    return "\n    ".join(lines)

def gen_src(template, table, config):
    namespace_prefix = gen_namespace_prefix(config["namespace"])
    states = map(lambda x : gen_state(template, x, config), table["table"])
    states_text = "\n  ".join(states)
    nonterminals_to_strings = "\n    ".join(map(gen_nonterminal_to_strings,\
                                              table["grammar"]["nonterminals"]\
                                              .items()))
    return string.Template(string.Template(template) \
            .safe_substitute(namespace=namespace_prefix, states=states_text, \
                             state_count=len(table["table"]),\
                             nonterminals_to_strings=nonterminals_to_strings,\
                             productions_to_strings\
                             =gen_productions_to_strings(table["grammar"]),\
                             mangled_productions_src=\
                             gen_mangled_productions_src(table["grammar"]))) \
              .safe_substitute(config)

def main():
    argparser = argparse.ArgumentParser()
    argparser.add_argument("json")
    argparser.add_argument("config")
    argparser.add_argument("dest")

    args = argparser.parse_args()
    table = json.load(open(args.json, "r"))
    config = json.load(open(args.config, "r"))
    dest = args.dest
    header_file = open(dest + "/include/" + config["class_name"] + ".hpp", "w+")
    src_file = open(dest + "/src/" + config["class_name"] + ".cpp", "w+")
    header_file.write(gen_header(header_template, table, config))
    src_file.write(gen_src(src_template, table, config))
    header_file.close()
    src_file.close()

if __name__ == '__main__':
    main()
