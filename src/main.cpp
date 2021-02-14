

#include "LexerGenerator.h"
#include "LexerDFA.hpp"

#include "ParserGenerator.h"
#include "LALRParser.hpp"

using namespace Manta;

void testParser(const string& rulesFilepath, const string& codeFilepath) {
    // Parser
    ParserGenerator generator;
    std::cout << "Parsing rules from \"" << rulesFilepath << "\"\n";
    auto parser = generator.createParserFromFile(rulesFilepath);
    if (parser) {
        // Print out the transition table.
        std::cout << parser->printTable() << endl << endl;
        std::cout << "Description parse successful.\n\n";
        auto program = parser->parseCodeFile(codeFilepath);
        if (program) {
            std::cout << program->printTree() << endl;
        }
        else {
            std::cout << "Failure. Printing parse trace:\n";
            std::cout << parser->getParseTrace() << "\n";
        }

        std::cout << "\n\n" << LALRParser::printAsMathematica(program) << "\n\n";
    }
    else {
        std::cout << "Failure." << endl;
    }

    auto header = generator.generateCodeFile();
    std::ofstream fout("../output/header.h");
    if (fout.fail()) {
        std::cout << "Failed to open file.";
        return;
    }
    fout << header;
    fout.close();
}

int main(int argc, char** argv) {
    LexerGenerator test;
    test.add_lexeme("String", "\\@+");
    test.add_lexeme("Whitespaces", "\\s+");
    test.add_lexeme("Punctuation", ". | , | ! | ?");
    test.add_lexeme("Integer", "\\d+");
    test.add_lexeme("Double", "\\d+ . \\d*");
    test.add_lexeme("Float", "\\d+ . \\d*f");
    test.add_lexeme("Hexidecimal", "0x([A-F] | \\d)+");
    test.add_lexeme("EOF", "\0");
    test.add_reserved("for");
    test.add_reserved("String");

    auto test_lexer = test.create_lexer();
    test_lexer->set_string_to_lex("String for, hello world! 3.145, 9, 3.2f, 0x01FF");
    std::cout << lexAllToString(test_lexer) << "\n";

    LexerGenerator gen;
    gen.add_reserved("int");
    gen.add_reserved("float");
    gen.add_reserved("string");
    auto lexer = gen.create_lexer("../config/simple-rules.txt");

    lexer->set_string_to_lex("hello 123 +234.3\nWorld! \n int x, ++x,   string str = \"hi guys\"\n\n");
    std::cout << lexAllToString(lexer) << "\n";

    std::cout << "--> Done with parsing section.\n\n";

    // Test CC
    testParser("../config/simple-rules.txt", "../config/code-ex.txt");

    return 0;
}