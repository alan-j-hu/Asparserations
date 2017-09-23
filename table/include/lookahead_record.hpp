#ifndef _ASPARSERATIONS_TABLE_LOOKAHEAD_RECORD_H_
#define _ASPARSERATIONS_TABLE_LOOKAHEAD_RECORD_H_

#include "../grammar/include/token.hpp"
#include <map>

namespace asparserations {
  namespace table {
    class Lookahead_Record
    {
    public:
      Lookahead_Record();
      
    private:
      std::map<foo,bar> _propagated_lookaheads;
      std::map<const Token*,bar> _spontaneous_lookaheads;
      
    };
  }
}

#endif
