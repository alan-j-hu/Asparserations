#!/usr/bin/python3
import argparse
import json
import string
import sys

header_template = """
#ifndef _ASPARSERATIONS__GENERATED_${class_name}_
#define _ASPARSERATIONS__GENERATED_${class_name}_

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

/**
 */
class Node
  {
  public:
    Node(const $payload&, const char*, const char*);
    Node(const $payload&, const std::vector<Node*>&);
    const $payload& payload() const;
    const std::vector<Node*>& children() const;
    const char* begin() const;
    const char* end() const;
    ~Node();
  private:
    $payload _payload;
    std::vector<Node*> _children;
    const char* _begin;
    const char* _end;
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

  struct _Production
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
    std::map<Token,std::pair<const State*,std::set<const _Production*>>>actions;
    std::map<Nonterminal,const State*> gotos;
  };

  std::vector<State> _states;
  $callback& _callback;
  $lexer& _lexer;
  std::vector<std::pair<Node*,const State*>> _stack;
  std::unique_ptr<Productions> _productions;
  Node* _root;

  void _process(const State&, const char*);
  void _reduce(const _Production&);
};
$end_namespace

#endif
"""

src_template = """#include <iostream>
#include <stdexcept>
#include "../include/$class_name.hpp"
$src_front

$namespace::Node::Node(
  const $payload& payload,
  const char* begin,
  const char* end)
  : _payload(payload), _begin(begin), _end(end) {}

$namespace::Node::Node(const $payload& payload,
                       const std::vector<Node*>& children)
{
  if(children.empty())
    throw std::runtime_error("Zero children,"
                             "call Node(const char*, const char*) instead");
  _payload = payload;
  _children = children;
  _begin = children.front()->begin();
  _end = children.back()->end();
}

const $payload& $namespace::Node::payload() const
{
  return _payload;
}

const std::vector<$namespace::Node*>& $namespace::Node::children() const
{
  return _children;
}

const char* $namespace::Node::begin() const
{
  return _begin;
}

const char* $namespace::Node::end() const
{
  return _end;
}

$namespace::Node::~Node()
{
  for(Node* node : _children) delete node;
}

$namespace::$class_name::Productions::Productions()
  : $mangled_productions_src
{
}

$namespace::$class_name::$class_name($lexer& lexer, $callback& callback)
  : _lexer(lexer), _callback(callback), _productions(new Productions()),
    _root(nullptr), _states($state_count)
{
  $states
}

$namespace::Node* $namespace::$class_name::parse(const std::string& input)
{
  _process(_states.front(), input.data());
  while(!_stack.empty()) {
    _process(*_stack.back().second, _stack.back().first->end());
  }
  return _root;
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
  for(auto pair : _stack) {
    delete pair.first;
  }
}

void $namespace::$class_name::_process(const State& state, const char* c)
{
  for(auto& action : state.actions) {
    auto result = _lexer.expect(action.first, c);
    if(result.second) {
      if(action.second.first != nullptr) {
        // std::cout << (long)c << " " << (long)result.first << std::endl;
        _stack.emplace_back(
          new Node(_callback.call(action.first,
                                  std::string(result.first.first,
                                              result.first.second)),
                   result.first.first, result.first.second),
          action.second.first
        );
        return;
      }
      if(!action.second.second.empty()) {
        _reduce(**action.second.second.begin());
        return;
      }
    }
  }
  throw std::runtime_error("Failed parse");
}

void $namespace::$class_name::_reduce
(const $namespace::$class_name::_Production& production)
{
  if(_stack.empty()) throw std::runtime_error("Can't reduce empty stack");
  Node* node = nullptr;
  if(production.child_count == 0) {
    node = new Node(_callback.call(production.nonterminal,
                                   production.production,
                                   {}),
                    _stack.back().first->end(),
                    _stack.back().first->end());
  } else {
    std::vector<Node*> popped;
    for(int i = 0; i < production.child_count; ++i) {
      if(_stack.empty()) throw std::runtime_error("Stack underflow");
      popped.push_back(_stack.back().first);
      _stack.pop_back();
    }
    auto vec = std::vector<Node*>(popped.rbegin(), popped.rend());
    node = new Node(_callback.call(production.nonterminal,
                                   production.production,
                                   vec),
                    vec);
  }
  if(production.nonterminal == Nonterminal::__accept__) {
    _root = node;
    return;
  }
  const State* state;
  if(_stack.empty()) {
    state = &_states[0];
  } else {
    state = _stack.back().second;
  }
  auto iter = state->gotos.find(production.nonterminal);
  if(iter == _stack.back().second->gotos.end()) {
    throw std::runtime_error("Unknown nonterminal");
  }
  _stack.emplace_back(node, iter->second);
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
            lines += "_Production " + name + "_" + prodname + ";\n    "
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
            action_str += "&_states["+str(action["shift"])+"], {\n          "
        reduce_strs = map(lambda x :
                          "&_productions->" + x["nonterminal"]
                          + "_" + x["production"],\
                          action["reduces"])
        reduce_str = ",\n          ".join(reduce_strs)
        action_str += reduce_str + "\n        }}\n      }"
        actions.append(action_str)
    for nonterminal, index in state["gotos"].items():
        goto_str = "{Nonterminal::" + nonterminal \
                   + ", &_states[" + str(index) + "]}"
        gotos.append(goto_str)

    actions_str = "{\n      " + ",\n      ".join(actions) + "\n    }"
    gotos_str = ",\n        ".join(gotos)
    return "_states[" + str(state["index"]) \
        + "] = State {\n    .actions = " + actions_str \
        + ",\n    .gotos = {\n        " + gotos_str + "\n    }\n  };"

def gen_nonterminal_to_strings(nonterminal):
    name, wildcard = nonterminal
    return "case Nonterminal::" + name + ": return \"" + name + "\";"

def gen_productions_to_strings(grammar):
    names = set()
    for name,productions in grammar["nonterminals"].items():
        for prodname,wildcard in productions.items():
            names.add(prodname)
    lines = map(lambda p: "case Production::" + p + ": return \"" + p + "\";",\
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
                             productions_to_strings=gen_productions_to_strings(table["grammar"]),\
                             mangled_productions_src=gen_mangled_productions_src(table["grammar"]))) \
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
