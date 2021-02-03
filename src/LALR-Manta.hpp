#ifndef __LALR_PARSER_GENERATOR_HPP__
#define __LALR_PARSER_GENERATOR_HPP__

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html

#include <utility>

#include "ParseNode.h"
#include "parser-classes.hpp"

namespace Manta {

    // Terminals can either be represented as literals (if they are reserved words or keywords)
    // or by by %eof, %newline, %number, %string, or %operator.

    class LALRParser {
    public:
        //! \brief Use the parser to parse the code in the file.
        std::shared_ptr<ParseNode> parseCodeFile(const string&);

        //! \brief Pretty print the transition table.
        std::string printTable();

        //! \brief Get the parse trace string.
        std::string getParseTrace();

        friend class ParserGenerator;

    private:

        LALRParser(std::map<int, string> inverse_production_map_,
                   int start_production_,
                   int total_symbols_,
                   std::vector<std::vector<Entry>> parse_table_,
                   std::vector<State> all_states_,
                   Lexer lexer_)
           : inverse_production_map(std::move(inverse_production_map_)), start_production(start_production_),
              total_symbols(total_symbols_), parse_table(std::move(parse_table_)), all_states(std::move(all_states_)),
              lexer(lexer_) {};

        //! \brief A lexer.
        Lexer lexer;

        //! \brief Maps production numbers to production names.
        std::map<int, string> inverse_production_map;

        //! \brief Which production start points to.
        int start_production = 0;

        //! \brief The total number of lexer ids plus production symbols. The number of columns in the parse_table.
        int total_symbols = 0;

        //! \brief The parse table. It is a vector so we can add new states.
        //!
        //! The pair is [ action, state ].
        //! 0 - Error.
        //! 1 - Shift.
        //! 2 - Reduce.
        //! 3 - Accept.
        std::vector<std::vector<Entry>> parse_table;

        //! \brief All the different states.
        //!
        //! Used for pretty - printing the transition table.
        std::vector<State> all_states;

        //! \brief A string that records the history of the parse.
        std::string parse_trace;
    };

}
#endif // __LALR_PARSER_GENERATOR_HPP__
