//
// Created by Nathaniel Rupprecht on 7/24/23.
//

#include "manta/generator/typesystem/TypeSystem.h"
// Other files.

namespace manta {

std::string to_string(TSGeneralType type) {
  switch (type) {
    case TSGeneralType::Vector:
      return "Vector";
    case TSGeneralType::String:
      return "String";
    case TSGeneralType::SharedPointer:
      return "SharedPointer";
    case TSGeneralType::Structure:
      return "Structure";
    case TSGeneralType::Enumeration:
      return "Enumeration";
    default:
      MANTA_FAIL("unrecogmized TSGeneralType");
  }
}

// =======================================================================================
//  StructureConstructor
// =======================================================================================

StructureConstructor& StructureConstructor::WithArguments(
    std::vector<std::pair<const TypeDescription*, ArgName>> args) {
  arguments = std::move(args);
  return *this;
}

StructureConstructor& StructureConstructor::WithParentConstuctor(
    std::vector<std::pair<const TypeDescriptionStructure*, std::vector<std::variant<ArgName, Value>>>>
        constructors) {
  parent_constructors = std::move(constructors);
  return *this;
}

StructureConstructor& StructureConstructor::WithListInitializedArgs(
    std::vector<std::tuple<ArgName, FieldName>> list_init_args) {
  list_initialized_args = std::move(list_init_args);
  return *this;
}

StructureConstructor& StructureConstructor::WithAdditionalInitializations(
    std::vector<std::pair<FieldName, Value>> inits) {
  additional_initializations = std::move(inits);
  return *this;
}

// =======================================================================================
//  StructureConstructor
// =======================================================================================

TypeDescriptionStructure::TypeDescriptionStructure(std::string name)
    : type_name(std::move(name))
    , TypeDescription(TSGeneralType::Structure) {}

void TypeDescriptionStructure::AddField(const std::string& field_name, const TypeDescription* field_type) {
  if (auto it = fields.find(field_name); it == fields.end()) {
    // New field.
    fields[field_name] = field_type;
  }
  else {
    // Preexisting field. Make sure types match.
    MANTA_ASSERT(field_type->HashID() == it->second->HashID(),
                 "field " << type_name << "::" << field_name
                          << " specified multiple times, but types do not match. Type was "
                          << field_type->Write() << ", new type is " << it->second->Write());
    // Nothing to add, since it already exists.
  }
}

void TypeDescriptionStructure::AddParent(const TypeDescriptionStructure* parent) {
  parent_classes.insert(parent);
}

void TypeDescriptionStructure::AddConstructor(const StructureConstructor& constructor) {
  // Make sure that all referenced fields are actually fields of the structure.
  std::set<std::string> arg_names;
  for (auto& [_, arg_name] : constructor.arguments) {
    arg_names.insert(arg_name);
  }

  // TODO: The rest of the validation of the constructor.

  for (auto& [field, _] : constructor.additional_initializations) {
    MANTA_REQUIRE(fields.contains(field), "field '" << field << "' is not a field of " << Write());
  }
  constructors.push_back(constructor);
}

void TypeDescriptionStructure::AddFunction(const StructureFunction& function) {
  if (function.is_override) {
    // TODO: Check that the structure has a parent that has a virtual or override function matching this
    //  function.
  }
  functions.push_back(function);
}

std::string TypeDescriptionStructure::Write() const {
  // Default implementation, so we can still instantiate TypeDescription(enum) to get
  // the base hash.
  return "Structure:" + type_name;
}

std::size_t TypeDescriptionStructure::PotentialHashID(const std::string& type_name) {
  auto hash = TypeDescription(TSGeneralType::Vector).HashID();
  HashCombine(hash, type_name);
  return hash;
}

std::size_t TypeDescriptionStructure::HashStructure() const {
  auto hash = TypeDescription::HashStructure();
  for (auto& parent : parent_classes) {
    HashCombine(hash, parent->HashStructure());
  }
  for (auto& [name, type] : fields) {
    HashCombine(hash, name);
    HashCombine(hash, type->HashStructure());
  }
  return hash;
}

std::size_t TypeDescriptionStructure::HashID() const {
  auto hash = TypeDescription::HashID();
  HashCombine(hash, type_name);
  return hash;
}

// =======================================================================================
//  TypeDescriptionVector
// =======================================================================================

TypeDescriptionVector::TypeDescriptionVector(const TypeDescription* element_type)
    : BasicTypeDescription(TSGeneralType::Vector)
    , element_type(element_type) {}

std::string TypeDescriptionVector::Write() const {
  return to_string(general_type) + "<" + element_type->Write() + ">";
}

std::size_t TypeDescriptionVector::HashStructure() const {
  auto hash = TypeDescription::HashStructure();
  HashCombine(hash, element_type->HashStructure());
  return hash;
}

std::size_t TypeDescriptionVector::HashID() const {
  auto hash = TypeDescription::HashID();
  HashCombine(hash, element_type->HashID());
  return hash;
}

std::size_t TypeDescriptionVector::PotentialHashID(const TypeDescription* vector_type) {
  auto hash = TypeDescription(TSGeneralType::Vector).HashID();
  HashCombine(hash, vector_type->HashID());
  return hash;
}

// =======================================================================================
//  TypeDescriptionSharedPointer
// =======================================================================================

TypeDescriptionSharedPointer::TypeDescriptionSharedPointer(const TypeDescription* type)
    : BasicTypeDescription(TSGeneralType::SharedPointer)
    , pointed_type(type) {}

std::string TypeDescriptionSharedPointer::Write() const {
  return "SharedPointer<" + pointed_type->Write() + ">";
}

std::size_t TypeDescriptionSharedPointer::HashStructure() const {
  auto hash = TypeDescription::HashStructure();
  // If a type has a pointer to its own type and we use Hash instead of HashID,
  // then Hash() goes into an infinite recursion. Pointers are 'structurally different'
  // if they point to different types.
  HashCombine(hash, pointed_type->HashID());
  return hash;
}

std::size_t TypeDescriptionSharedPointer::HashID() const {
  auto hash = TypeDescription::HashID();
  HashCombine(hash, pointed_type->HashID());
  return hash;
}

std::size_t TypeDescriptionSharedPointer::PotentialHashID(const TypeDescription* pointed_type) {
  auto hash = TypeDescription(TSGeneralType::SharedPointer).HashID();
  HashCombine(hash, pointed_type->HashID());
  return hash;
}

// =======================================================================================
//  TypeDescriptionEnum
// =======================================================================================

TypeDescriptionEnum::TypeDescriptionEnum(const std::string& enum_name)
    : BasicTypeDescription(TSGeneralType::Enumeration)
    , enum_name_(enum_name) {
  MANTA_REQUIRE(!enum_name.empty(), "enumeration must have a name");
}

std::string TypeDescriptionEnum::Write() const {
  return "Enum:" + enum_name_;
}

std::size_t TypeDescriptionEnum::HashStructure() const {
  auto hash = TypeDescription::HashStructure();
  HashCombine(hash, enum_name_);
  return hash;
}

std::size_t TypeDescriptionEnum::HashID() const {
  return PotentialHashID(enum_name_);
}

std::size_t TypeDescriptionEnum::PotentialHashID(const std::string& name) {
  auto hash = TypeDescription(TSGeneralType::Enumeration).HashID();
  HashCombine(hash, name);
  return hash;
}

const std::string& TypeDescriptionEnum::GetName() const {
  return enum_name_;
}
void TypeDescriptionEnum::AddOption(const std::string& enum_option) {
  options_.insert(enum_option);
}

const std::set<std::string>& TypeDescriptionEnum::GetOptions() const {
  return options_;
}

// =======================================================================================
//  TypeSystem
// =======================================================================================

TypeDescriptionEnum* TypeSystem::Enum(const std::string& enum_name) {
  auto hash = TypeDescriptionEnum::PotentialHashID(enum_name);
  if (auto it = types_.find(hash); it != types_.end()) {
    return dynamic_cast<TypeDescriptionEnum*>(it->second.get());
  }
  auto ptr = std::make_shared<TypeDescriptionEnum>(enum_name);
  auto [it, _] = types_.emplace(hash, std::move(ptr));
  return dynamic_cast<TypeDescriptionEnum*>(it->second.get());
}

const TypeDescriptionSharedPointer* TypeSystem::SharedPointer(const TypeDescription* pointed_type) {
  auto hash = TypeDescriptionSharedPointer::PotentialHashID(pointed_type);
  if (auto it = types_.find(hash); it != types_.end()) {
    return dynamic_cast<const TypeDescriptionSharedPointer*>(it->second.get());
  }
  auto ptr = std::make_shared<TypeDescriptionSharedPointer>(pointed_type);
  auto [it, _] = types_.emplace(hash, std::move(ptr));
  return dynamic_cast<const TypeDescriptionSharedPointer*>(it->second.get());
}

const TypeDescriptionVector* TypeSystem::Vector(const TypeDescription* element_type) {
  auto hash = TypeDescriptionVector::PotentialHashID(element_type);
  if (auto it = types_.find(hash); it != types_.end()) {
    return dynamic_cast<const TypeDescriptionVector*>(it->second.get());
  }
  auto ptr = std::make_shared<TypeDescriptionVector>(element_type);
  auto [it, _] = types_.emplace(hash, std::move(ptr));
  return dynamic_cast<const TypeDescriptionVector*>(it->second.get());
}

TypeDescriptionStructure* TypeSystem::Structure(const std::string& type_name) {
  auto hash = TypeDescriptionStructure::PotentialHashID(type_name);
  if (auto it = types_.find(hash); it != types_.end()) {
    return dynamic_cast<TypeDescriptionStructure*>(it->second.get());
  }
  auto ptr = std::make_shared<TypeDescriptionStructure>(type_name);
  auto [it, _] = types_.emplace(hash, std::move(ptr));
  return dynamic_cast<TypeDescriptionStructure*>(it->second.get());
}

}  // namespace manta
