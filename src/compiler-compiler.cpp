#include "compiler-compiler.hpp"

namespace Manta {

  CompilerCompiler::CompilerCompiler() {
    reserved.insert("empty");
    reserved.insert("endl");
  }

  bool CompilerCompiler::readFile(string fileName) {
    // Open the file
    std::ifstream fin(fileName);
    if (fin.fail()) {
      cout << "Failed to open file [" << fileName << "].\n";
      return false;
    }

    // To store the file.
    parser_description.clear();
    // Read in the whole file
    char c;
    fin.get(c);
    while (!fin.eof()) {
      // Make sure there is a space before each newline. For parsing purposes.
      if (c=='\n') parser_description.push_back(' '); 
      parser_description.push_back(c);
      fin.get(c);
    }
    fin.close();

    // Parse the description
    return parseDescription();
  }

  bool CompilerCompiler::parseDescription() {
    // Make sure there is something to parse
    if (parser_description.empty()) return false;

    // Add a newline to terminate parser_description, if one doesn't already 
    if (parser_description.at(parser_description.size()-1)!='\n') parser_description.push_back('\n');

    /// First stage: get productions from the description, and check correctness.

    // The size of the string
    int size = parser_description.size();
    // Number of ::= symbols on the line. There should be one.
    int equals_symbs = 0;
    // Name of the production name.
    string production_name = "";
    // The current production rule being developed.
    ProductionRule current_rule;
    // An iterator to the current production in the production map.
    map<string, Production>::iterator current_production;

    // Parse string
    for (int i=0; i<size; ++i) {
      char c = parser_description[i];

      // Start of a comment
      if (c=='#') {
        // Pass comments
        while (i<size && parser_description[i]!='\n') ++i;
      }
      // Start of a production
      else if (c=='<') { 
        string prod = "";
        // Parse to the end
        while (++i<size && parser_description[i]!='>') prod.push_back(parser_description[i]);
        // Check that we terminated on a '>'
        if (i==size || parser_description[i]!='>' || prod.empty()) {
          cout << "Error in parsing description. Should have ended on a '>' but didn't. Exiting.";
          return false;
        }
        // Clean the production name
        clean_name(prod);
        // If there is currently no production name, set this as the head production.
        if (production_name.empty()) {
          production_name = prod;
          if (!register_production(prod)) {
            cout << "Cannot create a production rule for reserved production [" << prod << "]." << endl;
            return false;
          }
          current_production = productions.find(prod);
        }
        // Otherwise, add to the production rule
        else {
          current_rule.push_back(Token(prod, TokenType::Production));
          // Put the production name into the map if it is not there
          register_production(prod);
        }

      }
      // Start of an enum reference
      else if (c=='$') {
        string ref = get_word(i, ' ');
        // If there is no current production rule, this is an error
        if (production_name.empty()) {
          cout << "Error in parsing description. A literal has occured before a production definition. Exiting.\n";
          return false;
        }
        // Otherwise, add to the production rule
        else current_rule.push_back(Token(ref, TokenType::LexType));
      }
      // Setting ::= operator
      else if (c==':' && i+2<size && parser_description[i+1]==':' && parser_description[i+2]=='=') { // Production rule
        ++equals_symbs;
        if (1<equals_symbs) {
          cout << "Multiple \"::=\" encountered in a single line. This is an error. Exiting.\n";
          return false;
        }
        i += 3; // Go beyond the ::= sign.
      }
      // Newline
      else if (c=='\n') {
        // End of the production. Push current rule.
        if (!current_rule.empty() && current_production!=productions.end()) current_production->second.rules.push_back(current_rule);
        // Clear data
        production_name = "";
        current_rule.clear();
        current_production = productions.end();
        // Set to zero
        equals_symbs = 0;
      }
      // Literal
      else if (c=='\"') {
        // Get the literal
        string lit = get_word(i, '\"');
        // If there is no current production rule, this is an error
        if (production_name.empty()) {
          cout << "Error in parsing description. A literal has occured before a production definition. Exiting.\n";
          return false;
        }
        // Otherwise, add to the production rule
        else current_rule.push_back(Token(lit, TokenType::Literal));
      }
      // Or
      else if (c=='|') {
        // Push current rule.
        if (!current_rule.empty() && current_production!=productions.end()) current_production->second.rules.push_back(current_rule);
        // Start new rule.
        current_rule.clear();
      }
      // Ignore spaces
      else if (c==' '); 
      // Unrecognized
      else {
        cout << "Unrecognized start character, [" << c << "]. Place: " << i << ". Exiting.\n";
        return false;
      }
    }

    /// Second stage: Check validity.
    /// Third stage: create a parser from the productions.
    write_programs();

    // Everything is done successfully.
    return true;
  }

