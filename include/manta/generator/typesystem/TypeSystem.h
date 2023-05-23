//
// Created by Nathaniel Rupprecht on 5/14/23.
//

#pragma once

#include "manta/utility/utility.hpp"
#include "manta/utility/Exceptions.h"
#include <utility>

namespace manta {

enum class ASTGeneralType {
  Vector, String, SharedPointer, Structure, Enumeration
};

inline std::string to_string(ASTGeneralType type) {
  switch (type) {
    case ASTGeneralType::Vector: {
      return "Vector";
    }
    case ASTGeneralType::String: {
      return "String";
    }
    case ASTGeneralType::SharedPointer: {
      return "SharedPointer";
    }
    case ASTGeneralType::Structure: {
      return "Structure";
    }
    case ASTGeneralType::Enumeration: {
      return "Enumeration";
    }
  }
}

//! \brief Base class for the possible types of member variables in an AST node.
struct TypeDescription {
  explicit TypeDescription(ASTGeneralType type)
      : general_type(type) {}

  //! \brief The general type, whether this is a structure, or one of the built in types.
  ASTGeneralType general_type;

  //! \brief  Write a description of the type. Note that this is independent of code generation,
  //!         this is for reporting, error, and debugging purposes.
  NO_DISCARD virtual std::string Write() const {
    // Default implementation, so we can still instantiate TypeDescription(enum) to get the base hash.
    return to_string(general_type);
  }

  //! \brief Hash a description of the type, this can be used to determine if types are structurally equal.
  NO_DISCARD virtual std::size_t HashStructure() const {
    return static_cast<std::size_t>(general_type);
  }

  //! \brief Hash the identifier of the type. Types with different names but the same structure (like nodes)
  //! can return different HashIDs, while, conversely, as we modify the type layout of a node type, the HashID
  //! will not change.
  NO_DISCARD virtual std::size_t HashID() const {
    return static_cast<std::size_t>(general_type);
  }
};

//! \brief  Type description for a "basic" type. Basic types will be mapped to types in target programming languages
//!         in a language dependent way.
//!
struct BasicTypeDescription : public TypeDescription {
  explicit BasicTypeDescription(ASTGeneralType type) : TypeDescription(type) {}
};

class TypeDescriptionStructure;

//! \brief  Helper structure that defines a simple constructor for a TypeDescriptionStructure.
//!
struct StructureConstructor {
  using ArgName = std::string;
  using FieldName = std::string;

  //! \brief  Represents an argument value.
  //!
  //!         NOTE: This breaks the separation between abstract structure and codegen (e.g. by using C++ style
  //!         syntax for enums), but I am allowing it for now.
  struct Value {
    std::string literal;
  };

  std::vector<std::pair<const TypeDescription*, ArgName>> arguments{};

  //! \brief  Call parent constructors.
  std::vector<
      // Description of how to call a single constructor. Can call either with arguments, or with specific values.
      std::pair<const TypeDescriptionStructure*, std::vector<std::variant<ArgName, Value>>>
  > parent_constructors{};

  //! \brief  Triples of the argument type description, name of the constructor argument, and what field it initializes.
  //!
  std::vector<std::tuple<ArgName, FieldName>> list_initialized_args{};

  //! \brief  Additional fields, values to initialize in addition to the arguments.
  //!

  //!
  std::vector<std::pair<FieldName, Value>> additional_initializations{};
};

//! \brief  Type description for a structure, or compound type.
//!
struct TypeDescriptionStructure : public TypeDescription {
  explicit TypeDescriptionStructure(std::string name)
      : type_name(std::move(name)), TypeDescription(ASTGeneralType::Structure) {}

  void AddField(const std::string& field_name, const TypeDescription* field_type) {
    if (auto it = fields.find(field_name); it == fields.end()) {
      // New field.
      fields[field_name] = field_type;
    }
    else {
      // Preexisting field. Make sure types match.
      MANTA_ASSERT(
          field_type->HashID() == it->second->HashID(),
          "field " << type_name << "::" << field_name << " specified multiple times, but types do not match. Type was "
                   << field_type->Write() << ", new type is " << it->second->Write());
      // Nothing to add, since it already exists.
    }
  }

