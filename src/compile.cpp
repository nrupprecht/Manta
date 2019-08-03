
#include "LALR-Manta.hpp"

using namespace Manta;

int main(int argc, char** argv) {

  // Test CC

  LALRGenerator generator;
  bool success = generator.parseDescription("src/simple-rules.txt");

  if (success) {
    cout << "Description parse successful.\n\n";
    ParseNode *program = generator.parseCodeFile("src/code-ex.txt");

    cout << "Program:\n" << *program << endl;
  }
  else {
    cout << "Failure." << endl;
  }

  return 0;
}