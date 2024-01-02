//
// Created by Nathaniel Rupprecht on 5/14/23.
//

#pragma once

#include <utility>
#include <deque>

#include <Lightning/Lightning.h>

#include "manta/utility/Exceptions.h"
#include "manta/utility/utility.hpp"

namespace manta {

//! \brief The general Type System category enum.
enum class TSGeneralType
{
  Vector,
  String,
  Integer,
  Float,
  SharedPointer,
  Structure,
  Enumeration,
  Function,
};

//! \brief Function to serialize a TSGeneralType enum to a string.
std::string to_string(TSGeneralType type);

//! \brief Base class for the possible types of member variables in an AST node.
struct TypeDescription {
  explicit TypeDescription(TSGeneralType type)
      : general_type(type) {}

  //! \brief The general type, whether this is a structure, or one of the built in types.
  TSGeneralType general_type;

  //! \brief  Write a description of the type. Note that this is independent of code
  //! generation, this is for reporting, error, and debugging purposes.
  NO_DISCARD virtual std::string Write() const {
    // Default implementation, so we can still instantiate TypeDescription(enum) to get
    // the base hash.
    return to_string(general_type);
  }

  //! \brief Hash a description of the type, this can be used to determine if types are
  //! structurally equal.
  NO_DISCARD virtual std::size_t HashStructure() const { return static_cast<std::size_t>(general_type); }

  //! \brief Hash the identifier of the type. Types with different names but the same
  //! structure (like nodes) can return different HashIDs, while, conversely, as we modify
  //! the type layout of a node type, the HashID will not change.
  NO_DISCARD virtual std::size_t HashID() const { return static_cast<std::size_t>(general_type); }
};

//! \brief To-string function for a type description. Allows a TypeDescription to be
//! streamed into a logger.
inline std::string to_string(const TypeDescription& description) {
  return description.Write();
}

//! \brief A type along with modifiers.
struct ElaboratedType {
  const TypeDescription* arg_type {};
  bool is_const {};
  bool is_ref {};
};

//! \brief Type description for a "basic" type. Basic types will be mapped to types in
//! target programming languages in a language dependent way.
//!
struct BasicTypeDescription : public TypeDescription {
  explicit BasicTypeDescription(TSGeneralType type)
      : TypeDescription(type) {}
};

// Forward declare
class TypeDescriptionStructure;

//! \brief Helper structure that defines a simple constructor for a
//! TypeDescriptionStructure.
//!
struct StructureConstructor {
  using ArgName = std::string;
  using FieldName = std::string;

  //! \brief Represents an argument value.
  //!
  //! NOTE: This breaks the separation between abstract structure and codegen (e.g. by
  //! using C++ style syntax for enums), but I am allowing it for now.
  struct Value {
    std::string literal;
  };

  std::vector<std::pair<const TypeDescription*, ArgName>> arguments {};

  //! \brief  Call parent constructors.
  std::vector<
      // Description of how to call a single constructor. Can call either with arguments,
      // or with specific values.
      std::pair<const TypeDescriptionStructure*, std::vector<std::variant<ArgName, Value>>>>
      parent_constructors {};

  //! \brief List initialized fields that initialize the data from constructor arguments.
  //!
  std::vector<std::tuple<ArgName, FieldName>> list_initialized_args {};

  //! \brief  Additional fields, values to initialize in addition to the arguments.
  //!
  std::vector<std::pair<FieldName, Value>> additional_initializations {};

  // =====================================================================================
  //  Builder functions
  // =====================================================================================

  StructureConstructor& WithArguments(std::vector<std::pair<const TypeDescription*, ArgName>> args);

  StructureConstructor& WithParentConstuctor(
      std::vector<std::pair<const TypeDescriptionStructure*, std::vector<std::variant<ArgName, Value>>>>
          constructors);

  StructureConstructor& WithListInitializedArgs(std::vector<std::tuple<ArgName, FieldName>> list_init_args);

  StructureConstructor& WithAdditionalInitializations(std::vector<std::pair<FieldName, Value>> inits);
};

//! \brief Type description for a function.
struct FunctionType : public TypeDescription {
  FunctionType() : TypeDescription(TSGeneralType::Function) {}

  //! \brief The function's arguments.
  std::vector<ElaboratedType> arguments{};

  //! \brief The return type of the function, or nothing (for "void").
  std::optional<ElaboratedType> return_type{};

  bool IsValid() const {
    for (const auto& arg : arguments) {
      if (!arg.arg_type) return false;
    }
    if (return_type && !return_type->arg_type) {
      return false;
    }
    return true;
  }

  // =====================================================================================
  //  Builder functions
  // =====================================================================================

  FunctionType& WithArguments(std::vector<ElaboratedType> args) {
    arguments = std::move(args);
    return *this;
  }

