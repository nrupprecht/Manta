
#include "LALR-Manta.hpp"

#include "dynamic_object.hpp"

#include "lexer-dfa.hpp"

using namespace Manta;

void testParser(const string& rules, const string& code) {
  // Parser
  LALRGenerator generator;

  bool success = generator.parseDescription(rules);
  if (success) {
    // Print out the transition table.
    cout << generator.printTable() << endl << endl;
    cout << "Description parse successful.\n\n";
    ParseNode *program = generator.parseCodeFile(code);
    if (program) {
      //cout << "Program:\n" << *program << endl;

      cout << program->printTree() << endl;
    }
    else {
      cout << "Failure. Printing parse trace:\n";
      cout << generator.getParseTrace() << endl;
    }
  }
  else cout << "Failure." << endl;
}

int main(int argc, char** argv) {

  // DynamicObject dynamic;
  // cout << dynamic.top_type()->to_string() << endl;
  // dynamic.define_variable("my_int", "int");
  // VariableData *my_int = dynamic.top_variable();
  // cout << my_int->to_string() << endl;

  // int value = 2;
  // set_int(value, my_int);
  // cout << "Int should be set to " << value << ": " << my_int->to_string() << endl;
  // // Define a new type of struct.
  // structure_definer int_pair2("IntPair");
  // int_pair2.add_member("first", "int");
  // int_pair2.add_member("second", "int");
  // dynamic.define_type(int_pair2);
  // dynamic.define_variable("my_struct2", "IntPair");
  // auto my_struct2 = dynamic.top_variable();
  // cout << "my_struct2: " << my_struct2->to_string() << endl;
  // my_struct2->set("first", *my_int);
  // cout << "my_struct2: " << my_struct2->to_string() << endl;

  LexerDFA lexer;
  lexer.create_parser("config/lexer-description.txt");
  lexer.add_reserved("int");
  lexer.add_reserved("float");
  lexer.add_reserved("string");

  lexer.parse_string("hello 123 +234.3\nWorld!\n int x, ++x, string str = \"hi guys\"");

  if (0<=lexer.accepts_empty()) 
    cout << "\nAccepts empty: " << lexer.lexeme_name(lexer.accepts_empty()) << "\n\n";

  bool continuing_possible = true;
  while (lexer.any_remaining() && continuing_possible) {
    Token tok = lexer.get_token();
    int check_status = lexer.check_status();

    switch (check_status) {
      case 0: { // All good.
        cout << tok.type << ", [" << tok.literal << "]\n";

        if (tok.literal.empty()) {
          cout << "Empty literal. Stopping.";
          continuing_possible = false;
        }

        break;
      }
      case 1: {
        cout << "Could not accept token. Literal: [" << tok.literal << "]. Next char: [" << lexer.peek() << "] (char " << (int)lexer.peek() << ").\n";
        continuing_possible = false;
        break;
      }
      case 2: {
        cout << "DFA instream was null.\n";
        continuing_possible = false;
        break;
      }
    }
  }

  // Test CC
  //testParser("config/simple-rules.txt", "config/code-ex.txt");

  return 0;
}