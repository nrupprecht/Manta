//
// Created by Nathaniel Rupprecht on 5/18/23.
//

#include "manta/generator/typesystem/ASTNodeDescription.h"
// Other files.
#include "manta/utility/Formatting.h"

using namespace manta;

ASTNodeManager::ASTNodeManager() {
  // Create the NodeType enum.
  node_enum_ = type_system_.Enum("ASTNodeType");
  node_enum_->AddOption("Literal");

  nonterminal_enum_ = type_system_.Enum("NonterminalType");
  nonterminal_enum_->AddOption("Terminal");  // Actually not a non-terminal

  // In real code, this is actually a typedef for an integer.
  // TODO: Change to Integer type.
  item_id_type_ = type_system_.Structure("ItemID");

  // Create node base structure.
  node_base_ = type_system_.Structure("ASTNodeBase");
  node_base_->AddField("node_type", node_enum_);
  node_base_->AddField("nonterminal_type", nonterminal_enum_);
  node_base_->AddField("item_id", item_id_type_);
  node_base_->AddConstructor(StructureConstructor {
      // Constructor arguments
      {{node_enum_, "node_type"}, {nonterminal_enum_, "nonterminal_type"}, {item_id_type_, "item_id"}},
      // Parent constructors
      {},
      // List initialized fields.
      {{"node_type", "node_type"}, {"nonterminal_type", "nonterminal_type"}, {"item_id", "item_id"}},
      // Additional list initialized values
      {}});

  // Create lexeme node.
  lexeme_node_ = type_system_.Structure("ASTLexeme");
  lexeme_node_->AddField("literal", type_system_.String());
  lexeme_node_->AddParent(node_base_);
  lexeme_node_->AddConstructor(StructureConstructor {
      // Constructor arguments
      {{type_system_.String(), "literal"}},
      // Parent constructors
      {
          // Initialize the base class ASTNodeBase
          {node_base_,
           {StructureConstructor::Value {"ASTNodeType::Literal"},
            StructureConstructor::Value {"NonterminalType::Terminal"},
            StructureConstructor::Value {"-1"} /* To denote no item */}},
      },
      // List initialized args, (arg name, field name)
      {{"literal", "literal"}},
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

NO_DISCARD TypeDescriptionEnum* ASTNodeManager::GetNonterminalEnum() const {
  return nonterminal_enum_;
}

TypeDescriptionStructure* ASTNodeManager::GetItemID() const {
  return item_id_type_;
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
  // TODO: Remove C++ specific codegen.
  code_gen.AddComment(out, " Forward declare AST node types.");
  for (auto& [nonterminal_id, nonterminal_types] : node_types_for_nonterminal_) {

    // Write child types.
    for (auto& [name, type] : nonterminal_types.child_types) {
      LOG_SEV(Info) << "Generating definition for node type " << formatting::CLBB(name) << ".";
      out << "struct " << code_gen.WriteName(type) << ";\n";
    }
  }
  // Forward declare lexeme.
  code_gen.AddComment(out, " Lexeme type");
  out << "struct " << code_gen.WriteName(lexeme_node_) << ";\n";
  code_gen.AddBreak(out); // Break

  // Write the AST node type enum definitions.
  code_gen.AddComment(out, "! \\brief Define the enums for each node type.");
  code_gen.WriteDefinition(out, node_enum_);
  code_gen.AddBreak(out); // Break

  // Write the node type enum's to_string function.
  code_gen.AddComment(out, "! \\brief Define function to write AST node type enums.");
  code_gen.GenerateEnumToStringFunction(out, node_enum_);
  code_gen.AddBreak(out); // Break

  // Write the enum definitions.
  code_gen.AddComment(out, "! \\brief Define the enums for each non-terminal type.");
  code_gen.WriteDefinition(out, nonterminal_enum_);
  code_gen.AddBreak(out); // Break

  // Write the non-terminal type enum's to_string function.
  code_gen.AddComment(out, "! \\brief Define function to write the non-terminal type enum.");
  code_gen.GenerateEnumToStringFunction(out, nonterminal_enum_);
  code_gen.AddBreak(out); // Break

  // Write the visitor.
  code_gen.AddComment(out, "! \\brief Base visitor class.");
  code_gen.WriteDefinition(out, visitor_type_);
  code_gen.AddBreak(out); // Break

  code_gen.AddComment(out, "! \\brief Define the base node type for all AST nodes.");
  code_gen.WriteDefinition(out, node_base_);
  code_gen.AddBreak(out); // Break

  code_gen.AddComment(out, "! \\brief Node for basic lexemes.");
  code_gen.WriteDefinition(out, lexeme_node_);
  code_gen.AddBreak(out); // Break

  for (auto& [nonterminal_id, nonterminal_types] : node_types_for_nonterminal_) {
    code_gen.AddComment(out, " ============================================================");
    code_gen.AddComment(out,
                        "  Nodes for non-terminal " + std::to_string(nonterminal_id) + "'s productions.");
    code_gen.AddComment(out, " ============================================================");
    code_gen.AddBreak(out); // Break

    // Write base type definition.
    if (nonterminal_types.base_type) {
      LOG_SEV(Info) << "There was a parent class for nodes associated with non-terminal type "
                    << nonterminal_id << ", generating definition for node type '"
                    << nonterminal_types.base_type->type_name << "'.";
      code_gen.AddComment(out,
                          "! \\brief Parent type for non-terminal type " + std::to_string(nonterminal_id));
      
    }

    // Write all child types.
    for (auto& [name, type] : nonterminal_types.child_types) {
      LOG_SEV(Info) << "Generating definition for node type " << formatting::CLBB(name) << ".";
      code_gen.WriteDefinition(out, type);
      code_gen.AddBreak(out); // Break
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

TypeSystem& ASTNodeManager::GetTypeSystem() {
  return type_system_;
}
