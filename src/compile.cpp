
#include "LALR-Manta.hpp"

using namespace Manta;

int main(int argc, char** argv) {

  // Test CC

  LALRGenerator generator;
  generator.parseDescription("src/simple-rules.txt");

  //generator.parseCodeFile("src/code-ex.txt");

  return 0;
}