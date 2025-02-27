//
// Created by Nathaniel Rupprecht on 5/14/23.
//

#pragma once

#include <deque>
#include <utility>

#include <Lightning/Lightning.h>

#include "manta/utility/Exceptions.h"
#include "manta/utility/Utility.h"

namespace manta {

//! \brief The general Type System category enum.
enum class TSGeneralType {
  Vector,
  String,
  Integer,
  Float,
  SharedPointer,
  Structure,
  Enumeration,
  Function,
  Custom,
};

//! \brief Function to serialize a TSGeneralType enum to a string.
std::string to_string(TSGeneralType type);

//! \brief Base class for the possible types of member variables in an AST node.
struct TypeDescription {
  explicit TypeDescription(TSGeneralType type) : general_type(type) {}

  virtual ~TypeDescription() = default;

  //! \brief The general type, whether this is a structure, or one of the built in types.
  TSGeneralType general_type;

  //! \brief Write a description of the type. Note that this is independent of code generation, this is for
  //!        reporting, error, and debugging purposes.
  NO_DISCARD virtual std::string Write() const {
    // Default implementation, so we can still instantiate TypeDescription(enum) to get
    // the base hash.
    return to_string(general_type);
  }

  //! \brief Hash a description of the type, this can be used to determine if types are
  //!        structurally equal.
  NO_DISCARD virtual std::size_t HashStructure() const { return static_cast<std::size_t>(general_type); }

  //! \brief Hash the identifier of the type.
  //!
  //! Types with different names but the same structure (like nodes) can return different HashIDs, while,
  //! conversely, as we modify the type layout of a node type, the HashID will not change.
  NO_DISCARD virtual std::size_t HashID() const { return static_cast<std::size_t>(general_type); }
};

//! \brief To-string function for a type description.
//!
//! Allows a TypeDescription to be streamed into a logger.
inline std::string to_string(const TypeDescription& description) {
  return description.Write();
}

//! \brief A type along with modifiers.
struct ElaboratedType {
  const TypeDescription* arg_type {};
  bool is_const {};
  bool is_ref {};

  bool operator<(const ElaboratedType& other) const {
    return std::tie(arg_type, is_const, is_ref) < std::tie(other.arg_type, other.is_const, other.is_ref);
  }
};

//! \brief Type description for a "basic" type.
//!
//! Basic types will be mapped to types in target programming languages in a language dependent way.
struct BasicTypeDescription : TypeDescription {
  explicit BasicTypeDescription(TSGeneralType type) : TypeDescription(type) {}
};

// Forward declare
struct TypeDescriptionStructure;

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

  std::vector<std::pair<const TypeDescription*, ArgName>> arguments {};

  //! \brief  Call parent constructors.
  std::vector<
      // Description of how to call a single constructor. Can call either with arguments,
      // or with specific values.
      std::pair<const TypeDescriptionStructure*, std::vector<std::variant<ArgName, Value>>>>
      parent_constructors {};

  //! \brief List initialized fields that initialize the data from constructor arguments.
  std::vector<std::tuple<ArgName, FieldName>> list_initialized_args {};

  //! \brief Additional fields, values to initialize in addition to the arguments.
  std::vector<std::pair<FieldName, Value>> additional_initializations {};

  //! \brief Constructor function body. This is literally inserted as the body, as is.
  std::string function_body {};

  // =====================================================================================
  //  Builder functions
  // =====================================================================================

  StructureConstructor& WithArguments(std::vector<std::pair<const TypeDescription*, ArgName>> args);

  StructureConstructor& WithParentConstuctor(
      std::vector<std::pair<const TypeDescriptionStructure*, std::vector<std::variant<ArgName, Value>>>>
          constructors);

  StructureConstructor& WithListInitializedArgs(std::vector<std::tuple<ArgName, FieldName>> list_init_args);

  StructureConstructor& WithAdditionalInitializations(std::vector<std::pair<FieldName, Value>> inits);

  StructureConstructor& WithBody(std::string_view body);
};

//! \brief Type description for a function.
struct FunctionType : TypeDescription {
  FunctionType() : TypeDescription(TSGeneralType::Function) {}

  //! \brief The function's arguments.
  std::vector<ElaboratedType> arguments {};

  //! \brief The return type of the function, or nothing (for "void").
  std::optional<ElaboratedType> return_type {};

  bool IsValid() const {
    for (const auto& arg : arguments) {
      if (!arg.arg_type) return false;
    }
    if (return_type && !return_type->arg_type) {
      return false;
    }
    return true;
  }

  auto operator<=>(const FunctionType& other) const {
    return std::tie(arguments, return_type) <=> std::tie(other.arguments, other.return_type);
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

  FunctionType& VoidFunction() {
    return_type = {};
    return *this;
  }
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
  FunctionType function_type {};

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
    MANTA_REQUIRE(argument_names.size() == function_type.arguments.size(),
                  "mismatch between number of argument names and number of arguments");
    MANTA_REQUIRE(function_type.IsValid(), "invalid function type");
    MANTA_REQUIRE(!function_name.empty(), "function name cannot be empty");
  }

  //! \brief Create a copy of the function and bind it to the structure.
  //!
  //! Adds the function to the structure, and sets the structure as the binder in the StructureFunction
  //! instance. Returns a reference to the newly created bound function value inside the structure.
  StructureFunction& BindToStructure(TypeDescriptionStructure* structure,
                                     bool is_const,
                                     bool is_override) const;

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

