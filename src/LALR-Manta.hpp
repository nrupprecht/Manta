#ifndef __LALR_PARSER_GENERATOR_HPP__
#define __LALR_PARSER_GENERATOR_HPP__

// http://smlweb.cpsc.ucalgary.ca/start.html
// https://web.cs.dal.ca/~sjackson/lalr1.html

#include <utility>

#include "parser-classes.hpp"

namespace Manta {

    struct ParseNode : public std::enable_shared_from_this<ParseNode> {
        explicit ParseNode(string d) : designator(std::move(d)) {};

        ParseNode(string d, std::shared_ptr<ParseNode> p)
                : designator(std::move(d)), parent(std::move(p)) {};

        ParseNode(const ParseNode& node) {
            *this = node;
        }

        ParseNode& operator=(const ParseNode& node) {
            designator = node.designator;
            parent = node.parent;
            for (const auto& child : node.children) {
                auto new_child = std::make_shared<ParseNode>("");
                *new_child = *child;
                new_child->parent = shared_from_this();
                children.push_back(new_child);
            }
            return *this;
        }

        inline void add(const string& str) {
            children.push_back(std::make_shared<ParseNode>(str, shared_from_this()));
        }

        inline void add(const std::shared_ptr<ParseNode>& node) {
            node->parent = shared_from_this();
            children.push_back(node);
        }

        friend ostream& operator<<(ostream& out, const ParseNode& node) {
            // Make sure we dont print actual newlines or things like that.
            string alias = node.designator;
            if      (alias=="\n") alias = "\\n";
            else if (alias=="\t") alias = "\\t";

            // Print out node.
            out << "[ " << alias;
            int size = node.children.size();
            if (0<size) {
                out << ": {";
                for (int i = 0; i < size; ++i) {
                    out << *node.children[i];
                    if (i != 0) {
                        out << ", ";
                    }
                }
                out << "}";
            }
            out << " ]";
            // Return the stream.
            return out;
        }

        std::string printTerminals() {
            // If this is a terminal.
            if (children.empty()) {
                string alias = designator;
                if      (alias=="\n") alias = "\\n";
                else if (alias=="\t") alias = "\\t";
                return alias + " ";
            }
            // If this is a production.
            string str;
            //for (auto it = children.rbegin(); it!=children.rend(); ++it)
            for (auto & it : children) {
                str += it->printTerminals();
            }
            // Return.
            return str;
        }

        string printTree(int level=0) {
            string str;
            string alias = designator;
            if      (alias=="\n") alias = "\\n";
            else if (alias=="\t") alias = "\\t";
            str += repeat('|', level) + alias;
            if (!children.empty()) {
                str += '\n';
                //for (int i=children.size()-1; 0<=i; --i) {
                for (int i=0; i<children.size(); ++i) {
                    str += children[i]->printTree(level+1);
                    if (i!=children.size()-1) str += "\n";
                }
            }
            // Return the string.
            return str;
        }

        //! Node label.
        std::string designator;

        std::shared_ptr<ParseNode> parent = nullptr;
        vector<std::shared_ptr<ParseNode>> children;
    };

    // Terminals can either be represented as literals (if they are reserved words or keywords)
    // or by by %eof, %newline, %number, %string, or %operator.

    class LALRGenerator {
    public:
        //! \brief Parse a description of a grammer to create a parser.
        bool parseDescription(const string&);

        bool parseDescription(std::istream& stream);

        //! \brief Use the parser to parse the code in the file.
        std::shared_ptr<ParseNode> parseCodeFile(const string&);

        //! \brief Pretty print the transition table.
        std::string printTable();

        //! \brief Get the parse trace string.
        std::string getParseTrace();

        class UnrecognizedSymbol;

    private:
        inline void getProductions(std::istream&, int);

        inline std::shared_ptr<ParseNode> getInstructions(std::istream& fin, int pid);

        inline int registerProduction(const string&);

        //! \brief Shifts the production numbers from being negative to being positive numbers after the last lexer
        //! token number.
        inline void shiftProductionNumbers();

        //! \brief A lexer.
        Lexer lexer;

        // Compute the LR0 table from the grammar.
        bool computeLR0();

        int addState(const State&);
        void computeGoto(int);
        State closure(int);
        State advanceDot(const State&, int);

        void completeTable();
        void assertEntry(int, int, const Entry&);
        void computeLookahead();
        void tryRuleInState(int, const Item&);

        //! \brief Trys to find a state in all_states. Returns -1 for failure.
        inline int findState(const State&);

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

    class LALRGenerator::UnrecognizedSymbol : public std::exception {
    public:
        UnrecognizedSymbol(const std::string& symbol) {
            message = "Unrecognized symbol: " + symbol;
        }

        const char* what() const noexcept override {
            return message.c_str();
        }
    private:
        std::string message;
    };

}
#endif // __LALR_PARSER_GENERATOR_HPP__
