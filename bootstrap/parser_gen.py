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

/**
 */
class $class_name
{
public:

  class Node
  {
  public:
    Node(const char*, const char*);
    Node(const std::vector<Node*>&);
    const std::vector<Node*>& children() const;
    const char* begin() const;
    const char* end() const;
    ~Node();
  private:
    std::vector<Node*> _children;
    const char* _begin;
    const char* _end;
  };

  $class_name($lexer&);
  Node* parse(const std::string&);
  static std::string nonterminal_to_string(Nonterminal);
  ~$class_name();
private:

  struct Production
  {
    const Nonterminal nonterminal;
    unsigned int child_count;
  };

  struct Productions
  {
    $productions
  };

  struct State
  {
    std::map<Token,std::pair<const State*,std::set<const Production*>>> actions;
    std::map<Nonterminal,const State*> gotos;
  };

  std::vector<State> _states;
  $lexer& _lexer;
  std::vector<std::pair<Node*,const State*>> _stack;
  std::unique_ptr<Productions> _productions;
  Node* _root;

  void _process(const State&, const char*);
  void _reduce(const Production&);
};
$end_namespace

#endif
"""

src_template = """#include <iostream>
#include <stdexcept>
#include "../include/$class_name.hpp"
$src_front

$namespace::$class_name::Node::Node(const char* begin, const char* end)
  : _begin(begin), _end(end) {}

$namespace::$class_name::Node::Node(const std::vector<Node*>& children)
{
  if(children.empty())
    throw std::runtime_error("Zero children,"
                             "call Node(const char*, const char*) instead");
  _children = children;
  _begin = children.front()->begin();
  _end = children.back()->end();
}

const std::vector<$namespace::$class_name::Node*>&
$namespace::$class_name::Node::children() const
{
  return _children;
}

const char* $namespace::$class_name::Node::begin() const
{
  return _begin;
}

const char* $namespace::$class_name::Node::end() const
{
  return _end;
}

$namespace::$class_name::Node::~Node()
{
  for(Node* node : _children) delete node;
}

$namespace::$class_name::$class_name($lexer& lexer)
  : _lexer(lexer), _productions(new Productions()),
    _root(nullptr), _states($state_count)
{
  $states
}

$namespace::$class_name::Node*
$namespace::$class_name::parse(const std::string& input)
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
        _stack.emplace_back(new Node(c, result.first), action.second.first);
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
(const $namespace::$class_name::Production& production)
{
  if(_stack.empty()) throw std::runtime_error("Can't reduce empty stack");
  Node* node = nullptr;
  if(production.child_count == 0) {
    node = new Node(_stack.back().first->end(), _stack.back().first->end());
  } else {
    std::vector<Node*> popped;
    for(int i = 0; i < production.child_count; ++i) {
      if(_stack.empty()) throw std::runtime_error("Stack underflow");
      popped.push_back(_stack.back().first);
      _stack.pop_back();
    }
    node = new Node(std::vector<Node*>(popped.rbegin(), popped.rend()));
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

def list_to_str(ls, separated):
    needs_sep = False
    ret = ""
    for elem in ls:
        if needs_sep:
            ret += separated
        else:
            needs_sep = True
        ret += elem
    return ret

def gen_namespace_decls(namespaces):
    begin = ""
    end = ""
    for namespace in namespaces:
        begin += "namespace " + namespace + " {\n"
        end = "} // " + namespace + "\n" + end
    return {"begin_namespace" : begin, "end_namespace" : end}

def gen_production_list(grammar):
    lines = ""
    for name,productions in grammar["nonterminals"].items():
        for prodname,symbols in productions.items():
            lines += "Production " + name + "_" + prodname + " {Nonterminal::" \
                     + name + ", " + str(len(symbols)) + "};\n    "
    return lines

def gen_header(template, table, config):
    tokens = list_to_str(table["grammar"]["tokens"], ",\n  ")
    nonterminal_list = []
    for name, wildcard in table["grammar"]["nonterminals"].items():
        nonterminal_list.append(name)
    nonterminals = list_to_str(nonterminal_list, ",\n    ")
    productions = gen_production_list(table["grammar"])
    # Lost in stupid parentheses
    return string.Template( \
             string.Template( \
               string.Template(template) \
               .safe_substitute(config)) \
             .safe_substitute(tokens=tokens, nonterminals=nonterminals, \
                              productions=productions, \
                              state_count=str(len(table["table"])))) \
           .substitute(gen_namespace_decls(config["namespace"]))

def gen_namespace_prefix(namespaces):
    return list_to_str(namespaces, "::")

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
        reduce_str = list_to_str(reduce_strs, ",\n          ")
        action_str += reduce_str + "\n        }}\n      }"
        actions.append(action_str)
    for nonterminal, index in state["gotos"].items():
        goto_str = "{Nonterminal::" + nonterminal \
                   + ", &_states[" + str(index) + "]}"
        gotos.append(goto_str)

    actions_str = "{\n      " \
                  + list_to_str(actions, ",\n      ") + "\n    }"
    gotos_str = list_to_str(gotos, ",\n        ")
    return "_states[" + str(state["index"]) \
        + "] = State {\n    .actions = " + actions_str \
        + ",\n    .gotos = {\n        " + gotos_str + "\n    }\n  };"

def gen_nonterminals_to_strings(nonterminal):
    name, wildcard = nonterminal
    return "case Nonterminal::" + name + ": return \"" + name + "\";"

def gen_src(template, table, config):
    namespace_prefix = gen_namespace_prefix(config["namespace"])
    states = map(lambda x : gen_state(template, x, config), table["table"])
    states_text = list_to_str(states, "\n  ")
    nonterminals_to_strings = list_to_str(map(gen_nonterminals_to_strings,\
                                              table["grammar"]["nonterminals"]\
                                              .items()),\
                                         "\n    ")
    return string.Template(string.Template(template) \
            .safe_substitute(namespace=namespace_prefix, states=states_text, \
                             state_count=len(table["table"]),\
                             nonterminals_to_strings=nonterminals_to_strings)) \
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