  FunctionValue& WithType(FunctionType type) {
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
  const TypeDescriptionStructure* binding_structure {};

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

//! \brief  Type description for a structure, or compound type.
//!
//! This is the most complex type, as I also allow it to have constructors which can do different types of
//! initializations.
struct TypeDescriptionStructure final : TypeDescription {
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

  //! \brief Check whether the structure contains a field of a particular name.
  NO_DISCARD bool ContainsField(const std::string& name) const;

  auto GetField(const std::string& field_name) {
    return std::ranges::find_if(fields,
                                [&field_name](const auto& field) { return field.first == field_name; });
  }

  const TypeDescription*& GetFieldType(const std::string& field_name) {
    auto it =
        std::ranges::find_if(fields, [&field_name](const auto& field) { return field.first == field_name; });
    MANTA_ASSERT(it != fields.end(), "field '" << field_name << "' not found in " << type_name);
    return it->second;
  }

  void SetFieldType(const std::string& field_name, const TypeDescription* type) {
    if (auto it = GetField(field_name); it != fields.end()) {
      it->second = type;
    }
    else {
      fields.emplace_back(field_name, type);
    }
  }

  //! \brief The name of the type.
  const std::string type_name;

  //! \brief The fields of the structure. Using a vector instead of a field so you can control the order of
  //!        the fields.
  std::vector<std::pair<std::string, const TypeDescription*>> fields;

  //! \brief The set of base classes of the structure.
  //!
  //! Stored as a vector so we don't have pointer values influencing the order of the classes between
  //! different runs. The AddParent function ensures that we don't add the same parent class twice.
  std::vector<const TypeDescriptionStructure*> parent_classes;

  //! \brief Constructors for the structure. Storing as a deque so that we don't invalidate pointers.
  std::deque<StructureConstructor> constructors;

  //! \brief Functions for the structure. Storing as a deque so that we don't invalidate pointers.
  std::deque<StructureFunction> functions;

  //! \brief Field for injecting arbitrary user defined code into the structure. This could include things
  //!        like additional function or variable definitions.
  std::string adhoc_code {};
};

inline StructureFunction& FunctionValue::BindToStructure(TypeDescriptionStructure* structure,
                                                         bool is_const,
                                                         bool is_override) const {
  StructureFunction function {*this};
  function.is_const_function = is_const;
  function.is_override       = is_override;
  function.binding_structure = structure;
  return structure->AddFunction(function);
}


//! \brief Represents a vector or ordered collection of objects of some other type.
struct TypeDescriptionVector final : BasicTypeDescription {
  explicit TypeDescriptionVector(const TypeDescription* element_type);

  NO_DISCARD std::string Write() const override;

  NO_DISCARD std::size_t HashStructure() const override;

  NO_DISCARD std::size_t HashID() const override;

  static std::size_t PotentialHashID(const TypeDescription* vector_type);

  const TypeDescription* element_type {};
};

//! \brief Represents a shared pointer to an object of some other type.
struct TypeDescriptionSharedPointer final : BasicTypeDescription {
  explicit TypeDescriptionSharedPointer(const TypeDescription* type);

  NO_DISCARD std::string Write() const override;

  NO_DISCARD std::size_t HashStructure() const override;

  NO_DISCARD std::size_t HashID() const override;

  static std::size_t PotentialHashID(const TypeDescription* pointed_type);

  const TypeDescription* pointed_type {};
};

//! \brief Represents an enumeration type.
struct TypeDescriptionEnum final : BasicTypeDescription {
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
struct TypeDescriptionString final : TypeDescription {
  TypeDescriptionString() : TypeDescription(TSGeneralType::String) {}
};

//! \brief Represents an integer field.
struct TypeDescriptionInteger final : TypeDescription {
  TypeDescriptionInteger() : TypeDescription(TSGeneralType::Integer) {}
};

//! \brief Represents an floating point field.
struct TypeDescriptionFloat final : TypeDescription {
  TypeDescriptionFloat() : TypeDescription(TSGeneralType::Float) {}
};

//! \brief Represents some custom, user defined type.
//!
//! This is useful for things like defining functions in arbitrary code, like reduction functions, that return
//! some language specific type.
struct TypeDescriptionCustom final : TypeDescription {
  explicit TypeDescriptionCustom(const std::string& type_declaration) noexcept
      : TypeDescription(TSGeneralType::Custom)
      , type_declaration(type_declaration) {}

  NO_DISCARD std::string Write() const override { return type_declaration; }
  NO_DISCARD std::size_t HashStructure() const override { return HashID(); }
  NO_DISCARD std::size_t HashID() const override { return std::hash<std::string> {}(type_declaration); }

  //! \brief The declaration of the type in the native language.
  std::string type_declaration;
};

//! \brief A class that manages a universe of type.
//!
//! Allows for the creation of types, generally by composing other types from the TypeSystem into new types,
//! like vectors of types, pointers to types, structures with other types as fields, etc.
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

  //! \brief Create a custom type.
  const TypeDescriptionCustom* Custom(const std::string& type_declaration);

private:
  //! \brief The string type.
  const TypeDescriptionString string_type_;

  //! \brief The integer type.
  const TypeDescriptionInteger integer_type_;

  //! \brief The floating point type.
  const TypeDescriptionFloat float_type_;

  //! \brief All types managed by the typesystem.
  std::map<std::size_t, std::shared_ptr<TypeDescription>> types_;
};

//! \brief Add a little color formatter for logging TypeDescriptions.
inline void format_logstream(const TypeDescription& type_description, lightning::RefBundle& handler) {
  handler << lightning::AnsiColor8Bit(type_description.Write(),
                                      lightning::formatting::AnsiForegroundColor::BrightBlue);
}

}  // namespace manta