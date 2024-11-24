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
struct ASTNodeGeneral_3_newlines;
struct ASTNodeGeneral_4_newlines;
struct ASTNode_Parser;
struct ASTNodeBase_optional_data_block;
struct ASTNodeGeneral_2_optional_data_block;
struct ASTNode_1_optional_data_block;
struct ASTNode_19_data_block;
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
struct ASTNodeBase_resolution;
struct ASTNodeGeneral_16_resolution;
struct ASTNode_17_resolution;
struct ASTNode_actions;
struct ASTNodeBase_code_block;
struct ASTNodeGeneral_18_code_block;
struct ASTNode_code_block;
struct ASTNodeBase_production_atom;
struct ASTNode_identifier;
struct ASTNode_lexeme_name;
struct ASTNode_literal;
struct ASTNode_null;
struct ASTNode_resolution_atoms;
struct ASTNodeBase_resolution_atom;
struct ASTNode_assoc;
struct ASTNode_prec;
struct ASTNode_action;
struct ASTNode_arguments;
struct ASTNodeBase_arg;
struct ASTNode_NullArg;
struct ASTNode_PosArg;
struct ASTNode_StringArg;
struct ASTNodeBase_data_definitions;
struct ASTNode_20_data_definitions;
struct ASTNode_21_data_definitions;
struct ASTNodeBase_data_definition;
struct ASTNode_define_code_block;
struct ASTNode_import;
struct ASTNode_inherit;
struct ASTNode_22_import_statement;
struct ASTNode_23_inherit_statement;
struct ASTNode_24_general_code_block;
// Lexeme type
struct ASTLexeme;

//! \brief Define the enums for each node type.
enum class ASTNodeType{
  Literal,
  Type_ASTNodeBase_arg,
  Type_ASTNodeBase_code_block,
  Type_ASTNodeBase_data_definition,
  Type_ASTNodeBase_data_definitions,
  Type_ASTNodeBase_lexeme_line,
  Type_ASTNodeBase_lexer_definitions,
  Type_ASTNodeBase_newlines,
  Type_ASTNodeBase_optional_data_block,
  Type_ASTNodeBase_parser_definitions,
  Type_ASTNodeBase_production_and_data,
  Type_ASTNodeBase_production_atom,
  Type_ASTNodeBase_resolution,
  Type_ASTNodeBase_resolution_atom,
  Type_ASTNodeGeneral_13_or,
  Type_ASTNodeGeneral_16_resolution,
  Type_ASTNodeGeneral_18_code_block,
  Type_ASTNodeGeneral_2_optional_data_block,
  Type_ASTNodeGeneral_3_newlines,
  Type_ASTNodeGeneral_4_newlines,
  Type_ASTNode_0_start,
  Type_ASTNode_10_start_indicator,
  Type_ASTNode_11_parser_definitions,
  Type_ASTNode_12_parser_definitions,
  Type_ASTNode_14_production_and_data,
  Type_ASTNode_15_production_and_data,
  Type_ASTNode_17_resolution,
  Type_ASTNode_19_data_block,
  Type_ASTNode_1_optional_data_block,
  Type_ASTNode_20_data_definitions,
  Type_ASTNode_21_data_definitions,
  Type_ASTNode_22_import_statement,
  Type_ASTNode_23_inherit_statement,
  Type_ASTNode_24_general_code_block,
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
  Type_ASTNode_assoc,
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
  Type_ASTNode_prec,
  Type_ASTNode_production_atoms,
  Type_ASTNode_resolution_atoms,
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
  case ASTNodeType::Type_ASTNodeBase_lexeme_line:
    return "Type_ASTNodeBase_lexeme_line";
  case ASTNodeType::Type_ASTNodeBase_lexer_definitions:
    return "Type_ASTNodeBase_lexer_definitions";
  case ASTNodeType::Type_ASTNodeBase_newlines:
    return "Type_ASTNodeBase_newlines";
  case ASTNodeType::Type_ASTNodeBase_optional_data_block:
    return "Type_ASTNodeBase_optional_data_block";
  case ASTNodeType::Type_ASTNodeBase_parser_definitions:
    return "Type_ASTNodeBase_parser_definitions";
  case ASTNodeType::Type_ASTNodeBase_production_and_data:
    return "Type_ASTNodeBase_production_and_data";
  case ASTNodeType::Type_ASTNodeBase_production_atom:
    return "Type_ASTNodeBase_production_atom";
  case ASTNodeType::Type_ASTNodeBase_resolution:
    return "Type_ASTNodeBase_resolution";
  case ASTNodeType::Type_ASTNodeBase_resolution_atom:
    return "Type_ASTNodeBase_resolution_atom";
  case ASTNodeType::Type_ASTNodeGeneral_13_or:
    return "Type_ASTNodeGeneral_13_or";
  case ASTNodeType::Type_ASTNodeGeneral_16_resolution:
    return "Type_ASTNodeGeneral_16_resolution";
  case ASTNodeType::Type_ASTNodeGeneral_18_code_block:
    return "Type_ASTNodeGeneral_18_code_block";
  case ASTNodeType::Type_ASTNodeGeneral_2_optional_data_block:
    return "Type_ASTNodeGeneral_2_optional_data_block";
  case ASTNodeType::Type_ASTNodeGeneral_3_newlines:
    return "Type_ASTNodeGeneral_3_newlines";
  case ASTNodeType::Type_ASTNodeGeneral_4_newlines:
    return "Type_ASTNodeGeneral_4_newlines";
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
  case ASTNodeType::Type_ASTNode_17_resolution:
    return "Type_ASTNode_17_resolution";
  case ASTNodeType::Type_ASTNode_19_data_block:
    return "Type_ASTNode_19_data_block";
  case ASTNodeType::Type_ASTNode_1_optional_data_block:
    return "Type_ASTNode_1_optional_data_block";
  case ASTNodeType::Type_ASTNode_20_data_definitions:
    return "Type_ASTNode_20_data_definitions";
  case ASTNodeType::Type_ASTNode_21_data_definitions:
    return "Type_ASTNode_21_data_definitions";
  case ASTNodeType::Type_ASTNode_22_import_statement:
    return "Type_ASTNode_22_import_statement";
  case ASTNodeType::Type_ASTNode_23_inherit_statement:
    return "Type_ASTNode_23_inherit_statement";
  case ASTNodeType::Type_ASTNode_24_general_code_block:
    return "Type_ASTNode_24_general_code_block";
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
  case ASTNodeType::Type_ASTNode_assoc:
    return "Type_ASTNode_assoc";
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
  case ASTNodeType::Type_ASTNode_prec:
    return "Type_ASTNode_prec";
  case ASTNodeType::Type_ASTNode_production_atoms:
    return "Type_ASTNode_production_atoms";
  case ASTNodeType::Type_ASTNode_resolution_atoms:
    return "Type_ASTNode_resolution_atoms";
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
  lexeme_definition,
  lexeme_line,
  lexer_block,
  lexer_definitions,
  newlines,
  optional_data_block,
  or_,
  parser_block,
  parser_definition,
  parser_definitions,
  production_and_data,
  production_atom,
  production_atoms,
  production_lines,
  resolution,
  resolution_atom,
  resolution_atoms,
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
  case NonterminalType::optional_data_block:
    return "optional_data_block";
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
  case NonterminalType::resolution:
    return "resolution";
  case NonterminalType::resolution_atom:
    return "resolution_atom";
  case NonterminalType::resolution_atoms:
    return "resolution_atoms";
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
  virtual void Visit(ASTNodeGeneral_3_newlines& object) = 0;
  virtual void Visit(ASTNodeGeneral_4_newlines& object) = 0;
  virtual void Visit(ASTNode_Parser& object) = 0;
  virtual void Visit(ASTNodeBase_optional_data_block& object) = 0;
  virtual void Visit(ASTNodeGeneral_2_optional_data_block& object) = 0;
  virtual void Visit(ASTNode_1_optional_data_block& object) = 0;
  virtual void Visit(ASTNode_19_data_block& object) = 0;
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
  virtual void Visit(ASTNodeBase_resolution& object) = 0;
  virtual void Visit(ASTNodeGeneral_16_resolution& object) = 0;
  virtual void Visit(ASTNode_17_resolution& object) = 0;
  virtual void Visit(ASTNode_actions& object) = 0;
  virtual void Visit(ASTNodeBase_code_block& object) = 0;
  virtual void Visit(ASTNodeGeneral_18_code_block& object) = 0;
  virtual void Visit(ASTNode_code_block& object) = 0;
  virtual void Visit(ASTNodeBase_production_atom& object) = 0;
  virtual void Visit(ASTNode_identifier& object) = 0;
  virtual void Visit(ASTNode_lexeme_name& object) = 0;
  virtual void Visit(ASTNode_literal& object) = 0;
  virtual void Visit(ASTNode_null& object) = 0;
  virtual void Visit(ASTNode_resolution_atoms& object) = 0;
  virtual void Visit(ASTNodeBase_resolution_atom& object) = 0;
  virtual void Visit(ASTNode_assoc& object) = 0;
  virtual void Visit(ASTNode_prec& object) = 0;
  virtual void Visit(ASTNode_action& object) = 0;
  virtual void Visit(ASTNode_arguments& object) = 0;
  virtual void Visit(ASTNodeBase_arg& object) = 0;
  virtual void Visit(ASTNode_NullArg& object) = 0;
  virtual void Visit(ASTNode_PosArg& object) = 0;
  virtual void Visit(ASTNode_StringArg& object) = 0;
  virtual void Visit(ASTNodeBase_data_definitions& object) = 0;
  virtual void Visit(ASTNode_20_data_definitions& object) = 0;
  virtual void Visit(ASTNode_21_data_definitions& object) = 0;
  virtual void Visit(ASTNodeBase_data_definition& object) = 0;
  virtual void Visit(ASTNode_define_code_block& object) = 0;
  virtual void Visit(ASTNode_import& object) = 0;
  virtual void Visit(ASTNode_inherit& object) = 0;
  virtual void Visit(ASTNode_22_import_statement& object) = 0;
  virtual void Visit(ASTNode_23_inherit_statement& object) = 0;
  virtual void Visit(ASTNode_24_general_code_block& object) = 0;
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
//  Nodes for non-terminal 35's productions.
// ============================================================

struct ASTNode_0_start : ASTNodeBase {
  explicit ASTNode_0_start(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_0_start, NonterminalType::start, item_id) {}

