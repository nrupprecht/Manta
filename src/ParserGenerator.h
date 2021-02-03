//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#ifndef MANTACLION_PARSERGENERATOR_H
#define MANTACLION_PARSERGENERATOR_H

#include "ParseNode.h"

namespace Manta {

    class ParserGenerator {
    public:

        //! \brief Parse a description of a grammer to create a parser.
        std::shared_ptr<class LALRParser> createParserFromFile(const std::string& filename);

        std::shared_ptr<class LALRParser> createParserFromStream(std::istream &stream);

    private:

        inline void getProductions(std::istream &, int);

        inline std::shared_ptr<ParseNode> getInstructions(std::istream &fin, int pid);

        inline int registerProduction(const string &);

        //! \brief Shifts the production numbers from being negative to being positive numbers after the last lexer
        //! token number.
        inline void shiftProductionNumbers();

        //! \brief A lexer.
        Lexer lexer;

        // Compute the LR0 table from the grammar.
        bool computeLR0();

        int addState(const State &);

        void computeGoto(int);

        State closure(int);

        State advanceDot(const State &, int);

        void completeTable();

        void assertEntry(int, int, const Entry &);

        void computeLookahead();

        void tryRuleInState(int, const Item &);

        //! \brief Trys to find a state in all_states. Returns -1 for failure.
        inline int findState(const State &);

        //! \brief Maps production names to production numbers.
        std::map<string, int> production_map;
        std::map<int, string> inverse_production_map;

        //! \brief The number of terminals in the correspondence vector.
        int num_productions = 0;

        //! \brief The productions for each nonterminal. A State (here) is essentially a set of production rules.
        std::map<int, State> productions_for;

        //! \brief All the productions.
        std::vector<Item> all_productions;

        //! \brief Which production start points to.
        int start_production = 0;

        //! \brief The total number of lexer ids plus production symbols. The number of columns in the parse_table.
        int total_symbols = 0;

        //! \brief The number to assign to the next production.
        int next_production_number = -1;

        //! \brief The parse table. It is a vector so we can add new states.
        //!
        //! The pair is [ action, state ].
        //! 0 - Error.
        //! 1 - Shift.
        //! 2 - Reduce.
        //! 3 - Accept.
        std::vector<std::vector<Entry> > parse_table;

        //! \brief All the different states.
        std::vector<State> all_states;

        //! \brief Work list for creating table.
        std::deque<int> work_list;

        //! \brief A flag that should be set to false if something fails.
        bool status = true;

        //! \brief A string that records the history of the parse.
        std::string parse_trace;

    };

}
#endif //MANTACLION_PARSERGENERATOR_H
