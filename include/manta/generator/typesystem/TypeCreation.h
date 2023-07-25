//
// Created by Nathaniel Rupprecht on 4/12/23.
//

#pragma once

#include "manta/generator/DescriptionParser.h"
#include "manta/generator/ParserGenerator.h"
#include "manta/generator/typesystem/TypeDeduction.h"
#include "manta/generator/typesystem/TypeRelationship.h"

namespace manta::typesystem {

using FieldTracker = std::pair<NonterminalID, std::string>;

struct ParserTypeData {
  ASTNodeManager node_manager;
  std::map<std::string, std::vector<TypeRelationship>> relationships;
  std::map<std::string, NonterminalID> nonterminals_for_type;
  std::map<unsigned, std::string> node_types_for_item;
};

class ParserDataToTypeManager {
public:
  ParserDataToTypeManager(bool tag_generated_field_names,
                          bool generated_nodes_have_node_in_name) noexcept;

  //! \brief Based on the parser description, creates a collection of data that is needed
  //! for managing the parser AST nodes and determining their types. Note: does not
  //! determine the types of the fields in the nodes, but detects all the referencing
  //! relationships between fields in different nodes
  //!
  //! Returns (in a struct)
  //! node_manager: An ASTNodeManager that
  //!
  //! relationships: All the relationships between fields in different nodes, tells us how
  //! to construct one node from another.
  //!
  //! nonterminals_for_type: the nonterminal ID
  //! associated with each type. In general, since every non-terminal can have multiple
  //! productions, and each production can result in a different node type, there can be
  //! multiple types with the same nonterminal ID (each from a different production for
  //! the same type).
  //!
  //! node_types_for_item: the AST node type name that is created by each production item.
  //!
  ParserTypeData& CreateRelationships(
      const std::shared_ptr<const ParserData>& parser_data);

  //! \brief Function that deduces what types all fields of all AST nodes must have, or
  //! raises that the type constraints cannot be satisfied.
  //!
  TypeDeduction DeduceTypes();

private:
  // ==================================================================================================
  //  Private member variables.
  // ==================================================================================================

  std::function<std::string(const std::string&)> field_name_sanitizer_ {};

  bool tag_generated_field_names_ = false;
  bool generated_nodes_have_node_in_name_ = true;

  ParserTypeData parser_type_data_;

  std::shared_ptr<ProductionRulesData> production_rules_data_;

  ASTNodeManager& node_manager() { return parser_type_data_.node_manager; }
  std::map<std::string, std::vector<TypeRelationship>>& relationships() {
    return parser_type_data_.relationships;
  }
  std::map<std::string, NonterminalID>& nonterminals_for_type() {
    return parser_type_data_.nonterminals_for_type;
  }
  std::map<unsigned, std::string>& node_types_for_item() {
    return parser_type_data_.node_types_for_item;
  }

  // ==================================================================================================
  //  Private member functions.
  // ==================================================================================================

  void determineBaseTypes(TypeDeduction& deduction);

  std::tuple<int, NonterminalID, std::optional<std::string>> getSourceData(
      const std::string& argument_string, const Item& item);

  void createGeneralNode(const std::string& node_type_name,
                         NonterminalID nonterminal_id,
                         const std::string& nonterminal_name,
                         const Item& item,
                         std::map<std::string, NonterminalID>& nonterminals_for_type);

  void processFieldCommand(const std::vector<std::shared_ptr<ParseNode>>& arguments,
                           const Item& item,
                           unsigned item_number,
                           TypeDescriptionStructure* node_type_description);

  void processAppendCommand(const std::vector<std::shared_ptr<ParseNode>>& arguments,
                            const Item& item,
                            unsigned item_number,
                            TypeDescriptionStructure* node_type_description);

  void processPushCommand(const std::vector<std::shared_ptr<ParseNode>>& arguments,
                          const Item& item,
                          unsigned,  // item_number
                          TypeDescriptionStructure* node_type_description);

  const TypeDescription* makeType(CheckType check_type,
                                  const TypeDescription* source_type);

  const TypeDescription* deduceTypesDFS(
      FieldTracker& target_field,
      TypeDeduction& deduction,
      const std::map<FieldTracker, std::vector<TypeRelationship*>>&
          unsolved_relationships,
      std::set<FieldTracker>& referenced_fields);
};

}  // namespace manta::typesystem