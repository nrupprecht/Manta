

#include "LexerGenerator.h"
#include "LexerDFA.hpp"

#include "ParserGenerator.h"
#include "LALR-Manta.hpp"

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
            std::cout << parser->getParseTrace() << endl;
        }
    }
    else std::cout << "Failure." << endl;
}

int main(int argc, char** argv) {

    LexerGenerator gen;
    gen.add_reserved("int");
    gen.add_reserved("float");
    gen.add_reserved("string");
    auto lexer = gen.create_lexer("../config/lexer-description.txt");

    lexer->set_string_to_parse("hello 123 +234.3\nWorld! \n int x, ++x,   string str = \"hi guys\"\n\n");

    if (0 <= lexer->accepts_empty()) {
        std::cout << "\nAccepts empty: " << lexer->lexeme_name(lexer->accepts_empty()) << "\n\n";
    }

    bool continuing_possible = true;
    while (lexer->any_remaining() && continuing_possible) {
        Token tok = lexer->get_token();
        int check_status = lexer->check_status();

        switch (check_status) {
            case 0: { // All good.
                std::cout << lexer->lexeme_name(tok.type) << ": [" << tok.to_string() << "]\n";
                if (tok.literal.empty()) {
                    std::cout << "Empty literal. Stopping.";
                    continuing_possible = false;
                }
                break;
            }
            case 1: {
                std::cout << "Could not accept token. Literal: [" << tok.literal << "]. Next char: ["
                    << lexer->peek() << "] (char " << (int)lexer->peek() << ").\n";
                continuing_possible = false;
                break;
            }
            case 2: {
                std::cout << "DFA instream was null.\n";
                continuing_possible = false;
                break;
            }
            default:
                std::cout << "Unrecognized status.\n";
                break;
        }
    }

    std::cout << "--> Done with parsing section.\n\n";

    // Test CC
    testParser("../config/simple-rules.txt", "../config/code-ex.txt");

    return 0;
}