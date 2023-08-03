//
// Created by Nathaniel Rupprecht on 5/18/23.
//

#include "manta/generator/typesystem/ASTNodeDescription.h"
// Other files.
#include "manta/utility/Formatting.h"

using namespace manta;
using namespace manta;

ASTNodeManager::ASTNodeManager() {
  // Create the NodeType enum.
  node_enum_ = type_system_.Enum("ASTNodeType");
  node_enum_->AddOption("Literal");

  // Create node base structure.
  node_base_ = type_system_.Structure("ASTNodeBase");
  node_base_->AddField("node_type", node_enum_);
  node_base_->AddConstructor(StructureConstructor {{{node_enum_, "node_type"}},  // Arguments
                                                   {},  // Parent constructors
                                                   {
                                                       {"node_type", "node_type"},
                                                   },
                                                   {}});

  // Create lexeme node.
  lexeme_node_ = type_system_.Structure("ASTLexeme");
  lexeme_node_->AddField("literal", type_system_.String());
  lexeme_node_->AddParent(node_base_);
  lexeme_node_->AddConstructor(StructureConstructor {
      {{type_system_.String(), "literal"}},  // Argument list (type, arg name)
      {{node_base_,
        {
            StructureConstructor::Value {"ASTNodeType::Literal"},
        }}},
      {{"literal", "literal"}},  // List initialized args, (arg name, field name)
      {}  // Additional initialized values (field name, value)
  });

  // Create the visitor struct. We will add the visitor functions at the end, once we know all the node types.
  visitor_type_ = type_system_.Structure("ASTNodeVisitor");
}

TypeDescriptionStructure* ASTNodeManager::GetASTNodeBase() const {
  return node_base_;
}

TypeDescriptionStructure* ASTNodeManager::GetASTLexeme() const {
  return lexeme_node_;
}

NO_DISCARD TypeDescriptionStructure* ASTNodeManager::GetVisitorStructure() const {
  return visitor_type_;
}

TypeDescriptionEnum* ASTNodeManager::GetASTNodeType() const {
  return node_enum_;
}

TypeDescriptionStructure* ASTNodeManager::GetNodeDescription(const std::string& type_name,
                                                             NonterminalID nonterminal_id) {
  auto node_type = GetNodeDescription(type_name);
  node_types_for_nonterminal_[nonterminal_id].child_types.emplace(type_name, node_type);
  node_enum_->AddOption("Type_" + type_name);

  return node_type;
}

NO_DISCARD TypeDescriptionStructure* ASTNodeManager::GetNodeDescription(const std::string& type_name) {
  return type_system_.Structure(type_name);
}

void ASTNodeManager::CreateAllDefinitions(std::ostream& out, const CodeGen& code_gen) const {
  // TODO: This should really go in CPP codegen, it is not specific to the types, but to codegen.

  MANTA_REQUIRE(node_enum_, "the node enum must be defined");
  MANTA_REQUIRE(node_base_, "the AST node base class must be defined");

  LOG_SEV(Info) << "";  // Log break.

  // Declare all node types.

  for (auto& [nonterminal_id, nonterminal_types] : node_types_for_nonterminal_) {
    // Write base type definition.
    if (nonterminal_types.base_type) {
      out << "struct " << code_gen.WriteName(nonterminal_types.base_type) << ";\n";
    }

    // Write all child types.
    for (auto& [name, type] : nonterminal_types.child_types) {
      LOG_SEV(Info) << "Generating definition for node type " << formatting::CLBB(name) << ".";
      out << "struct " << code_gen.WriteName(type) << ";\n";
    }
  }
  // Forward declare lexeme.
  out << "// Lexeme type.\n";
  out << "struct " << code_gen.WriteName(lexeme_node_) << ";\n";

  out << "\n";

  // Write the enum definitions.
  out << "//! \\brief Define the enums for each node type.\n//!\n";
  code_gen.WriteDefinition(out, node_enum_);
  out << "\n";

  // Write the node type enum's to_string function.
  out << "//! \\brief Define function to write enums.\n//!\n";
  out << "inline std::string to_string(" << node_enum_->GetName() << " type) {\n";
  out << "  switch (type) {\n";
  for (auto& option : node_enum_->GetOptions()) {
    out << "  case " << node_enum_->GetName() << "::" << option << ":\n";
    out << "    return \"" << option << "\";\n";
  }
  out << "  }\n}\n\n";

  // Write the visitor.
  out << "//! \\brief Base visitor class.\n//!\n";
  code_gen.WriteDefinition(out, visitor_type_);
  out << "\n\n";

  out << "//! \\brief Define the base node type for all AST nodes.\n//!\n";
  code_gen.WriteDefinition(out, node_base_);
  out << "\n\n";

  out << "//! \\brief Node for basic lexemes.\n//!\n";
  code_gen.WriteDefinition(out, lexeme_node_);
  out << "\n\n";

  for (auto& [nonterminal_id, nonterminal_types] : node_types_for_nonterminal_) {
    out << "// ============================================================\n";
    out << "//  Nodes for non-terminal " << nonterminal_id << "'s productions.\n";
    out << "// ============================================================\n\n";
    // Write base type definition.
    if (nonterminal_types.base_type) {
      LOG_SEV(Info) << "There was a parent class for nodes associated with non-terminal type "
                    << nonterminal_id << ", generating definition for node type '"
                    << nonterminal_types.base_type->type_name << "'.";
      // out << "//! \\brief Base node type for nodes associated with nonterminal " <<
      // nonterminal_id << " productions.\n//!\n"; code_gen.WriteDefinition(out,
      // nonterminal_types.base_type); out << "\n\n";
    }

    // Write all child types.
    for (auto& [name, type] : nonterminal_types.child_types) {
      LOG_SEV(Info) << "Generating definition for node type " << formatting::CLBB(name) << ".";
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

const std::map<NonterminalID, ASTNodeManager::NonterminalTypes>&
ASTNodeManager::GetAllNodeTypesForNonterminals() const {
  return node_types_for_nonterminal_;
}