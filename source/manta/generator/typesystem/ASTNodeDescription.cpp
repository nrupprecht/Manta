//
// Created by Nathaniel Rupprecht on 5/18/23.
//

#include "manta/generator/typesystem/ASTNodeDescription.h"
#include <Lightning/Lightning.h>

using namespace manta;
using namespace manta;

ASTNodeManager::ASTNodeManager() {
  // Create the NodeType enum.
  node_enum_ = type_system_.Enum("ASTNodeType");
  node_enum_->AddOption("Literal");

  // Create node base structure.
  node_base_ = type_system_.Structure("ASTNodeBase");
  node_base_->AddField("node_type", node_enum_);
  node_base_->AddConstructor(StructureConstructor{
      {{node_enum_, "node_type"}}, // Arguments
      {}, // Parent constructors
      {{"node_type", "node_type"},},
      {}
  });

  // Create lexeme node.
  lexeme_node_ = type_system_.Structure("ASTLexeme");
  lexeme_node_->AddField("literal", type_system_.String());
  lexeme_node_->AddParent(node_base_);
  lexeme_node_->AddConstructor(StructureConstructor{
      {{type_system_.String(), "literal"}}, // Argument list (type, arg name)
      {{node_base_, {StructureConstructor::Value{"ASTNodeType::Literal"},}}},
      {{"literal", "literal"}}, // List initialized args, (arg name, field name)
      {} // Additional initialized values (field name, value)
  });
}

const TypeDescriptionStructure* ASTNodeManager::GetASTNodeBase() const {
  return node_base_;
}

const TypeDescriptionStructure* ASTNodeManager::GetASTLexeme() const {
  return lexeme_node_;
}

TypeDescriptionEnum* ASTNodeManager::GetASTNodeType() const {
  return node_enum_;
}

TypeDescriptionStructure* ASTNodeManager::GetNodeDescription(const std::string& type_name, NonterminalID nonterminal_id) {
  auto node_type = GetNodeDescription(type_name);
  node_types_for_nonterminal_[nonterminal_id].child_types.emplace(type_name, node_type);
  node_enum_->AddOption("Type_" + type_name);

  return node_type;
}

NO_DISCARD TypeDescriptionStructure* ASTNodeManager::GetNodeDescription(const std::string& type_name) {
  return type_system_.Structure(type_name);
}

void ASTNodeManager::CreateAllDefinitions(std::ostream& out, const CodeGen& code_gen) const {
  MANTA_REQUIRE(node_enum_, "the node enum must be defined");
  MANTA_REQUIRE(node_base_, "the AST node base class must be defined");

  LOG_SEV(Info) << ""; // Log break.

  // Declare all node types.

  for (auto&[nonterminal_id, nonterminal_types]: node_types_for_nonterminal_) {
    // Write base type definition.
    if (nonterminal_types.base_type) {
      out << "struct " << code_gen.WriteName(nonterminal_types.base_type) << ";\n";
    }

    // Write all child types.
    for (auto&[name, type]: nonterminal_types.child_types) {
      LOG_SEV(Info) << "Generating definition for node type '" << name << "'.";
      out << "struct " << code_gen.WriteName(type) << ";\n";
    }
  }
  out << "\n\n";


  out << "//! \\brief Define the enums for each node type.\n//!\n";
  code_gen.WriteDefinition(out, node_enum_);
  out << "\n";
  out << "//! \\brief Define the base node type for all AST nodes.\n//!\n";
  code_gen.WriteDefinition(out, node_base_);
  out << "\n\n";

  out << "//! \\brief Node for basic lexemes.\n//!\n";
  code_gen.WriteDefinition(out, lexeme_node_);
  out << "\n\n";

  for (auto&[nonterminal_id, nonterminal_types]: node_types_for_nonterminal_) {
    out << "// ============================================================\n";
    out << "//  Nodes for nonterminal " << nonterminal_id << " productions.\n";
    out << "// ============================================================\n\n";
    // Write base type definition.
    if (nonterminal_types.base_type) {
      LOG_SEV(Info) << "There was a parent class for nodes associated with nontermainl type " << nonterminal_id
                    << ", generating definition for node type '" << nonterminal_types.base_type->type_name << "'.";
      // out << "//! \\brief Base node type for nodes associated with nonterminal " << nonterminal_id << " productions.\n//!\n";
      // code_gen.WriteDefinition(out, nonterminal_types.base_type);
      // out << "\n\n";
    }

    // Write all child types.
    for (auto&[name, type]: nonterminal_types.child_types) {
      LOG_SEV(Info) << "Generating definition for node type '" << name << "'.";
      code_gen.WriteDefinition(out, type);
      out << "\n\n";
    }
  }
}

NO_DISCARD const TypeDescription* ASTNodeManager::GetStringType() const {
  return type_system_.String();
}

const TypeDescription* ASTNodeManager::MakeShared(const TypeDescription* pointed_type) {
  return type_system_.SharedPointer(pointed_type);
}

const TypeDescription* ASTNodeManager::MakeVector(const TypeDescription* vector_type) {
  return type_system_.Vector(vector_type);
}

ASTNodeManager::NonterminalTypes& ASTNodeManager::GetNonterminalTypes(NonterminalID id) {
  return node_types_for_nonterminal_.at(id);
}