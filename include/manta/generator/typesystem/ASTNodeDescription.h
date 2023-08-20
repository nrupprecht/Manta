#pragma once

#include "manta/generator/typesystem/CodeGen.h"

namespace manta {

using NonterminalID = int;

//! \brief Class that keeps track of and owns the data for all types.
//!
//! This class also knows how to generate C++ code representing the nodes, but you can
//! imagine that this class could be left as just an IR of all the nodes, and separate
//! classes could do the actual codegen into a target language.
//!
class ASTNodeManager {
public:
  struct NonterminalTypes {
    //! \brief The base type for all nodes associated with this non-terminal.
    TypeDescriptionStructure* base_type;
    //! \brief All child types of the base type for the non-terminal.
    std::map<std::string, TypeDescriptionStructure*> child_types;
  };

  //! \brief Initialize the node manager's type system with an enum for node types, a
  //! base class for AST nodes, and a node type for string
  ASTNodeManager();

  NO_DISCARD TypeDescriptionStructure* GetASTNodeBase() const;

  NO_DISCARD TypeDescriptionStructure* GetASTLexeme() const;

  NO_DISCARD TypeDescriptionStructure* GetVisitorStructure() const;

  //! \brief Get the enum that enumerate all the ASTNodeTypes.
  NO_DISCARD TypeDescriptionEnum* GetASTNodeType() const;

  //! \brief Get the enum that enumerate all the non-terminals.
  NO_DISCARD TypeDescriptionEnum* GetNonterminalEnum() const;

  TypeDescriptionStructure* GetNodeDescription(const std::string& type_name, NonterminalID nonterminal_id);

  NO_DISCARD TypeDescriptionStructure* GetNodeDescription(const std::string& type_name);

  //! \brief Write the code for all AST node definitions to an ostream.
  void CreateAllDefinitions(std::ostream& out, const CodeGen& code_gen) const;

  NO_DISCARD const TypeDescription* GetStringType() const;

  const TypeDescription* MakeShared(const TypeDescription* pointed_type);

  const TypeDescription* MakeVector(const TypeDescription* vector_type);

  NonterminalTypes& GetNonterminalTypes(NonterminalID id);

  const std::map<NonterminalID, NonterminalTypes>& GetAllNodeTypesForNonterminals() const;

  //! \brief Directly get the node manager's type system.
  TypeSystem& GetTypeSystem();

private:
  TypeSystem type_system_;

  //! \brief The enum type for the AST nodes.
  TypeDescriptionEnum* node_enum_;

  //! \brief The enum type for non-terminals.
  TypeDescriptionEnum* nonterminal_enum_;

  //! \brief The type for the ASTNodeBase.
  TypeDescriptionStructure* node_base_;

  //! \brief The type for lexemes.
  TypeDescriptionStructure* lexeme_node_;

  //! \brief The type for the ASTNode visitor.
  TypeDescriptionStructure* visitor_type_;

  // Map from non-terminal to the set of node types that productions for this node can
  // product.
  std::map<NonterminalID, NonterminalTypes> node_types_for_nonterminal_ {};
};

}  // namespace manta