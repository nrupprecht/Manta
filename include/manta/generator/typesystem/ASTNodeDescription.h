#pragma once

#include <utility>

#include "manta/utility/utility.hpp"
#include "manta/utility/Exceptions.h"

namespace manta {
using NonterminalID = int;
}

namespace {

std::string toUpper(std::string str) {
  std::for_each(str.begin(), str.end(), [](auto& c) {
    if (isalpha(c)) c = std::toupper(c);
  });
  return str;
}

} // namespace <unnamed>

namespace manta {

using ItemID = unsigned;

enum class ASTGeneralType {
  Vector, String, Node, SharedPointer
};

//! \brief Base class for the possible types of member variables in an AST node.
struct ASTType {
  explicit ASTType(ASTGeneralType type) : general_type(type) {}
  ASTGeneralType general_type;

  NO_DISCARD virtual std::string Write() const { return ""; } // So we can still instantiate ASTType(enum) to get the base hash.

  //! \brief Hash a description of the type, this can be used to determine if types are structurally equal.
  NO_DISCARD virtual std::size_t Hash() const {
    return static_cast<std::size_t>(general_type);
  }

  //! \brief Hash the identifier of the type. Types with different names but the same structure (like nodes)
  //! can return different HashIDs, while, conversely, as we modify the type layout of a node type, the HashID
  //! will not change.
  NO_DISCARD virtual std::size_t HashID() const {
    return static_cast<std::size_t>(general_type);
  }
};

//! \brief Represents a vector field of some type.
struct ASTTypeVector : public ASTType {
  explicit ASTTypeVector(const ASTType* type)
      : ASTType(ASTGeneralType::Vector), vector_type(type) {}

  const ASTType* vector_type{};

  NO_DISCARD std::string Write() const override {
    return "std::vector<" + vector_type->Write() + ">";
  }

  NO_DISCARD std::size_t Hash() const override {
    auto hash = ASTType::Hash();
    HashCombine(hash, vector_type->Hash());
    return hash;
  }

  std::size_t HashID() const override {
    auto hash = ASTType::Hash();
    HashCombine(hash, vector_type->HashID());
    return hash;
  }

  static std::size_t PotentialHashID(const ASTType* vector_type) {
    auto hash = ASTType(ASTGeneralType::Vector).HashID();
    HashCombine(hash, vector_type->HashID());
    return hash;
  }
};

//! \brief Represents a shared pointer of some type.
struct ASTTypeSharedPointer : public ASTType {
  explicit ASTTypeSharedPointer(const ASTType* type)
      : ASTType(ASTGeneralType::SharedPointer), pointed_type(type) {}

  const ASTType* pointed_type{};

  NO_DISCARD std::string Write() const override {
    return "std::shared_ptr<" + pointed_type->Write() + ">";
  }

  NO_DISCARD std::size_t Hash() const override {
    auto hash = ASTType::Hash();
    // If a type has a pointer to its own type and we use Hash instead of HashID,
    // then Hash() goes into an infinite recursion. Pointers are 'structurally different'
    // if they point to different types.
    HashCombine(hash, pointed_type->HashID());
    return hash;
  }

  std::size_t HashID() const override {
    return Hash();
  }

  static std::size_t PotentialHashID(const ASTType* pointed_type) {
    auto hash = ASTType(ASTGeneralType::SharedPointer).HashID();
    HashCombine(hash, pointed_type->HashID());
    return hash;
  }
};

//! \brief Represents a string field.
struct ASTTypeString : public ASTType {
  ASTTypeString() : ASTType(ASTGeneralType::String) {}

  NO_DISCARD std::string Write() const override { return "std::string"; }
};

//! \brief A description of the node type to create. This is itself a type, the type of this kind of AST node.
struct ASTNodeDescription : public ASTType {
  ASTNodeDescription(std::string node_type_name)
      : node_type_name(std::move(node_type_name)), ASTType(ASTGeneralType::Node) {}

  std::string node_type_name{};

  std::map<std::string, const ASTType*> members_{};

