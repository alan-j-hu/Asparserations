#ifndef _ASPARSERATIONS_CODEGEN_SCHEME_CODE_GENERATOR_
#define _ASPARSERATIONS_CODEGEN_SCHEME_CODE_GENERATOR_

#include "code_generator.hpp"

namespace asparserations {
  namespace table {
    class Table;
  }
  namespace codegen {
    class Scheme_Code_Generator : Code_Generator
    {
    public:
      Scheme_Code_Generator(const table::Table&);
      const std::string& code() const;
    private:
      std::string _code;
      const table::Table& _table;
      void _generate();
    };
  }
}

#endif