  bool CompilerCompiler::writeFile(string directory, string fileName) {
    // Write header file
    string header_name;
    if (directory=="") header_name = fileName + ".hpp";
    else header_name = directory + "/" + fileName + ".hpp";
    std::ofstream fout(header_name); 
    if (fout.fail()) {
      cout << "File [" << header_name << "] failed to open.\n";
      return false;
    }
    fout << header;
    fout.close();
    // Write .cpp file
    string cpp_name;
    if (directory=="") cpp_name = fileName + ".cpp";
    else cpp_name = directory + "/" + fileName + ".cpp";
    fout.open(cpp_name);
    if (fout.fail()) {
      cout << "File [" << cpp_name << "] failed to open.\n";
      return false;
    }
    fout << "#include \"" << fileName << ".hpp\"\n\n";
    fout << program;
    fout.close();
    // Done
    return true;
  }

  inline bool CompilerCompiler::check_validity() {
    int bad_rules = 0;
    bool has_program = false;
    for (auto pr : productions) {
      Production &production = pr.second;
      // Make sure there is at least one production rule
      if (production.rules.empty()) {
        cout << "Production <" << production.name << "> has no associated rules.\n";
        ++bad_rules;
      }
      // Check if this is the <program> production.
      if (production.name=="program") has_program = true;
    }
    if (bad_rules>0) {
      cout << "--> Not all productions defined. Fix your parser definition! Exiting.\n";
      return false;
    }

    // Check that a <program> production is defined.
    if (!has_program) {
      cout << "--> We need a \"<program> ::= ...\" production to define our entry point. Add one! Exiting.\n";
    }

    // Check that production rules are good (will terminate)
    // TODO.

    // Everything is fine.
    return true;
  }

  inline bool CompilerCompiler::write_programs() {
    // We must have valid productions to write a working program.
    if (!check_validity()) return false;
    // Write programs
    write_header();
    write_cpp();
    // Success.
    return true;
  }

  inline void CompilerCompiler::write_header() {
    // Clear
    header.clear(); 

    // Macro guard
    header += "#ifndef __CC_PARSER_HPP__MANTA__\n#define __CC_PARSER_HPP__MANTA__\n\n";
    // Includes
    header += "#include <vector>\nusing std::vector;\n\n";
    header += "#include <string>\nusing std::string;\n\n";
    // Include lexer
    header += "#include \"lexer.hpp\"\n\n";
    // Namespace
    header += "namespace Manta {\n\n";
    // CC_Parser class
    header += "  class " + parser_class_name + " {\n  public:\n    bool parse(const vector<Lexeme>&);\n\n  private:\n";
    header += "    // Production identification functions.\n";
    for (const auto &prod : productions) header += "    bool check_" + prod.first + "(int&);\n";
    header += "\n    // List of lexemes\n";
    header += "    vector<" + lexeme_struct_name + "> lex_list;\n";
    if (use_debug_statements) {
      header += "\n    //! \\brief Debug pretty printing.\n    int level = 0;\n\n";
      header += "    //! \\brief Function for printing spaces, for pretty printing of debug messages.\n";
      header += "    inline string tabs() {\n      string tbs;\n      for (int i=0; i<level; ++i) tbs += \"| \";\n      return tbs;\n    }\n";
    }
    header += "  };\n\n}\n";
    header += "#endif // __CC_PARSER_HPP__MANTA__";
  }