  std::shared_ptr<ASTNodeBase_optional_data_block> data_block{};
  std::shared_ptr<ASTNode_5_lexer_block> lexer_block{};
  std::shared_ptr<ASTNode_Parser> parser_block{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 36's productions.
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
//  Nodes for non-terminal 37's productions.
// ============================================================

//! \brief Parent type for non-terminal type 37
struct ASTNodeBase_newlines : ASTNodeBase {
  ASTNodeBase_newlines(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::newlines, item_id) {}

  std::string newlines{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_3_newlines : ASTNodeBase_newlines {
  explicit ASTNodeGeneral_3_newlines(const ItemID& item_id)
    : ASTNodeBase_newlines(ASTNodeType::Type_ASTNodeGeneral_3_newlines, item_id) {}

  std::shared_ptr<ASTNodeBase_newlines> newlines_node{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_4_newlines : ASTNodeBase_newlines {
  explicit ASTNodeGeneral_4_newlines(const ItemID& item_id)
    : ASTNodeBase_newlines(ASTNodeType::Type_ASTNodeGeneral_4_newlines, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 38's productions.
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
//  Nodes for non-terminal 39's productions.
// ============================================================

//! \brief Parent type for non-terminal type 39
struct ASTNodeBase_optional_data_block : ASTNodeBase {
  ASTNodeBase_optional_data_block(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::optional_data_block, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_2_optional_data_block : ASTNodeBase_optional_data_block {
  explicit ASTNodeGeneral_2_optional_data_block(const ItemID& item_id)
    : ASTNodeBase_optional_data_block(ASTNodeType::Type_ASTNodeGeneral_2_optional_data_block, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_1_optional_data_block : ASTNodeBase_optional_data_block {
  explicit ASTNode_1_optional_data_block(const ItemID& item_id)
    : ASTNodeBase_optional_data_block(ASTNodeType::Type_ASTNode_1_optional_data_block, item_id) {}

  std::shared_ptr<ASTNode_19_data_block> data_block{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 40's productions.
// ============================================================

struct ASTNode_19_data_block : ASTNodeBase {
  explicit ASTNode_19_data_block(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_19_data_block, NonterminalType::data_block, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_data_definition>> definitions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 41's productions.
// ============================================================

//! \brief Parent type for non-terminal type 41
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
//  Nodes for non-terminal 42's productions.
// ============================================================

//! \brief Parent type for non-terminal type 42
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
//  Nodes for non-terminal 43's productions.
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
//  Nodes for non-terminal 44's productions.
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
//  Nodes for non-terminal 45's productions.
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
//  Nodes for non-terminal 46's productions.
// ============================================================

//! \brief Parent type for non-terminal type 46
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
//  Nodes for non-terminal 47's productions.
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
//  Nodes for non-terminal 48's productions.
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
//  Nodes for non-terminal 49's productions.
// ============================================================

//! \brief Parent type for non-terminal type 49
struct ASTNodeBase_production_and_data : ASTNodeBase {
  ASTNodeBase_production_and_data(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::production_and_data, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_production_atom>> atoms{};
  std::shared_ptr<ASTNodeBase_resolution> resolution{};

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
//  Nodes for non-terminal 50's productions.
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
//  Nodes for non-terminal 51's productions.
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
//  Nodes for non-terminal 52's productions.
// ============================================================

//! \brief Parent type for non-terminal type 52
struct ASTNodeBase_resolution : ASTNodeBase {
  ASTNodeBase_resolution(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::resolution, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_16_resolution : ASTNodeBase_resolution {
  explicit ASTNodeGeneral_16_resolution(const ItemID& item_id)
    : ASTNodeBase_resolution(ASTNodeType::Type_ASTNodeGeneral_16_resolution, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_17_resolution : ASTNodeBase_resolution {
  explicit ASTNode_17_resolution(const ItemID& item_id)
    : ASTNodeBase_resolution(ASTNodeType::Type_ASTNode_17_resolution, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_resolution_atom>> atoms{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 53's productions.
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
//  Nodes for non-terminal 54's productions.
// ============================================================

//! \brief Parent type for non-terminal type 54
struct ASTNodeBase_code_block : ASTNodeBase {
  ASTNodeBase_code_block(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::code_block, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNodeGeneral_18_code_block : ASTNodeBase_code_block {
  explicit ASTNodeGeneral_18_code_block(const ItemID& item_id)
    : ASTNodeBase_code_block(ASTNodeType::Type_ASTNodeGeneral_18_code_block, item_id) {}


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
//  Nodes for non-terminal 55's productions.
// ============================================================

//! \brief Parent type for non-terminal type 55
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
//  Nodes for non-terminal 56's productions.
// ============================================================

struct ASTNode_resolution_atoms : ASTNodeBase {
  explicit ASTNode_resolution_atoms(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_resolution_atoms, NonterminalType::resolution_atoms, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_resolution_atom>> atoms{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 57's productions.
// ============================================================

//! \brief Parent type for non-terminal type 57
struct ASTNodeBase_resolution_atom : ASTNodeBase {
  ASTNodeBase_resolution_atom(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::resolution_atom, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_assoc : ASTNodeBase_resolution_atom {
  explicit ASTNode_assoc(const ItemID& item_id)
    : ASTNodeBase_resolution_atom(ASTNodeType::Type_ASTNode_assoc, item_id) {}

  std::string assoc{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_prec : ASTNodeBase_resolution_atom {
  explicit ASTNode_prec(const ItemID& item_id)
    : ASTNodeBase_resolution_atom(ASTNodeType::Type_ASTNode_prec, item_id) {}

  std::string prec{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 58's productions.
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
//  Nodes for non-terminal 59's productions.
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
//  Nodes for non-terminal 60's productions.
// ============================================================

//! \brief Parent type for non-terminal type 60
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
//  Nodes for non-terminal 61's productions.
// ============================================================

//! \brief Parent type for non-terminal type 61
struct ASTNodeBase_data_definitions : ASTNodeBase {
  ASTNodeBase_data_definitions(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::data_definitions, item_id) {}

  std::vector<std::shared_ptr<ASTNodeBase_data_definition>> definitions{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_20_data_definitions : ASTNodeBase_data_definitions {
  explicit ASTNode_20_data_definitions(const ItemID& item_id)
    : ASTNodeBase_data_definitions(ASTNodeType::Type_ASTNode_20_data_definitions, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

struct ASTNode_21_data_definitions : ASTNodeBase_data_definitions {
  explicit ASTNode_21_data_definitions(const ItemID& item_id)
    : ASTNodeBase_data_definitions(ASTNodeType::Type_ASTNode_21_data_definitions, item_id) {}


  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 62's productions.
// ============================================================

//! \brief Parent type for non-terminal type 62
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
//  Nodes for non-terminal 63's productions.
// ============================================================

struct ASTNode_22_import_statement : ASTNodeBase {
  explicit ASTNode_22_import_statement(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_22_import_statement, NonterminalType::import_statement, item_id) {}

  std::string path{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 64's productions.
// ============================================================

struct ASTNode_23_inherit_statement : ASTNodeBase {
  explicit ASTNode_23_inherit_statement(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_23_inherit_statement, NonterminalType::inherit_statement, item_id) {}

  std::string base{};
  std::string visitor_name{};

  void Accept(ASTNodeVisitor& visitor) override {
    visitor.Visit(*this);
  }
};

// ============================================================
//  Nodes for non-terminal 65's productions.
// ============================================================

struct ASTNode_24_general_code_block : ASTNodeBase {
  explicit ASTNode_24_general_code_block(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNode_24_general_code_block, NonterminalType::general_code_block, item_id) {}

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
      const std::shared_ptr<ASTNodeBase_optional_data_block>& argument_4,
      const std::string& argument_5);

  std::shared_ptr<ASTNode_1_optional_data_block>
  ReduceTo_ASTNode_1_optional_data_block_ViaItem_1(
      const std::shared_ptr<ASTNode_19_data_block>& argument_0);

  std::shared_ptr<ASTNodeGeneral_2_optional_data_block>
  ReduceTo_ASTNodeGeneral_2_optional_data_block_ViaItem_2();

  std::shared_ptr<ASTNodeGeneral_3_newlines>
  ReduceTo_ASTNodeGeneral_3_newlines_ViaItem_3(
      const std::shared_ptr<ASTNodeBase_newlines>& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNodeGeneral_4_newlines>
  ReduceTo_ASTNodeGeneral_4_newlines_ViaItem_4(
      const std::string& argument_0);

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
      const std::shared_ptr<ASTNode_production_atoms>& argument_0,
      const std::shared_ptr<ASTNodeBase_resolution>& argument_1);

  std::shared_ptr<ASTNode_15_production_and_data>
  ReduceTo_ASTNode_15_production_and_data_ViaItem_21(
      const std::shared_ptr<ASTNode_production_atoms>& argument_0,
      const std::shared_ptr<ASTNodeBase_resolution>& argument_1,
      const std::string& argument_2,
      const std::shared_ptr<ASTNode_actions>& argument_3,
      const std::shared_ptr<ASTNodeBase_code_block>& argument_4);

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

  std::shared_ptr<ASTNodeGeneral_16_resolution>
  ReduceTo_ASTNodeGeneral_16_resolution_ViaItem_28();

  std::shared_ptr<ASTNode_17_resolution>
  ReduceTo_ASTNode_17_resolution_ViaItem_29(
      const std::string& argument_0,
      const std::shared_ptr<ASTNode_resolution_atoms>& argument_1);

  std::shared_ptr<ASTNode_resolution_atoms>
  ReduceTo_ASTNode_resolution_atoms_ViaItem_30(
      const std::shared_ptr<ASTNode_resolution_atoms>& argument_0,
      const std::shared_ptr<ASTNodeBase_resolution_atom>& argument_1);

  std::shared_ptr<ASTNode_resolution_atoms>
  ReduceTo_ASTNode_resolution_atoms_ViaItem_31(
      const std::shared_ptr<ASTNodeBase_resolution_atom>& argument_0);

  std::shared_ptr<ASTNode_assoc>
  ReduceTo_ASTNode_assoc_ViaItem_32(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNode_prec>
  ReduceTo_ASTNode_prec_ViaItem_33(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNode_actions>
  ReduceTo_ASTNode_actions_ViaItem_34(
      const std::shared_ptr<ASTNode_actions>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNode_action>& argument_2);

  std::shared_ptr<ASTNode_actions>
  ReduceTo_ASTNode_actions_ViaItem_35(
      const std::shared_ptr<ASTNode_action>& argument_0);

  std::shared_ptr<ASTNode_action>
  ReduceTo_ASTNode_action_ViaItem_36(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNode_arguments>& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNode_arguments>
  ReduceTo_ASTNode_arguments_ViaItem_37(
      const std::shared_ptr<ASTNode_arguments>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_arg>& argument_2);

  std::shared_ptr<ASTNode_arguments>
  ReduceTo_ASTNode_arguments_ViaItem_38(
      const std::shared_ptr<ASTNodeBase_arg>& argument_0);

  std::shared_ptr<ASTNode_PosArg>
  ReduceTo_ASTNode_PosArg_ViaItem_39(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_PosArg>
  ReduceTo_ASTNode_PosArg_ViaItem_40(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNode_StringArg>
  ReduceTo_ASTNode_StringArg_ViaItem_41(
      const std::string& argument_0);

  std::shared_ptr<ASTNode_NullArg>
  ReduceTo_ASTNode_NullArg_ViaItem_42(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_18_code_block>
  ReduceTo_ASTNodeGeneral_18_code_block_ViaItem_43();

  std::shared_ptr<ASTNode_code_block>
  ReduceTo_ASTNode_code_block_ViaItem_44(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNode_19_data_block>
  ReduceTo_ASTNode_19_data_block_ViaItem_45(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNodeBase_data_definitions>& argument_2,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
      const std::string& argument_4);

  std::shared_ptr<ASTNode_20_data_definitions>
  ReduceTo_ASTNode_20_data_definitions_ViaItem_46(
      const std::shared_ptr<ASTNodeBase_data_definitions>& argument_0,
      const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
      const std::shared_ptr<ASTNodeBase_data_definition>& argument_2);

  std::shared_ptr<ASTNode_21_data_definitions>
  ReduceTo_ASTNode_21_data_definitions_ViaItem_47(
      const std::shared_ptr<ASTNodeBase_data_definition>& argument_0);

  std::shared_ptr<ASTNode_import>
  ReduceTo_ASTNode_import_ViaItem_48(
      const std::shared_ptr<ASTNode_22_import_statement>& argument_0);

  std::shared_ptr<ASTNode_inherit>
  ReduceTo_ASTNode_inherit_ViaItem_49(
      const std::shared_ptr<ASTNode_23_inherit_statement>& argument_0);

  std::shared_ptr<ASTNode_define_code_block>
  ReduceTo_ASTNode_define_code_block_ViaItem_50(
      const std::shared_ptr<ASTNode_24_general_code_block>& argument_0);

  std::shared_ptr<ASTNode_22_import_statement>
  ReduceTo_ASTNode_22_import_statement_ViaItem_51(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNode_23_inherit_statement>
  ReduceTo_ASTNode_23_inherit_statement_ViaItem_52(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNode_24_general_code_block>
  ReduceTo_ASTNode_24_general_code_block_ViaItem_53(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2,
      const std::string& argument_3);

};

inline Parser::Parser() {
  using namespace manta;

  start_nonterminal_ = 35;
  // Allocate space for the parser table.
  parse_table_.assign(101, std::vector<Entry>(66,Entry()));

  // Create the table. There are better, though more difficult, ways to serialize this information.
  parse_table_[0][18] = Entry(1);  // Shift
  parse_table_[0][35] = Entry(true);  // Accept
  parse_table_[0][36] = Entry(2);  // Shift
  parse_table_[1][16] = Entry(3);  // Shift
  parse_table_[1][37] = Entry(4);  // Shift
  parse_table_[2][16] = Entry(3);  // Shift
  parse_table_[2][37] = Entry(5);  // Shift
  parse_table_[3][0] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][2] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][4] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][16] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][19] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][20] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][21] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][22] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][31] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][32] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][33] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[3][34] = Entry(Item(37, 4, 0, {16}, 4));  // Reduce
  parse_table_[4][2] = Entry(6);  // Shift
  parse_table_[4][16] = Entry(7);  // Shift
  parse_table_[4][20] = Entry(8);  // Shift
  parse_table_[4][41] = Entry(9);  // Shift
  parse_table_[4][42] = Entry(10);  // Shift
  parse_table_[4][43] = Entry(11);  // Shift
  parse_table_[4][44] = Entry(12);  // Shift
  parse_table_[5][16] = Entry(7);  // Shift
  parse_table_[5][21] = Entry(13);  // Shift
  parse_table_[5][38] = Entry(14);  // Shift
  parse_table_[6][3] = Entry(15);  // Shift
  parse_table_[7][0] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][2] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][4] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][16] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][19] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][20] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][21] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][22] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][31] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][32] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][33] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[7][34] = Entry(Item(37, 3, 0, {37, 16}, 3));  // Reduce
  parse_table_[8][4] = Entry(16);  // Shift
  parse_table_[9][16] = Entry(3);  // Shift
  parse_table_[9][37] = Entry(17);  // Shift
  parse_table_[10][16] = Entry(Item(41, 7, 0, {42}, 7));  // Reduce
  parse_table_[11][16] = Entry(Item(42, 8, 0, {43}, 8));  // Reduce
  parse_table_[12][16] = Entry(Item(42, 9, 0, {44}, 9));  // Reduce
  parse_table_[13][16] = Entry(3);  // Shift
  parse_table_[13][37] = Entry(18);  // Shift
  parse_table_[14][16] = Entry(3);  // Shift
  parse_table_[14][37] = Entry(19);  // Shift
  parse_table_[15][6] = Entry(20);  // Shift
  parse_table_[16][16] = Entry(Item(44, 11, 0, {20, 4}, 11));  // Reduce
  parse_table_[17][2] = Entry(6);  // Shift
  parse_table_[17][16] = Entry(7);  // Shift
  parse_table_[17][19] = Entry(21);  // Shift
  parse_table_[17][20] = Entry(8);  // Shift
  parse_table_[17][42] = Entry(22);  // Shift
  parse_table_[17][43] = Entry(11);  // Shift
  parse_table_[17][44] = Entry(12);  // Shift
  parse_table_[18][16] = Entry(7);  // Shift
  parse_table_[18][22] = Entry(23);  // Shift
  parse_table_[18][45] = Entry(24);  // Shift
  parse_table_[19][0] = Entry(Item(39, 2, 0, {}, 2));  // Reduce
  parse_table_[19][16] = Entry(7);  // Shift
  parse_table_[19][31] = Entry(25);  // Shift
  parse_table_[19][39] = Entry(26);  // Shift
  parse_table_[19][40] = Entry(27);  // Shift
  parse_table_[20][16] = Entry(Item(43, 10, 0, {2, 3, 6}, 10));  // Reduce
  parse_table_[21][0] = Entry(Item(36, 5, 0, {18, 37, 41, 37, 19}, 5));  // Reduce
  parse_table_[21][16] = Entry(Item(36, 5, 0, {18, 37, 41, 37, 19}, 5));  // Reduce
  parse_table_[22][16] = Entry(Item(41, 6, 0, {41, 37, 42}, 6));  // Reduce
  parse_table_[23][4] = Entry(28);  // Shift
  parse_table_[24][16] = Entry(3);  // Shift
  parse_table_[24][37] = Entry(29);  // Shift
  parse_table_[25][16] = Entry(3);  // Shift
  parse_table_[25][37] = Entry(30);  // Shift
  parse_table_[26][0] = Entry(31);  // Shift
  parse_table_[27][0] = Entry(Item(39, 1, 0, {40}, 1));  // Reduce
  parse_table_[28][16] = Entry(Item(45, 13, 0, {22, 4}, 13));  // Reduce
  parse_table_[29][4] = Entry(32);  // Shift
  parse_table_[29][16] = Entry(7);  // Shift
  parse_table_[29][46] = Entry(33);  // Shift
  parse_table_[29][47] = Entry(34);  // Shift
  parse_table_[30][16] = Entry(7);  // Shift
  parse_table_[30][32] = Entry(35);  // Shift
  parse_table_[30][33] = Entry(36);  // Shift
  parse_table_[30][34] = Entry(37);  // Shift
  parse_table_[30][61] = Entry(38);  // Shift
  parse_table_[30][62] = Entry(39);  // Shift
  parse_table_[30][63] = Entry(40);  // Shift
  parse_table_[30][64] = Entry(41);  // Shift
  parse_table_[30][65] = Entry(42);  // Shift
  parse_table_[31][0] = Entry(Item(35, 0, 0, {36, 37, 38, 37, 39, 0}, 0));  // Reduce
  parse_table_[32][23] = Entry(43);  // Shift
  parse_table_[33][16] = Entry(3);  // Shift
  parse_table_[33][37] = Entry(44);  // Shift
  parse_table_[34][16] = Entry(Item(46, 15, 0, {47}, 15));  // Reduce
  parse_table_[35][12] = Entry(45);  // Shift
  parse_table_[36][4] = Entry(46);  // Shift
  parse_table_[37][4] = Entry(47);  // Shift
  parse_table_[38][16] = Entry(3);  // Shift
  parse_table_[38][37] = Entry(48);  // Shift
  parse_table_[39][16] = Entry(Item(61, 47, 0, {62}, 47));  // Reduce
  parse_table_[40][16] = Entry(Item(62, 48, 0, {63}, 48));  // Reduce
  parse_table_[41][16] = Entry(Item(62, 49, 0, {64}, 49));  // Reduce
  parse_table_[42][16] = Entry(Item(62, 50, 0, {65}, 50));  // Reduce
  parse_table_[43][2] = Entry(49);  // Shift
  parse_table_[43][4] = Entry(50);  // Shift
  parse_table_[43][7] = Entry(51);  // Shift
  parse_table_[43][10] = Entry(52);  // Shift
  parse_table_[43][48] = Entry(53);  // Shift
  parse_table_[43][49] = Entry(54);  // Shift
  parse_table_[43][51] = Entry(55);  // Shift
  parse_table_[43][55] = Entry(56);  // Shift
  parse_table_[44][4] = Entry(32);  // Shift
  parse_table_[44][16] = Entry(7);  // Shift
  parse_table_[44][19] = Entry(57);  // Shift
  parse_table_[44][47] = Entry(58);  // Shift
  parse_table_[45][16] = Entry(Item(63, 51, 0, {32, 12}, 51));  // Reduce
  parse_table_[46][13] = Entry(59);  // Shift
  parse_table_[47][30] = Entry(60);  // Shift
  parse_table_[48][16] = Entry(7);  // Shift
  parse_table_[48][19] = Entry(61);  // Shift
  parse_table_[48][32] = Entry(35);  // Shift
  parse_table_[48][33] = Entry(36);  // Shift
  parse_table_[48][34] = Entry(37);  // Shift
  parse_table_[48][62] = Entry(62);  // Shift
  parse_table_[48][63] = Entry(40);  // Shift
  parse_table_[48][64] = Entry(41);  // Shift
  parse_table_[48][65] = Entry(42);  // Shift
  parse_table_[49][2] = Entry(Item(55, 26, 0, {2}, 26));  // Reduce
  parse_table_[49][3] = Entry(Item(55, 26, 0, {2}, 26));  // Reduce
  parse_table_[49][4] = Entry(Item(55, 26, 0, {2}, 26));  // Reduce
  parse_table_[49][7] = Entry(Item(55, 26, 0, {2}, 26));  // Reduce
  parse_table_[49][8] = Entry(Item(55, 26, 0, {2}, 26));  // Reduce
  parse_table_[49][10] = Entry(Item(55, 26, 0, {2}, 26));  // Reduce
  parse_table_[49][16] = Entry(Item(55, 26, 0, {2}, 26));  // Reduce
  parse_table_[49][23] = Entry(Item(55, 26, 0, {2}, 26));  // Reduce
  parse_table_[50][2] = Entry(Item(55, 24, 0, {4}, 24));  // Reduce
  parse_table_[50][3] = Entry(Item(55, 24, 0, {4}, 24));  // Reduce
  parse_table_[50][4] = Entry(Item(55, 24, 0, {4}, 24));  // Reduce
  parse_table_[50][7] = Entry(Item(55, 24, 0, {4}, 24));  // Reduce
  parse_table_[50][8] = Entry(Item(55, 24, 0, {4}, 24));  // Reduce
  parse_table_[50][10] = Entry(Item(55, 24, 0, {4}, 24));  // Reduce
  parse_table_[50][16] = Entry(Item(55, 24, 0, {4}, 24));  // Reduce
  parse_table_[50][23] = Entry(Item(55, 24, 0, {4}, 24));  // Reduce
  parse_table_[51][2] = Entry(Item(55, 25, 0, {7}, 25));  // Reduce
  parse_table_[51][3] = Entry(Item(55, 25, 0, {7}, 25));  // Reduce
  parse_table_[51][4] = Entry(Item(55, 25, 0, {7}, 25));  // Reduce
  parse_table_[51][7] = Entry(Item(55, 25, 0, {7}, 25));  // Reduce
  parse_table_[51][8] = Entry(Item(55, 25, 0, {7}, 25));  // Reduce
  parse_table_[51][10] = Entry(Item(55, 25, 0, {7}, 25));  // Reduce
  parse_table_[51][16] = Entry(Item(55, 25, 0, {7}, 25));  // Reduce
  parse_table_[51][23] = Entry(Item(55, 25, 0, {7}, 25));  // Reduce
  parse_table_[52][2] = Entry(Item(55, 27, 0, {10}, 27));  // Reduce
  parse_table_[52][3] = Entry(Item(55, 27, 0, {10}, 27));  // Reduce
  parse_table_[52][4] = Entry(Item(55, 27, 0, {10}, 27));  // Reduce
  parse_table_[52][7] = Entry(Item(55, 27, 0, {10}, 27));  // Reduce
  parse_table_[52][8] = Entry(Item(55, 27, 0, {10}, 27));  // Reduce
  parse_table_[52][10] = Entry(Item(55, 27, 0, {10}, 27));  // Reduce
  parse_table_[52][16] = Entry(Item(55, 27, 0, {10}, 27));  // Reduce
  parse_table_[52][23] = Entry(Item(55, 27, 0, {10}, 27));  // Reduce
  parse_table_[53][8] = Entry(63);  // Shift
  parse_table_[53][16] = Entry(Item(47, 16, 0, {4, 23, 48}, 16));  // Reduce
  parse_table_[53][50] = Entry(64);  // Shift
  parse_table_[54][8] = Entry(Item(48, 17, 0, {49}, 17));  // Reduce
  parse_table_[54][16] = Entry(Item(48, 17, 0, {49}, 17));  // Reduce
  parse_table_[55][2] = Entry(49);  // Shift
  parse_table_[55][3] = Entry(Item(52, 28, 0, {}, 28));  // Reduce
  parse_table_[55][4] = Entry(50);  // Shift
  parse_table_[55][7] = Entry(51);  // Shift
  parse_table_[55][8] = Entry(Item(52, 28, 0, {}, 28));  // Reduce
  parse_table_[55][10] = Entry(52);  // Shift
  parse_table_[55][16] = Entry(Item(52, 28, 0, {}, 28));  // Reduce
  parse_table_[55][23] = Entry(65);  // Shift
  parse_table_[55][52] = Entry(66);  // Shift
  parse_table_[55][55] = Entry(67);  // Shift
  parse_table_[56][2] = Entry(Item(51, 23, 0, {55}, 23));  // Reduce
  parse_table_[56][3] = Entry(Item(51, 23, 0, {55}, 23));  // Reduce
  parse_table_[56][4] = Entry(Item(51, 23, 0, {55}, 23));  // Reduce
  parse_table_[56][7] = Entry(Item(51, 23, 0, {55}, 23));  // Reduce
  parse_table_[56][8] = Entry(Item(51, 23, 0, {55}, 23));  // Reduce
  parse_table_[56][10] = Entry(Item(51, 23, 0, {55}, 23));  // Reduce
  parse_table_[56][16] = Entry(Item(51, 23, 0, {55}, 23));  // Reduce
  parse_table_[56][23] = Entry(Item(51, 23, 0, {55}, 23));  // Reduce
  parse_table_[57][16] = Entry(Item(38, 12, 0, {21, 37, 45, 37, 46, 37, 19}, 12));  // Reduce
  parse_table_[58][16] = Entry(Item(46, 14, 0, {46, 37, 47}, 14));  // Reduce
  parse_table_[59][16] = Entry(Item(64, 52, 0, {33, 4, 13}, 52));  // Reduce
  parse_table_[60][14] = Entry(68);  // Shift
  parse_table_[61][0] = Entry(Item(40, 45, 0, {31, 37, 61, 37, 19}, 45));  // Reduce
  parse_table_[62][16] = Entry(Item(61, 46, 0, {61, 37, 62}, 46));  // Reduce
  parse_table_[63][2] = Entry(Item(50, 19, 0, {8}, 19));  // Reduce
  parse_table_[63][4] = Entry(Item(50, 19, 0, {8}, 19));  // Reduce
  parse_table_[63][7] = Entry(Item(50, 19, 0, {8}, 19));  // Reduce
  parse_table_[63][10] = Entry(Item(50, 19, 0, {8}, 19));  // Reduce
  parse_table_[64][2] = Entry(49);  // Shift
  parse_table_[64][4] = Entry(50);  // Shift
  parse_table_[64][7] = Entry(51);  // Shift
  parse_table_[64][10] = Entry(52);  // Shift
  parse_table_[64][49] = Entry(69);  // Shift
  parse_table_[64][51] = Entry(55);  // Shift
  parse_table_[64][55] = Entry(56);  // Shift
  parse_table_[65][24] = Entry(70);  // Shift
  parse_table_[65][27] = Entry(71);  // Shift
  parse_table_[65][56] = Entry(72);  // Shift
  parse_table_[65][57] = Entry(73);  // Shift
  parse_table_[66][3] = Entry(74);  // Shift
  parse_table_[66][8] = Entry(Item(49, 20, 0, {51, 52}, 20));  // Reduce
  parse_table_[66][16] = Entry(Item(49, 20, 0, {51, 52}, 20));  // Reduce
  parse_table_[67][2] = Entry(Item(51, 22, 0, {51, 55}, 22));  // Reduce
  parse_table_[67][3] = Entry(Item(51, 22, 0, {51, 55}, 22));  // Reduce
  parse_table_[67][4] = Entry(Item(51, 22, 0, {51, 55}, 22));  // Reduce
  parse_table_[67][7] = Entry(Item(51, 22, 0, {51, 55}, 22));  // Reduce
  parse_table_[67][8] = Entry(Item(51, 22, 0, {51, 55}, 22));  // Reduce
  parse_table_[67][10] = Entry(Item(51, 22, 0, {51, 55}, 22));  // Reduce
  parse_table_[67][16] = Entry(Item(51, 22, 0, {51, 55}, 22));  // Reduce
  parse_table_[67][23] = Entry(Item(51, 22, 0, {51, 55}, 22));  // Reduce
  parse_table_[68][16] = Entry(Item(65, 53, 0, {34, 4, 30, 14}, 53));  // Reduce
  parse_table_[69][8] = Entry(Item(48, 18, 0, {48, 50, 49}, 18));  // Reduce
  parse_table_[69][16] = Entry(Item(48, 18, 0, {48, 50, 49}, 18));  // Reduce
  parse_table_[70][25] = Entry(75);  // Shift
  parse_table_[71][25] = Entry(76);  // Shift
  parse_table_[72][3] = Entry(Item(52, 29, 0, {23, 56}, 29));  // Reduce
  parse_table_[72][8] = Entry(Item(52, 29, 0, {23, 56}, 29));  // Reduce
  parse_table_[72][16] = Entry(Item(52, 29, 0, {23, 56}, 29));  // Reduce
  parse_table_[72][24] = Entry(70);  // Shift
  parse_table_[72][27] = Entry(71);  // Shift
  parse_table_[72][57] = Entry(77);  // Shift
  parse_table_[73][3] = Entry(Item(56, 31, 0, {57}, 31));  // Reduce
  parse_table_[73][8] = Entry(Item(56, 31, 0, {57}, 31));  // Reduce
  parse_table_[73][16] = Entry(Item(56, 31, 0, {57}, 31));  // Reduce
  parse_table_[73][24] = Entry(Item(56, 31, 0, {57}, 31));  // Reduce
  parse_table_[73][27] = Entry(Item(56, 31, 0, {57}, 31));  // Reduce
  parse_table_[74][4] = Entry(78);  // Shift
  parse_table_[74][53] = Entry(79);  // Shift
  parse_table_[74][58] = Entry(80);  // Shift
  parse_table_[75][4] = Entry(81);  // Shift
  parse_table_[76][5] = Entry(82);  // Shift
  parse_table_[77][3] = Entry(Item(56, 30, 0, {56, 57}, 30));  // Reduce
  parse_table_[77][8] = Entry(Item(56, 30, 0, {56, 57}, 30));  // Reduce
  parse_table_[77][16] = Entry(Item(56, 30, 0, {56, 57}, 30));  // Reduce
  parse_table_[77][24] = Entry(Item(56, 30, 0, {56, 57}, 30));  // Reduce
  parse_table_[77][27] = Entry(Item(56, 30, 0, {56, 57}, 30));  // Reduce
  parse_table_[78][25] = Entry(83);  // Shift
  parse_table_[79][8] = Entry(Item(54, 43, 0, {}, 43));  // Reduce
  parse_table_[79][16] = Entry(Item(54, 43, 0, {}, 43));  // Reduce
  parse_table_[79][28] = Entry(84);  // Shift
  parse_table_[79][30] = Entry(85);  // Shift
  parse_table_[79][54] = Entry(86);  // Shift
  parse_table_[80][8] = Entry(Item(53, 35, 0, {58}, 35));  // Reduce
  parse_table_[80][16] = Entry(Item(53, 35, 0, {58}, 35));  // Reduce
  parse_table_[80][28] = Entry(Item(53, 35, 0, {58}, 35));  // Reduce
  parse_table_[80][30] = Entry(Item(53, 35, 0, {58}, 35));  // Reduce
  parse_table_[81][26] = Entry(87);  // Shift
  parse_table_[82][26] = Entry(88);  // Shift
  parse_table_[83][7] = Entry(89);  // Shift
  parse_table_[83][9] = Entry(90);  // Shift
  parse_table_[83][29] = Entry(91);  // Shift
  parse_table_[83][59] = Entry(92);  // Shift
  parse_table_[83][60] = Entry(93);  // Shift
  parse_table_[84][4] = Entry(78);  // Shift
  parse_table_[84][58] = Entry(94);  // Shift
  parse_table_[85][4] = Entry(95);  // Shift
  parse_table_[86][8] = Entry(Item(49, 21, 0, {51, 52, 3, 53, 54}, 21));  // Reduce
  parse_table_[86][16] = Entry(Item(49, 21, 0, {51, 52, 3, 53, 54}, 21));  // Reduce
  parse_table_[87][3] = Entry(Item(57, 32, 0, {24, 25, 4, 26}, 32));  // Reduce
  parse_table_[87][8] = Entry(Item(57, 32, 0, {24, 25, 4, 26}, 32));  // Reduce
  parse_table_[87][16] = Entry(Item(57, 32, 0, {24, 25, 4, 26}, 32));  // Reduce
  parse_table_[87][24] = Entry(Item(57, 32, 0, {24, 25, 4, 26}, 32));  // Reduce
  parse_table_[87][27] = Entry(Item(57, 32, 0, {24, 25, 4, 26}, 32));  // Reduce
  parse_table_[88][3] = Entry(Item(57, 33, 0, {27, 25, 5, 26}, 33));  // Reduce
  parse_table_[88][8] = Entry(Item(57, 33, 0, {27, 25, 5, 26}, 33));  // Reduce
  parse_table_[88][16] = Entry(Item(57, 33, 0, {27, 25, 5, 26}, 33));  // Reduce
  parse_table_[88][24] = Entry(Item(57, 33, 0, {27, 25, 5, 26}, 33));  // Reduce
  parse_table_[88][27] = Entry(Item(57, 33, 0, {27, 25, 5, 26}, 33));  // Reduce
  parse_table_[89][26] = Entry(Item(60, 41, 0, {7}, 41));  // Reduce
  parse_table_[89][28] = Entry(Item(60, 41, 0, {7}, 41));  // Reduce
  parse_table_[90][1] = Entry(96);  // Shift
  parse_table_[90][26] = Entry(Item(60, 39, 0, {9}, 39));  // Reduce
  parse_table_[90][28] = Entry(Item(60, 39, 0, {9}, 39));  // Reduce
  parse_table_[91][26] = Entry(Item(60, 42, 0, {29}, 42));  // Reduce
  parse_table_[91][28] = Entry(Item(60, 42, 0, {29}, 42));  // Reduce
  parse_table_[92][26] = Entry(97);  // Shift
  parse_table_[92][28] = Entry(98);  // Shift
  parse_table_[93][26] = Entry(Item(59, 38, 0, {60}, 38));  // Reduce
  parse_table_[93][28] = Entry(Item(59, 38, 0, {60}, 38));  // Reduce
  parse_table_[94][8] = Entry(Item(53, 34, 0, {53, 28, 58}, 34));  // Reduce
  parse_table_[94][16] = Entry(Item(53, 34, 0, {53, 28, 58}, 34));  // Reduce
  parse_table_[94][28] = Entry(Item(53, 34, 0, {53, 28, 58}, 34));  // Reduce
  parse_table_[94][30] = Entry(Item(53, 34, 0, {53, 28, 58}, 34));  // Reduce
  parse_table_[95][14] = Entry(99);  // Shift
  parse_table_[96][26] = Entry(Item(60, 40, 0, {9, 1}, 40));  // Reduce
  parse_table_[96][28] = Entry(Item(60, 40, 0, {9, 1}, 40));  // Reduce
  parse_table_[97][8] = Entry(Item(58, 36, 0, {4, 25, 59, 26}, 36));  // Reduce
  parse_table_[97][16] = Entry(Item(58, 36, 0, {4, 25, 59, 26}, 36));  // Reduce
  parse_table_[97][28] = Entry(Item(58, 36, 0, {4, 25, 59, 26}, 36));  // Reduce
  parse_table_[97][30] = Entry(Item(58, 36, 0, {4, 25, 59, 26}, 36));  // Reduce
  parse_table_[98][7] = Entry(89);  // Shift
  parse_table_[98][9] = Entry(90);  // Shift
  parse_table_[98][29] = Entry(91);  // Shift
  parse_table_[98][60] = Entry(100);  // Shift
  parse_table_[99][8] = Entry(Item(54, 44, 0, {30, 4, 14}, 44));  // Reduce
  parse_table_[99][16] = Entry(Item(54, 44, 0, {30, 4, 14}, 44));  // Reduce
  parse_table_[100][26] = Entry(Item(59, 37, 0, {59, 28, 60}, 37));  // Reduce
  parse_table_[100][28] = Entry(Item(59, 37, 0, {59, 28, 60}, 37));  // Reduce

  // Create inverse non-terminal map.
  inverse_nonterminal_map_.emplace(35, "start");
  inverse_nonterminal_map_.emplace(36, "lexer_block");
  inverse_nonterminal_map_.emplace(37, "newlines");
  inverse_nonterminal_map_.emplace(38, "parser_block");
  inverse_nonterminal_map_.emplace(39, "optional_data_block");
  inverse_nonterminal_map_.emplace(40, "data_block");
  inverse_nonterminal_map_.emplace(41, "lexer_definitions");
  inverse_nonterminal_map_.emplace(42, "lexeme_line");
  inverse_nonterminal_map_.emplace(43, "lexeme_definition");
  inverse_nonterminal_map_.emplace(44, "skip_indicator");
  inverse_nonterminal_map_.emplace(45, "start_indicator");
  inverse_nonterminal_map_.emplace(46, "parser_definitions");
  inverse_nonterminal_map_.emplace(47, "parser_definition");
  inverse_nonterminal_map_.emplace(48, "production_lines");
  inverse_nonterminal_map_.emplace(49, "production_and_data");
  inverse_nonterminal_map_.emplace(50, "or");
  inverse_nonterminal_map_.emplace(51, "production_atoms");
  inverse_nonterminal_map_.emplace(52, "resolution");
  inverse_nonterminal_map_.emplace(53, "actions");
  inverse_nonterminal_map_.emplace(54, "code_block");
  inverse_nonterminal_map_.emplace(55, "production_atom");
  inverse_nonterminal_map_.emplace(56, "resolution_atoms");
  inverse_nonterminal_map_.emplace(57, "resolution_atom");
  inverse_nonterminal_map_.emplace(58, "action");
  inverse_nonterminal_map_.emplace(59, "args");
  inverse_nonterminal_map_.emplace(60, "arg");
  inverse_nonterminal_map_.emplace(61, "data_definitions");
  inverse_nonterminal_map_.emplace(62, "data_definition");
  inverse_nonterminal_map_.emplace(63, "import_statement");
  inverse_nonterminal_map_.emplace(64, "inherit_statement");
  inverse_nonterminal_map_.emplace(65, "general_code_block");

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
  lexer_generator->AddLexeme("integer", "\\d+", 1);  // Lexeme #5
  lexer_generator->AddLexeme("regex", " r ` ( \\\\ ` | [~ `] )* ` ", 1);  // Lexeme #6
  lexer_generator->AddLexeme("literal", " \" ( \\\\\" | [~\"] )* \" ", 1);  // Lexeme #7
  lexer_generator->AddLexeme("or", "\\|", 1);  // Lexeme #8
  lexer_generator->AddLexeme("pos_arg", "$ \\d+", 1);  // Lexeme #9
  lexer_generator->AddLexeme("null", "$null", 1);  // Lexeme #10
  lexer_generator->AddLexeme("comment", "# [~\\n]* \\n", 1);  // Lexeme #11
  lexer_generator->AddLexeme("path", "\\@+/(\\@+/)+\\@+.h", 1);  // Lexeme #12
  lexer_generator->AddLexeme("classname", "( \\@ | _ )+(::( \\@ | _ )+)*", 1);  // Lexeme #13
  lexer_generator->AddLexeme("code", " \\{ [~%\\}]* %\\} ", 1);  // Lexeme #14
  lexer_generator->AddLexeme("spaces", "\\s+", 1);  // Lexeme #15
  lexer_generator->AddLexeme("newlines", "\\n+", 1);  // Lexeme #16
  lexer_generator->AddLexeme("continuation", "\\\\ \\n", 1);  // Lexeme #17
  lexer_generator->AddReserved(".Lexer", 2);  // Lexeme #18
  lexer_generator->AddReserved(".End", 2);  // Lexeme #19
  lexer_generator->AddReserved(".Skip", 2);  // Lexeme #20
  lexer_generator->AddReserved(".Parser", 2);  // Lexeme #21
  lexer_generator->AddReserved(".Start", 2);  // Lexeme #22
  lexer_generator->AddReserved("->", 2);  // Lexeme #23
  lexer_generator->AddReserved("assoc", 2);  // Lexeme #24
  lexer_generator->AddReserved("(", 2);  // Lexeme #25
  lexer_generator->AddReserved(")", 2);  // Lexeme #26
  lexer_generator->AddReserved("prec", 2);  // Lexeme #27
  lexer_generator->AddReserved(",", 2);  // Lexeme #28
  lexer_generator->AddReserved("$null", 2);  // Lexeme #29
  lexer_generator->AddReserved("%", 2);  // Lexeme #30
  lexer_generator->AddReserved(".Data", 2);  // Lexeme #31
  lexer_generator->AddReserved("@import", 2);  // Lexeme #32
  lexer_generator->AddReserved("inherits", 2);  // Lexeme #33
  lexer_generator->AddReserved("code", 2);  // Lexeme #34

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
          std::reinterpret_pointer_cast<ASTNodeBase_optional_data_block>(collected_nodes[4]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[5].get())->literal);
    }
    case 1: {
      REDUCE_ASSERT(1, 1, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_1_optional_data_block_ViaItem_1'.";
      return ReduceTo_ASTNode_1_optional_data_block_ViaItem_1(
          std::reinterpret_pointer_cast<ASTNode_19_data_block>(collected_nodes[0]));
    }
    case 2: {
      REDUCE_ASSERT(0, 2, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_2_optional_data_block_ViaItem_2'.";
      return ReduceTo_ASTNodeGeneral_2_optional_data_block_ViaItem_2();
    }
    case 3: {
      REDUCE_ASSERT(2, 3, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_3_newlines_ViaItem_3'.";
      return ReduceTo_ASTNodeGeneral_3_newlines_ViaItem_3(
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 4: {
      REDUCE_ASSERT(1, 4, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_4_newlines_ViaItem_4'.";
      return ReduceTo_ASTNodeGeneral_4_newlines_ViaItem_4(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
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
      REDUCE_ASSERT(2, 20, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_14_production_and_data_ViaItem_20'.";
      return ReduceTo_ASTNode_14_production_and_data_ViaItem_20(
          std::reinterpret_pointer_cast<ASTNode_production_atoms>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_resolution>(collected_nodes[1]));
    }
    case 21: {
      REDUCE_ASSERT(5, 21, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_15_production_and_data_ViaItem_21'.";
      return ReduceTo_ASTNode_15_production_and_data_ViaItem_21(
          std::reinterpret_pointer_cast<ASTNode_production_atoms>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_resolution>(collected_nodes[1]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          std::reinterpret_pointer_cast<ASTNode_actions>(collected_nodes[3]),
          std::reinterpret_pointer_cast<ASTNodeBase_code_block>(collected_nodes[4]));
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
      REDUCE_ASSERT(0, 28, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_16_resolution_ViaItem_28'.";
      return ReduceTo_ASTNodeGeneral_16_resolution_ViaItem_28();
    }
    case 29: {
      REDUCE_ASSERT(2, 29, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_17_resolution_ViaItem_29'.";
      return ReduceTo_ASTNode_17_resolution_ViaItem_29(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNode_resolution_atoms>(collected_nodes[1]));
    }
    case 30: {
      REDUCE_ASSERT(2, 30, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_resolution_atoms_ViaItem_30'.";
      return ReduceTo_ASTNode_resolution_atoms_ViaItem_30(
          std::reinterpret_pointer_cast<ASTNode_resolution_atoms>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_resolution_atom>(collected_nodes[1]));
    }
    case 31: {
      REDUCE_ASSERT(1, 31, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_resolution_atoms_ViaItem_31'.";
      return ReduceTo_ASTNode_resolution_atoms_ViaItem_31(
          std::reinterpret_pointer_cast<ASTNodeBase_resolution_atom>(collected_nodes[0]));
    }
    case 32: {
      REDUCE_ASSERT(4, 32, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_assoc_ViaItem_32'.";
      return ReduceTo_ASTNode_assoc_ViaItem_32(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 33: {
      REDUCE_ASSERT(4, 33, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_prec_ViaItem_33'.";
      return ReduceTo_ASTNode_prec_ViaItem_33(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 34: {
      REDUCE_ASSERT(3, 34, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_actions_ViaItem_34'.";
      return ReduceTo_ASTNode_actions_ViaItem_34(
          std::reinterpret_pointer_cast<ASTNode_actions>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNode_action>(collected_nodes[2]));
    }
    case 35: {
      REDUCE_ASSERT(1, 35, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_actions_ViaItem_35'.";
      return ReduceTo_ASTNode_actions_ViaItem_35(
          std::reinterpret_pointer_cast<ASTNode_action>(collected_nodes[0]));
    }
    case 36: {
      REDUCE_ASSERT(4, 36, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_action_ViaItem_36'.";
      return ReduceTo_ASTNode_action_ViaItem_36(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNode_arguments>(collected_nodes[2]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 37: {
      REDUCE_ASSERT(3, 37, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_arguments_ViaItem_37'.";
      return ReduceTo_ASTNode_arguments_ViaItem_37(
          std::reinterpret_pointer_cast<ASTNode_arguments>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_arg>(collected_nodes[2]));
    }
    case 38: {
      REDUCE_ASSERT(1, 38, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_arguments_ViaItem_38'.";
      return ReduceTo_ASTNode_arguments_ViaItem_38(
          std::reinterpret_pointer_cast<ASTNodeBase_arg>(collected_nodes[0]));
    }
    case 39: {
      REDUCE_ASSERT(1, 39, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_PosArg_ViaItem_39'.";
      return ReduceTo_ASTNode_PosArg_ViaItem_39(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 40: {
      REDUCE_ASSERT(2, 40, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_PosArg_ViaItem_40'.";
      return ReduceTo_ASTNode_PosArg_ViaItem_40(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 41: {
      REDUCE_ASSERT(1, 41, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_StringArg_ViaItem_41'.";
      return ReduceTo_ASTNode_StringArg_ViaItem_41(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 42: {
      REDUCE_ASSERT(1, 42, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_NullArg_ViaItem_42'.";
      return ReduceTo_ASTNode_NullArg_ViaItem_42(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 43: {
      REDUCE_ASSERT(0, 43, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_18_code_block_ViaItem_43'.";
      return ReduceTo_ASTNodeGeneral_18_code_block_ViaItem_43();
    }
    case 44: {
      REDUCE_ASSERT(3, 44, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_code_block_ViaItem_44'.";
      return ReduceTo_ASTNode_code_block_ViaItem_44(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 45: {
      REDUCE_ASSERT(5, 45, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_19_data_block_ViaItem_45'.";
      return ReduceTo_ASTNode_19_data_block_ViaItem_45(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_data_definitions>(collected_nodes[2]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[3]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[4].get())->literal);
    }
    case 46: {
      REDUCE_ASSERT(3, 46, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_20_data_definitions_ViaItem_46'.";
      return ReduceTo_ASTNode_20_data_definitions_ViaItem_46(
          std::reinterpret_pointer_cast<ASTNodeBase_data_definitions>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_newlines>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_data_definition>(collected_nodes[2]));
    }
    case 47: {
      REDUCE_ASSERT(1, 47, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_21_data_definitions_ViaItem_47'.";
      return ReduceTo_ASTNode_21_data_definitions_ViaItem_47(
          std::reinterpret_pointer_cast<ASTNodeBase_data_definition>(collected_nodes[0]));
    }
    case 48: {
      REDUCE_ASSERT(1, 48, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_import_ViaItem_48'.";
      return ReduceTo_ASTNode_import_ViaItem_48(
          std::reinterpret_pointer_cast<ASTNode_22_import_statement>(collected_nodes[0]));
    }
    case 49: {
      REDUCE_ASSERT(1, 49, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_inherit_ViaItem_49'.";
      return ReduceTo_ASTNode_inherit_ViaItem_49(
          std::reinterpret_pointer_cast<ASTNode_23_inherit_statement>(collected_nodes[0]));
    }
    case 50: {
      REDUCE_ASSERT(1, 50, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_define_code_block_ViaItem_50'.";
      return ReduceTo_ASTNode_define_code_block_ViaItem_50(
          std::reinterpret_pointer_cast<ASTNode_24_general_code_block>(collected_nodes[0]));
    }
    case 51: {
      REDUCE_ASSERT(2, 51, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_22_import_statement_ViaItem_51'.";
      return ReduceTo_ASTNode_22_import_statement_ViaItem_51(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 52: {
      REDUCE_ASSERT(3, 52, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_23_inherit_statement_ViaItem_52'.";
      return ReduceTo_ASTNode_23_inherit_statement_ViaItem_52(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 53: {
      REDUCE_ASSERT(4, 53, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNode_24_general_code_block_ViaItem_53'.";
      return ReduceTo_ASTNode_24_general_code_block_ViaItem_53(
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
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_optional_data_block>& argument_4,
    [[maybe_unused]] const std::string& argument_5) {
  auto new_node = std::make_shared<ASTNode_0_start>(0);

  // Set fields in the new node.
  new_node->lexer_block = argument_0;
  new_node->parser_block = argument_2;
  new_node->data_block = argument_4;

  return new_node;
}

inline std::shared_ptr<ASTNode_1_optional_data_block>
Parser::ReduceTo_ASTNode_1_optional_data_block_ViaItem_1(
    [[maybe_unused]] const std::shared_ptr<ASTNode_19_data_block>& argument_0) {
  auto new_node = std::make_shared<ASTNode_1_optional_data_block>(1);

  // Set fields in the new node.
  new_node->data_block = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_2_optional_data_block>
Parser::ReduceTo_ASTNodeGeneral_2_optional_data_block_ViaItem_2() {
  return std::make_shared<ASTNodeGeneral_2_optional_data_block>(2);
}

inline std::shared_ptr<ASTNodeGeneral_3_newlines>
Parser::ReduceTo_ASTNodeGeneral_3_newlines_ViaItem_3(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_3_newlines>(3);

  // Set fields in the new node.
  new_node->newlines_node = argument_0;
  new_node->newlines = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_4_newlines>
Parser::ReduceTo_ASTNodeGeneral_4_newlines_ViaItem_4(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_4_newlines>(4);

  // Set fields in the new node.
  new_node->newlines = argument_0;

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
    [[maybe_unused]] const std::shared_ptr<ASTNode_production_atoms>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_resolution>& argument_1) {
  auto new_node = std::make_shared<ASTNode_14_production_and_data>(20);

  // Set fields in the new node.
  new_node->atoms = argument_0->atoms;
  new_node->resolution = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_15_production_and_data>
Parser::ReduceTo_ASTNode_15_production_and_data_ViaItem_21(
    [[maybe_unused]] const std::shared_ptr<ASTNode_production_atoms>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_resolution>& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNode_actions>& argument_3,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_code_block>& argument_4) {
  auto new_node = std::make_shared<ASTNode_15_production_and_data>(21);

  // Set fields in the new node.
  new_node->atoms = argument_0->atoms;
  new_node->resolution = argument_1;
  new_node->actions = argument_3->actions;
  new_node->code = argument_4;

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

inline std::shared_ptr<ASTNodeGeneral_16_resolution>
Parser::ReduceTo_ASTNodeGeneral_16_resolution_ViaItem_28() {
  return std::make_shared<ASTNodeGeneral_16_resolution>(28);
}

inline std::shared_ptr<ASTNode_17_resolution>
Parser::ReduceTo_ASTNode_17_resolution_ViaItem_29(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNode_resolution_atoms>& argument_1) {
  auto new_node = std::make_shared<ASTNode_17_resolution>(29);

  // Set fields in the new node.
  new_node->atoms = argument_1->atoms;

  return new_node;
}

inline std::shared_ptr<ASTNode_resolution_atoms>
Parser::ReduceTo_ASTNode_resolution_atoms_ViaItem_30(
    [[maybe_unused]] const std::shared_ptr<ASTNode_resolution_atoms>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_resolution_atom>& argument_1) {
  auto new_node = std::make_shared<ASTNode_resolution_atoms>(30);

  // Set fields in the new node.
  new_node->atoms.insert(new_node->atoms.end(), argument_0->atoms.cbegin(), argument_0->atoms.cend());
  new_node->atoms.push_back(argument_1);

  return new_node;
}

inline std::shared_ptr<ASTNode_resolution_atoms>
Parser::ReduceTo_ASTNode_resolution_atoms_ViaItem_31(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_resolution_atom>& argument_0) {
  auto new_node = std::make_shared<ASTNode_resolution_atoms>(31);

  // Set fields in the new node.
  new_node->atoms.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_assoc>
Parser::ReduceTo_ASTNode_assoc_ViaItem_32(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNode_assoc>(32);

  // Set fields in the new node.
  new_node->assoc = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_prec>
Parser::ReduceTo_ASTNode_prec_ViaItem_33(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNode_prec>(33);

  // Set fields in the new node.
  new_node->prec = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_actions>
Parser::ReduceTo_ASTNode_actions_ViaItem_34(
    [[maybe_unused]] const std::shared_ptr<ASTNode_actions>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_action>& argument_2) {
  auto new_node = std::make_shared<ASTNode_actions>(34);

  // Set fields in the new node.
  new_node->actions.insert(new_node->actions.end(), argument_0->actions.cbegin(), argument_0->actions.cend());
  new_node->actions.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNode_actions>
Parser::ReduceTo_ASTNode_actions_ViaItem_35(
    [[maybe_unused]] const std::shared_ptr<ASTNode_action>& argument_0) {
  auto new_node = std::make_shared<ASTNode_actions>(35);

  // Set fields in the new node.
  new_node->actions.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_action>
Parser::ReduceTo_ASTNode_action_ViaItem_36(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNode_arguments>& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNode_action>(36);

  // Set fields in the new node.
  new_node->fn_name = argument_0;
  new_node->arguments = argument_2->arguments;

  return new_node;
}

inline std::shared_ptr<ASTNode_arguments>
Parser::ReduceTo_ASTNode_arguments_ViaItem_37(
    [[maybe_unused]] const std::shared_ptr<ASTNode_arguments>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_arg>& argument_2) {
  auto new_node = std::make_shared<ASTNode_arguments>(37);

  // Set fields in the new node.
  new_node->arguments.insert(new_node->arguments.end(), argument_0->arguments.cbegin(), argument_0->arguments.cend());
  new_node->arguments.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNode_arguments>
Parser::ReduceTo_ASTNode_arguments_ViaItem_38(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_arg>& argument_0) {
  auto new_node = std::make_shared<ASTNode_arguments>(38);

  // Set fields in the new node.
  new_node->arguments.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_PosArg>
Parser::ReduceTo_ASTNode_PosArg_ViaItem_39(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNode_PosArg>(39);

  // Set fields in the new node.
  new_node->pos = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_PosArg>
Parser::ReduceTo_ASTNode_PosArg_ViaItem_40(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNode_PosArg>(40);

  // Set fields in the new node.
  new_node->pos = argument_0;
  new_node->field_name = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_StringArg>
Parser::ReduceTo_ASTNode_StringArg_ViaItem_41(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNode_StringArg>(41);

  // Set fields in the new node.
  new_node->argument = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNode_NullArg>
Parser::ReduceTo_ASTNode_NullArg_ViaItem_42(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNode_NullArg>(42);
}

inline std::shared_ptr<ASTNodeGeneral_18_code_block>
Parser::ReduceTo_ASTNodeGeneral_18_code_block_ViaItem_43() {
  return std::make_shared<ASTNodeGeneral_18_code_block>(43);
}

inline std::shared_ptr<ASTNode_code_block>
Parser::ReduceTo_ASTNode_code_block_ViaItem_44(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNode_code_block>(44);

  // Set fields in the new node.
  new_node->visitor_name = argument_1;
  new_node->code_body = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNode_19_data_block>
Parser::ReduceTo_ASTNode_19_data_block_ViaItem_45(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_data_definitions>& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_3,
    [[maybe_unused]] const std::string& argument_4) {
  auto new_node = std::make_shared<ASTNode_19_data_block>(45);

  // Set fields in the new node.
  new_node->definitions = argument_2->definitions;

  return new_node;
}

inline std::shared_ptr<ASTNode_20_data_definitions>
Parser::ReduceTo_ASTNode_20_data_definitions_ViaItem_46(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_data_definitions>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_newlines>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_data_definition>& argument_2) {
  auto new_node = std::make_shared<ASTNode_20_data_definitions>(46);

  // Set fields in the new node.
  new_node->definitions.insert(new_node->definitions.end(), argument_0->definitions.cbegin(), argument_0->definitions.cend());
  new_node->definitions.push_back(argument_2);

  return new_node;
}

inline std::shared_ptr<ASTNode_21_data_definitions>
Parser::ReduceTo_ASTNode_21_data_definitions_ViaItem_47(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_data_definition>& argument_0) {
  auto new_node = std::make_shared<ASTNode_21_data_definitions>(47);

  // Set fields in the new node.
  new_node->definitions.push_back(argument_0);

  return new_node;
}

inline std::shared_ptr<ASTNode_import>
Parser::ReduceTo_ASTNode_import_ViaItem_48(
    [[maybe_unused]] const std::shared_ptr<ASTNode_22_import_statement>& argument_0) {
  auto new_node = std::make_shared<ASTNode_import>(48);

  // Set fields in the new node.
  new_node->path = argument_0->path;

  return new_node;
}

inline std::shared_ptr<ASTNode_inherit>
Parser::ReduceTo_ASTNode_inherit_ViaItem_49(
    [[maybe_unused]] const std::shared_ptr<ASTNode_23_inherit_statement>& argument_0) {
  auto new_node = std::make_shared<ASTNode_inherit>(49);

  // Set fields in the new node.
  new_node->visitor_name = argument_0->visitor_name;
  new_node->base = argument_0->base;

  return new_node;
}

inline std::shared_ptr<ASTNode_define_code_block>
Parser::ReduceTo_ASTNode_define_code_block_ViaItem_50(
    [[maybe_unused]] const std::shared_ptr<ASTNode_24_general_code_block>& argument_0) {
  auto new_node = std::make_shared<ASTNode_define_code_block>(50);

  // Set fields in the new node.
  new_node->class_name = argument_0->class_name;
  new_node->code = argument_0->code;

  return new_node;
}

inline std::shared_ptr<ASTNode_22_import_statement>
Parser::ReduceTo_ASTNode_22_import_statement_ViaItem_51(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNode_22_import_statement>(51);

  // Set fields in the new node.
  new_node->path = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNode_23_inherit_statement>
Parser::ReduceTo_ASTNode_23_inherit_statement_ViaItem_52(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNode_23_inherit_statement>(52);

  // Set fields in the new node.
  new_node->visitor_name = argument_1;
  new_node->base = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNode_24_general_code_block>
Parser::ReduceTo_ASTNode_24_general_code_block_ViaItem_53(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNode_24_general_code_block>(53);

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
  void Visit(ASTNodeGeneral_16_resolution& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_18_code_block& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_2_optional_data_block& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_3_newlines& object) override { /* No body defined */ }
  void Visit(ASTNodeGeneral_4_newlines& object) override { /* No body defined */ }
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
  void Visit(ASTNode_17_resolution& object) override {
    for (auto& atom : object.atoms) {
      atom->Accept(*this);
    }
  }
  void Visit(ASTNode_19_data_block& object) override {
    LOG_SEV(Info) << "Getting definitions.";
    for (auto& definition : object.definitions) {
      definition->Accept(*this);
    }
  }
  void Visit(ASTNode_1_optional_data_block& object) override {
    Visit(*object.data_block);
  }
  void Visit(ASTNode_20_data_definitions& object) override { /* No body defined */ }
  void Visit(ASTNode_21_data_definitions& object) override { /* No body defined */ }
  void Visit(ASTNode_22_import_statement& object) override { /* No body defined */ }
  void Visit(ASTNode_23_inherit_statement& object) override { /* No body defined */ }
  void Visit(ASTNode_24_general_code_block& object) override { /* No body defined */ }
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
    if (object.item_id == 39) {
      LOG_SEV(Debug) << "Pos arg: ARG:" << object.pos;
      addArgumentToAction(object.pos.substr(1));
    } else if (object.item_id == 40) {
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
    if (object.item_id == 34) {
      for (auto& action : object.actions) {
        action->Accept(*this);
      }
    } else if (object.item_id == 35) {
      for (auto& action : object.actions) {
        action->Accept(*this);
      }
    } 
  }
  void Visit(ASTNode_arguments& object) override { /* No body defined */ }
  void Visit(ASTNode_assoc& object) override {
    LOG_SEV(Debug) << "Assoc: " << object.assoc;
    current_item_.res_info.assoc = toAssociation(object.assoc);
  }
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
  void Visit(ASTNode_prec& object) override {
    LOG_SEV(Debug) << "Prec: " << object.prec;
    current_item_.res_info.precedence = manta::stoi(object.prec);
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
  void Visit(ASTNode_resolution_atoms& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_newlines& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_optional_data_block& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_lexer_definitions& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_lexeme_line& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_parser_definitions& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_production_and_data& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_resolution& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_code_block& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_production_atom& object) override { /* No body defined */ }
  void Visit(ASTNodeBase_resolution_atom& object) override { /* No body defined */ }
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

  manta::Associativity toAssociation(const std::string& str) {
    using namespace manta;
    if (str == "Left") return Associativity::LEFT;
    if (str == "Right") return Associativity::RIGHT;
    if (str == "None") return Associativity::NONE;
    MANTA_FAIL("unknown associativity: " << str);
  }
  // End ad-hoc code.
};

#define MANTA_PARSER_GENERATED