  std::set<ASTNodeDescription*> parent_classes{};

  NO_DISCARD std::string Write() const override {
    return node_type_name;
  }

  void AddField(const std::string& field_name, const ASTType* field_type) {
    if (auto it = members_.find(field_name); it == members_.end()) {
      // New field.
      members_[field_name] = field_type;
    }
    else {
      // Preexisting field. Make sure types match.
      MANTA_ASSERT(
          field_type->Hash() == it->second->Hash(),
          "field " << node_type_name << "::" << field_name << " specified multiple times, but types do not match. Type was "
                   << field_type->Write() << ", new type is " << it->second->Write());
      // Nothing to add, since it already exists.
    }
  }

  NO_DISCARD std::size_t Hash() const override {
    auto hash = ASTType::Hash();
    HashCombine(hash, node_type_name);
    for (auto& parent: parent_classes) {
      HashCombine(hash, parent->Hash());
    }
    for (auto&[name, type]: members_) {
      HashCombine(hash, name);
      HashCombine(hash, type->Hash());
    }
    return hash;
  }

  //! \brief Nodes are classes, so their ID is just their name.
  std::size_t HashID() const override {
    auto hash = ASTType::Hash();
    HashCombine(hash, node_type_name);
    return hash;
  }
};

//! \brief Class that keeps track of and owns the data for all types.
//!
//! This class also knows how to generate C++ code representing the nodes, but you can imagine that this class could
//! be left as just an IR of all the nodes, and separate classes could do the actual codegen into a target language.
//!
class ASTNodeManager {
 public:
  ASTNodeManager() {
    // String type is always present.
    auto str_type = std::make_shared<ASTTypeString>();
    add(str_type);
  }

  ASTNodeDescription* GetNodeDescription(const std::string& type_name, unsigned nonterminal_id) {
    // If the
    auto& types = node_types_for_nonterminal_[nonterminal_id];
    if (auto it = types.find(type_name); it != types.end()) {
      return it->second;
    }
    else {
      auto new_entry = std::make_shared<ASTNodeDescription>(type_name);
      add(new_entry);
      type_descriptions_[type_name] = new_entry.get();
      types[type_name] = new_entry.get();
      return new_entry.get();
    }
  }

  NO_DISCARD ASTNodeDescription* GetNodeDescription(const std::string& type_name) const {
    if (auto it = type_descriptions_.find(type_name); it != type_descriptions_.end()) {
      return it->second;
    }
    return nullptr;
  }

  //! \brief Write the C++ code for all AST node definitions to an ostream.
  void CreateAllDefinitions(std::ostream& out) const {
    out << "//! \\brief Define the enums for each node type.\n//!\n";
    out << "enum class ASTNodeType {\n";
    out << "  // Node for basic lexemes.\n";
    out << "  AST_Lexeme,\n";
    for (auto&[nonterminal_id, types_for_nonterminal]: node_types_for_nonterminal_) {
      out << "  // Enums for productions associated with non-terminal " << nonterminal_id << ".\n";
      for (auto&[_, description]: types_for_nonterminal) {
        out << "  " << description->node_type_name << ",\n";
      }
    }
    out << "};\n\n";

    // Write definition of ASTNodeBase
    out << "//! \\brief The base class for all AST nodes.\n";
    out << "//! \n";
    out << "struct ASTNodeBase {\n";
    out << "  explicit ASTNodeBase(ASTNodeType node_type) : node_type(node_type) {}\n\n";
    out << "  const ASTNodeType node_type;\n";
    // TODO: Add visitor pattern function or other additional members?
    out << "};\n\n";

    // Add node for terminals
    out << "//! \\brief Node type for lexemes. Just contains the literal.\n";
    out << "//! \n";
    out << "struct ASTLexeme : public ASTNodeBase {\n";
    out << "  ASTLexeme() : ASTNodeBase(ASTNodeType::AST_Lexeme) {}\n\n";
    out << "  const std::string literal;\n";
    out << "};\n\n";

    // Forward declare all structures.
    out << "// ============================================================================\n";
    out << "//  Forward declare all AST node structures.\n";
    out << "// ============================================================================\n\n";
    for (auto&[nonterminal_id, types_for_nonterminal]: node_types_for_nonterminal_) {
      for (auto&[_, description]: types_for_nonterminal) {
        out << "struct " << description->node_type_name << ";\n";
      }
    }
    out << "\n";

    for (auto&[nonterminal_id, types_for_nonterminal]: node_types_for_nonterminal_) {
      out << "// ============================================================================\n";
      out << "//  AST nodes from reduction of non-terminal " << nonterminal_id << ".\n";
      out << "// ============================================================================\n\n";

      for (auto&[_, description]: types_for_nonterminal) {
        writeNodeTypeDeclaration(out, description);
      }
    }
  }