  void AddParent(const TypeDescriptionStructure* parent) {
    parent_classes.insert(parent);
  }

  void AddConstructor(const StructureConstructor& constructor) {
    // Make sure that all referenced fields are actually fields of the structure.
    std::set<std::string> arg_names;
    for (auto&[_, arg_name]: constructor.arguments) {
      arg_names.insert(arg_name);
    }

    // TODO: The rest of the validation of the constructor.

    for (auto&[field, _]: constructor.additional_initializations) {
      MANTA_REQUIRE(fields.contains(field), "field '" << field << "' is not a field of " << Write());
    }
    constructors.push_back(constructor);
  }

  NO_DISCARD std::string Write() const override {
    // Default implementation, so we can still instantiate TypeDescription(enum) to get the base hash.
    return "Structure:" + type_name;
  }

  static std::size_t PotentialHashID(const std::string& type_name) {
    auto hash = TypeDescription(ASTGeneralType::Vector).HashID();
    HashCombine(hash, type_name);
    return hash;
  }

  NO_DISCARD std::size_t HashStructure() const override {
    auto hash = TypeDescription::HashStructure();
    for (auto& parent: parent_classes) {
      HashCombine(hash, parent->HashStructure());
    }
    for (auto&[name, type]: fields) {
      HashCombine(hash, name);
      HashCombine(hash, type->HashStructure());
    }
    return hash;
  }

  //! \brief Nodes are classes, so their ID is just their name.
  NO_DISCARD std::size_t HashID() const override {
    auto hash = TypeDescription::HashID();
    HashCombine(hash, type_name);
    return hash;
  }

  //! \brief The name of the type.
  const std::string type_name;

  //! \brief The fields of the structure.
  std::map<std::string, const TypeDescription*> fields;

  //! \brief The set of base classes of the structure.
  std::set<const TypeDescriptionStructure*> parent_classes;

  //! \brief Constructors for the structure.
  std::vector<StructureConstructor> constructors;
};

//! \brief Represents a vector or ordered collection of objects.
struct TypeDescriptionVector : public BasicTypeDescription {
  explicit TypeDescriptionVector(const TypeDescription* element_type)
      : BasicTypeDescription(ASTGeneralType::Vector), element_type(element_type) {}

  const TypeDescription* element_type{};

  NO_DISCARD std::string Write() const override {
    return to_string(general_type) + "<" + element_type->Write() + ">";
  }

  NO_DISCARD std::size_t HashStructure() const override {
    auto hash = TypeDescription::HashStructure();
    HashCombine(hash, element_type->HashStructure());
    return hash;
  }

  NO_DISCARD std::size_t HashID() const override {
    auto hash = TypeDescription::HashID();
    HashCombine(hash, element_type->HashID());
    return hash;
  }

  static std::size_t PotentialHashID(const TypeDescription* vector_type) {
    auto hash = TypeDescription(ASTGeneralType::Vector).HashID();
    HashCombine(hash, vector_type->HashID());
    return hash;
  }
};

//! \brief Represents a shared pointer of some type.
struct TypeDescriptionSharedPointer : public BasicTypeDescription {
  explicit TypeDescriptionSharedPointer(const TypeDescription* type)
      : BasicTypeDescription(ASTGeneralType::SharedPointer), pointed_type(type) {}

  const TypeDescription* pointed_type{};

  NO_DISCARD std::string Write() const override {
    return "SharedPointer<" + pointed_type->Write() + ">";
  }

  NO_DISCARD std::size_t HashStructure() const override {
    auto hash = TypeDescription::HashStructure();
    // If a type has a pointer to its own type and we use Hash instead of HashID,
    // then Hash() goes into an infinite recursion. Pointers are 'structurally different'
    // if they point to different types.
    HashCombine(hash, pointed_type->HashID());
    return hash;
  }

  NO_DISCARD std::size_t HashID() const override {
    auto hash = TypeDescription::HashID();
    HashCombine(hash, pointed_type->HashID());
    return hash;
  }