  inline void CompilerCompiler::write_cpp() {
    // Clear
    program.clear();

    //*******
    use_debug_statements = true;
    //*******

    // Write .cpp file
    program += "namespace Manta {\n\n";
    // Main parse function
    program += "  bool " + parser_class_name + "::parse(const vector<" + lexeme_struct_name + ">& input) {\n";
    program += "    lex_list = input;\n";
    program += "    int point = 0;\n";
    if (use_debug_statements) program += "\t\tlevel = 0;\n";
    program += "    bool success = check_program(point);\n";
    if (use_debug_statements) {
      program += "    if (point==lex_list.size()) cout << \"Whole program parsed.\\n\";\n";
      program += "    else cout << \"Not all parsed. Point is \" << point << \", size is \" << lex_list.size() << \".\\n\";\n";
    }
    program += "    return (success && point==lex_list.size());\n";
    program += "  }\n\n";

    // Finder functions
    for (const auto &prod : productions) {
      // Get data from the productions
      string name = prod.first;
      auto &rules = prod.second.rules;
      // Write a function
      program += "  bool " + parser_class_name + "::check_" + prod.first + "(int& point) {\n";
      program += "    bool valid = false;\n";
      program += "    int local_point = point;\n\n";

      if (use_debug_statements) {
        program += "    // Debug statement.\n";
        program += "    cout << tabs() << \"Entering function [" + prod.first + "]. Point = \" << point << \"";
        program += ", First lexeme: [\" << lex_list[point].literal << \"]\\n\";\n";
        program += "    ++level;\n";
        program += "    // <---\n\n";
      }

      for (int r=0; r<rules.size(); ++r) {
        int i=0;
        bool terminal = false;
        for (auto &tok : rules[r]) {
          // If the first token in the rule
          if (i==0) {
            // If this is the first rule
            if (r==0) program += "    if (";
            else program += "    if (!valid && ";
            program += create_check(tok) +") ";
            // If the first check was a check on a terminal, then we must add an "else --local_point;" clause.
            terminal = (tok.type!=TokenType::Production);

            // If the rule size is 1, we can simplify.
            if (rules[r].size()==1) program += "valid = true;\n";
            // Else.
            else program += "{\n      bool valid_branch = true;\n";
          }
          // Not the first token in the rule.
          else {
            if (use_debug_statements) {
              program += "      // Debug statement.\n";
              program += "      if (valid_branch) cout << tabs() << \"Checking for ";
              if (tok.type==TokenType::Production) program += "production [" + tok.identifier + "]";
              else if (tok.type==TokenType::Literal) program += "literal \\\"" + tok.identifier + "\\\"";
              else if (tok.type==TokenType::LexType) program += "lex type [" + tok.identifier + "]";
              program += ". Next token has literal [\"<< lex_list[local_point].literal << \"].\\n\";\n";
              program += "      // <---\n\n";
            }
            program += "      if (valid_branch && " + create_check(tok, false) + ") valid_branch = false;\n";
          }
          // Increment token counter
          ++i;
        }
        // If the rule size is 1, we can simplify.
        if (rules[r].size()==1) {
          if (terminal) program += "    else if (!valid) --local_point;\n";
        }
        // Else.
        else {
          program += "      // If all test have been passed.\n";
          program += "      if (valid_branch) valid = true;\n";
          program += "      else local_point = point; // To be ready to try the next option.\n";
          program += "    }\n";
          // If the first check was a check on a literal, then we must add an "else --local_point;" clause.
          if (terminal) program += "    else if (!valid) --local_point;\n";
        }
      }
      program += "\n";


      if (use_debug_statements) {
        program += "    // Debug statement.\n";
        program += "    --level;\n\n";
        program += "    cout << tabs() << \"Exiting function [" + prod.first + "] with \" << (valid ? \"success\" : \"failure\") << \".\\n\";\n";
        program += "    // <---\n";
      }

      // End of the function - determine return value.
      program += "    // Did any rule succeed?\n";
      program += "    if (valid) {\n";
      program += "      point = local_point;\n";
      program += "      return true;\n";
      program += "    }\n    // Else, return false.\n";
      program += "    return false;\n";
      program += "  }\n\n";

    }
    program += "}";
  }

  inline bool CompilerCompiler::register_production(const string& prod) {
    // Check if prod is a reserved word.
    if (reserved.find(prod)!=reserved.end()) return false;
    // Otherwise, check if we should add it to the map.
    if (productions.find(prod)==productions.end()) productions.insert(pair<string, Production>(prod, Production(prod)));
    // Return true
    return true;
  }

  inline void CompilerCompiler::clean_name(string& name) {
    // Replace invalid characters with underscores.
    for (auto &c : name) {
      if (!(isalpha(c) || isdigit(c) || c=='_')) c='_';
    }
  }

  inline string CompilerCompiler::create_check(const Token& tok, bool cmp) {
    // Equals or not equals?
    string eq = cmp ? "==" : "!=";
    // Create a check
    if (tok.type==TokenType::Literal) return "lex_list[local_point++].literal" + eq + "\"" + tok.identifier + "\"";
    else if (tok.type==TokenType::LexType) return "lex_list[local_point++].type" + eq + "LexType::" + tok.identifier;
    else {
      if (tok.identifier=="empty") return cmp ? "true" : "false";
      else return static_cast<string>(cmp ? "" : "!") + "check_" + tok.identifier + "(local_point)";
    }
  }

  inline string CompilerCompiler::get_word(int& i, char terminator) {
    int size = parser_description.size();
    string word = "";
    // Parse to the end.
    while (++i<size && parser_description[i]!=terminator) {
      // Get next char.
      char c = parser_description[i];
      // Check for escape characters.
      if (c=='\\') {
        if (i+1==size) {
          cout << "Ended in the middle of a literal. Error.\n";
          return word;
        }
        // Get the escaped character
        c = parser_description[++i];
        if     (c=='\\') word.push_back('\\');
        else if (c=='n') word.push_back('\n');
        else if (c=='t') word.push_back('\t');
        else if (c=='\'') word.push_back('\'');
        else if (c=='\"') word.push_back('\"');
        // Allow for using the terminator character if it is escaped via '\'
        else if (c==terminator) word.push_back(terminator);
      }
      else word.push_back(c);
    }

    // This would be an error.
    if (parser_description[i]!=terminator || word.empty()) cout << "Error in word gathering.\n";

    // Return the word.
    return word;
  }

}