  FunctionType& WithReturnType(std::optional<ElaboratedType> ret) {
    return_type = std::move(ret);
    return *this;
  }
};

// Forward declare for StructureFunction
class StructureFunction;

//! \brief Base class for bound and unbound function values. This includes the function's body, which is written in an
//! arbitrary language.
//!
//! This base class, by itself, represents an unbound function.
struct FunctionValue {
  //! \brief The function's name.
  std::string function_name{};

  //! \brief The function's type.
  FunctionType function_type{};

  //! \brief The names of the function's arguments. There must be as many arguments as there are function arguments, and
  //! the names must be unique if they are not empty.
  std::vector<std::string> argument_names;

  //! \brief Contents of the function (if not virtual). I don't have a "language independent" way of doing
  //! this right now, so I am just assuming this is literally the code that should go into the function.
  //! If nullopt, this is a virtual function.
  std::optional<std::string> function_body{};

  //! \brief Check whether the function is virtual. NOTE: This only really makes sense for StructureFunction.
  bool IsVirtual() const { return !function_body; }

  virtual const FunctionValue& Validate() const {
    MANTA_REQUIRE(argument_names.size() == function_type.arguments.size(),
                  "mismatch between number of argument names and number of arguments");
    MANTA_REQUIRE(function_type.IsValid(), "invalid function type");
    MANTA_REQUIRE(!function_name.empty(), "function name cannot be empty");
    return *this;
  }

  //! \brief Create a copy of the function and bind it to the structure. Adds the function to the structure, and sets
  //! the structure as the binder in the StructureFunction instance. Returns a reference to the newly created bound
  //! function value inside the structure.
  StructureFunction& BindToStructure(class TypeDescriptionStructure* structure, bool is_const, bool is_override);

  // =====================================================================================
  //  Builder functions
  // =====================================================================================

  FunctionValue& WithName(std::string name) {
    function_name = std::move(name);
    return *this;
  }

  FunctionValue& WithType(FunctionType type) {
    function_type = std::move(type);
    return *this;
  }

  FunctionValue& WithArgumentNames(std::vector<std::string> names) {
    argument_names = std::move(names);
    return *this;
  }

  //! \brief Explicitly set the body of the function to be non-existent, indicating that it is virtual.
  //! This is the default anyways.
  FunctionValue& WithBody(std::string body) {
    function_body = std::move(body);
    return *this;
  }

  FunctionValue& WithEmptyBody() {
    function_body = ""; // Non-null (not virtual), but empty.
    return *this;
  }
};

//! \brief A structure that represents a function of a record or structure, a bound function.
//!
struct StructureFunction : public FunctionValue {
  StructureFunction() = default;
  StructureFunction(FunctionValue value) {
    function_name = std::move(value.function_name);
    function_type = std::move(value.function_type);
    argument_names = std::move(value.argument_names);
    function_body = std::move(value.function_body);
  }

//! \brief The structure that the function is bound to.
  class TypeDescriptionStructure* binding_structure{};

  //! \brief Whether the function is a constant function.
  bool is_const_function{false};

  //! \brief Whether the function overrides another function.
  bool is_override{false};

