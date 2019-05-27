#ifndef __COMPILER_COMPILER_HPP__MANTA__
#define __COMPILER_COMPILER_HPP__MANTA__

#include "utility.hpp"

namespace Manta {

  enum class TokenType { Production, Literal, LexType };

  struct Token {
    //! \brief Constructor.
    Token (string id, TokenType t) : identifier(id), type(t) {};

    //! \brief The identifier, or literal.
    string identifier;

    //! \brief The type of token this is.
    TokenType type;
  };

  //! \brief A production rule is a list of tokens.
  typedef list<Token> ProductionRule;

  //! \brief A production. Has a name and definitions.
  struct Production {
    Production(string n) : name(n) {};

    //! \brief The name of the production.
    string name;

    //! \brief Rules associated with the production.
    vector<ProductionRule> rules;

    //! \brief Whether there is at least one rule for this production.
    bool defined() { return !rules.empty(); }
  };


  class CompilerCompiler {
  public:
    CompilerCompiler();

    bool readFile(string);

    bool parseDescription();

    bool writeFile(string, string);

  private:
    //! \brief Check the validity of the production rules.
    inline bool check_validity();

    //! \brief Write programs based on the stored production rules.
    inline bool write_programs();
    inline void write_header();
    inline void write_cpp();

    //! \brief Add a production symbol to the map if it is not already there. If it is a reserved production, return false.
    inline bool register_production(const string&);

    //! \brief Make sure that production names only have valid characters, no spaces, etc.
    inline void clean_name(string&);

    inline string create_check(const Token&, bool=true);

    //! \brief Get a word from withing delimiters, terminating with the provided character.
    inline string get_word(int&, char);

    //! \brief Production rule information, indexed by production name.
    map<string, Production> productions;

    //! \brief Reserved production keywords.
    //!
    //! E.g. <empty>
    set<string> reserved;

    //! \brief A string that describes the intended parser.
    string parser_description;

    //! \brief The lexer program header.
    string header;

    //! \brief The lexer program cpp file.
    string program;

    //! \brief If true, insert debug statements into the program.
    bool use_debug_statements = true;;

    //! \brief The name of the Lexeme struct.
    string lexeme_struct_name = "Lexeme";
    //! \brief The name of the parser class.
    string parser_class_name = "CC_Parser";

  };


};  

#endif // __COMPILER_COMPILER_HPP__MANTA__