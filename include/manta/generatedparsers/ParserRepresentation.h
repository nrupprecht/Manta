#pragma once

#include "manta/generator/DescriptionParser.h"
#include "manta/utility/Formatting.h"
#include <vector>
#include <string>
// Include the support for the parser.
#include "manta/generator/ParserDriver.h"
#include "manta/generator/LexerGenerator.h"
#include <Lightning/Lightning.h>
#include "manta/utility/Formatting.h"


using namespace manta::formatting;
using manta::ItemID;

#define REDUCE_ASSERT(count, item, size) \
  MANTA_REQUIRE(count <= size, "in reduction "  #item  ", not enough nodes in the collect vector, needed at least " #count ", actual size was " << size)

// Forward declare AST node types.
struct ASTNode_0_start;
struct ASTNode_5_lexer_block;
struct ASTNodeBase_newlines;
struct ASTNodeGeneral_1_newlines;
struct ASTNodeGeneral_2_newlines;
struct ASTNode_Parser;
struct ASTNode_17_data_block;
struct ASTNodeBase_lexeme_def_or;
struct ASTNodeGeneral_3_lexeme_def_or;
struct ASTNodeGeneral_4_lexeme_def_or;
struct ASTNodeBase_lexer_definitions;
struct ASTNode_6_lexer_definitions;
struct ASTNode_7_lexer_definitions;
struct ASTNodeBase_lexeme_line;
struct ASTNode_Lexeme;
struct ASTNode_Skip;
struct ASTNode_8_lexeme_definition;
struct ASTNode_9_skip_indicator;
struct ASTNode_10_start_indicator;
struct ASTNodeBase_parser_definitions;
struct ASTNode_11_parser_definitions;
struct ASTNode_12_parser_definitions;
struct ASTNode_parser;
struct ASTNode_options;
struct ASTNodeBase_production_and_data;
struct ASTNode_14_production_and_data;
struct ASTNode_15_production_and_data;
struct ASTNodeGeneral_13_or;
struct ASTNode_production_atoms;
struct ASTNode_actions;
struct ASTNodeBase_code_block;
struct ASTNodeGeneral_16_code_block;
struct ASTNode_code_block;
struct ASTNodeBase_production_atom;
struct ASTNode_identifier;
struct ASTNode_lexeme_name;
struct ASTNode_literal;
struct ASTNode_null;
struct ASTNode_action;
struct ASTNode_arguments;
struct ASTNodeBase_arg;
struct ASTNode_NullArg;
struct ASTNode_PosArg;
struct ASTNode_StringArg;
struct ASTNodeBase_data_definitions;
struct ASTNode_18_data_definitions;
struct ASTNode_19_data_definitions;
struct ASTNodeBase_data_definition;
struct ASTNode_define_code_block;
struct ASTNode_import;
struct ASTNode_inherit;
struct ASTNode_20_import_statement;
struct ASTNode_21_inherit_statement;
struct ASTNode_22_general_code_block;
// Lexeme type
struct ASTLexeme;

//! \brief Define the enums for each node type.
enum class ASTNodeType{
  Literal,
  Type_ASTNodeBase_arg,
  Type_ASTNodeBase_code_block,
  Type_ASTNodeBase_data_definition,
  Type_ASTNodeBase_data_definitions,
  Type_ASTNodeBase_lexeme_def_or,
  Type_ASTNodeBase_lexeme_line,
  Type_ASTNodeBase_lexer_definitions,
  Type_ASTNodeBase_newlines,
  Type_ASTNodeBase_parser_definitions,
  Type_ASTNodeBase_production_and_data,
  Type_ASTNodeBase_production_atom,
  Type_ASTNodeGeneral_13_or,
  Type_ASTNodeGeneral_16_code_block,
  Type_ASTNodeGeneral_1_newlines,
  Type_ASTNodeGeneral_2_newlines,
  Type_ASTNodeGeneral_3_lexeme_def_or,
  Type_ASTNodeGeneral_4_lexeme_def_or,
  Type_ASTNode_0_start,
  Type_ASTNode_10_start_indicator,
  Type_ASTNode_11_parser_definitions,
  Type_ASTNode_12_parser_definitions,
  Type_ASTNode_14_production_and_data,
  Type_ASTNode_15_production_and_data,
  Type_ASTNode_17_data_block,
  Type_ASTNode_18_data_definitions,
  Type_ASTNode_19_data_definitions,
  Type_ASTNode_20_import_statement,
  Type_ASTNode_21_inherit_statement,
  Type_ASTNode_22_general_code_block,
  Type_ASTNode_5_lexer_block,
  Type_ASTNode_6_lexer_definitions,
  Type_ASTNode_7_lexer_definitions,
  Type_ASTNode_8_lexeme_definition,
  Type_ASTNode_9_skip_indicator,
  Type_ASTNode_Lexeme,
  Type_ASTNode_NullArg,
  Type_ASTNode_Parser,
  Type_ASTNode_PosArg,
  Type_ASTNode_Skip,
  Type_ASTNode_StringArg,
  Type_ASTNode_action,
  Type_ASTNode_actions,
  Type_ASTNode_arguments,
  Type_ASTNode_code_block,
  Type_ASTNode_define_code_block,
  Type_ASTNode_identifier,
  Type_ASTNode_import,
  Type_ASTNode_inherit,
  Type_ASTNode_lexeme_name,
  Type_ASTNode_literal,
  Type_ASTNode_null,
  Type_ASTNode_options,
  Type_ASTNode_parser,
  Type_ASTNode_production_atoms,
};

//! \brief Define function to write AST node type enums.
inline const char* to_string(ASTNodeType type) {
  switch (type) {
  case ASTNodeType::Literal:
    return "Literal";
  case ASTNodeType::Type_ASTNodeBase_arg:
    return "Type_ASTNodeBase_arg";
  case ASTNodeType::Type_ASTNodeBase_code_block:
    return "Type_ASTNodeBase_code_block";
  case ASTNodeType::Type_ASTNodeBase_data_definition:
    return "Type_ASTNodeBase_data_definition";
  case ASTNodeType::Type_ASTNodeBase_data_definitions:
    return "Type_ASTNodeBase_data_definitions";
  case ASTNodeType::Type_ASTNodeBase_lexeme_def_or:
    return "Type_ASTNodeBase_lexeme_def_or";
  case ASTNodeType::Type_ASTNodeBase_lexeme_line:
    return "Type_ASTNodeBase_lexeme_line";
  case ASTNodeType::Type_ASTNodeBase_lexer_definitions:
    return "Type_ASTNodeBase_lexer_definitions";
  case ASTNodeType::Type_ASTNodeBase_newlines:
    return "Type_ASTNodeBase_newlines";
  case ASTNodeType::Type_ASTNodeBase_parser_definitions:
    return "Type_ASTNodeBase_parser_definitions";
  case ASTNodeType::Type_ASTNodeBase_production_and_data:
    return "Type_ASTNodeBase_production_and_data";
  case ASTNodeType::Type_ASTNodeBase_production_atom:
    return "Type_ASTNodeBase_production_atom";
  case ASTNodeType::Type_ASTNodeGeneral_13_or:
    return "Type_ASTNodeGeneral_13_or";
  case ASTNodeType::Type_ASTNodeGeneral_16_code_block:
    return "Type_ASTNodeGeneral_16_code_block";
  case ASTNodeType::Type_ASTNodeGeneral_1_newlines:
    return "Type_ASTNodeGeneral_1_newlines";
  case ASTNodeType::Type_ASTNodeGeneral_2_newlines:
    return "Type_ASTNodeGeneral_2_newlines";
  case ASTNodeType::Type_ASTNodeGeneral_3_lexeme_def_or:
    return "Type_ASTNodeGeneral_3_lexeme_def_or";
  case ASTNodeType::Type_ASTNodeGeneral_4_lexeme_def_or:
    return "Type_ASTNodeGeneral_4_lexeme_def_or";
  case ASTNodeType::Type_ASTNode_0_start:
    return "Type_ASTNode_0_start";
  case ASTNodeType::Type_ASTNode_10_start_indicator:
    return "Type_ASTNode_10_start_indicator";
  case ASTNodeType::Type_ASTNode_11_parser_definitions:
    return "Type_ASTNode_11_parser_definitions";
  case ASTNodeType::Type_ASTNode_12_parser_definitions:
    return "Type_ASTNode_12_parser_definitions";
  case ASTNodeType::Type_ASTNode_14_production_and_data:
    return "Type_ASTNode_14_production_and_data";
  case ASTNodeType::Type_ASTNode_15_production_and_data:
    return "Type_ASTNode_15_production_and_data";
  case ASTNodeType::Type_ASTNode_17_data_block:
    return "Type_ASTNode_17_data_block";
  case ASTNodeType::Type_ASTNode_18_data_definitions:
    return "Type_ASTNode_18_data_definitions";
  case ASTNodeType::Type_ASTNode_19_data_definitions:
    return "Type_ASTNode_19_data_definitions";
  case ASTNodeType::Type_ASTNode_20_import_statement:
    return "Type_ASTNode_20_import_statement";
  case ASTNodeType::Type_ASTNode_21_inherit_statement:
    return "Type_ASTNode_21_inherit_statement";
  case ASTNodeType::Type_ASTNode_22_general_code_block:
    return "Type_ASTNode_22_general_code_block";
  case ASTNodeType::Type_ASTNode_5_lexer_block:
    return "Type_ASTNode_5_lexer_block";
  case ASTNodeType::Type_ASTNode_6_lexer_definitions:
    return "Type_ASTNode_6_lexer_definitions";
  case ASTNodeType::Type_ASTNode_7_lexer_definitions:
    return "Type_ASTNode_7_lexer_definitions";
  case ASTNodeType::Type_ASTNode_8_lexeme_definition:
    return "Type_ASTNode_8_lexeme_definition";
  case ASTNodeType::Type_ASTNode_9_skip_indicator:
    return "Type_ASTNode_9_skip_indicator";
  case ASTNodeType::Type_ASTNode_Lexeme:
    return "Type_ASTNode_Lexeme";
  case ASTNodeType::Type_ASTNode_NullArg:
    return "Type_ASTNode_NullArg";
  case ASTNodeType::Type_ASTNode_Parser:
    return "Type_ASTNode_Parser";
  case ASTNodeType::Type_ASTNode_PosArg:
    return "Type_ASTNode_PosArg";
  case ASTNodeType::Type_ASTNode_Skip:
    return "Type_ASTNode_Skip";
  case ASTNodeType::Type_ASTNode_StringArg:
    return "Type_ASTNode_StringArg";
  case ASTNodeType::Type_ASTNode_action:
    return "Type_ASTNode_action";
  case ASTNodeType::Type_ASTNode_actions:
    return "Type_ASTNode_actions";
  case ASTNodeType::Type_ASTNode_arguments:
    return "Type_ASTNode_arguments";
  case ASTNodeType::Type_ASTNode_code_block:
    return "Type_ASTNode_code_block";
  case ASTNodeType::Type_ASTNode_define_code_block:
    return "Type_ASTNode_define_code_block";
  case ASTNodeType::Type_ASTNode_identifier:
    return "Type_ASTNode_identifier";
  case ASTNodeType::Type_ASTNode_import:
    return "Type_ASTNode_import";
  case ASTNodeType::Type_ASTNode_inherit:
    return "Type_ASTNode_inherit";
  case ASTNodeType::Type_ASTNode_lexeme_name:
    return "Type_ASTNode_lexeme_name";
  case ASTNodeType::Type_ASTNode_literal:
    return "Type_ASTNode_literal";
  case ASTNodeType::Type_ASTNode_null:
    return "Type_ASTNode_null";
  case ASTNodeType::Type_ASTNode_options:
    return "Type_ASTNode_options";
  case ASTNodeType::Type_ASTNode_parser:
    return "Type_ASTNode_parser";
  case ASTNodeType::Type_ASTNode_production_atoms:
    return "Type_ASTNode_production_atoms";
  // Default case for unrecognized enums.
  default:
     MANTA_FAIL("unrecognized enumeration");
  }
}

//! \brief Define the enums for each non-terminal type.
enum class NonterminalType{
  Terminal,
  action,
  actions,
  arg,
  args,
  code_block,
  data_block,
  data_definition,
  data_definitions,
  general_code_block,
  import_statement,
  inherit_statement,
  lexeme_def_or,
  lexeme_definition,
  lexeme_line,
  lexer_block,
  lexer_definitions,
  newlines,
  or_,
  parser_block,
  parser_definition,
  parser_definitions,
  production_and_data,
  production_atom,
  production_atoms,
  production_lines,
  skip_indicator,
  start,
  start_indicator,
};

//! \brief Define function to write the non-terminal type enum.
inline const char* to_string(NonterminalType type) {
  switch (type) {
  case NonterminalType::Terminal:
    return "Terminal";
  case NonterminalType::action:
    return "action";
  case NonterminalType::actions:
    return "actions";
  case NonterminalType::arg:
    return "arg";
  case NonterminalType::args:
    return "args";
  case NonterminalType::code_block:
    return "code_block";
  case NonterminalType::data_block:
    return "data_block";
  case NonterminalType::data_definition:
    return "data_definition";
  case NonterminalType::data_definitions:
    return "data_definitions";
  case NonterminalType::general_code_block:
    return "general_code_block";
  case NonterminalType::import_statement:
    return "import_statement";
  case NonterminalType::inherit_statement:
    return "inherit_statement";
  case NonterminalType::lexeme_def_or:
    return "lexeme_def_or";
  case NonterminalType::lexeme_definition:
    return "lexeme_definition";
  case NonterminalType::lexeme_line:
    return "lexeme_line";
  case NonterminalType::lexer_block:
    return "lexer_block";
  case NonterminalType::lexer_definitions:
    return "lexer_definitions";
  case NonterminalType::newlines:
    return "newlines";
  case NonterminalType::or_:
    return "or_";
  case NonterminalType::parser_block:
    return "parser_block";
  case NonterminalType::parser_definition:
    return "parser_definition";
  case NonterminalType::parser_definitions:
    return "parser_definitions";
  case NonterminalType::production_and_data:
    return "production_and_data";
  case NonterminalType::production_atom:
    return "production_atom";
  case NonterminalType::production_atoms:
    return "production_atoms";
  case NonterminalType::production_lines:
    return "production_lines";
  case NonterminalType::skip_indicator:
    return "skip_indicator";
  case NonterminalType::start:
    return "start";
  case NonterminalType::start_indicator:
    return "start_indicator";
  // Default case for unrecognized enums.
  default:
     MANTA_FAIL("unrecognized enumeration");
  }
}

//! \brief Base visitor class.
struct ASTNodeVisitor {
  virtual ~ASTNodeVisitor() = default;

  virtual void Visit(ASTNode_0_start& object) = 0;
  virtual void Visit(ASTNode_5_lexer_block& object) = 0;
  virtual void Visit(ASTNodeBase_newlines& object) = 0;
  virtual void Visit(ASTNodeGeneral_1_newlines& object) = 0;
  virtual void Visit(ASTNodeGeneral_2_newlines& object) = 0;
  virtual void Visit(ASTNode_Parser& object) = 0;
  virtual void Visit(ASTNode_17_data_block& object) = 0;
  virtual void Visit(ASTNodeBase_lexeme_def_or& object) = 0;
  virtual void Visit(ASTNodeGeneral_3_lexeme_def_or& object) = 0;
  virtual void Visit(ASTNodeGeneral_4_lexeme_def_or& object) = 0;
  virtual void Visit(ASTNodeBase_lexer_definitions& object) = 0;
  virtual void Visit(ASTNode_6_lexer_definitions& object) = 0;
  virtual void Visit(ASTNode_7_lexer_definitions& object) = 0;
  virtual void Visit(ASTNodeBase_lexeme_line& object) = 0;
  virtual void Visit(ASTNode_Lexeme& object) = 0;
  virtual void Visit(ASTNode_Skip& object) = 0;
  virtual void Visit(ASTNode_8_lexeme_definition& object) = 0;
  virtual void Visit(ASTNode_9_skip_indicator& object) = 0;
  virtual void Visit(ASTNode_10_start_indicator& object) = 0;
  virtual void Visit(ASTNodeBase_parser_definitions& object) = 0;
  virtual void Visit(ASTNode_11_parser_definitions& object) = 0;
  virtual void Visit(ASTNode_12_parser_definitions& object) = 0;
  virtual void Visit(ASTNode_parser& object) = 0;
  virtual void Visit(ASTNode_options& object) = 0;
  virtual void Visit(ASTNodeBase_production_and_data& object) = 0;
  virtual void Visit(ASTNode_14_production_and_data& object) = 0;
  virtual void Visit(ASTNode_15_production_and_data& object) = 0;
  virtual void Visit(ASTNodeGeneral_13_or& object) = 0;
  virtual void Visit(ASTNode_production_atoms& object) = 0;
  virtual void Visit(ASTNode_actions& object) = 0;
  virtual void Visit(ASTNodeBase_code_block& object) = 0;
  virtual void Visit(ASTNodeGeneral_16_code_block& object) = 0;
  virtual void Visit(ASTNode_code_block& object) = 0;
  virtual void Visit(ASTNodeBase_production_atom& object) = 0;
  virtual void Visit(ASTNode_identifier& object) = 0;
  virtual void Visit(ASTNode_lexeme_name& object) = 0;
  virtual void Visit(ASTNode_literal& object) = 0;
  virtual void Visit(ASTNode_null& object) = 0;
  virtual void Visit(ASTNode_action& object) = 0;
  virtual void Visit(ASTNode_arguments& object) = 0;
  virtual void Visit(ASTNodeBase_arg& object) = 0;
  virtual void Visit(ASTNode_NullArg& object) = 0;
  virtual void Visit(ASTNode_PosArg& object) = 0;
  virtual void Visit(ASTNode_StringArg& object) = 0;
  virtual void Visit(ASTNodeBase_data_definitions& object) = 0;
  virtual void Visit(ASTNode_18_data_definitions& object) = 0;
  virtual void Visit(ASTNode_19_data_definitions& object) = 0;
  virtual void Visit(ASTNodeBase_data_definition& object) = 0;
  virtual void Visit(ASTNode_define_code_block& object) = 0;
  virtual void Visit(ASTNode_import& object) = 0;
  virtual void Visit(ASTNode_inherit& object) = 0;
  virtual void Visit(ASTNode_20_import_statement& object) = 0;
  virtual void Visit(ASTNode_21_inherit_statement& object) = 0;
  virtual void Visit(ASTNode_22_general_code_block& object) = 0;
  virtual void Visit(ASTLexeme& object) = 0;
};

//! \brief Define the base node type for all AST nodes.
struct ASTNodeBase {
  ASTNodeBase(const ASTNodeType& node_type, const NonterminalType& nonterminal_type, const ItemID& item_id)
    : node_type(node_type), nonterminal_type(nonterminal_type), item_id(item_id) {}
  virtual ~ASTNodeBase() = default;

  ItemID item_id{};
  ASTNodeType node_type{};
  NonterminalType nonterminal_type{};

  virtual void Accept(ASTNodeVisitor& visitor) = 0;
};

//! \brief Node for basic lexemes.
struct ASTLexeme : ASTNodeBase {
  explicit ASTLexeme(const std::string& literal)
    : ASTNodeBase(ASTNodeType::Literal, NonterminalType::Terminal, -1), literal(literal) {}

  std::string literal{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 32's productions.
// ============================================================

struct ASTNode_0_start : ASTNodeBase {
  explicit ASTNode_0_start(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_0_start, NonterminalType::start, item_id) {}

  std::shared_ptr<ASTNode_17_data_block> data_block{};
  std::shared_ptr<ASTNode_5_lexer_block> lexer_block{};
  std::shared_ptr<ASTNode_Parser> parser_block{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 33's productions.
// ============================================================

struct ASTNode_5_lexer_block : ASTNodeBase {
  explicit ASTNode_5_lexer_block(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_5_lexer_block, NonterminalType::lexer_block, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_lexeme_line>> definitions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 34's productions.
// ============================================================

//! \brief Parent type for non-terminal type 34
struct ASTNodeBase_newlines : ASTNodeBase {
  ASTNodeBase_newlines(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::newlines, item_id) {}

  std::string newlines{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_1_newlines : ASTNodeBase_newlines {
  explicit ASTNodeGeneral_1_newlines(const ItemID& item_id)
    : ASTNodeBase_newlines(ASTNodeType::Type_ASTNodeGeneral_1_newlines, item_id) {}

  std::shared_ptr<ASTNodeBase_newlines> newlines_node{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_2_newlines : ASTNodeBase_newlines {
  explicit ASTNodeGeneral_2_newlines(const ItemID& item_id)
    : ASTNodeBase_newlines(ASTNodeType::Type_ASTNodeGeneral_2_newlines, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 35's productions.
// ============================================================

struct ASTNode_Parser : ASTNodeBase {
  explicit ASTNode_Parser(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_Parser, NonterminalType::parser_block, item_id) {}

  std::vector<std::shared_ptr<ASTNode_parser>> definition{};
  std::string start_name{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 36's productions.
// ============================================================

struct ASTNode_17_data_block : ASTNodeBase {
  explicit ASTNode_17_data_block(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_17_data_block, NonterminalType::data_block, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_data_definition>> definitions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 37's productions.
// ============================================================

//! \brief Parent type for non-terminal type 37
struct ASTNodeBase_lexeme_def_or : ASTNodeBase {
  ASTNodeBase_lexeme_def_or(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::lexeme_def_or, item_id) {}

  std::string or_{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_3_lexeme_def_or : ASTNodeBase_lexeme_def_or {
  explicit ASTNodeGeneral_3_lexeme_def_or(const ItemID& item_id)
    : ASTNodeBase_lexeme_def_or(ASTNodeType::Type_ASTNodeGeneral_3_lexeme_def_or, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_4_lexeme_def_or : ASTNodeBase_lexeme_def_or {
  explicit ASTNodeGeneral_4_lexeme_def_or(const ItemID& item_id)
    : ASTNodeBase_lexeme_def_or(ASTNodeType::Type_ASTNodeGeneral_4_lexeme_def_or, item_id) {}

  std::string newlines{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 38's productions.
// ============================================================

//! \brief Parent type for non-terminal type 38
struct ASTNodeBase_lexer_definitions : ASTNodeBase {
  ASTNodeBase_lexer_definitions(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::lexer_definitions, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_lexeme_line>> definitions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_6_lexer_definitions : ASTNodeBase_lexer_definitions {
  explicit ASTNode_6_lexer_definitions(const ItemID& item_id)
    : ASTNodeBase_lexer_definitions(ASTNodeType::Type_ASTNode_6_lexer_definitions, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_7_lexer_definitions : ASTNodeBase_lexer_definitions {
  explicit ASTNode_7_lexer_definitions(const ItemID& item_id)
    : ASTNodeBase_lexer_definitions(ASTNodeType::Type_ASTNode_7_lexer_definitions, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 39's productions.
// ============================================================

//! \brief Parent type for non-terminal type 39
struct ASTNodeBase_lexeme_line : ASTNodeBase {
  ASTNodeBase_lexeme_line(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::lexeme_line, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_Lexeme : ASTNodeBase_lexeme_line {
  explicit ASTNode_Lexeme(const ItemID& item_id)
    : ASTNodeBase_lexeme_line(ASTNodeType::Type_ASTNode_Lexeme, item_id) {}

  std::string name{};
  std::string regex{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_Skip : ASTNodeBase_lexeme_line {
  explicit ASTNode_Skip(const ItemID& item_id)
    : ASTNodeBase_lexeme_line(ASTNodeType::Type_ASTNode_Skip, item_id) {}

  std::string identifier{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 40's productions.
// ============================================================

struct ASTNode_8_lexeme_definition : ASTNodeBase {
  explicit ASTNode_8_lexeme_definition(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_8_lexeme_definition, NonterminalType::lexeme_definition, item_id) {}

  std::string name{};
  std::string regex{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 41's productions.
// ============================================================

struct ASTNode_9_skip_indicator : ASTNodeBase {
  explicit ASTNode_9_skip_indicator(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_9_skip_indicator, NonterminalType::skip_indicator, item_id) {}

  std::string identifier{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 42's productions.
// ============================================================

struct ASTNode_10_start_indicator : ASTNodeBase {
  explicit ASTNode_10_start_indicator(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_10_start_indicator, NonterminalType::start_indicator, item_id) {}

  std::string start_name{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 43's productions.
// ============================================================

//! \brief Parent type for non-terminal type 43
struct ASTNodeBase_parser_definitions : ASTNodeBase {
  ASTNodeBase_parser_definitions(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::parser_definitions, item_id) {}

  std::vector<std::shared_ptr<ASTNode_parser>> definitions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_11_parser_definitions : ASTNodeBase_parser_definitions {
  explicit ASTNode_11_parser_definitions(const ItemID& item_id)
    : ASTNodeBase_parser_definitions(ASTNodeType::Type_ASTNode_11_parser_definitions, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_12_parser_definitions : ASTNodeBase_parser_definitions {
  explicit ASTNode_12_parser_definitions(const ItemID& item_id)
    : ASTNodeBase_parser_definitions(ASTNodeType::Type_ASTNode_12_parser_definitions, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 44's productions.
// ============================================================

struct ASTNode_parser : ASTNodeBase {
  explicit ASTNode_parser(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_parser, NonterminalType::parser_definition, item_id) {}

  std::string nonterminal_name{};
  std::vector<std::shared_ptr<ASTNodeBase_production_and_data>> productions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 45's productions.
// ============================================================

struct ASTNode_options : ASTNodeBase {
  explicit ASTNode_options(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_options, NonterminalType::production_lines, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_production_and_data>> productions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 46's productions.
// ============================================================

//! \brief Parent type for non-terminal type 46
struct ASTNodeBase_production_and_data : ASTNodeBase {
  ASTNodeBase_production_and_data(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::production_and_data, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_production_atom>> atoms{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_14_production_and_data : ASTNodeBase_production_and_data {
  explicit ASTNode_14_production_and_data(const ItemID& item_id)
    : ASTNodeBase_production_and_data(ASTNodeType::Type_ASTNode_14_production_and_data, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_15_production_and_data : ASTNodeBase_production_and_data {
  explicit ASTNode_15_production_and_data(const ItemID& item_id)
    : ASTNodeBase_production_and_data(ASTNodeType::Type_ASTNode_15_production_and_data, item_id) {}

  std::vector<std::shared_ptr<ASTNode_action>> actions{};
  std::shared_ptr<ASTNodeBase_code_block> code{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 47's productions.
// ============================================================

struct ASTNodeGeneral_13_or : ASTNodeBase {
  explicit ASTNodeGeneral_13_or(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNodeGeneral_13_or, NonterminalType::or_, item_id) {}

  std::string or_{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 48's productions.
// ============================================================

struct ASTNode_production_atoms : ASTNodeBase {
  explicit ASTNode_production_atoms(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_production_atoms, NonterminalType::production_atoms, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_production_atom>> atoms{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 49's productions.
// ============================================================

struct ASTNode_actions : ASTNodeBase {
  explicit ASTNode_actions(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_actions, NonterminalType::actions, item_id) {}

  std::vector<std::shared_ptr<ASTNode_action>> actions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 50's productions.
// ============================================================

//! \brief Parent type for non-terminal type 50
struct ASTNodeBase_code_block : ASTNodeBase {
  ASTNodeBase_code_block(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::code_block, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_16_code_block : ASTNodeBase_code_block {
  explicit ASTNodeGeneral_16_code_block(const ItemID& item_id)
    : ASTNodeBase_code_block(ASTNodeType::Type_ASTNodeGeneral_16_code_block, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_code_block : ASTNodeBase_code_block {
  explicit ASTNode_code_block(const ItemID& item_id)
    : ASTNodeBase_code_block(ASTNodeType::Type_ASTNode_code_block, item_id) {}

  std::string code_body{};
  std::string visitor_name{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 51's productions.
// ============================================================

//! \brief Parent type for non-terminal type 51
struct ASTNodeBase_production_atom : ASTNodeBase {
  ASTNodeBase_production_atom(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::production_atom, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_identifier : ASTNodeBase_production_atom {
  explicit ASTNode_identifier(const ItemID& item_id)
    : ASTNodeBase_production_atom(ASTNodeType::Type_ASTNode_identifier, item_id) {}

  std::string identifier{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_lexeme_name : ASTNodeBase_production_atom {
  explicit ASTNode_lexeme_name(const ItemID& item_id)
    : ASTNodeBase_production_atom(ASTNodeType::Type_ASTNode_lexeme_name, item_id) {}

  std::string lexeme_name{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_literal : ASTNodeBase_production_atom {
  explicit ASTNode_literal(const ItemID& item_id)
    : ASTNodeBase_production_atom(ASTNodeType::Type_ASTNode_literal, item_id) {}

  std::string literal{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_null : ASTNodeBase_production_atom {
  explicit ASTNode_null(const ItemID& item_id)
    : ASTNodeBase_production_atom(ASTNodeType::Type_ASTNode_null, item_id) {}

  std::string null{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 52's productions.
// ============================================================

struct ASTNode_action : ASTNodeBase {
  explicit ASTNode_action(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_action, NonterminalType::action, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_arg>> arguments{};
  std::string fn_name{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 53's productions.
// ============================================================

struct ASTNode_arguments : ASTNodeBase {
  explicit ASTNode_arguments(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_arguments, NonterminalType::args, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_arg>> arguments{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 54's productions.
// ============================================================

//! \brief Parent type for non-terminal type 54
struct ASTNodeBase_arg : ASTNodeBase {
  ASTNodeBase_arg(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::arg, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_NullArg : ASTNodeBase_arg {
  explicit ASTNode_NullArg(const ItemID& item_id)
    : ASTNodeBase_arg(ASTNodeType::Type_ASTNode_NullArg, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_PosArg : ASTNodeBase_arg {
  explicit ASTNode_PosArg(const ItemID& item_id)
    : ASTNodeBase_arg(ASTNodeType::Type_ASTNode_PosArg, item_id) {}

  std::string field_name{};
  std::string pos{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_StringArg : ASTNodeBase_arg {
  explicit ASTNode_StringArg(const ItemID& item_id)
    : ASTNodeBase_arg(ASTNodeType::Type_ASTNode_StringArg, item_id) {}

  std::string argument{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 55's productions.
// ============================================================

//! \brief Parent type for non-terminal type 55
struct ASTNodeBase_data_definitions : ASTNodeBase {
  ASTNodeBase_data_definitions(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::data_definitions, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_data_definition>> definitions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_18_data_definitions : ASTNodeBase_data_definitions {
  explicit ASTNode_18_data_definitions(const ItemID& item_id)
    : ASTNodeBase_data_definitions(ASTNodeType::Type_ASTNode_18_data_definitions, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_19_data_definitions : ASTNodeBase_data_definitions {
  explicit ASTNode_19_data_definitions(const ItemID& item_id)
    : ASTNodeBase_data_definitions(ASTNodeType::Type_ASTNode_19_data_definitions, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 56's productions.
// ============================================================

//! \brief Parent type for non-terminal type 56
struct ASTNodeBase_data_definition : ASTNodeBase {
  ASTNodeBase_data_definition(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::data_definition, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_define_code_block : ASTNodeBase_data_definition {
  explicit ASTNode_define_code_block(const ItemID& item_id)
    : ASTNodeBase_data_definition(ASTNodeType::Type_ASTNode_define_code_block, item_id) {}

  std::string class_name{};
  std::string code{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_import : ASTNodeBase_data_definition {
  explicit ASTNode_import(const ItemID& item_id)
    : ASTNodeBase_data_definition(ASTNodeType::Type_ASTNode_import, item_id) {}

  std::string path{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_inherit : ASTNodeBase_data_definition {
  explicit ASTNode_inherit(const ItemID& item_id)
    : ASTNodeBase_data_definition(ASTNodeType::Type_ASTNode_inherit, item_id) {}

  std::string base{};
  std::string visitor_name{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 57's productions.
// ============================================================

struct ASTNode_20_import_statement : ASTNodeBase {
  explicit ASTNode_20_import_statement(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_20_import_statement, NonterminalType::import_statement, item_id) {}

  std::string path{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 58's productions.
// ============================================================

struct ASTNode_21_inherit_statement : ASTNodeBase {
  explicit ASTNode_21_inherit_statement(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_21_inherit_statement, NonterminalType::inherit_statement, item_id) {}

  std::string base{};
  std::string visitor_name{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 59's productions.
// ============================================================

struct ASTNode_22_general_code_block : ASTNodeBase {
  explicit ASTNode_22_general_code_block(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_22_general_code_block, NonterminalType::general_code_block, item_id) {}

  std::string class_name{};
  std::string code{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};



// ========================================================================
//  LALR Parser.
// ========================================================================

class Parser : public manta::ParserDriverBase<ASTNodeBase, ASTLexeme,Parser> {
  friend class manta::ParserDriverBase<ASTNodeBase, ASTLexeme, Parser>;
public:
  //! \brief Constructor, initializes the parser.
  Parser();

  //! \brief Function to parse the input.
  std::shared_ptr<ASTNodeBase> ParseInput();

protected:
  //! \brief Function that sets up the lexer.
  void createLexer();

  //! \brief The reduce function, which allows this parser to call the reduction functions.
  std::shared_ptr<ASTNodeBase> reduce(unsigned reduction_id, const std::vector<std::shared_ptr<ASTNodeBase>>& collected_nodes);

  std::shared_ptr<ASTNode_0_start>
  ReduceTo_ASTNode_0_start_ViaItem_0(
      const std::shared_ptr<ASTNode_5_lexer_block>& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNode_Parser>& argument_2,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
      const std::shared_ptr<ASTNode_17_data_block>& argument_4,
      const std::string& argument_5);

  std::shared_ptr<ASTNodeGeneral_1_newlines>
  ReduceTo_ASTNodeGeneral_1_newlines_ViaItem_1(
      const std::shared_ptr<ASTNodeBase_newlines>& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNodeGeneral_2_newlines>
  ReduceTo_ASTNodeGeneral_2_newlines_ViaItem_2(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_3_lexeme_def_or>
  ReduceTo_ASTNodeGeneral_3_lexeme_def_or_ViaItem_3(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_4_lexeme_def_or>
  ReduceTo_ASTNodeGeneral_4_lexeme_def_or_ViaItem_4(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNode_5_lexer_block>
  ReduceTo_ASTNode_5_lexer_block_ViaItem_5(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNodeBase_lexer_definitions>& argument_2,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
      const std::string& argument_4);

  std::shared_ptr<ASTNode_6_lexer_definitions>
  ReduceTo_ASTNode_6_lexer_definitions_ViaItem_6(
      const std::shared_ptr<ASTNodeBase_lexer_definitions>& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNodeBase_lexeme_line>& argument_2);

  std::shared_ptr<ASTNode_7_lexer_definitions>
  ReduceTo_ASTNode_7_lexer_definitions_ViaItem_7(
      const std::shared_ptr<ASTNodeBase_lexeme_line>& argument_0);

  std::shared_ptr<ASTNode_Lexeme>
  ReduceTo_ASTNode_Lexeme_ViaItem_8(
      const std::shared_ptr<ASTNode_8_lexeme_definition>& argument_0);

  std::shared_ptr<ASTNode_Skip>
  ReduceTo_ASTNode_Skip_ViaItem_9(
      const std::shared_ptr<ASTNode_9_skip_indicator>& argument_0);

  std::shared_ptr<ASTNode_8_lexeme_definition>
  ReduceTo_ASTNode_8_lexeme_definition_ViaItem_10(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNode_9_skip_indicator>
  ReduceTo_ASTNode_9_skip_indicator_ViaItem_11(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNode_Parser>
  ReduceTo_ASTNode_Parser_ViaItem_12(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNode_10_start_indicator>& argument_2,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
      const std::shared_ptr<ASTNodeBase_parser_definitions>& argument_4,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_5,
      const std::string& argument_6);

  std::shared_ptr<ASTNode_10_start_indicator>
  ReduceTo_ASTNode_10_start_indicator_ViaItem_13(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNode_11_parser_definitions>
  ReduceTo_ASTNode_11_parser_definitions_ViaItem_14(
      const std::shared_ptr<ASTNodeBase_parser_definitions>& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNode_parser>& argument_2);

  std::shared_ptr<ASTNode_12_parser_definitions>
  ReduceTo_ASTNode_12_parser_definitions_ViaItem_15(
      const std::shared_ptr<ASTNode_parser>& argument_0);

  std::shared_ptr<ASTNode_parser>
  ReduceTo_ASTNode_parser_ViaItem_16(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNode_options>& argument_2);

  std::shared_ptr<ASTNode_options>
  ReduceTo_ASTNode_options_ViaItem_17(
      const std::shared_ptr<ASTNodeBase_production_and_data>& argument_0);

  std::shared_ptr<ASTNode_options>
  ReduceTo_ASTNode_options_ViaItem_18(
      const std::shared_ptr<ASTNode_options>& argument_0,
      const std::shared_ptr<ASTNodeGeneral_13_or>& argument_1,
      const std::shared_ptr<ASTNodeBase_production_and_data>& argument_2);

  std::shared_ptr<ASTNodeGeneral_13_or>
  ReduceTo_ASTNodeGeneral_13_or_ViaItem_19(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_14_production_and_data>
  ReduceTo_ASTNode_14_production_and_data_ViaItem_20(
      const std::shared_ptr<ASTNode_production_atoms>& argument_0);

  std::shared_ptr<ASTNode_15_production_and_data>
  ReduceTo_ASTNode_15_production_and_data_ViaItem_21(
      const std::shared_ptr<ASTNode_production_atoms>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNode_actions>& argument_2,
      const std::shared_ptr<ASTNodeBase_code_block>& argument_3);

  std::shared_ptr<ASTNode_production_atoms>
  ReduceTo_ASTNode_production_atoms_ViaItem_22(
      const std::shared_ptr<ASTNode_production_atoms>& argument_0,
      const std::shared_ptr<ASTNodeBase_production_atom>& argument_1);

  std::shared_ptr<ASTNode_production_atoms>
  ReduceTo_ASTNode_production_atoms_ViaItem_23(
      const std::shared_ptr<ASTNodeBase_production_atom>& argument_0);

  std::shared_ptr<ASTNode_identifier>
  ReduceTo_ASTNode_identifier_ViaItem_24(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_literal>
  ReduceTo_ASTNode_literal_ViaItem_25(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_lexeme_name>
  ReduceTo_ASTNode_lexeme_name_ViaItem_26(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_null>
  ReduceTo_ASTNode_null_ViaItem_27(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_actions>
  ReduceTo_ASTNode_actions_ViaItem_28(
      const std::shared_ptr<ASTNode_actions>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNode_action>& argument_2);

  std::shared_ptr<ASTNode_actions>
  ReduceTo_ASTNode_actions_ViaItem_29(
      const std::shared_ptr<ASTNode_action>& argument_0);

  std::shared_ptr<ASTNode_action>
  ReduceTo_ASTNode_action_ViaItem_30(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNode_arguments>& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNode_arguments>
  ReduceTo_ASTNode_arguments_ViaItem_31(
      const std::shared_ptr<ASTNode_arguments>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_arg>& argument_2);

  std::shared_ptr<ASTNode_arguments>
  ReduceTo_ASTNode_arguments_ViaItem_32(
      const std::shared_ptr<ASTNodeBase_arg>& argument_0);

  std::shared_ptr<ASTNode_PosArg>
  ReduceTo_ASTNode_PosArg_ViaItem_33(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_PosArg>
  ReduceTo_ASTNode_PosArg_ViaItem_34(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNode_StringArg>
  ReduceTo_ASTNode_StringArg_ViaItem_35(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_NullArg>
  ReduceTo_ASTNode_NullArg_ViaItem_36(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_16_code_block>
  ReduceTo_ASTNodeGeneral_16_code_block_ViaItem_37();

  std::shared_ptr<ASTNode_code_block>
  ReduceTo_ASTNode_code_block_ViaItem_38(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNode_17_data_block>
  ReduceTo_ASTNode_17_data_block_ViaItem_39(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNodeBase_data_definitions>& argument_2,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
      const std::string& argument_4);

  std::shared_ptr<ASTNode_18_data_definitions>
  ReduceTo_ASTNode_18_data_definitions_ViaItem_40(
      const std::shared_ptr<ASTNodeBase_data_definitions>& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNodeBase_data_definition>& argument_2);

  std::shared_ptr<ASTNode_19_data_definitions>
  ReduceTo_ASTNode_19_data_definitions_ViaItem_41(
      const std::shared_ptr<ASTNodeBase_data_definition>& argument_0);

  std::shared_ptr<ASTNode_import>
  ReduceTo_ASTNode_import_ViaItem_42(
      const std::shared_ptr<ASTNode_20_import_statement>& argument_0);

  std::shared_ptr<ASTNode_inherit>
  ReduceTo_ASTNode_inherit_ViaItem_43(
      const std::shared_ptr<ASTNode_21_inherit_statement>& argument_0);

  std::shared_ptr<ASTNode_define_code_block>
  ReduceTo_ASTNode_define_code_block_ViaItem_44(
      const std::shared_ptr<ASTNode_22_general_code_block>& argument_0);

  std::shared_ptr<ASTNode_20_import_statement>
  ReduceTo_ASTNode_20_import_statement_ViaItem_45(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNode_21_inherit_statement>
  ReduceTo_ASTNode_21_inherit_statement_ViaItem_46(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNode_22_general_code_block>
  ReduceTo_ASTNode_22_general_code_block_ViaItem_47(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2,
      const std::string& argument_3);

};

inline Parser::Parser() {
  using namespace manta;

  start_nonterminal_ = 32;
  // Allocate space for the parser table.
  parse_table_.assign(87, std::vector<Entry>(60,Entry()));

  // Create the table. There are better, though more difficult, ways to serialize this information.
  parse_table_[0][17] = Entry(1);  // Shift
  parse_table_[0][32] = Entry(true);  // Accept
  parse_table_[0][33] = Entry(2);  // Shift
  parse_table_[1][15] = Entry(3);  // Shift
  parse_table_[1][34] = Entry(4);  // Shift
  parse_table_[2][15] = Entry(3);  // Shift
  parse_table_[2][34] = Entry(5);  // Shift
  parse_table_[3][2] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][4] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][15] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][18] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][19] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][20] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][21] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][28] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][29] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][30] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[3][31] = Entry(Item(34, 2, 0, {15}, 2));  // Reduce
  parse_table_[4][2] = Entry(6);  // Shift
  parse_table_[4][15] = Entry(7);  // Shift
  parse_table_[4][19] = Entry(8);  // Shift
  parse_table_[4][38] = Entry(9);  // Shift
  parse_table_[4][39] = Entry(10);  // Shift
  parse_table_[4][40] = Entry(11);  // Shift
  parse_table_[4][41] = Entry(12);  // Shift
  parse_table_[5][15] = Entry(7);  // Shift
  parse_table_[5][20] = Entry(13);  // Shift
  parse_table_[5][35] = Entry(14);  // Shift
  parse_table_[6][3] = Entry(15);  // Shift
  parse_table_[7][2] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][4] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][15] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][18] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][19] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][20] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][21] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][28] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][29] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][30] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[7][31] = Entry(Item(34, 1, 0, {34, 15}, 1));  // Reduce
  parse_table_[8][4] = Entry(16);  // Shift
  parse_table_[9][15] = Entry(3);  // Shift
  parse_table_[9][34] = Entry(17);  // Shift
  parse_table_[10][15] = Entry(Item(38, 7, 0, {39}, 7));  // Reduce
  parse_table_[11][15] = Entry(Item(39, 8, 0, {40}, 8));  // Reduce
  parse_table_[12][15] = Entry(Item(39, 9, 0, {41}, 9));  // Reduce
  parse_table_[13][15] = Entry(3);  // Shift
  parse_table_[13][34] = Entry(18);  // Shift
  parse_table_[14][15] = Entry(3);  // Shift
  parse_table_[14][34] = Entry(19);  // Shift
  parse_table_[15][5] = Entry(20);  // Shift
  parse_table_[16][15] = Entry(Item(41, 11, 0, {19, 4}, 11));  // Reduce
  parse_table_[17][2] = Entry(6);  // Shift
  parse_table_[17][15] = Entry(7);  // Shift
  parse_table_[17][18] = Entry(21);  // Shift
  parse_table_[17][19] = Entry(8);  // Shift
  parse_table_[17][39] = Entry(22);  // Shift
  parse_table_[17][40] = Entry(11);  // Shift
  parse_table_[17][41] = Entry(12);  // Shift
  parse_table_[18][15] = Entry(7);  // Shift
  parse_table_[18][21] = Entry(23);  // Shift
  parse_table_[18][42] = Entry(24);  // Shift
  parse_table_[19][15] = Entry(7);  // Shift
  parse_table_[19][28] = Entry(25);  // Shift
  parse_table_[19][36] = Entry(26);  // Shift
  parse_table_[20][15] = Entry(Item(40, 10, 0, {2, 3, 5}, 10));  // Reduce
  parse_table_[21][0] = Entry(Item(33, 5, 0, {17, 34, 38, 34, 18}, 5));  // Reduce
  parse_table_[21][15] = Entry(Item(33, 5, 0, {17, 34, 38, 34, 18}, 5));  // Reduce
  parse_table_[22][15] = Entry(Item(38, 6, 0, {38, 34, 39}, 6));  // Reduce
  parse_table_[23][4] = Entry(27);  // Shift
  parse_table_[24][15] = Entry(3);  // Shift
  parse_table_[24][34] = Entry(28);  // Shift
  parse_table_[25][15] = Entry(3);  // Shift
  parse_table_[25][34] = Entry(29);  // Shift
  parse_table_[26][0] = Entry(30);  // Shift
  parse_table_[27][15] = Entry(Item(42, 13, 0, {21, 4}, 13));  // Reduce
  parse_table_[28][4] = Entry(31);  // Shift
  parse_table_[28][15] = Entry(7);  // Shift
  parse_table_[28][43] = Entry(32);  // Shift
  parse_table_[28][44] = Entry(33);  // Shift
  parse_table_[29][15] = Entry(7);  // Shift
  parse_table_[29][29] = Entry(34);  // Shift
  parse_table_[29][30] = Entry(35);  // Shift
  parse_table_[29][31] = Entry(36);  // Shift
  parse_table_[29][55] = Entry(37);  // Shift
  parse_table_[29][56] = Entry(38);  // Shift
  parse_table_[29][57] = Entry(39);  // Shift
  parse_table_[29][58] = Entry(40);  // Shift
  parse_table_[29][59] = Entry(41);  // Shift
  parse_table_[30][0] = Entry(Item(32, 0, 0, {33, 34, 35, 34, 36, 0}, 0));  // Reduce
  parse_table_[31][22] = Entry(42);  // Shift
  parse_table_[32][15] = Entry(3);  // Shift
  parse_table_[32][34] = Entry(43);  // Shift
  parse_table_[33][15] = Entry(Item(43, 15, 0, {44}, 15));  // Reduce
  parse_table_[34][11] = Entry(44);  // Shift
  parse_table_[35][4] = Entry(45);  // Shift
  parse_table_[36][4] = Entry(46);  // Shift
  parse_table_[37][15] = Entry(3);  // Shift
  parse_table_[37][34] = Entry(47);  // Shift
  parse_table_[38][15] = Entry(Item(55, 41, 0, {56}, 41));  // Reduce
  parse_table_[39][15] = Entry(Item(56, 42, 0, {57}, 42));  // Reduce
  parse_table_[40][15] = Entry(Item(56, 43, 0, {58}, 43));  // Reduce
  parse_table_[41][15] = Entry(Item(56, 44, 0, {59}, 44));  // Reduce
  parse_table_[42][2] = Entry(48);  // Shift
  parse_table_[42][4] = Entry(49);  // Shift
  parse_table_[42][6] = Entry(50);  // Shift
  parse_table_[42][9] = Entry(51);  // Shift
  parse_table_[42][45] = Entry(52);  // Shift
  parse_table_[42][46] = Entry(53);  // Shift
  parse_table_[42][48] = Entry(54);  // Shift
  parse_table_[42][51] = Entry(55);  // Shift
  parse_table_[43][4] = Entry(31);  // Shift
  parse_table_[43][15] = Entry(7);  // Shift
  parse_table_[43][18] = Entry(56);  // Shift
  parse_table_[43][44] = Entry(57);  // Shift
  parse_table_[44][15] = Entry(Item(57, 45, 0, {29, 11}, 45));  // Reduce
  parse_table_[45][12] = Entry(58);  // Shift
  parse_table_[46][27] = Entry(59);  // Shift
  parse_table_[47][15] = Entry(7);  // Shift
  parse_table_[47][18] = Entry(60);  // Shift
  parse_table_[47][29] = Entry(34);  // Shift
  parse_table_[47][30] = Entry(35);  // Shift
  parse_table_[47][31] = Entry(36);  // Shift
  parse_table_[47][56] = Entry(61);  // Shift
  parse_table_[47][57] = Entry(39);  // Shift
  parse_table_[47][58] = Entry(40);  // Shift
  parse_table_[47][59] = Entry(41);  // Shift
  parse_table_[48][2] = Entry(Item(51, 26, 0, {2}, 26));  // Reduce
  parse_table_[48][3] = Entry(Item(51, 26, 0, {2}, 26));  // Reduce
  parse_table_[48][4] = Entry(Item(51, 26, 0, {2}, 26));  // Reduce
  parse_table_[48][6] = Entry(Item(51, 26, 0, {2}, 26));  // Reduce
  parse_table_[48][7] = Entry(Item(51, 26, 0, {2}, 26));  // Reduce
  parse_table_[48][9] = Entry(Item(51, 26, 0, {2}, 26));  // Reduce
  parse_table_[48][15] = Entry(Item(51, 26, 0, {2}, 26));  // Reduce
  parse_table_[49][2] = Entry(Item(51, 24, 0, {4}, 24));  // Reduce
  parse_table_[49][3] = Entry(Item(51, 24, 0, {4}, 24));  // Reduce
  parse_table_[49][4] = Entry(Item(51, 24, 0, {4}, 24));  // Reduce
  parse_table_[49][6] = Entry(Item(51, 24, 0, {4}, 24));  // Reduce
  parse_table_[49][7] = Entry(Item(51, 24, 0, {4}, 24));  // Reduce
  parse_table_[49][9] = Entry(Item(51, 24, 0, {4}, 24));  // Reduce
  parse_table_[49][15] = Entry(Item(51, 24, 0, {4}, 24));  // Reduce
  parse_table_[50][2] = Entry(Item(51, 25, 0, {6}, 25));  // Reduce
  parse_table_[50][3] = Entry(Item(51, 25, 0, {6}, 25));  // Reduce
  parse_table_[50][4] = Entry(Item(51, 25, 0, {6}, 25));  // Reduce
  parse_table_[50][6] = Entry(Item(51, 25, 0, {6}, 25));  // Reduce
  parse_table_[50][7] = Entry(Item(51, 25, 0, {6}, 25));  // Reduce
  parse_table_[50][9] = Entry(Item(51, 25, 0, {6}, 25));  // Reduce
  parse_table_[50][15] = Entry(Item(51, 25, 0, {6}, 25));  // Reduce
  parse_table_[51][2] = Entry(Item(51, 27, 0, {9}, 27));  // Reduce
  parse_table_[51][3] = Entry(Item(51, 27, 0, {9}, 27));  // Reduce
  parse_table_[51][4] = Entry(Item(51, 27, 0, {9}, 27));  // Reduce
  parse_table_[51][6] = Entry(Item(51, 27, 0, {9}, 27));  // Reduce
  parse_table_[51][7] = Entry(Item(51, 27, 0, {9}, 27));  // Reduce
  parse_table_[51][9] = Entry(Item(51, 27, 0, {9}, 27));  // Reduce
  parse_table_[51][15] = Entry(Item(51, 27, 0, {9}, 27));  // Reduce
  parse_table_[52][7] = Entry(62);  // Shift
  parse_table_[52][15] = Entry(Item(44, 16, 0, {4, 22, 45}, 16));  // Reduce
  parse_table_[52][47] = Entry(63);  // Shift
  parse_table_[53][7] = Entry(Item(45, 17, 0, {46}, 17));  // Reduce
  parse_table_[53][15] = Entry(Item(45, 17, 0, {46}, 17));  // Reduce
  parse_table_[54][2] = Entry(48);  // Shift
  parse_table_[54][3] = Entry(64);  // Shift
  parse_table_[54][4] = Entry(49);  // Shift
  parse_table_[54][6] = Entry(50);  // Shift
  parse_table_[54][7] = Entry(Item(46, 20, 0, {48}, 20));  // Reduce
  parse_table_[54][9] = Entry(51);  // Shift
  parse_table_[54][15] = Entry(Item(46, 20, 0, {48}, 20));  // Reduce
  parse_table_[54][51] = Entry(65);  // Shift
  parse_table_[55][2] = Entry(Item(48, 23, 0, {51}, 23));  // Reduce
  parse_table_[55][3] = Entry(Item(48, 23, 0, {51}, 23));  // Reduce
  parse_table_[55][4] = Entry(Item(48, 23, 0, {51}, 23));  // Reduce
  parse_table_[55][6] = Entry(Item(48, 23, 0, {51}, 23));  // Reduce
  parse_table_[55][7] = Entry(Item(48, 23, 0, {51}, 23));  // Reduce
  parse_table_[55][9] = Entry(Item(48, 23, 0, {51}, 23));  // Reduce
  parse_table_[55][15] = Entry(Item(48, 23, 0, {51}, 23));  // Reduce
  parse_table_[56][15] = Entry(Item(35, 12, 0, {20, 34, 42, 34, 43, 34, 18}, 12));  // Reduce
  parse_table_[57][15] = Entry(Item(43, 14, 0, {43, 34, 44}, 14));  // Reduce
  parse_table_[58][15] = Entry(Item(58, 46, 0, {30, 4, 12}, 46));  // Reduce
  parse_table_[59][13] = Entry(66);  // Shift
  parse_table_[60][0] = Entry(Item(36, 39, 0, {28, 34, 55, 34, 18}, 39));  // Reduce
  parse_table_[61][15] = Entry(Item(55, 40, 0, {55, 34, 56}, 40));  // Reduce
  parse_table_[62][2] = Entry(Item(47, 19, 0, {7}, 19));  // Reduce
  parse_table_[62][4] = Entry(Item(47, 19, 0, {7}, 19));  // Reduce
  parse_table_[62][6] = Entry(Item(47, 19, 0, {7}, 19));  // Reduce
  parse_table_[62][9] = Entry(Item(47, 19, 0, {7}, 19));  // Reduce
  parse_table_[63][2] = Entry(48);  // Shift
  parse_table_[63][4] = Entry(49);  // Shift
  parse_table_[63][6] = Entry(50);  // Shift
  parse_table_[63][9] = Entry(51);  // Shift
  parse_table_[63][46] = Entry(67);  // Shift
  parse_table_[63][48] = Entry(54);  // Shift
  parse_table_[63][51] = Entry(55);  // Shift
  parse_table_[64][4] = Entry(68);  // Shift
  parse_table_[64][49] = Entry(69);  // Shift
  parse_table_[64][52] = Entry(70);  // Shift
  parse_table_[65][2] = Entry(Item(48, 22, 0, {48, 51}, 22));  // Reduce
  parse_table_[65][3] = Entry(Item(48, 22, 0, {48, 51}, 22));  // Reduce
  parse_table_[65][4] = Entry(Item(48, 22, 0, {48, 51}, 22));  // Reduce
  parse_table_[65][6] = Entry(Item(48, 22, 0, {48, 51}, 22));  // Reduce
  parse_table_[65][7] = Entry(Item(48, 22, 0, {48, 51}, 22));  // Reduce
  parse_table_[65][9] = Entry(Item(48, 22, 0, {48, 51}, 22));  // Reduce
  parse_table_[65][15] = Entry(Item(48, 22, 0, {48, 51}, 22));  // Reduce
  parse_table_[66][15] = Entry(Item(59, 47, 0, {31, 4, 27, 13}, 47));  // Reduce
  parse_table_[67][7] = Entry(Item(45, 18, 0, {45, 47, 46}, 18));  // Reduce
  parse_table_[67][15] = Entry(Item(45, 18, 0, {45, 47, 46}, 18));  // Reduce
  parse_table_[68][24] = Entry(71);  // Shift
  parse_table_[69][7] = Entry(Item(50, 37, 0, {}, 37));  // Reduce
  parse_table_[69][15] = Entry(Item(50, 37, 0, {}, 37));  // Reduce
  parse_table_[69][23] = Entry(72);  // Shift
  parse_table_[69][27] = Entry(73);  // Shift
  parse_table_[69][50] = Entry(74);  // Shift
  parse_table_[70][7] = Entry(Item(49, 29, 0, {52}, 29));  // Reduce
  parse_table_[70][15] = Entry(Item(49, 29, 0, {52}, 29));  // Reduce
  parse_table_[70][23] = Entry(Item(49, 29, 0, {52}, 29));  // Reduce
  parse_table_[70][27] = Entry(Item(49, 29, 0, {52}, 29));  // Reduce
  parse_table_[71][6] = Entry(75);  // Shift
  parse_table_[71][8] = Entry(76);  // Shift
  parse_table_[71][26] = Entry(77);  // Shift
  parse_table_[71][53] = Entry(78);  // Shift
  parse_table_[71][54] = Entry(79);  // Shift
  parse_table_[72][4] = Entry(68);  // Shift
  parse_table_[72][52] = Entry(80);  // Shift
  parse_table_[73][4] = Entry(81);  // Shift
  parse_table_[74][7] = Entry(Item(46, 21, 0, {48, 3, 49, 50}, 21));  // Reduce
  parse_table_[74][15] = Entry(Item(46, 21, 0, {48, 3, 49, 50}, 21));  // Reduce
  parse_table_[75][23] = Entry(Item(54, 35, 0, {6}, 35));  // Reduce
  parse_table_[75][25] = Entry(Item(54, 35, 0, {6}, 35));  // Reduce
  parse_table_[76][1] = Entry(82);  // Shift
  parse_table_[76][23] = Entry(Item(54, 33, 0, {8}, 33));  // Reduce
  parse_table_[76][25] = Entry(Item(54, 33, 0, {8}, 33));  // Reduce
  parse_table_[77][23] = Entry(Item(54, 36, 0, {26}, 36));  // Reduce
  parse_table_[77][25] = Entry(Item(54, 36, 0, {26}, 36));  // Reduce
  parse_table_[78][23] = Entry(83);  // Shift
  parse_table_[78][25] = Entry(84);  // Shift
  parse_table_[79][23] = Entry(Item(53, 32, 0, {54}, 32));  // Reduce
  parse_table_[79][25] = Entry(Item(53, 32, 0, {54}, 32));  // Reduce
  parse_table_[80][7] = Entry(Item(49, 28, 0, {49, 23, 52}, 28));  // Reduce
  parse_table_[80][15] = Entry(Item(49, 28, 0, {49, 23, 52}, 28));  // Reduce
  parse_table_[80][23] = Entry(Item(49, 28, 0, {49, 23, 52}, 28));  // Reduce
  parse_table_[80][27] = Entry(Item(49, 28, 0, {49, 23, 52}, 28));  // Reduce
  parse_table_[81][13] = Entry(85);  // Shift
  parse_table_[82][23] = Entry(Item(54, 34, 0, {8, 1}, 34));  // Reduce
  parse_table_[82][25] = Entry(Item(54, 34, 0, {8, 1}, 34));  // Reduce
  parse_table_[83][6] = Entry(75);  // Shift
  parse_table_[83][8] = Entry(76);  // Shift
  parse_table_[83][26] = Entry(77);  // Shift
  parse_table_[83][54] = Entry(86);  // Shift
  parse_table_[84][7] = Entry(Item(52, 30, 0, {4, 24, 53, 25}, 30));  // Reduce
  parse_table_[84][15] = Entry(Item(52, 30, 0, {4, 24, 53, 25}, 30));  // Reduce
  parse_table_[84][23] = Entry(Item(52, 30, 0, {4, 24, 53, 25}, 30));  // Reduce
  parse_table_[84][27] = Entry(Item(52, 30, 0, {4, 24, 53, 25}, 30));  // Reduce
  parse_table_[85][7] = Entry(Item(50, 38, 0, {27, 4, 13}, 38));  // Reduce
  parse_table_[85][15] = Entry(Item(50, 38, 0, {27, 4, 13}, 38));  // Reduce
  parse_table_[86][23] = Entry(Item(53, 31, 0, {53, 23, 54}, 31));  // Reduce
  parse_table_[86][25] = Entry(Item(53, 31, 0, {53, 23, 54}, 31));  // Reduce

  // Create inverse non-terminal map.
  inverse_nonterminal_map_.emplace(32, "start");
  inverse_nonterminal_map_.emplace(33, "lexer_block");
  inverse_nonterminal_map_.emplace(34, "newlines");
  inverse_nonterminal_map_.emplace(35, "parser_block");
  inverse_nonterminal_map_.emplace(36, "data_block");
  inverse_nonterminal_map_.emplace(37, "lexeme_def_or");
  inverse_nonterminal_map_.emplace(38, "lexer_definitions");
  inverse_nonterminal_map_.emplace(39, "lexeme_line");
  inverse_nonterminal_map_.emplace(40, "lexeme_definition");
  inverse_nonterminal_map_.emplace(41, "skip_indicator");
  inverse_nonterminal_map_.emplace(42, "start_indicator");
  inverse_nonterminal_map_.emplace(43, "parser_definitions");
  inverse_nonterminal_map_.emplace(44, "parser_definition");
  inverse_nonterminal_map_.emplace(45, "production_lines");
  inverse_nonterminal_map_.emplace(46, "production_and_data");
  inverse_nonterminal_map_.emplace(47, "or");
  inverse_nonterminal_map_.emplace(48, "production_atoms");
  inverse_nonterminal_map_.emplace(49, "actions");
  inverse_nonterminal_map_.emplace(50, "code_block");
  inverse_nonterminal_map_.emplace(51, "production_atom");
  inverse_nonterminal_map_.emplace(52, "action");
  inverse_nonterminal_map_.emplace(53, "args");
  inverse_nonterminal_map_.emplace(54, "arg");
  inverse_nonterminal_map_.emplace(55, "data_definitions");
  inverse_nonterminal_map_.emplace(56, "data_definition");
  inverse_nonterminal_map_.emplace(57, "import_statement");
  inverse_nonterminal_map_.emplace(58, "inherit_statement");
  inverse_nonterminal_map_.emplace(59, "general_code_block");

  createLexer();
}

inline std::shared_ptr<ASTNodeBase> Parser::ParseInput() {
  return parse();
}

inline void Parser::createLexer() {
  auto lexer_generator = std::make_shared<manta::LexerGenerator>();

  // Lexeme "eof" will be automatically added as the first (0-th) lexeme.
  lexer_generator->AddLexeme("special_symbol", ".(\\@ | _)+", 1);  // Lexeme #1
  lexer_generator->AddLexeme("lexeme_name", "@(\\@ | _)+", 1);  // Lexeme #2
  lexer_generator->AddLexeme("colon", ":", 1);  // Lexeme #3
  lexer_generator->AddLexeme("identifier", "( \\@ | _ )+", 1);  // Lexeme #4
  lexer_generator->AddLexeme("regex", " r ` ( \\\\ ` | [~ `] )* ` ", 1);  // Lexeme #5
  lexer_generator->AddLexeme("literal", " \" ( \\\\\" | [~\"] )* \" ", 1);  // Lexeme #6
  lexer_generator->AddLexeme("or", "\\|", 1);  // Lexeme #7
  lexer_generator->AddLexeme("pos_arg", "$ \\d+", 1);  // Lexeme #8
  lexer_generator->AddLexeme("null", "$null", 1);  // Lexeme #9
  lexer_generator->AddLexeme("comment", "# [~\\n]* \\n", 1);  // Lexeme #10
  lexer_generator->AddLexeme("path", "\\@+/(\\@+/)+\\@+.h", 1);  // Lexeme #11
  lexer_generator->AddLexeme("classname", "( \\@ | _ )+(::( \\@ | _ )+)*", 1);  // Lexeme #12
  lexer_generator->AddLexeme("code", " \\{ [~%\\}]* %\\} ", 1);  // Lexeme #13
  lexer_generator->AddLexeme("spaces", "\\s+", 1);  // Lexeme #14
  lexer_generator->AddLexeme("newlines", "\\n+", 1);  // Lexeme #15
  lexer_generator->AddLexeme("continuation", "\\\\ \\n", 1);  // Lexeme #16
  lexer_generator->AddReserved(".Lexer", 2);  // Lexeme #17
  lexer_generator->AddReserved(".End", 2);  // Lexeme #18
  lexer_generator->AddReserved(".Skip", 2);  // Lexeme #19
  lexer_generator->AddReserved(".Parser", 2);  // Lexeme #20
  lexer_generator->AddReserved(".Start", 2);  // Lexeme #21
  lexer_generator->AddReserved("->", 2);  // Lexeme #22
  lexer_generator->AddReserved(",", 2);  // Lexeme #23
  lexer_generator->AddReserved("(", 2);  // Lexeme #24
  lexer_generator->AddReserved(")", 2);  // Lexeme #25
  lexer_generator->AddReserved("$null", 2);  // Lexeme #26
  lexer_generator->AddReserved("%", 2);  // Lexeme #27
  lexer_generator->AddReserved(".Data", 2);  // Lexeme #28
  lexer_generator->AddReserved("@import", 2);  // Lexeme #29
  lexer_generator->AddReserved("inherits", 2);  // Lexeme #30
  lexer_generator->AddReserved("code", 2);  // Lexeme #31

  // Add the skip-lexemes (these will be lexed, but skipped, by the lexer).
  lexer_generator->AddSkip("comment");
  lexer_generator->AddSkip("spaces");
  lexer_generator->AddSkip("continuation");

  lexer_ = lexer_generator->CreateLexer();
}

inline std::shared_ptr<ASTNodeBase> Parser::reduce(unsigned reduction_id, const std::vector<std::shared_ptr<ASTNodeBase>>& collected_nodes) {
  switch (reduction_id) {
    case 0: {
      REDUCE_ASSERT(6, 0, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_0_start_ViaItem_0'.";
      return ReduceTo_ASTNode_0_start_ViaItem_0(
          std::reinterpret_pointer_cast<ASTNode_5_lexer_block>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNode_Parser>(collected_nodes[2]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[3]),
          std::reinterpret_pointer_cast<ASTNode_17_data_block>(collected_nodes[4]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[5].get())->literal);
    }
    case 1: {
      REDUCE_ASSERT(2, 1, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_1_newlines_ViaItem_1'.";
      return ReduceTo_ASTNodeGeneral_1_newlines_ViaItem_1(
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 2: {
      REDUCE_ASSERT(1, 2, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_2_newlines_ViaItem_2'.";
      return ReduceTo_ASTNodeGeneral_2_newlines_ViaItem_2(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 3: {
      REDUCE_ASSERT(1, 3, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_3_lexeme_def_or_ViaItem_3'.";
      return ReduceTo_ASTNodeGeneral_3_lexeme_def_or_ViaItem_3(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 4: {
      REDUCE_ASSERT(2, 4, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_4_lexeme_def_or_ViaItem_4'.";
      return ReduceTo_ASTNodeGeneral_4_lexeme_def_or_ViaItem_4(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 5: {
      REDUCE_ASSERT(5, 5, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_5_lexer_block_ViaItem_5'.";
      return ReduceTo_ASTNode_5_lexer_block_ViaItem_5(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_lexer_definitions>(collected_nodes[2]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[3]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[4].get())->literal);
    }
    case 6: {
      REDUCE_ASSERT(3, 6, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_6_lexer_definitions_ViaItem_6'.";
      return ReduceTo_ASTNode_6_lexer_definitions_ViaItem_6(
          std::reinterpret_pointer_cast<ASTNodeBase_lexer_definitions>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_lexeme_line>(collected_nodes[2]));
    }
    case 7: {
      REDUCE_ASSERT(1, 7, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_7_lexer_definitions_ViaItem_7'.";
      return ReduceTo_ASTNode_7_lexer_definitions_ViaItem_7(
          std::reinterpret_pointer_cast<ASTNodeBase_lexeme_line>(collected_nodes[0]));
    }
    case 8: {
      REDUCE_ASSERT(1, 8, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_Lexeme_ViaItem_8'.";
      return ReduceTo_ASTNode_Lexeme_ViaItem_8(
          std::reinterpret_pointer_cast<ASTNode_8_lexeme_definition>(collected_nodes[0]));
    }
    case 9: {
      REDUCE_ASSERT(1, 9, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_Skip_ViaItem_9'.";
      return ReduceTo_ASTNode_Skip_ViaItem_9(
          std::reinterpret_pointer_cast<ASTNode_9_skip_indicator>(collected_nodes[0]));
    }
    case 10: {
      REDUCE_ASSERT(3, 10, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_8_lexeme_definition_ViaItem_10'.";
      return ReduceTo_ASTNode_8_lexeme_definition_ViaItem_10(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 11: {
      REDUCE_ASSERT(2, 11, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_9_skip_indicator_ViaItem_11'.";
      return ReduceTo_ASTNode_9_skip_indicator_ViaItem_11(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 12: {
      REDUCE_ASSERT(7, 12, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_Parser_ViaItem_12'.";
      return ReduceTo_ASTNode_Parser_ViaItem_12(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNode_10_start_indicator>(collected_nodes[2]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[3]),
          std::reinterpret_pointer_cast<ASTNodeBase_parser_definitions>(collected_nodes[4]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[5]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[6].get())->literal);
    }
    case 13: {
      REDUCE_ASSERT(2, 13, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_10_start_indicator_ViaItem_13'.";
      return ReduceTo_ASTNode_10_start_indicator_ViaItem_13(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 14: {
      REDUCE_ASSERT(3, 14, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_11_parser_definitions_ViaItem_14'.";
      return ReduceTo_ASTNode_11_parser_definitions_ViaItem_14(
          std::reinterpret_pointer_cast<ASTNodeBase_parser_definitions>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNode_parser>(collected_nodes[2]));
    }
    case 15: {
      REDUCE_ASSERT(1, 15, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_12_parser_definitions_ViaItem_15'.";
      return ReduceTo_ASTNode_12_parser_definitions_ViaItem_15(
          std::reinterpret_pointer_cast<ASTNode_parser>(collected_nodes[0]));
    }
    case 16: {
      REDUCE_ASSERT(3, 16, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_parser_ViaItem_16'.";
      return ReduceTo_ASTNode_parser_ViaItem_16(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNode_options>(collected_nodes[2]));
    }
    case 17: {
      REDUCE_ASSERT(1, 17, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_options_ViaItem_17'.";
      return ReduceTo_ASTNode_options_ViaItem_17(
          std::reinterpret_pointer_cast<ASTNodeBase_production_and_data>(collected_nodes[0]));
    }
    case 18: {
      REDUCE_ASSERT(3, 18, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_options_ViaItem_18'.";
      return ReduceTo_ASTNode_options_ViaItem_18(
          std::reinterpret_pointer_cast<ASTNode_options>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeGeneral_13_or>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_production_and_data>(collected_nodes[2]));
    }
    case 19: {
      REDUCE_ASSERT(1, 19, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_13_or_ViaItem_19'.";
      return ReduceTo_ASTNodeGeneral_13_or_ViaItem_19(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 20: {
      REDUCE_ASSERT(1, 20, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_14_production_and_data_ViaItem_20'.";
      return ReduceTo_ASTNode_14_production_and_data_ViaItem_20(
          std::reinterpret_pointer_cast<ASTNode_production_atoms>(collected_nodes[0]));
    }
    case 21: {
      REDUCE_ASSERT(4, 21, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_15_production_and_data_ViaItem_21'.";
      return ReduceTo_ASTNode_15_production_and_data_ViaItem_21(
          std::reinterpret_pointer_cast<ASTNode_production_atoms>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNode_actions>(collected_nodes[2]),
          std::reinterpret_pointer_cast<ASTNodeBase_code_block>(collected_nodes[3]));
    }
    case 22: {
      REDUCE_ASSERT(2, 22, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_production_atoms_ViaItem_22'.";
      return ReduceTo_ASTNode_production_atoms_ViaItem_22(
          std::reinterpret_pointer_cast<ASTNode_production_atoms>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_production_atom>(collected_nodes[1]));
    }
    case 23: {
      REDUCE_ASSERT(1, 23, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_production_atoms_ViaItem_23'.";
      return ReduceTo_ASTNode_production_atoms_ViaItem_23(
          std::reinterpret_pointer_cast<ASTNodeBase_production_atom>(collected_nodes[0]));
    }
    case 24: {
      REDUCE_ASSERT(1, 24, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_identifier_ViaItem_24'.";
      return ReduceTo_ASTNode_identifier_ViaItem_24(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 25: {
      REDUCE_ASSERT(1, 25, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_literal_ViaItem_25'.";
      return ReduceTo_ASTNode_literal_ViaItem_25(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 26: {
      REDUCE_ASSERT(1, 26, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_lexeme_name_ViaItem_26'.";
      return ReduceTo_ASTNode_lexeme_name_ViaItem_26(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 27: {
      REDUCE_ASSERT(1, 27, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_null_ViaItem_27'.";
      return ReduceTo_ASTNode_null_ViaItem_27(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 28: {
      REDUCE_ASSERT(3, 28, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_actions_ViaItem_28'.";
      return ReduceTo_ASTNode_actions_ViaItem_28(
          std::reinterpret_pointer_cast<ASTNode_actions>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNode_action>(collected_nodes[2]));
    }
    case 29: {
      REDUCE_ASSERT(1, 29, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_actions_ViaItem_29'.";
      return ReduceTo_ASTNode_actions_ViaItem_29(
          std::reinterpret_pointer_cast<ASTNode_action>(collected_nodes[0]));
    }
    case 30: {
      REDUCE_ASSERT(4, 30, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_action_ViaItem_30'.";
      return ReduceTo_ASTNode_action_ViaItem_30(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNode_arguments>(collected_nodes[2]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 31: {
      REDUCE_ASSERT(3, 31, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_arguments_ViaItem_31'.";
      return ReduceTo_ASTNode_arguments_ViaItem_31(
          std::reinterpret_pointer_cast<ASTNode_arguments>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_arg>(collected_nodes[2]));
    }
    case 32: {
      REDUCE_ASSERT(1, 32, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_arguments_ViaItem_32'.";
      return ReduceTo_ASTNode_arguments_ViaItem_32(
          std::reinterpret_pointer_cast<ASTNodeBase_arg>(collected_nodes[0]));
    }
    case 33: {
      REDUCE_ASSERT(1, 33, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_PosArg_ViaItem_33'.";
      return ReduceTo_ASTNode_PosArg_ViaItem_33(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 34: {
      REDUCE_ASSERT(2, 34, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_PosArg_ViaItem_34'.";
      return ReduceTo_ASTNode_PosArg_ViaItem_34(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 35: {
      REDUCE_ASSERT(1, 35, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_StringArg_ViaItem_35'.";
      return ReduceTo_ASTNode_StringArg_ViaItem_35(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 36: {
      REDUCE_ASSERT(1, 36, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_NullArg_ViaItem_36'.";
      return ReduceTo_ASTNode_NullArg_ViaItem_36(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 37: {
      REDUCE_ASSERT(0, 37, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_16_code_block_ViaItem_37'.";
      return ReduceTo_ASTNodeGeneral_16_code_block_ViaItem_37();
    }
    case 38: {
      REDUCE_ASSERT(3, 38, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_code_block_ViaItem_38'.";
      return ReduceTo_ASTNode_code_block_ViaItem_38(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 39: {
      REDUCE_ASSERT(5, 39, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_17_data_block_ViaItem_39'.";
      return ReduceTo_ASTNode_17_data_block_ViaItem_39(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_data_definitions>(collected_nodes[2]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[3]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[4].get())->literal);
    }
    case 40: {
      REDUCE_ASSERT(3, 40, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_18_data_definitions_ViaItem_40'.";
      return ReduceTo_ASTNode_18_data_definitions_ViaItem_40(
          std::reinterpret_pointer_cast<ASTNodeBase_data_definitions>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_data_definition>(collected_nodes[2]));
    }
    case 41: {
      REDUCE_ASSERT(1, 41, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_19_data_definitions_ViaItem_41'.";
      return ReduceTo_ASTNode_19_data_definitions_ViaItem_41(
          std::reinterpret_pointer_cast<ASTNodeBase_data_definition>(collected_nodes[0]));
    }
    case 42: {
      REDUCE_ASSERT(1, 42, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_import_ViaItem_42'.";
      return ReduceTo_ASTNode_import_ViaItem_42(
          std::reinterpret_pointer_cast<ASTNode_20_import_statement>(collected_nodes[0]));
    }
    case 43: {
      REDUCE_ASSERT(1, 43, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_inherit_ViaItem_43'.";
      return ReduceTo_ASTNode_inherit_ViaItem_43(
          std::reinterpret_pointer_cast<ASTNode_21_inherit_statement>(collected_nodes[0]));
    }
    case 44: {
      REDUCE_ASSERT(1, 44, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_define_code_block_ViaItem_44'.";
      return ReduceTo_ASTNode_define_code_block_ViaItem_44(
          std::reinterpret_pointer_cast<ASTNode_22_general_code_block>(collected_nodes[0]));
    }
    case 45: {
      REDUCE_ASSERT(2, 45, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_20_import_statement_ViaItem_45'.";
      return ReduceTo_ASTNode_20_import_statement_ViaItem_45(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 46: {
      REDUCE_ASSERT(3, 46, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_21_inherit_statement_ViaItem_46'.";
      return ReduceTo_ASTNode_21_inherit_statement_ViaItem_46(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 47: {
      REDUCE_ASSERT(4, 47, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_22_general_code_block_ViaItem_47'.";
      return ReduceTo_ASTNode_22_general_code_block_ViaItem_47(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    default: {
      MANTA_FAIL("unrecognized production" << reduction_id << ", cannot reduce");
    }
  }
}

inline std::shared_ptr<ASTNode_0_start>
Parser::ReduceTo_ASTNode_0_start_ViaItem_0(
    [[maybe_unused]] const std::shared_ptr<ASTNode_5_lexer_block>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_Parser>& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
    [[maybe_unused]] const std::shared_ptr<ASTNode_17_data_block>& argument_4,
    [[maybe_unused]] const std::string& argument_5) {
  auto new_node = std::make_shared<ASTNode_0_start>(0);

  // Set fields in the new node.
  new_node->lexer_block = argument_0;
  new_node->parser_block = argument_2;
  new_node->data_block = argument_4;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_1_newlines>
Parser::ReduceTo_ASTNodeGeneral_1_newlines_ViaItem_1(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_1_newlines>(1);

  // Set fields in the new node.
  new_node->newlines_node = argument_0;
  new_node->newlines = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_2_newlines>
Parser::ReduceTo_ASTNodeGeneral_2_newlines_ViaItem_2(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_2_newlines>(2);

  // Set fields in the new node.
  new_node->newlines = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_3_lexeme_def_or>
Parser::ReduceTo_ASTNodeGeneral_3_lexeme_def_or_ViaItem_3(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_3_lexeme_def_or>(3);

  // Set fields in the new node.
  new_node->or_ = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_4_lexeme_def_or>
Parser::ReduceTo_ASTNodeGeneral_4_lexeme_def_or_ViaItem_4(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_4_lexeme_def_or>(4);

  // Set fields in the new node.
  new_node->newlines = argument_0;
  new_node->or_ = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_5_lexer_block>
Parser::ReduceTo_ASTNode_5_lexer_block_ViaItem_5(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_lexer_definitions>& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
    [[maybe_unused]] const std::string& argument_4) {
  auto new_node = std::make_shared<ASTNode_5_lexer_block>(5);

  // Set fields in the new node.
  new_node->definitions = argument_2->definitions;

  return new_node;
}

inline std::shared_ptr<ASTNode_6_lexer_definitions>
Parser::ReduceTo_ASTNode_6_lexer_definitions_ViaItem_6(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_lexer_definitions>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_lexeme_line>& argument_2) {
  auto new_node = std::make_shared<ASTNode_6_lexer_definitions>(6);

  // Set fields in the new node.
  new_node->definitions.insert(new_node->definitions.end(), argument_0->definitions.cbegin(), argument_0->definitions.cend());
  new_node->definitions.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNode_7_lexer_definitions>
Parser::ReduceTo_ASTNode_7_lexer_definitions_ViaItem_7(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_lexeme_line>& argument_0) {
  auto new_node = std::make_shared<ASTNode_7_lexer_definitions>(7);

  // Set fields in the new node.
  new_node->definitions.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_Lexeme>
Parser::ReduceTo_ASTNode_Lexeme_ViaItem_8(
    [[maybe_unused]] const std::shared_ptr<ASTNode_8_lexeme_definition>& argument_0) {
  auto new_node = std::make_shared<ASTNode_Lexeme>(8);

  // Set fields in the new node.
  new_node->name = argument_0->name;
  new_node->regex = argument_0->regex;

  return new_node;
}

inline std::shared_ptr<ASTNode_Skip>
Parser::ReduceTo_ASTNode_Skip_ViaItem_9(
    [[maybe_unused]] const std::shared_ptr<ASTNode_9_skip_indicator>& argument_0) {
  auto new_node = std::make_shared<ASTNode_Skip>(9);

  // Set fields in the new node.
  new_node->identifier = argument_0->identifier;

  return new_node;
}

inline std::shared_ptr<ASTNode_8_lexeme_definition>
Parser::ReduceTo_ASTNode_8_lexeme_definition_ViaItem_10(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNode_8_lexeme_definition>(10);

  // Set fields in the new node.
  new_node->name = argument_0;
  new_node->regex = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNode_9_skip_indicator>
Parser::ReduceTo_ASTNode_9_skip_indicator_ViaItem_11(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNode_9_skip_indicator>(11);

  // Set fields in the new node.
  new_node->identifier = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_Parser>
Parser::ReduceTo_ASTNode_Parser_ViaItem_12(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_10_start_indicator>& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parser_definitions>& argument_4,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_5,
    [[maybe_unused]] const std::string& argument_6) {
  auto new_node = std::make_shared<ASTNode_Parser>(12);

  // Set fields in the new node.
  new_node->start_name = argument_2->start_name;
  new_node->definition = argument_4->definitions;

  return new_node;
}

inline std::shared_ptr<ASTNode_10_start_indicator>
Parser::ReduceTo_ASTNode_10_start_indicator_ViaItem_13(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNode_10_start_indicator>(13);

  // Set fields in the new node.
  new_node->start_name = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_11_parser_definitions>
Parser::ReduceTo_ASTNode_11_parser_definitions_ViaItem_14(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parser_definitions>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_parser>& argument_2) {
  auto new_node = std::make_shared<ASTNode_11_parser_definitions>(14);

  // Set fields in the new node.
  new_node->definitions.insert(new_node->definitions.end(), argument_0->definitions.cbegin(), argument_0->definitions.cend());
  new_node->definitions.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNode_12_parser_definitions>
Parser::ReduceTo_ASTNode_12_parser_definitions_ViaItem_15(
    [[maybe_unused]] const std::shared_ptr<ASTNode_parser>& argument_0) {
  auto new_node = std::make_shared<ASTNode_12_parser_definitions>(15);

  // Set fields in the new node.
  new_node->definitions.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_parser>
Parser::ReduceTo_ASTNode_parser_ViaItem_16(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_options>& argument_2) {
  auto new_node = std::make_shared<ASTNode_parser>(16);

  // Set fields in the new node.
  new_node->nonterminal_name = argument_0;
  new_node->productions = argument_2->productions;

  return new_node;
}

inline std::shared_ptr<ASTNode_options>
Parser::ReduceTo_ASTNode_options_ViaItem_17(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_production_and_data>& argument_0) {
  auto new_node = std::make_shared<ASTNode_options>(17);

  // Set fields in the new node.
  new_node->productions.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_options>
Parser::ReduceTo_ASTNode_options_ViaItem_18(
    [[maybe_unused]] const std::shared_ptr<ASTNode_options>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeGeneral_13_or>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_production_and_data>& argument_2) {
  auto new_node = std::make_shared<ASTNode_options>(18);

  // Set fields in the new node.
  new_node->productions.insert(new_node->productions.end(), argument_0->productions.cbegin(), argument_0->productions.cend());
  new_node->productions.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_13_or>
Parser::ReduceTo_ASTNodeGeneral_13_or_ViaItem_19(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_13_or>(19);

  // Set fields in the new node.
  new_node->or_ = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_14_production_and_data>
Parser::ReduceTo_ASTNode_14_production_and_data_ViaItem_20(
    [[maybe_unused]] const std::shared_ptr<ASTNode_production_atoms>& argument_0) {
  auto new_node = std::make_shared<ASTNode_14_production_and_data>(20);

  // Set fields in the new node.
  new_node->atoms = argument_0->atoms;

  return new_node;
}

inline std::shared_ptr<ASTNode_15_production_and_data>
Parser::ReduceTo_ASTNode_15_production_and_data_ViaItem_21(
    [[maybe_unused]] const std::shared_ptr<ASTNode_production_atoms>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_actions>& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_code_block>& argument_3) {
  auto new_node = std::make_shared<ASTNode_15_production_and_data>(21);

  // Set fields in the new node.
  new_node->atoms = argument_0->atoms;
  new_node->actions = argument_2->actions;
  new_node->code = argument_3;

  return new_node;
}

inline std::shared_ptr<ASTNode_production_atoms>
Parser::ReduceTo_ASTNode_production_atoms_ViaItem_22(
    [[maybe_unused]] const std::shared_ptr<ASTNode_production_atoms>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_production_atom>& argument_1) {
  auto new_node = std::make_shared<ASTNode_production_atoms>(22);

  // Set fields in the new node.
  new_node->atoms.insert(new_node->atoms.end(), argument_0->atoms.cbegin(), argument_0->atoms.cend());
  new_node->atoms.push_back(argument_1);

  return new_node;
}

inline std::shared_ptr<ASTNode_production_atoms>
Parser::ReduceTo_ASTNode_production_atoms_ViaItem_23(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_production_atom>& argument_0) {
  auto new_node = std::make_shared<ASTNode_production_atoms>(23);

  // Set fields in the new node.
  new_node->atoms.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_identifier>
Parser::ReduceTo_ASTNode_identifier_ViaItem_24(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNode_identifier>(24);

  // Set fields in the new node.
  new_node->identifier = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_literal>
Parser::ReduceTo_ASTNode_literal_ViaItem_25(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNode_literal>(25);

  // Set fields in the new node.
  new_node->literal = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_lexeme_name>
Parser::ReduceTo_ASTNode_lexeme_name_ViaItem_26(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNode_lexeme_name>(26);

  // Set fields in the new node.
  new_node->lexeme_name = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_null>
Parser::ReduceTo_ASTNode_null_ViaItem_27(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNode_null>(27);

  // Set fields in the new node.
  new_node->null = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_actions>
Parser::ReduceTo_ASTNode_actions_ViaItem_28(
    [[maybe_unused]] const std::shared_ptr<ASTNode_actions>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_action>& argument_2) {
  auto new_node = std::make_shared<ASTNode_actions>(28);

  // Set fields in the new node.
  new_node->actions.insert(new_node->actions.end(), argument_0->actions.cbegin(), argument_0->actions.cend());
  new_node->actions.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNode_actions>
Parser::ReduceTo_ASTNode_actions_ViaItem_29(
    [[maybe_unused]] const std::shared_ptr<ASTNode_action>& argument_0) {
  auto new_node = std::make_shared<ASTNode_actions>(29);

  // Set fields in the new node.
  new_node->actions.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_action>
Parser::ReduceTo_ASTNode_action_ViaItem_30(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_arguments>& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNode_action>(30);

  // Set fields in the new node.
  new_node->fn_name = argument_0;
  new_node->arguments = argument_2->arguments;

  return new_node;
}

inline std::shared_ptr<ASTNode_arguments>
Parser::ReduceTo_ASTNode_arguments_ViaItem_31(
    [[maybe_unused]] const std::shared_ptr<ASTNode_arguments>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_arg>& argument_2) {
  auto new_node = std::make_shared<ASTNode_arguments>(31);

  // Set fields in the new node.
  new_node->arguments.insert(new_node->arguments.end(), argument_0->arguments.cbegin(), argument_0->arguments.cend());
  new_node->arguments.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNode_arguments>
Parser::ReduceTo_ASTNode_arguments_ViaItem_32(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_arg>& argument_0) {
  auto new_node = std::make_shared<ASTNode_arguments>(32);

  // Set fields in the new node.
  new_node->arguments.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_PosArg>
Parser::ReduceTo_ASTNode_PosArg_ViaItem_33(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNode_PosArg>(33);

  // Set fields in the new node.
  new_node->pos = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_PosArg>
Parser::ReduceTo_ASTNode_PosArg_ViaItem_34(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNode_PosArg>(34);

  // Set fields in the new node.
  new_node->pos = argument_0;
  new_node->field_name = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_StringArg>
Parser::ReduceTo_ASTNode_StringArg_ViaItem_35(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNode_StringArg>(35);

  // Set fields in the new node.
  new_node->argument = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_NullArg>
Parser::ReduceTo_ASTNode_NullArg_ViaItem_36(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNode_NullArg>(36);
}

inline std::shared_ptr<ASTNodeGeneral_16_code_block>
Parser::ReduceTo_ASTNodeGeneral_16_code_block_ViaItem_37() {
  return std::make_shared<ASTNodeGeneral_16_code_block>(37);
}

inline std::shared_ptr<ASTNode_code_block>
Parser::ReduceTo_ASTNode_code_block_ViaItem_38(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNode_code_block>(38);

  // Set fields in the new node.
  new_node->visitor_name = argument_1;
  new_node->code_body = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNode_17_data_block>
Parser::ReduceTo_ASTNode_17_data_block_ViaItem_39(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_data_definitions>& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
    [[maybe_unused]] const std::string& argument_4) {
  auto new_node = std::make_shared<ASTNode_17_data_block>(39);

  // Set fields in the new node.
  new_node->definitions = argument_2->definitions;

  return new_node;
}

inline std::shared_ptr<ASTNode_18_data_definitions>
Parser::ReduceTo_ASTNode_18_data_definitions_ViaItem_40(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_data_definitions>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_data_definition>& argument_2) {
  auto new_node = std::make_shared<ASTNode_18_data_definitions>(40);

  // Set fields in the new node.
  new_node->definitions.insert(new_node->definitions.end(), argument_0->definitions.cbegin(), argument_0->definitions.cend());
  new_node->definitions.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNode_19_data_definitions>
Parser::ReduceTo_ASTNode_19_data_definitions_ViaItem_41(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_data_definition>& argument_0) {
  auto new_node = std::make_shared<ASTNode_19_data_definitions>(41);

  // Set fields in the new node.
  new_node->definitions.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_import>
Parser::ReduceTo_ASTNode_import_ViaItem_42(
    [[maybe_unused]] const std::shared_ptr<ASTNode_20_import_statement>& argument_0) {
  auto new_node = std::make_shared<ASTNode_import>(42);

  // Set fields in the new node.
  new_node->path = argument_0->path;

  return new_node;
}

inline std::shared_ptr<ASTNode_inherit>
Parser::ReduceTo_ASTNode_inherit_ViaItem_43(
    [[maybe_unused]] const std::shared_ptr<ASTNode_21_inherit_statement>& argument_0) {
  auto new_node = std::make_shared<ASTNode_inherit>(43);

  // Set fields in the new node.
  new_node->visitor_name = argument_0->visitor_name;
  new_node->base = argument_0->base;

  return new_node;
}

inline std::shared_ptr<ASTNode_define_code_block>
Parser::ReduceTo_ASTNode_define_code_block_ViaItem_44(
    [[maybe_unused]] const std::shared_ptr<ASTNode_22_general_code_block>& argument_0) {
  auto new_node = std::make_shared<ASTNode_define_code_block>(44);

  // Set fields in the new node.
  new_node->class_name = argument_0->class_name;
  new_node->code = argument_0->code;

  return new_node;
}

inline std::shared_ptr<ASTNode_20_import_statement>
Parser::ReduceTo_ASTNode_20_import_statement_ViaItem_45(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNode_20_import_statement>(45);

  // Set fields in the new node.
  new_node->path = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_21_inherit_statement>
Parser::ReduceTo_ASTNode_21_inherit_statement_ViaItem_46(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNode_21_inherit_statement>(46);

  // Set fields in the new node.
  new_node->visitor_name = argument_1;
  new_node->base = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNode_22_general_code_block>
Parser::ReduceTo_ASTNode_22_general_code_block_ViaItem_47(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNode_22_general_code_block>(47);

  // Set fields in the new node.
  new_node->class_name = argument_1;
  new_node->code = argument_3;

  return new_node;
}

struct ParserGenerator
  : ASTNodeVisitor
  , manta::DescriptionParser
  , manta::ProductionRulesBuilder {

  void Visit(ASTNodeGeneral_13_or& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_16_code_block& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_1_newlines& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_2_newlines& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_3_lexeme_def_or& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_4_lexeme_def_or& object) override { /* No body defined */ }
  void Visit(ASTNode_0_start& object) override {
    LOG_SEV(Info) << CLR("Starting to parse the description.");
    Visit(*object.lexer_block);
    Visit(*object.parser_block);
    Visit(*object.data_block);
    
    // Shift productions, so all terminals and nonterminals have positive numbers.
    shiftProductionNumbers();
    findStartProduction();
  }
  void Visit(ASTNode_10_start_indicator& object) override {
    // Set the start non-terminal
    production_rules_data_->start_nonterminal_name = object.start_name;
  }
  void Visit(ASTNode_11_parser_definitions& object) override {
    for (auto& definition : object.definitions) {
      definition->Accept(*this);
    }
  }
  void Visit(ASTNode_12_parser_definitions& object) override {
    for (auto& definition : object.definitions) {
      definition->Accept(*this);
    }
  }
  void Visit(ASTNode_14_production_and_data& object) override {
    makeNextItem();  // Create the item for this production.
    // Production and data with no actions.
    LOG_SEV(Debug) << "Production with no actions.";
    // TODO: Find a way to combine this with an actions node, just with an empty actions vector.
    // Visit(static_cast<ASTNodeBase_production_and_data&>(object));
    for (auto& atom : object.atoms) {
      atom->Accept(*this);
    }
    storeCurrent(); // Done creating the item, store it.
  }
  void Visit(ASTNode_15_production_and_data& object) override {
    makeNextItem();  // Create the item for this production.
    // Visit(static_cast<ASTNodeBase_production_and_data&>(object));
    
    LOG_SEV(Info) << "There are " << object.atoms.size() << " atoms and " << object.actions.size() << " actions.";
    for (auto& atom : object.atoms) {
      atom->Accept(*this);
    }
    for (auto& action : object.actions) {
      action->Accept(*this);
    }
    object.code->Accept(*this);
    
    storeCurrent(); // Done creating the item, store it.
  }
  void Visit(ASTNode_17_data_block& object) override {
    LOG_SEV(Info) << "Getting definitions.";
    for (auto& definition : object.definitions) {
      definition->Accept(*this);
    }
  }
  void Visit(ASTNode_18_data_definitions& object) override { /* No body defined */ }
  void Visit(ASTNode_19_data_definitions& object) override { /* No body defined */ }
  void Visit(ASTNode_20_import_statement& object) override { /* No body defined */ }
  void Visit(ASTNode_21_inherit_statement& object) override { /* No body defined */ }
  void Visit(ASTNode_22_general_code_block& object) override { /* No body defined */ }
  void Visit(ASTNode_5_lexer_block& object) override {
    LOG_SEV(Info) << "Getting lexer definitions.";
    for (auto& definition : object.definitions) {
      definition->Accept(*this);
    }
  }
  void Visit(ASTNode_6_lexer_definitions& object) override {
    for (auto& definition : object.definitions) {
      definition->Accept(*this);
    }
  }
  void Visit(ASTNode_7_lexer_definitions& object) override {
    for (auto& definition : object.definitions) {
      definition->Accept(*this);
    }
  }
  void Visit(ASTNode_8_lexeme_definition& object) override { /* No body defined */ }
  void Visit(ASTNode_9_skip_indicator& object) override { /* No body defined */ }
  void Visit(ASTNode_Lexeme& object) override {
    std::string_view name_view = object.name;
    std::string name{name_view.substr(1, name_view.size() - 1)};  // Remove the @
    // Get the r` and ` off the regex.
    auto regex = processRegex(object.regex);
    LOG_SEV(Debug) << "Adding lexeme: " << CLBG(name) << " with regex " << CLBB(regex) << ".";
    production_rules_data_->lexer_generator->AddLexeme(name, regex);
  }
  void Visit(ASTNode_NullArg& object) override { /* No body defined */ }
  void Visit(ASTNode_Parser& object) override {
    // Set the start production name.
    auto id = registerProduction(object.start_name);
    registerStartingProduction(id);
    LOG_SEV(Info) << "Setting production start name to "
                  << CLM(production_rules_data_->start_nonterminal_name) << ".";
    for (auto& definition : object.definition) {
      definition->Accept(*this);
    }
  }
  void Visit(ASTNode_PosArg& object) override {
    if (object.item_id == 33) {
      LOG_SEV(Debug) << "Pos arg: ARG:" << object.pos;
      addArgumentToAction(object.pos.substr(1));
    } else if (object.item_id == 34) {
      LOG_SEV(Debug) << "Pos arg: " << object.pos << " with field name: " << object.field_name << " ARG:"
                     << object.pos << object.field_name;
      // NOTE: Currently not doing anything special here.
      addArgumentToAction(object.pos.substr(1) + object.field_name);
    } 
  }
  void Visit(ASTNode_Skip& object) override {
    LOG_SEV(Debug) << "Setting lexeme " << object.identifier << " to be a skip lexeme.";
    production_rules_data_->lexer_generator->AddSkip(object.identifier);
  }
  void Visit(ASTNode_StringArg& object) override {
    // Remove the quotes.
    auto arg = object.argument.substr(1, object.argument.size() - 2);
    LOG_SEV(Debug) << "String arg: " << CLBG(arg) << " ARG:" << object.argument;
    
    addArgumentToAction(arg);
  }
  void Visit(ASTNode_action& object) override {
    // auto node = std::make_shared<ParseNode>(object.fn_name);
    
    registerAction(object.fn_name);
    for (auto& arg : object.arguments) {
      arg->Accept(*this);
    }
  }
  void Visit(ASTNode_actions& object) override {
    if (object.item_id == 28) {
      for (auto& action : object.actions) {
        action->Accept(*this);
      }
    } else if (object.item_id == 29) {
      for (auto& action : object.actions) {
        action->Accept(*this);
      }
    } 
  }
  void Visit(ASTNode_arguments& object) override { /* No body defined */ }
  void Visit(ASTNode_code_block& object) override {
    LOG_SEV(Info) << "Code block: " << CLBB(object.code_body);
    
    // Have to take off the beginning "{" and ending "% }".
    auto code = object.code_body.substr(1, object.code_body.size() - 3);
    
    auto item = getCurrentItemNumber();
    if (!item) {
      MANTA_THROW(UnexpectedInput, "could not get item number for reduction code");
    }
    if (object.visitor_name.empty() || object.visitor_name == "REDUCE") {
      production_rules_data_->reduction_code[*item] = code;
    }
    else {
      production_rules_data_->visitor_data.SetBodyForItem(
        object.visitor_name,
        *item,
        code);
    }
  }
  void Visit(ASTNode_define_code_block& object) override {
    LOG_SEV(Info) << "General code block:\n" << object.code;
    // Take off leading "{" and trailing "% }".
    auto code = object.code.substr(1, object.code.size() - 3);
    addGeneralCodeToVisitor(object.class_name, code);
  }
  void Visit(ASTNode_identifier& object) override {
    // Indicates a non-terminal.
    auto id = registerProduction(object.identifier);
    LOG_SEV(Info) << "Non-terminal: '" << object.identifier << "', id = " << id << ".";
    current_item_.AddToProduction(id);
  }
  void Visit(ASTNode_import& object) override {
    LOG_SEV(Info) << "Import " << object.path << ".";
    addImport(object.path);
  }
  void Visit(ASTNode_inherit& object) override {
    LOG_SEV(Info) << "Inherits " << object.visitor_name << " from " << object.base << ".";
    addParentClassForVisitor(object.visitor_name, object.base);
  }
  void Visit(ASTNode_lexeme_name& object) override {
    // Need to get rid of the '@' at the beginning.
    auto name = object.lexeme_name.substr(1);
    int id = getLexemeID(name);
    LOG_SEV(Info) << "Lexeme name: '" << name << "', id = " << id << ".";
    current_item_.AddToProduction(id);
  }
  void Visit(ASTNode_literal& object) override {
    // Get rid of quotes
    auto literal = object.literal.substr(1, object.literal.size() - 2);
    int id = production_rules_data_->lexer_generator->AddReserved(literal);
    LOG_SEV(Info) << "Literal: " << CLBG(literal) << "', id = " << id << ".";
    current_item_.AddToProduction(id);
  }
  void Visit(ASTNode_null& object) override {
    // Found a null
    LOG_SEV(Debug) << "Null found.";
  }
  void Visit(ASTNode_options& object) override {
    if (object.item_id == 17) {
      for (auto& production : object.productions) {
        production->Accept(*this);
      }
    } else if (object.item_id == 18) {
      for (auto& production : object.productions) {
        production->Accept(*this);
      }
    } 
  }
  void Visit(ASTNode_parser& object) override {
    LOG_SEV(Info) << "Reading definition for non-terminal " << CLY(object.nonterminal_name)
                      << ", there are " << object.productions.size() << " productions.";
    
    registerProductionDefinition(object.nonterminal_name);
    LOG_SEV(Debug) << "Registered nonterminal " << CLY(object.nonterminal_name) << " with id " << current_production_id_;
    for (auto& production : object.productions) {
      production->Accept(*this);
    }
  }
  void Visit(ASTNode_production_atoms& object) override {
    if (object.item_id == 22) {
      for (auto& atom : object.atoms) {
        atom->Accept(*this);
      }
    } else if (object.item_id == 23) {
      for (auto& atom : object.atoms) {
        atom->Accept(*this);
      }
    } 
  }
  void Visit(ASTNodeBase_newlines& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_lexeme_def_or& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_lexer_definitions& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_lexeme_line& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_parser_definitions& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_production_and_data& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_code_block& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_production_atom& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_arg& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_data_definitions& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_data_definition& object) override { /* No body defined */ }
  void Visit(ASTLexeme& object) override { /* No body defined */ }

  // Ad-hoc code.
  ParserGenerator() = default;

  std::shared_ptr<manta::ProductionRulesData> ParseDescription(std::istream& stream) override { return nullptr; }

  void storeCurrent() {
    LOG_SEV(Info) << "Storing item of size " << current_item_.Size() << ".";
    storeCurrentItem();
  }

  void registerAction(const std::string& action) {
    LOG_SEV(Debug) << "Registering action '" << action << "' for production " << current_production_id_ << ".";
    createAction(action);
  }

  std::string processRegex(const std::string& regex_in) {
    // Get the r` and ` off the regex.
    // Any \` should be replaced with just `
    std::string buffer;
    buffer.reserve(regex_in.size() - 3);
    auto size = regex_in.size() - 1;
    for (auto i = 2; i < size; ++i) {
      if (regex_in[i] == '\\' && i + 1 < size && regex_in[i + 1] == '`') {
        buffer.push_back('`');
        ++i;
      } else {
        buffer.push_back(regex_in[i]);
      }
    }
    return buffer;
  }
  // End ad-hoc code.
};

#define MANTA_PARSER_GENERATED
