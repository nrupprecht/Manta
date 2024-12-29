//
// Created by Nathaniel Rupprecht on 12/22/24.
//

#pragma once

#include <format>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "manta/utility/Exceptions.h"

namespace manta::typesystem {

class GeneralizedType;
class ProtoypeVisitor;

using SubstitutionCallback = std::function<void(std::ostream&, std::size_t, std::span<std::size_t>)>;

struct TypeVar {
  std::string var_name;

  auto operator<=>(const TypeVar&) const = default;
};

//! \brief What kind of prototype.
enum class PrototypeType {
  BASIC,
  OBJECT,
  ENUM,
  FUNCTION,
};

std::string_view to_string(PrototypeType type);

class Prototype {
public:
  Prototype(std::size_t num_type_vars, PrototypeType type) : num_type_vars_(num_type_vars), type_(type) {}

  virtual ~Prototype() = default;

  virtual std::string Write() const                            = 0;
  virtual std::string Write(const SubstitutionCallback& substitution_callback,
                            std::span<std::size_t> vars) const = 0;
  virtual std::size_t Hash() const                             = 0;

  virtual void Accept(ProtoypeVisitor& visitor) const = 0;

  PrototypeType GetPrototypeType() const { return type_; }

  std::size_t GetNumTypeVars() const { return num_type_vars_; }

protected:
  const std::size_t num_type_vars_;
  const PrototypeType type_;
};

struct ElaboratedType {
  //! \brief Base type. If this type is a type variable, this should be an IdentityType.
  const GeneralizedType* type;

  //! \brief Bindings from type variables in the prototype that this elaborated type is associated with to
  //!        type variables in the generalized type.
  //!        Any free variable in the type should be bound by a type variable here, so bindings.size() must be
  //!        equal to type->GetNumFreeVars().
  std::vector<std::size_t> bindings;

  bool is_const = false;
  bool is_ref   = false;

  std::size_t Hash() const;

  ElaboratedType& TypeBind(std::vector<std::size_t> bind);
};

//! \brief A prototype for a non-object type.
class BasicPrototype final : public Prototype {
public:
  explicit BasicPrototype(std::string name) : Prototype({}, PrototypeType::BASIC), name_(std::move(name)) {}

  std::string Write() const override;
  std::string Write(const SubstitutionCallback& substitution_callback,
                    std::span<std::size_t> vars) const override;

  std::size_t Hash() const override;

  const std::string& GetName() const { return name_; }

  void Accept(ProtoypeVisitor& visitor) const override;

private:
  std::string name_;
};

//! \brief Prototype for an enumeration.
class EnumeratedPrototype final : public Prototype {
public:
  EnumeratedPrototype(std::string name, std::map<std::string, const GeneralizedType*> options)
      : Prototype({}, PrototypeType::ENUM)
      , enum_name_(std::move(name))
      , options_(std::move(options)) {}

  std::string Write() const override;
  std::string Write(const SubstitutionCallback& substitution_callback,
                    std::span<std::size_t> vars) const override;

  const std::string& GetEnumName() const { return enum_name_; }

  std::size_t Hash() const override;

  void Accept(ProtoypeVisitor& visitor) const override;

  const std::map<std::string, const GeneralizedType*>& GetOptions() const { return options_; }

private:
  //! \brief The name of the enumeration.
  const std::string enum_name_;

  // TODO: Bindings for the type variables in the enumeration. Use a struct in the map with the bindings
  //       instead of just the type.

  //! \brief The enumeration options - map from names to the objects for the enums.
  const std::map<std::string, const GeneralizedType*> options_;
};

//! \brief Base type for functions
class FunctionPrototype final : public Prototype {
public:
  FunctionPrototype(std::size_t num_type_vars, std::vector<ElaboratedType> types)
      : Prototype(num_type_vars, PrototypeType::FUNCTION)
      , types_(std::move(types)) {
    MANTA_REQUIRE(types_.size() > 1, "function must have at least one argument");
  }

  std::string Write() const override;
  std::string Write(const SubstitutionCallback& substitution_callback,
                    std::span<std::size_t> vars) const override;

  std::size_t Hash() const override;

  std::size_t GetArity() const { return types_.size() - 1; }

  void Accept(ProtoypeVisitor& visitor) const override;

private:
  const std::vector<ElaboratedType> types_;
};


class ObjectPrototype;


//! \brief Helper structure that defines a simple constructor for a TypeDescriptionStructure.
struct StructureConstructor {
  using ArgName   = std::string;
  using FieldName = std::string;

