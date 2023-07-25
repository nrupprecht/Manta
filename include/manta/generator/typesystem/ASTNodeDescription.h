#pragma once

#include "manta/generator/typesystem/CodeGen.h"

namespace manta {

using NonterminalID = int;

//! \brief Class that keeps track of and owns the data for all types.
//!
//! This class also knows how to generate C++ code representing the nodes, but you can imagine that this class could
//! be left as just an IR of all the nodes, and separate classes could do the actual codegen into a target language.
//!
class ASTNodeManager {
public:
  struct NonterminalTypes {
    //! \brief The base type for all nodes associated with this non-terminal.
    TypeDescriptionStructure* base_type;
    //! \brief All child types of the base type for the non-terminal.
    std::map<std::string, TypeDescriptionStructure*> child_types;
  };

  ASTNodeManager();

  NO_DISCARD const TypeDescriptionStructure* GetASTNodeBase() const;

  NO_DISCARD const TypeDescriptionStructure* GetASTLexeme() const;

  NO_DISCARD TypeDescriptionEnum* GetASTNodeType() const;

  TypeDescriptionStructure* GetNodeDescription(const std::string& type_name, NonterminalID nonterminal_id);

  NO_DISCARD TypeDescriptionStructure* GetNodeDescription(const std::string& type_name);

  //! \brief Write the code for all AST node definitions to an ostream.
  void CreateAllDefinitions(std::ostream& out, const CodeGen& code_gen) const;

  NO_DISCARD const TypeDescription* GetStringType() const;

  const TypeDescription* MakeShared(const TypeDescription* pointed_type);

  const TypeDescription* MakeVector(const TypeDescription* vector_type);

  void writeNodeTypeDeclaration(std::ostream& out, const TypeDescriptionStructure* description) const {
    out << "struct " << description->type_name << " : ";

    // Parent classes
    if (description->parent_classes.empty()) {
      out << "public ASTNodeBase";
    }
    else {
      auto count = 0u;
      for (auto& parent: description->parent_classes) {
        if (count != 0) {
          out << ", ";
        }
        out << "public " << parent->type_name;
        count += 1;
      }
    }
    out << " {\n";

    // Initialize with the correct enum.
    out << "  //! \\brief Default constructor for " << description->type_name << ".\n  //!\n";
    out << "  " << description->type_name << "()\n";
    if (description->parent_classes.empty()) {
      out << "    : ASTNodeBase(ASTNodeType::" << description->type_name << ") {}\n\n";

      // Add second constructor, for passing the node subtype.
      out << "  //! \\brief Constructor to pass a sub-node-type up to the base.\n  //!\n";
      out << "  explicit " << description->type_name << "(ASTNodeType subtype)\n";
      out << "    : ASTNodeBase(subtype) {}\n\n";
    }
    else {
      // This is not general. I am assuming that there is one parent node, and it has the correct constructor.
      // TODO: Introduce enum subtypes for the node subtypes?
      out << "    : " << (*description->parent_classes.begin())->type_name << "(ASTNodeType::" << description->type_name << ") {}\n\n";
    }

    // Define all fields.
    for (auto[field_name, field_description]: description->fields) {
      // Write type.
      out << "  ";
      out << field_description->Write();
      out << " " << field_name << "{};\n";
    }
    out << "};\n\n";
  }

  NonterminalTypes& GetNonterminalTypes(NonterminalID id);

private:

  TypeSystem type_system_;

  //! \brief The type for enums for the nodes.
  TypeDescriptionEnum* node_enum_;

  //! \brief The type for the ASTNodeBase.
  TypeDescriptionStructure* node_base_;

  //! \brief type type for lexemes.
  TypeDescriptionStructure* lexeme_node_;

  // Map from non-terminal to the set of node types that productions for this node can product.
  std::map<NonterminalID, NonterminalTypes> node_types_for_nonterminal_{};
};

} // namespace manta