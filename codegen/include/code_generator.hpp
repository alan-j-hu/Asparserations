#ifndef ASPARSERATIONS_CODEGEN_CODE_GENERATOR__H_
#define ASPARSERATIONS_CODEGEN_CODE_GENERATOR_H_

#include <string>

namespace asparserations {
  namespace codegen {
    struct Code_Generator
    {
      virtual const std::string& code() const=0;
    };
  }
}

#endif