  //! \brief Represents an argument value.
  //!
  //! NOTE: This breaks the separation between abstract structure and codegen (e.g. by using C++ style syntax
  //! for enums), but I am allowing it for now.
  struct Value {
    std::string literal;
  };

  std::vector<std::pair<ElaboratedType, ArgName>> arguments {};

  //! \brief  Call parent constructors.
  std::vector<
      // Description of how to call a single constructor. Can call either with arguments,
      // or with specific values.
      std::pair<const GeneralizedType*, std::vector<std::variant<ArgName, Value>>>>
      parent_constructors {};

  //! \brief List initialized fields that initialize the data from constructor arguments.
  std::vector<std::tuple<ArgName, FieldName>> list_initialized_args {};

  //! \brief Additional fields, values to initialize in addition to the arguments.
  std::vector<std::pair<FieldName, Value>> additional_initializations {};

  //! \brief Constructor function body. This is literally inserted as the body, as is.
  std::string function_body;

  // =====================================================================================
  //  Builder functions
  // =====================================================================================

  StructureConstructor& WithArguments(std::vector<std::pair<ElaboratedType, ArgName>> args);

  StructureConstructor& WithParentConstuctor(
      std::vector<std::pair<const GeneralizedType*, std::vector<std::variant<ArgName, Value>>>> constructors);

  StructureConstructor& WithListInitializedArgs(std::vector<std::tuple<ArgName, FieldName>> list_init_args);

  StructureConstructor& WithAdditionalInitializations(std::vector<std::pair<FieldName, Value>> inits);

  StructureConstructor& WithBody(std::string_view body);
};

// Forward declare for StructureFunction
struct StructureFunction;

//! \brief Base class for bound and unbound function values.
//!
//! This includes the function's body, which is written in an arbitrary language. This base class, by itself,
//! represents an unbound function.
struct FunctionValue {
  virtual ~FunctionValue() = default;

  //! \brief The function's name.
  std::string function_name {};

  //! \brief The function's type.
  const FunctionPrototype* function_type;

  //! \brief The names of the function's arguments. There must be as many arguments as there are function
  //! arguments, and the names must be unique if they are not empty.
  std::vector<std::string> argument_names;

  //! \brief Contents of the function (if not virtual).
  //!
  //! I don't have a "language independent" way of doing this right now, so I am just assuming this is
  //! literally the code that should go into the function. If nullopt, this is a virtual function.
  std::optional<std::string> function_body {};

  std::size_t GetArity() const { return argument_names.size(); }

  //! \brief Check whether the function is virtual.
  //!
  //! NOTE: This only really makes sense for StructureFunction.
  bool IsVirtual() const { return !function_body; }

  virtual void Validate() const {
    MANTA_REQUIRE(argument_names.size() == function_type->GetArity(),
                  "mismatch between number of argument names and number of arguments");
    // MANTA_REQUIRE(function_type.IsValid(), "invalid function type");
    MANTA_REQUIRE(!function_name.empty(), "function name cannot be empty");
  }

  //! \brief Create a copy of the function and bind it to the structure.
  //!
  //! Adds the function to the structure, and sets the structure as the binder in the StructureFunction
  //! instance. Returns a reference to the newly created bound function value inside the structure.
  StructureFunction& BindToStructure(ObjectPrototype* structure, bool is_const, bool is_override) const;

  auto operator<=>(const FunctionValue& other) const {
    return std::tie(function_name, function_type, argument_names, function_body)
        <=> std::tie(other.function_name, other.function_type, other.argument_names, other.function_body);
  }

  // =====================================================================================
  //  Builder functions
  // =====================================================================================

  FunctionValue& WithName(std::string name) {
    function_name = std::move(name);
    return *this;
  }

  FunctionValue& WithType(FunctionPrototype* type) {
    function_type = std::move(type);
    return *this;
  }

  FunctionValue& WithArgumentNames(std::vector<std::string> names) {
    argument_names = std::move(names);
    return *this;
  }

  //! \brief Explicitly set the body of the function to be non-existent, indicating that it is virtual.
  //!
  //! This is the default anyways.
  FunctionValue& WithBody(std::string body) {
    function_body = std::move(body);
    return *this;
  }

  FunctionValue& WithEmptyBody() {
    function_body = "";  // Non-null (not virtual), but empty.
    return *this;
  }
};

//! \brief A structure that represents a function of a record or structure, a bound function.
struct StructureFunction : FunctionValue {
  StructureFunction() = default;

  StructureFunction(FunctionValue value) {
    function_name  = std::move(value.function_name);
    function_type  = std::move(value.function_type);
    argument_names = std::move(value.argument_names);
    function_body  = std::move(value.function_body);
  }

