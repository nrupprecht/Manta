#include "cparse.hpp"

namespace Manta {

  bool CC_Parser::parse(const vector<Lexeme>& input) {
    lex_list = input;
    int point = 0;
		level = 0;
    bool success = check_program(point);
    if (point==lex_list.size()) cout << "Whole program parsed.\n";
    else cout << "Not all parsed. Point is " << point << ", size is " << lex_list.size() << ".\n";
    return (success && point==lex_list.size());
  }

  bool CC_Parser::check_boolean(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [boolean]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (lex_list[local_point++].literal=="true") valid = true;
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].literal=="false") valid = true;
    else if (!valid) --local_point;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [boolean] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_boolean_statement(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [boolean_statement]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (check_boolean(local_point)) valid = true;
    if (!valid && check_term(local_point)) {
      bool valid_branch = true;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for production [cmp]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && !check_cmp(local_point)) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for production [term]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && !check_term(local_point)) valid_branch = false;
      // If all test have been passed.
      if (valid_branch) valid = true;
      else local_point = point; // To be ready to try the next option.
    }

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [boolean_statement] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_cmp(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [cmp]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (lex_list[local_point++].literal=="<") valid = true;
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].literal==">") valid = true;
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].literal=="<=") valid = true;
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].literal==">=") valid = true;
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].literal=="==") valid = true;
    else if (!valid) --local_point;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [cmp] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_math_statement(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [math_statement]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (lex_list[local_point++].literal=="++") {
      bool valid_branch = true;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [Identifier]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::Identifier) valid_branch = false;
      // If all test have been passed.
      if (valid_branch) valid = true;
      else local_point = point; // To be ready to try the next option.
    }
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].type==LexType::Identifier) {
      bool valid_branch = true;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for literal \"++\". Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].literal!="++") valid_branch = false;
      // If all test have been passed.
      if (valid_branch) valid = true;
      else local_point = point; // To be ready to try the next option.
    }
    else if (!valid) --local_point;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [math_statement] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_param(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [param]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (check_term(local_point)) valid = true;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [param] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_param_list(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [param_list]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (check_param(local_point)) {
      bool valid_branch = true;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for production [param_list]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && !check_param_list(local_point)) valid_branch = false;
      // If all test have been passed.
      if (valid_branch) valid = true;
      else local_point = point; // To be ready to try the next option.
    }
    if (!valid && true) valid = true;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [param_list] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_program(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [program]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (check_statement_list(local_point)) valid = true;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [program] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_statement(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [statement]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (lex_list[local_point++].literal=="if") {
      bool valid_branch = true;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for production [boolean]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && !check_boolean(local_point)) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for literal \"then\". Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].literal!="then") valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [OpenBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::OpenBrack) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [Identifier]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::Identifier) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [CloseBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::CloseBrack) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [NewLine]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::NewLine) valid_branch = false;
      // If all test have been passed.
      if (valid_branch) valid = true;
      else local_point = point; // To be ready to try the next option.
    }
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].literal=="while") {
      bool valid_branch = true;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for production [boolean]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && !check_boolean(local_point)) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [OpenBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::OpenBrack) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [Identifier]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::Identifier) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [CloseBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::CloseBrack) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [NewLine]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::NewLine) valid_branch = false;
      // If all test have been passed.
      if (valid_branch) valid = true;
      else local_point = point; // To be ready to try the next option.
    }
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].literal=="for") {
      bool valid_branch = true;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [Identifier]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::Identifier) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for literal \"=\". Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].literal!="=") valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [Number]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::Number) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for literal \",\". Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].literal!=",") valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for production [boolean_statement]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && !check_boolean_statement(local_point)) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for literal \",\". Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].literal!=",") valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for production [math_statement]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && !check_math_statement(local_point)) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [OpenBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::OpenBrack) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [Identifier]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::Identifier) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [CloseBrack]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::CloseBrack) valid_branch = false;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for lex type [NewLine]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && lex_list[local_point++].type!=LexType::NewLine) valid_branch = false;
      // If all test have been passed.
      if (valid_branch) valid = true;
      else local_point = point; // To be ready to try the next option.
    }
    else if (!valid) --local_point;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [statement] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_statement_list(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [statement_list]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (check_statement(local_point)) {
      bool valid_branch = true;
      // Debug statement.
      if (valid_branch) cout << tabs() << "Checking for production [statement_list]. Next token has literal ["<< lex_list[local_point].literal << "].\n";
      // <---

      if (valid_branch && !check_statement_list(local_point)) valid_branch = false;
      // If all test have been passed.
      if (valid_branch) valid = true;
      else local_point = point; // To be ready to try the next option.
    }
    if (!valid && check_statement(local_point)) valid = true;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [statement_list] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

  bool CC_Parser::check_term(int& point) {
    bool valid = false;
    int local_point = point;

    // Debug statement.
    cout << tabs() << "Entering function [term]. Point = " << point << ", First lexeme: [" << lex_list[point].literal << "]\n";
    ++level;
    // <---

    if (lex_list[local_point++].type==LexType::Identifier) valid = true;
    else if (!valid) --local_point;
    if (!valid && lex_list[local_point++].type==LexType::Number) valid = true;
    else if (!valid) --local_point;

    // Debug statement.
    --level;

    cout << tabs() << "Exiting function [term] with " << (valid ? "success" : "failure") << ".\n";
    // <---
    // Did any rule succeed?
    if (valid) {
      point = local_point;
      return true;
    }
    // Else, return false.
    return false;
  }

}