//
// Created by Nathaniel Rupprecht on 12/22/24.
//

#include <algorithm>
#include <numeric>
#include <ranges>

#include "manta/generator/typesystem/HigherTypeSystem.h"

#include "manta/utility/utility.hpp"

namespace manta::typesystem {

namespace {

std::string write(const std::string& name, const std::size_t num_type_vars) {
  if (num_type_vars == 0) {
    return name;
  }
  return std::format("{}[*{}]", name, num_type_vars);
}

}  // namespace

std::string_view to_string(PrototypeType type) {
  switch (type) {
    case PrototypeType::ENUM:
      return "ENUM";
    case PrototypeType::BASIC:
      return "BASIC";
    case PrototypeType::OBJECT:
      return "OBJECT";
    case PrototypeType::FUNCTION:
      return "FUNCTION";
    default:
      MANTA_FAIL("unrecognized PrototypeType");
  }
}

std::size_t ElaboratedType::Hash() const {
  auto hash = type->Hash();
  HashCombine(hash, is_const);
  HashCombine(hash, is_ref);
  return hash;
}

ElaboratedType& ElaboratedType::TypeBind(std::vector<std::size_t> bind) {
  MANTA_REQUIRE(type, "a type must have been set");
  MANTA_REQUIRE(type->GetNumFreeVars() == bind.size(),
                "number of bindings must match number of free variables");
  bindings = std::move(bind);
  return *this;
}

// ================================================================================================
//  BasicPrototype
// ================================================================================================

std::string BasicPrototype::Write() const {
  return write(name_, num_type_vars_);
}

std::string BasicPrototype::Write(const SubstitutionCallback& substitution_callback,
                                  std::span<std::size_t> vars) const {
  std::ostringstream out;
  out << name_;
  if (0 < num_type_vars_) {
    out << "<";
    for (std::size_t i = 0; i < num_type_vars_; ++i) {
      substitution_callback(out, i, vars);
      if (i + 1 < num_type_vars_) {
        out << ", ";
      }
    }
    out << ">";
  }
  return out.str();
}

std::size_t BasicPrototype::Hash() const {
  return std::hash<std::string> {}(name_);
}

void BasicPrototype::Accept(ProtoypeVisitor& visitor) const {
  visitor.Visit(*this);
}

// ================================================================================================
//  ObjectPrototype
// ================================================================================================

ObjectPrototype::ObjectPrototype(std::string name, std::size_t num_type_vars)
    : Prototype(num_type_vars, PrototypeType::OBJECT)
    , object_name_(std::move(name)) {}

std::string ObjectPrototype::Write() const {
  return "object " + write(object_name_, num_type_vars_);
}

std::string ObjectPrototype::Write(const SubstitutionCallback& substitution_callback,
                                   std::span<std::size_t> vars) const {
  std::ostringstream out;
  out << "object " << object_name_;
  if (0 < num_type_vars_) {
    out << "<";
    for (std::size_t i = 0; i < num_type_vars_; ++i) {
      substitution_callback(out, i, vars);
      if (i + 1 < num_type_vars_) {
        out << ", ";
      }
    }
    out << ">";
  }
  return out.str();
}

std::size_t ObjectPrototype::Hash() const {
  return std::hash<std::string> {}(object_name_);
}

void ObjectPrototype::Accept(ProtoypeVisitor& visitor) const {
  visitor.Visit(*this);
}

void ObjectPrototype::AddField(const std::string& field_name,
                               const GeneralizedType* field_type,
                               std::vector<std::size_t> binding) {
  fields_.emplace_back(Field {field_name, field_type, binding});
}

bool ObjectPrototype::RemoveField(const std::string& field_name) {
  auto it = std::ranges::find_if(fields_,
                                 [field_name](const Field& field) { return field.field_name == field_name; });
  if (it == fields_.end()) {
    return false;
  }
  fields_.erase(it);
  return true;
}

void ObjectPrototype::AddParent(const GeneralizedType* parent) {
  MANTA_REQUIRE(parent->GetPrototypeType() == PrototypeType::OBJECT, "parent must be an object");
  auto it = std::ranges::find(parent_types_, parent);
  if (it == parent_types_.end()) {
    parent_types_.push_back(parent);
  }
}

void ObjectPrototype::AddConstructor(const StructureConstructor& constructor) {
  // Make sure that all referenced fields are actually fields of the structure.
  std::set<std::string> arg_names;
  for (auto& [_, arg_name] : constructor.arguments) {
    arg_names.insert(arg_name);
  }

  // TODO: The rest of the validation of the constructor.

  for (auto& [field, _] : constructor.additional_initializations) {
    MANTA_REQUIRE(HasField(field), "field '" << field << "' is not a field of " << Write());
  }
  constructors_.push_back(constructor);
}

StructureFunction& ObjectPrototype::AddFunction(const StructureFunction& function) {
  functions_.push_back(function);
  return functions_.back();
}

bool ObjectPrototype::HasField(const std::string& name) const {
  return std::ranges::find_if(fields_, [name](const Field& field) { return field.field_name == name; })
      != fields_.end();
}

const ObjectPrototype::Field* ObjectPrototype::GetField(const std::string& name) const {
  auto it = std::ranges::find_if(fields_, [name](const Field& field) { return field.field_name == name; });
  if (it == fields_.end()) {
    return nullptr;
  }
  return &*it;
}

const std::vector<ObjectPrototype::Field>& ObjectPrototype::GetFields() const {
  return fields_;
}

const std::deque<StructureConstructor>& ObjectPrototype::GetConstructors() const {
  return constructors_;
}

const std::deque<StructureFunction>& ObjectPrototype::GetFunctions() const {
  return functions_;
}

const std::string& ObjectPrototype::GetAdhocCode() const {
  return adhoc_code_;
}

// ================================================================================================
//  EnumeratedPrototype
// ================================================================================================

std::string EnumeratedPrototype::Write() const {
  return "enum " + write(enum_name_, num_type_vars_);
}

std::string EnumeratedPrototype::Write(const SubstitutionCallback& substitution_callback,
                                       std::span<std::size_t> vars) const {
  std::ostringstream out;
  out << "enum " << enum_name_;
  if (0 < num_type_vars_) {
    out << "<";
    for (std::size_t i = 0; i < num_type_vars_; ++i) {
      substitution_callback(out, i, vars);
      if (i + 1 < num_type_vars_) {
        out << ", ";
      }
    }
    out << ">";
  }
  return out.str();
}

std::size_t EnumeratedPrototype::Hash() const {
  return std::hash<std::string> {}(enum_name_);
}

void EnumeratedPrototype::Accept(ProtoypeVisitor& visitor) const {
  visitor.Visit(*this);
}

std::string FunctionPrototype::Write() const {
  std::string output = "func ";
  if (0 < num_type_vars_) {
    output += std::format("[*{}] ", num_type_vars_);
  }
  for (size_t i = 0; i < types_.size(); ++i) {
    if (i != 0) {
      output += " -> ";
    }
    output += types_[i].type->Write();
  }
  return output;
}

std::string FunctionPrototype::Write(const SubstitutionCallback& substitution_callback,
                                     std::span<std::size_t> vars) const {
  std::ostringstream out;
  out << "func ";
  if (0 < num_type_vars_) {
    out << std::format("[*{}] ", num_type_vars_);
  }

  auto parenthesize_functions = [](auto& out, const auto& type, std::span<std::size_t> vars = {}) {
    if (type.GetPrototypeType() == PrototypeType::FUNCTION) {
      out << "(" << type.Write(vars) << ")";
      return;
    }
    out << type.Write(vars);
  };

  for (size_t i = 0; i < types_.size(); ++i) {
    auto& type = types_[i];
    if (i != 0) {
      out << " -> ";
    }
    if (type.type->IsType()) {
      if (type.is_const) out << "const ";
      if (type.is_ref) out << "&";
      parenthesize_functions(out, *type.type);
    }
    else {
      if (type.is_const) out << "const ";
      if (type.is_ref) out << "&";

      std::vector<std::size_t> indices;
      indices.reserve(type.bindings.size());
      for (auto binding : type.bindings) {
        indices.push_back(binding);
      }
      parenthesize_functions(out, *type.type, indices);
    }
  }
  return out.str();
}

std::size_t FunctionPrototype::Hash() const {
  std::size_t hash = 12;
  for (const auto& arg : types_) {
    HashCombine(hash, arg.Hash());
  }
  return hash;
}

void FunctionPrototype::Accept(ProtoypeVisitor& visitor) const {
  visitor.Visit(*this);
}

// =======================================================================================
//  StructureConstructor
// =======================================================================================

StructureConstructor& StructureConstructor::WithArguments(
    std::vector<std::pair<ElaboratedType, ArgName>> args) {
  arguments = std::move(args);
  return *this;
}

StructureConstructor& StructureConstructor::WithParentConstuctor(
    std::vector<std::pair<const GeneralizedType*, std::vector<std::variant<ArgName, Value>>>> constructors) {
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

StructureConstructor& StructureConstructor::WithBody(std::string_view body) {
  function_body = body;
  return *this;
}

// =======================================================================================
//  GeneralizedType
// =======================================================================================

std::string GeneralizedType::Write(std::span<std::size_t> vars) const {
  MANTA_REQUIRE(vars.empty() || vars.size() == GetNumFreeVars(),
                "input vars must be none or match the number of free vars, vars.size() = "
                    << vars.size() << ", GetNumFreeVars() = " << GetNumFreeVars());
  auto substitution_callback = std::bind_front(&GeneralizedType::writeNthSubstitution, this);
  std::vector<std::size_t> vars_in;
  if (vars.empty()) {
    vars_in.resize(GetNumFreeVars());
    // TODO: Have to change when a free var can map to multiple things in the substitution layer.
    std::iota(vars_in.begin(), vars_in.end(), 0);
  }
  if (vars.empty()) {
    vars = vars_in;
  }
  return prototype_->Write(substitution_callback, vars);
}

std::size_t GeneralizedType::Hash() const {
  std::size_t hash  = prototype_->Hash();
  std::size_t index = 38112;
  for (const auto& type : substituted_vars_) {
    if (!type) {
      HashCombine(hash, index);
    }
    else {
      HashCombine(hash, type->Hash());
    }
    index += 42;
  }
  for (const auto& mapping : free_vars_) {
    HashCombine(hash, mapping.Hash());
  }
  return hash;
}

void GeneralizedType::write(std::ostream& out, std::span<std::size_t> vars_in) const {
  // TODO.
  auto substitution_callback = std::bind_front(&GeneralizedType::writeNthSubstitution, this);
  out << prototype_->Write(substitution_callback, vars_in);
}

void GeneralizedType::writeNthSubstitution(std::ostream& out,
                                           std::size_t index,
                                           std::span<std::size_t> vars) const {
  // Get number of free var slots in the index-th entry of the substitution layer.
  std::size_t num_free_vars = 1;
  if (const auto type = substituted_vars_[index]) {
    num_free_vars = type->GetNumFreeVars();
  }
  std::vector vars_in(num_free_vars, std::numeric_limits<std::size_t>::max());

  std::size_t free_index = 0;
  for (auto& mappings : free_vars_) {
    for (auto& [subs_idx, sub_idx] : mappings.mapping) {
      if (subs_idx == index) {
        vars_in[sub_idx] = free_index;
      }
    }
    ++free_index;
  }

  if (substituted_vars_[index] == nullptr) {
    if (vars.empty()) {
      out << "$" << index;
    }
    else {
      out << "$" << vars[index];
    }
  }
  else {
    substituted_vars_[index]->write(out, vars_in);
  }
}

std::size_t GeneralizedType::VarMapping::Hash() const {
  std::size_t hash = 0;
  for (auto [idx1, idx2] : mapping) {
    HashCombine(hash, idx1);
    HashCombine(hash, idx2);
  }
  return hash;
}

const BasicPrototype* GeneralizedTypeSystem::RegisterBasicPrototype(const std::string& name) {
  BasicPrototype prototype(name);
  auto hash = prototype.Hash();
  if (auto it = prototypes_.find(hash); it != prototypes_.end()) {
    auto ptr = dynamic_cast<const BasicPrototype*>(it->second.get());
    MANTA_ASSERT(ptr, "object in map is not a BasicPrototype");
    return ptr;
  }
  auto [it, _] = prototypes_.emplace(hash, std::make_unique<BasicPrototype>(std::move(prototype)));
  return dynamic_cast<const BasicPrototype*>(it->second.get());
}

ObjectPrototype* GeneralizedTypeSystem::RegisterObjectPrototype(const std::string& object_name,
                                                                std::size_t num_type_vars) {
  ObjectPrototype prototype(object_name, num_type_vars);
  auto hash = prototype.Hash();
  if (auto it = prototypes_.find(hash); it != prototypes_.end()) {
    auto ptr = dynamic_cast<ObjectPrototype*>(it->second.get());
    MANTA_ASSERT(ptr, "object in map is not a ObjectPrototype");
    return ptr;
  }
  auto [it, _] = prototypes_.emplace(hash, std::make_unique<ObjectPrototype>(std::move(prototype)));
  return dynamic_cast<ObjectPrototype*>(it->second.get());
}

EnumeratedPrototype* GeneralizedTypeSystem::RegisterEnumeratedPrototype(
    std::string enum_name, std::map<std::string, const GeneralizedType*> options) {
  EnumeratedPrototype prototype(std::move(enum_name), std::move(options));
  auto hash = prototype.Hash();
  if (auto it = prototypes_.find(hash); it != prototypes_.end()) {
    const auto ptr = dynamic_cast<EnumeratedPrototype*>(it->second.get());
    MANTA_ASSERT(ptr, "object in map is not a BasicPrototype");
    return ptr;
  }
  auto [it, _] = prototypes_.emplace(hash, std::make_unique<EnumeratedPrototype>(std::move(prototype)));
  return dynamic_cast<EnumeratedPrototype*>(it->second.get());
}

EnumeratedPrototype* GeneralizedTypeSystem::RegisterEnumeratedPrototype(std::string enum_name,
                                                                        const std::set<std::string>& options,
                                                                        const GeneralizedType* type) {
  std::map<std::string, const GeneralizedType*> options_map;
  for (const auto& option : options) {
    options_map.emplace(option, type);
  }
  return RegisterEnumeratedPrototype(std::move(enum_name), std::move(options_map));
}


const FunctionPrototype* GeneralizedTypeSystem::RegisterFunctionPrototype(std::size_t num_type_vars,
                                                                          std::vector<ElaboratedType> types) {
  FunctionPrototype prototype(num_type_vars, std::move(types));
  auto hash = prototype.Hash();
  if (auto it = prototypes_.find(hash); it != prototypes_.end()) {
    const auto ptr = dynamic_cast<FunctionPrototype*>(it->second.get());
    MANTA_ASSERT(ptr, "object in map is not a FunctionPrototype");
    return ptr;
  }
  auto [it, _] = prototypes_.emplace(hash, std::make_unique<FunctionPrototype>(std::move(prototype)));
  return dynamic_cast<FunctionPrototype*>(it->second.get());
}

const GeneralizedType* GeneralizedTypeSystem::GetType(
    const Prototype* prototype, const std::map<std::size_t, const GeneralizedType*>& substitutions) {
  std::vector<const GeneralizedType*> initial_substitutions(prototype->GetNumTypeVars(), nullptr);
  std::vector<GeneralizedType::VarMapping> free_vars(prototype->GetNumTypeVars());
  for (std::size_t i = 0; i < prototype->GetNumTypeVars(); ++i) {
    free_vars[i] = GeneralizedType::VarMapping {{{i, 0}}};
  }
  const GeneralizedType generalized_type(prototype, initial_substitutions, free_vars);
  const auto base_type = getOrCreate(generalized_type);

  return Substitute(base_type, substitutions);
}

const GeneralizedType* GeneralizedTypeSystem::GetBasicType(const std::string& name) {
  const auto proto = RegisterBasicPrototype(name);
  return GetType(proto);
}

const GeneralizedType* GeneralizedTypeSystem::GetObjectType(const std::string& object_name,
                                                            std::size_t num_type_vars) {
  const auto proto = RegisterObjectPrototype(object_name, num_type_vars);
  return GetType(proto);
}

GeneralizedType* GeneralizedTypeSystem::GetEnumerationType(
    const std::string& enum_name, const std::map<std::string, const GeneralizedType*>& options) {
  const auto proto = RegisterEnumeratedPrototype(enum_name, options);
  const GeneralizedType generalized_type(proto, {}, {});
  return getOrCreate(generalized_type);
}

const GeneralizedType* GeneralizedTypeSystem::GetFunctionType(std::vector<ElaboratedType> types,
                                                              std::size_t num_type_vars) {
  auto proto = RegisterFunctionPrototype(num_type_vars, std::move(types));
  return GetType(proto);
}

const GeneralizedType* GeneralizedTypeSystem::Substitute(
    const GeneralizedType* type, const std::map<std::size_t, const GeneralizedType*>& substitutions) {
  MANTA_REQUIRE(substitutions.empty() || substitutions.rbegin()->first < type->prototype_->GetNumTypeVars(),
                "substitution index is out of range for the provided type, max index is "
                    << type->prototype_->GetNumTypeVars() - 1 << ", got " << substitutions.rbegin()->first);

  auto new_type = *type;
  // Substitute the type vars. Group substitutions by the entry in the type's substitution layer, so we can
  // derive new substituted types all at once.
  // E.g. if we have a generalized type
  // PROTO  A <  *       *        *  >
  //             |       |        |
  // SUBS        *    vec<*, *>   *
  //             0        1  2    3
  //
  // FREE      $0 -> 0, 1    $1 -> 2, 3
  //
  // And we substitute $0 -> INT, $1 -> FLOAT, we will need to generalize vec<*, *> to get vec<INT, FLOAT>
  // in the substitution layer.

  std::map<std::size_t /* subst layer */, std::map<std::size_t, const GeneralizedType*>>
      substitutions_by_subst_layer;
  for (const auto& [var_index, gen_type] : substitutions) {
    auto& mapping = new_type.free_vars_[var_index];
    for (const auto& [subst_layer_index, free_var_index] : mapping.mapping) {
      substitutions_by_subst_layer[subst_layer_index].emplace(free_var_index, gen_type);
    }
  }

  for (const auto& [subst_layer, substitutions] : substitutions_by_subst_layer) {
    auto& subst_entry = new_type.substituted_vars_[subst_layer];
    if (subst_entry == nullptr) {
      MANTA_ASSERT(substitutions.size() == 1, "expect only one substitution t obe possible");
      subst_entry = substitutions.begin()->second;
    }
    else {
      subst_entry = Substitute(subst_entry, substitutions);
    }
  }

  // Create the new free variables. The list may expand if a substituted type has more than one free type
  // variable, or contract if the substituted type is a concrete type (no free type variables).
  new_type.free_vars_.clear();
  for (std::size_t i = 0; i < type->free_vars_.size(); ++i) {
    // If the free var was substituted.
    if (auto it = substitutions.find(i); it != substitutions.end()) {
      for (std::size_t j = 0; j < it->second->GetNumFreeVars(); ++j) {
        auto& entry = new_type.free_vars_.emplace_back();
        entry.mapping.emplace_back(i, j);
      }
    }
    else {
      new_type.free_vars_.emplace_back(type->free_vars_[i]);
    }
  }

  return getOrCreate(new_type);
}

GeneralizedType* GeneralizedTypeSystem::getOrCreate(const GeneralizedType& generalized_type) {
  auto hash = generalized_type.Hash();
  if (auto it = generalized_types_.find(hash); it != generalized_types_.end()) {
    return it->second.get();
  }
  auto [it, _] = generalized_types_.emplace(hash, std::make_unique<GeneralizedType>(generalized_type));
  return it->second.get();
}


}  // namespace manta::typesystem