  //! \brief The structure that the function is bound to.
  const ObjectPrototype* binding_structure {};

  //! \brief Whether the function is a constant function.
  bool is_const_function {false};

  //! \brief Whether the function overrides another function.
  bool is_override {false};

  void Validate() const override {
    FunctionValue::Validate();
    MANTA_REQUIRE(binding_structure, "function must be bound to a structure");
  }

  // =====================================================================================
  //  Builder functions
  // =====================================================================================

  StructureFunction& WithConst(bool is_const) {
    is_const_function = is_const;
    return *this;
  }

  StructureFunction& AsVirtual() {
    function_body = std::nullopt;
    return *this;
  }

  StructureFunction& WithIsOverride(bool set_is_override) {
    is_override = set_is_override;
    return *this;
  }
};


//! \brief A prototype for an object type, which can have associated functions and fields.
class ObjectPrototype final : public Prototype {
public:
  struct Field {
    std::string field_name;
    ElaboratedType type;
  };

  //! \brief Create an object. Objects have nominative typing, if they have different names, they are
  //!        different types.
  explicit ObjectPrototype(std::string name, std::size_t num_type_vars = 0);

  std::string Write() const override;
  std::string Write(const SubstitutionCallback& substitution_callback,
                    std::span<std::size_t> vars) const override;

  std::size_t Hash() const override;

  void Accept(ProtoypeVisitor& visitor) const override;

  const std::string& GetObjectName() const { return object_name_; }

  //! \brief Add a field to the structure.
  void AddField(const std::string& field_name,
                const GeneralizedType* field_type,
                std::vector<std::size_t> binding = {});

  //! \brief Remove a field from a structure. Returns true if the field was there to be removed.
  bool RemoveField(const std::string& field_name);

  //! \brief Add a structure as a parent class of this structure.
  //!        TODO: Add bindings for the type variables in the parent.
  void AddParent(const GeneralizedType* parent);

  void AddConstructor(const StructureConstructor& constructor);

  //! \brief Add a bound method to the object.
  //!        TODO: Add bindings for the type variables in the function.
  StructureFunction& AddFunction(const StructureFunction& function);

  bool HasField(const std::string& name) const;

  const Field* GetField(const std::string& name) const;

  const std::vector<Field>& GetFields() const;

  const std::deque<StructureConstructor>& GetConstructors() const;

  const std::deque<StructureFunction>& GetFunctions() const;

  const std::string& GetAdhocCode() const;

private:
  const std::string object_name_;

  std::vector<Field> fields_;

  std::vector<const GeneralizedType*> parent_types_;

  //! \brief Constructors for the structure. Storing as a deque so that we don't invalidate pointers.
  std::deque<StructureConstructor> constructors_;

  //! \brief Functions for the structure. Storing as a deque so that we don't invalidate pointers.
  std::deque<StructureFunction> functions_;

  //! \brief Field for injecting arbitrary user defined code into the structure. This could include things
  //!        like additional function or variable definitions.
  std::string adhoc_code_ {};
};


class ProtoypeVisitor {
public:
  virtual ~ProtoypeVisitor()                               = default;
  virtual void Visit(const BasicPrototype& prototype)      = 0;
  virtual void Visit(const ObjectPrototype& prototype)     = 0;
  virtual void Visit(const EnumeratedPrototype& prototype) = 0;
  virtual void Visit(const FunctionPrototype& prototype)   = 0;
};

//! \brief Some function from types to a type.
class GeneralizedType {
public:
  std::string Write(std::span<std::size_t> vars = {}) const;

  //! \brief Get a hash that should be unique for each different generalized type.
  std::size_t Hash() const;

  //! \brief Returns true if the generalized type is a concrete type (no free type variables).
  bool IsType() const { return free_vars_.empty(); }

  //! \brief Returns true if the generalized type is a type constructor (has free type variables).
  bool IsTypeConstructor() const { return !IsType(); }

  //! \brief Returns whether the generalized type has any substitutions.
  bool HasSubstitutions() const {
    return std::ranges::any_of(substituted_vars_,
                               [](const GeneralizedType* type) { return type != nullptr; });
  }

  //! \brief Get the number of free type variables in the generalized type.
  std::size_t GetNumFreeVars() const { return free_vars_.size(); }

  //! \brief Get the type of the prototype (the "catagory" of the type).
  PrototypeType GetPrototypeType() const { return prototype_->GetPrototypeType(); }

  //! \brief Get the prototype.
  const Prototype* GetPrototype() const { return prototype_; }

