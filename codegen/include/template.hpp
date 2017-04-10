#include <functional>
#include <string>

namespace asparserations{
  namespace grammar {
    class Symbol;
  }
  namespace codegen {
    struct Template
    {
      std::function<std::string(const grammar::Symbol*)> shift_code;
      std::function<std::string(unsigned short int)> reduce_code;
      std::function<std::string(const std::string&)> state_code;
      std::function<std::string(const std::string&)> state_machine_code;
    };
  }
}