  const StructureFunction& Validate() const override {
    FunctionValue::Validate();
    MANTA_REQUIRE(binding_structure, "function must be bound to a structure");
    return *this;
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

//! \brief  Type description for a structure, or compound type.
//!
//! This is the most complex type, as I also allow it to have constructors which can do different types of
//! initializations.
struct TypeDescriptionStructure : public TypeDescription {
  explicit TypeDescriptionStructure(std::string name);

  //! \brief Add a field to the structure.
  void AddField(const std::string& field_name, const TypeDescription* field_type);
  //! \brief Remove a field from a structure. Returns true if the field was there to be removed.
  bool RemoveField(const std::string& field_name);

  //! \brief Add a structure as a parent class of this structure.
  void AddParent(const TypeDescriptionStructure* parent);

  void AddConstructor(const StructureConstructor& constructor);

  //! \brief Add a bound method to the object.
  StructureFunction& AddFunction(const StructureFunction& function);

  NO_DISCARD std::string Write() const override;

  static std::size_t PotentialHashID(const std::string& type_name);

  NO_DISCARD std::size_t HashStructure() const override;

  //! \brief Nodes are classes, so their ID is just their name.
  NO_DISCARD std::size_t HashID() const override;

  //! \brief The name of the type.
  const std::string type_name;

  //! \brief The fields of the structure.
  std::map<std::string, const TypeDescription*> fields;

  //! \brief The set of base classes of the structure.
  std::set<const TypeDescriptionStructure*> parent_classes;

  //! \brief Constructors for the structure. Storing as a deque so that we don't invalidate pointers.
  std::deque<StructureConstructor> constructors;

  //! \brief Functions for the structure. Storing as a deque so that we don't invalidate pointers.
  std::deque<StructureFunction> functions;

  //! \brief Field for injecting arbitrary user defined code into the structure. This could include things
  //! like additional function or variable definitions.
  std::string adhoc_code {};
};

inline StructureFunction& FunctionValue::BindToStructure(class TypeDescriptionStructure* structure,
                                                         bool is_const,
                                                         bool is_override) {
  StructureFunction function{*this};
  function.is_const_function = is_const;
  function.is_override = is_override;
  function.binding_structure = structure;
  return structure->AddFunction(function);
}


//! \brief Represents a vector or ordered collection of objects of some other type.
struct TypeDescriptionVector : public BasicTypeDescription {
  explicit TypeDescriptionVector(const TypeDescription* element_type);

  NO_DISCARD std::string Write() const override;

  NO_DISCARD std::size_t HashStructure() const override;

  NO_DISCARD std::size_t HashID() const override;

  static std::size_t PotentialHashID(const TypeDescription* vector_type);

  const TypeDescription* element_type {};
};

//! \brief Represents a shared pointer to an object of some other type.
struct TypeDescriptionSharedPointer : public BasicTypeDescription {
  explicit TypeDescriptionSharedPointer(const TypeDescription* type);

  NO_DISCARD std::string Write() const override;

  NO_DISCARD std::size_t HashStructure() const override;

  NO_DISCARD std::size_t HashID() const override;

  static std::size_t PotentialHashID(const TypeDescription* pointed_type);

  const TypeDescription* pointed_type {};
};

//! \brief Represents an enumeration type.
struct TypeDescriptionEnum : public BasicTypeDescription {
  explicit TypeDescriptionEnum(const std::string& enum_name);

  NO_DISCARD std::string Write() const override;

  NO_DISCARD std::size_t HashStructure() const override;

  NO_DISCARD std::size_t HashID() const override;

  static std::size_t PotentialHashID(const std::string& name);

  NO_DISCARD const std::string& GetName() const;
  void AddOption(const std::string& enum_option);

  //! \brief Get the set of Enum options (what the potential values of the enum can be).
  NO_DISCARD const std::set<std::string>& GetOptions() const;

private:
  //! \brief The name of the enumeration.
  const std::string enum_name_;

  //! \brief The enumeration options.
  std::set<std::string> options_;
};

//! \brief Represents a string field.
struct TypeDescriptionString : public TypeDescription {
  TypeDescriptionString()
      : TypeDescription(TSGeneralType::String) {}
};

//! \brief Represents an integer field.
struct TypeDescriptionInteger : public TypeDescription {
  TypeDescriptionInteger()
      : TypeDescription(TSGeneralType::Integer) {}
};

//! \brief Represents an floating point field.
struct TypeDescriptionFloat : public TypeDescription {
  TypeDescriptionFloat()
      : TypeDescription(TSGeneralType::Float) {}
};

//! \brief A class that manages a universe of type. Allows for the creation of types,
//! generally by composing other types from the TypeSystem into new types, like vectors of
//! types, pointers to types, structures with other types as fields, etc.
class TypeSystem {
public:
  //! \brief Get the string type.
  NO_DISCARD const TypeDescriptionString* String() const { return &string_type_; }

  //! \brief Get the integer type.
  NO_DISCARD const TypeDescriptionInteger* Integer() const { return &integer_type_; }

  //! \brief Get the floating point type.
  NO_DISCARD const TypeDescriptionFloat* Float() const { return &float_type_; }

  //! \brief Get an enumeration type by name.
  TypeDescriptionEnum* Enum(const std::string& enum_name);

  //! \brief Get a shared pointer type.
  const TypeDescriptionSharedPointer* SharedPointer(const TypeDescription* pointed_type);

  //! \brief Get a vector type.
  const TypeDescriptionVector* Vector(const TypeDescription* element_type);

  //! \brief Get a structure type.
  TypeDescriptionStructure* Structure(const std::string& type_name);

private:
  //! \brief The string type.
  const TypeDescriptionString string_type_;

  const TypeDescriptionInteger integer_type_;

  const TypeDescriptionFloat float_type_;

  //! \brief All types managed by the typesystem.
  std::map<std::size_t, std::shared_ptr<TypeDescription>> types_;
};

//! \brief Add a little color formatter for TypeDescriptions.
inline void format_logstream(const TypeDescription& type_description, lightning::RefBundle& handler) {
  handler << lightning::AnsiColor8Bit(type_description.Write(),
                                      lightning::formatting::AnsiForegroundColor::BrightBlue);
}

}  // namespace manta