  const GeneralizedType* GetSubstitution(std::size_t index) const {
    MANTA_REQUIRE(index < substituted_vars_.size(), "index out of bounds");
    return substituted_vars_[index];
  }

  //! \brief From a span of items representing a 1-1 mapping with the free variables of this type, select
  //!        the items that corresponds to feeding the variables to the index-th substituted type.
  template<typename T>
  std::vector<T> Select(std::span<T> vars, std::size_t index) const {
    MANTA_REQUIRE(vars.size() == free_vars_.size(),
                  "vars must be the same size as the number of free vars, sizes " << vars.size() << " and "
                                                                                  << free_vars_.size());
    if (vars.empty()) {
      return {};
    }
    MANTA_REQUIRE(
        index < substituted_vars_.size(),
        "index out of bounds, index = " << index << ", num substituted types = " << substituted_vars_.size());

    auto ptr = substituted_vars_[index];
    std::vector<T> selected(ptr ? ptr->GetNumFreeVars() : 1);
    std::ranges::fill(selected, std::numeric_limits<std::size_t>::max());
    for (std::size_t idx = 0; idx < free_vars_.size(); ++idx) {
      for (auto& [subst_index, var_in_subst] : free_vars_[idx].mapping) {
        if (subst_index == index) {
          selected[var_in_subst] = vars[idx];
        }
      }
    }

    return selected;
  }

  friend class GeneralizedTypeSystem;

private:
  struct VarMapping {
    //! \brief Index into the substituted variable and (if not null) the free variable into the substituted
    //!        type's free var layer.
    std::vector<std::pair<std::size_t, std::size_t>> mapping;

    std::size_t Hash() const;
  };

  GeneralizedType(const Prototype* prototype,
                  std::vector<const GeneralizedType*> substituted_vars,
                  std::vector<VarMapping> free_vars)
      : prototype_(prototype)
      , substituted_vars_(std::move(substituted_vars))
      , free_vars_(std::move(free_vars)) {}

  void write(std::ostream& out, std::span<std::size_t> vars_in) const;

  void writeNthSubstitution(std::ostream& out, std::size_t index, std::span<std::size_t> vars = {}) const;

  //! \brief The prototype that this generalized type is based on.
  const Prototype* prototype_ {};

  //! \brief Substitution layer - same size as the number of free variables in the prototype.
  //!        If the type pointer is null, this is a free variable (not substituted).
  std::vector<const GeneralizedType*> substituted_vars_;

  //! \brief Mapping from the n-th free variable to where the free variable is substituted into.
  //!        May be substituted either into the prototype (if the target in the substituted_vars vector is
  //!        null), or into a free variable in a generalized type in the substituted vars layer.
  std::vector<VarMapping> free_vars_;
};


class GeneralizedTypeSystem {
public:
  const BasicPrototype* RegisterBasicPrototype(const std::string& name);

  ObjectPrototype* RegisterObjectPrototype(const std::string& object_name, std::size_t num_type_vars = 0);

  EnumeratedPrototype* RegisterEnumeratedPrototype(std::string enum_name,
                                                   std::map<std::string, const GeneralizedType*> options);

  EnumeratedPrototype* RegisterEnumeratedPrototype(std::string enum_name,
                                                   const std::set<std::string>& options,
                                                   const GeneralizedType* type);

  const FunctionPrototype* RegisterFunctionPrototype(std::size_t num_type_vars,
                                                     std::vector<ElaboratedType> types);

  //! \brief Create a type by substituting zero or more type variables in a prototype.
  const GeneralizedType* GetType(const Prototype* prototype,
                                 const std::map<std::size_t, const GeneralizedType*>& substitutions = {});

  const GeneralizedType* GetBasicType(const std::string& name);
  const GeneralizedType* GetObjectType(const std::string& object_name, std::size_t num_type_vars = 0);
  GeneralizedType* GetEnumerationType(const std::string& enum_name,
                                      const std::map<std::string, const GeneralizedType*>& options);
  const GeneralizedType* GetFunctionType(std::vector<ElaboratedType> types, std::size_t num_type_vars = 0);

  //! \brief Get the generalized type resulting from substituting zero or more type variables in a generalized
  //!        type with free type variables.
  const GeneralizedType* Substitute(const GeneralizedType* type,
                                    const std::map<std::size_t, const GeneralizedType*>& substitutions);

private:
  GeneralizedType* getOrCreate(const GeneralizedType& generalized_type);

  std::map<std::size_t, std::unique_ptr<Prototype>> prototypes_;

  std::map<std::size_t, std::unique_ptr<GeneralizedType>> generalized_types_;
};


}  // namespace manta::typesystem