  NO_DISCARD ASTType* GetStringType() const {
    return all_types_.at(ASTTypeString{}.HashID()).get();
  }

  ASTType* MakeShared(const ASTType* pointed_type) {
    auto potential_hash_id = ASTTypeSharedPointer::PotentialHashID(pointed_type);
    if (auto it = all_types_.find(potential_hash_id); it != all_types_.end()) {
      return it->second.get();
    }
    auto new_type = std::make_shared<ASTTypeSharedPointer>(pointed_type);
    MANTA_ASSERT(potential_hash_id == new_type->HashID(), "error in the potential-hash-ID function");
    all_types_[potential_hash_id] = new_type;
    return new_type.get();
  }

  ASTType* MakeVector(const ASTType* vector_type) {
    auto potential_hash_id = ASTTypeVector::PotentialHashID(vector_type);
    if (auto it = all_types_.find(potential_hash_id); it != all_types_.end()) {
      return it->second.get();
    }
    auto new_type = std::make_shared<ASTTypeVector>(vector_type);
    MANTA_ASSERT(potential_hash_id == new_type->HashID(), "error in the potential-hash-ID function");
    all_types_[potential_hash_id] = new_type;
    return new_type.get();
  }

  // private:
  void add(const std::shared_ptr<ASTType>& type) {
    all_types_[type->HashID()] = type;
  }

  void writeNodeTypeDeclaration(std::ostream& out, const ASTNodeDescription* description) const {
    out << "struct " << description->node_type_name << " : ";

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
        out << "public " << parent->node_type_name;
        count += 1;
      }
    }
    out << " {\n";

    // Initialize with the correct enum.
    out << "  //! \\brief Default constructor for " << description->node_type_name << ".\n  //!\n";
    out << "  " << description->node_type_name << "()\n";
    if (description->parent_classes.empty()) {
      out << "    : ASTNodeBase(ASTNodeType::" << description->node_type_name << ") {}\n\n";

      // Add second constructor, for passing the node subtype.
      out << "  //! \\brief Constructor to pass a sub-node-type up to the base.\n  //!\n";
      out << "  explicit " << description->node_type_name << "(ASTNodeType subtype)\n";
      out << "    : ASTNodeBase(subtype) {}\n\n";
    }
    else {
      // This is not general. I am assuming that there is one parent node, and it has the correct constructor.
      // TODO: Introduce enum subtypes for the node subtypes?
      out << "    : " << (*description->parent_classes.begin())->node_type_name << "(ASTNodeType::" << description->node_type_name << ") {}\n\n";
    }

    // Define all fields.
    for (auto[field_name, field_description]: description->members_) {
      // Write type.
      out << "  ";
      out << field_description->Write();
      out << " " << field_name << "{};\n";
    }
    out << "};\n\n";
  }

  //! \brief Map from type name of a type to the description of that type.
  std::map<std::string, ASTNodeDescription*> type_descriptions_{};

  // Map from non-terminal to the set of node types that productions for this node can product.
  std::map<NonterminalID, std::map<std::string, ASTNodeDescription*>> node_types_for_nonterminal_{};

  std::map<std::size_t, std::shared_ptr<ASTType>> all_types_;
};

} // namespace manta