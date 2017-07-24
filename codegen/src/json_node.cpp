JSON_Node::JSON_Node(const std::vector<JSON_Node>& array)
  : _type(Type::Array), _array_val(array) {}

JSON_Node::JSON_Node(bool b)
  : _type(Type::Bool), _bool_val(b) {}
