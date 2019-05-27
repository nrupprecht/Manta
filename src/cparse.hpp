#ifndef __CC_PARSER_HPP__MANTA__
#define __CC_PARSER_HPP__MANTA__

#include <vector>
using std::vector;

#include <string>
using std::string;

#include "lexer.hpp"

namespace Manta {

  class CC_Parser {
  public:
    bool parse(const vector<Lexeme>&);

  private:
    // Production identification functions.
    bool check_boolean(int&);
    bool check_boolean_statement(int&);
    bool check_cmp(int&);
    bool check_math_statement(int&);
    bool check_param(int&);
    bool check_param_list(int&);
    bool check_program(int&);
    bool check_statement(int&);
    bool check_statement_list(int&);
    bool check_term(int&);

    // List of lexemes
    vector<Lexeme> lex_list;

    //! \brief Debug pretty printing.
    int level = 0;

    //! \brief Function for printing spaces, for pretty printing of debug messages.
    inline string tabs() {
      string tbs;
      for (int i=0; i<level; ++i) tbs += "| ";
      return tbs;
    }
  };

}
#endif // __CC_PARSER_HPP__MANTA__