  static std::size_t PotentialHashID(const TypeDescription* pointed_type) {
    auto hash = TypeDescription(ASTGeneralType::SharedPointer).HashID();
    HashCombine(hash, pointed_type->HashID());
    return hash;
  }
};

//! \brief Represents an enumeration type.
struct TypeDescriptionEnum : public BasicTypeDescription {
  explicit TypeDescriptionEnum(const std::string& enum_name)
      : BasicTypeDescription(ASTGeneralType::Enumeration), enum_name_(enum_name) {
    MANTA_REQUIRE(!enum_name.empty(), "enumeration must have a name");
  }

  NO_DISCARD std::string Write() const override {
    return "Enum:" + enum_name_;
  }

  NO_DISCARD std::size_t HashStructure() const override {
    auto hash = TypeDescription::HashStructure();
    HashCombine(hash, enum_name_);
    return hash;
  }

  NO_DISCARD std::size_t HashID() const override {
    return PotentialHashID(enum_name_);
  }

  static std::size_t PotentialHashID(const std::string& name) {
    auto hash = TypeDescription(ASTGeneralType::Enumeration).HashID();
    HashCombine(hash, name);
    return hash;
  }

  NO_DISCARD const std::string& GetName() const {
    return enum_name_;
  }

  void AddOption(const std::string& enum_option) {
    options_.insert(enum_option);
  }

  NO_DISCARD const std::set<std::string>& GetOptions() const {
    return options_;
  }

 private:
  const std::string enum_name_;

  std::set<std::string> options_;
};

//! \brief Represents a string field.
struct TypeDescriptionString : public TypeDescription {
  TypeDescriptionString() : TypeDescription(ASTGeneralType::String) {}
};

class TypeSystem {
 public:

  //! \brief Get the string type.
  NO_DISCARD const TypeDescriptionString* String() const {
    return &string_type_;
  }

  TypeDescriptionEnum* Enum(const std::string& enum_name) {
    auto hash = TypeDescriptionEnum::PotentialHashID(enum_name);
    if (auto it = types_.find(hash); it != types_.end()) {
      return dynamic_cast<TypeDescriptionEnum*>(it->second.get());
    }
    auto ptr = std::make_shared<TypeDescriptionEnum>(enum_name);
    auto[it, _] = types_.emplace(hash, std::move(ptr));
    return dynamic_cast<TypeDescriptionEnum*>(it->second.get());
  }

  //! \brief Get a shared pointer type.
  const TypeDescriptionSharedPointer* SharedPointer(const TypeDescription* pointed_type) {
    auto hash = TypeDescriptionSharedPointer::PotentialHashID(pointed_type);
    if (auto it = types_.find(hash); it != types_.end()) {
      return dynamic_cast<const TypeDescriptionSharedPointer*>(it->second.get());
    }
    auto ptr = std::make_shared<TypeDescriptionSharedPointer>(pointed_type);
    auto[it, _] = types_.emplace(hash, std::move(ptr));
    return dynamic_cast<const TypeDescriptionSharedPointer*>(it->second.get());
  }

  //! \brief Get a vector type.
  const TypeDescriptionVector* Vector(const TypeDescription* element_type) {
    auto hash = TypeDescriptionVector::PotentialHashID(element_type);
    if (auto it = types_.find(hash); it != types_.end()) {
      return dynamic_cast<const TypeDescriptionVector*>(it->second.get());
    }
    auto ptr = std::make_shared<TypeDescriptionVector>(element_type);
    auto[it, _] = types_.emplace(hash, std::move(ptr));
    return dynamic_cast<const TypeDescriptionVector*>(it->second.get());
  }

  //! \brief Get a structure type.
  TypeDescriptionStructure* Structure(const std::string& type_name) {
    auto hash = TypeDescriptionStructure::PotentialHashID(type_name);
    if (auto it = types_.find(hash); it != types_.end()) {
      return dynamic_cast<TypeDescriptionStructure*>(it->second.get());
    }
    auto ptr = std::make_shared<TypeDescriptionStructure>(type_name);
    auto[it, _] = types_.emplace(hash, std::move(ptr));
    return dynamic_cast<TypeDescriptionStructure*>(it->second.get());
  }

 private:

  //! \brief The string type.
  const TypeDescriptionString string_type_;

  //! \brief All types managed by the typesystem.
  std::map<std::size_t, std::shared_ptr<TypeDescription>> types_;
};

} // namespace manta
