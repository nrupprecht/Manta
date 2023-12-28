#pragma once

#include "manta/generator/ParserGenerator.h"
#include "manta/generator/typesystem/TypeDeduction.h"
#include "manta/generator/typesystem/TypeRelationship.h"

namespace manta {

//! \brief Class that creates the parser code from the generated types and description of the parser and
//! lexer.
//!
class ParserCodegen {
 public:
  void GenerateParserCode(std::ostream& code_out, const std::shared_ptr<const ParserData>& parser_data);

  void GenerateParserCode(std::ostream& code_out,
                          std::istream& parser_description,
                          ParserType parser_type);

  [[maybe_unused]] void SetTagGeneratedFieldNames(bool flag) { tag_generated_field_names = flag; }

  [[maybe_unused]] void SetGeneratedNodesHaveNodeInName(bool flag) {
    generated_nodes_have_node_in_name = flag;
  }

  void SetDescriptionParser(std::shared_ptr<DescriptionParser> description_parser) {
    description_parser_ = std::move(description_parser);
  }

  //! \brief Get the parser data created while parsing the description of the parser.
  std::shared_ptr<const ParserData> GetParserData() const { return parser_data_; }

 private:
  //! \brief Set up the base visitor class.
  TypeDescriptionStructure* createBaseVisitor(ASTNodeManager& node_manager) const;

  TypeDescriptionStructure* createPrintingVisitor(ASTNodeManager& node_manager) const;

  TypeDescriptionStructure* createVisitorFromTemplate(
      ASTNodeManager& node_manager,
      const VisitorData::Visitor& visitor_data,
      const std::map<ItemID, const TypeDescriptionStructure*>& node_types_for_item) const;

  TypeDescriptionStructure* createTreePrintVisitor(ASTNodeManager& node_manager) const;

  //! \brief If true, generated field names will be tagged with the argument number.
  //!
  //! This is useful if there will be multiple identical lexemes or non-terminals in a single reduction.
  //!
  bool tag_generated_field_names = false;

  //! \brief If true, generated field names for shared pointers to AST nodes will have a "_node" suffix attached.
  //!
  //! This is useful if there is a non-terminal and a lexeme with the same name in a single reduction.
  //!
  bool generated_nodes_have_node_in_name = true;

  //! \brief The description parser that will be used to parse the description of the parser.
  //!
  std::shared_ptr<DescriptionParser> description_parser_ = std::make_shared<HandWrittenDescriptionParser>();

  //! \brief Parser data created while parsing the description of the parser. Used to create diagnostics.
  //!
  std::shared_ptr<ParserData> parser_data_{};
};

}  // namespace manta