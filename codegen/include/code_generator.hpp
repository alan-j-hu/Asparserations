#ifndef _ASPARSERATIONS_CODEGEN_CODE_GENERATOR_
#define _ASPARSERATIONS_CODEGEN_CODE_GENERATOR_

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
