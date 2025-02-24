//
// Created by Nathaniel Rupprecht on 12/28/24.
//

#include "manta/generator/codegen/CodeGen.h"

namespace manta::codegen {

void CppCodegen::WriteName(std::ostream& out,
                           const typesystem::GeneralizedType* type,
                           std::span<std::string> var_names) {
  // Make this work more easily with recursion with the substituted types, which use nullptr to represent no
  // substitution.
  if (!type) {
    MANTA_ASSERT(var_names.size() == 1, "expected exactly one variable name");
    out << var_names[0];
    return;
  }

  switch (type->GetPrototypeType()) {
    case typesystem::PrototypeType::BASIC: {
      const auto basic = dynamic_cast<const typesystem::BasicPrototype*>(type->GetPrototype());
      out << basic->GetName();
      break;
    }
    case typesystem::PrototypeType::OBJECT: {
      const auto object = dynamic_cast<const typesystem::ObjectPrototype*>(type->GetPrototype());
      if (!object->GetNamespaceName().empty()) {
        out << object->GetNamespaceName() << "::";
      }
      out << object->GetObjectName();
      if (0 < object->GetNumTypeVars()) {
        out << "<";
        for (std::size_t i = 0; i < object->GetNumTypeVars(); ++i) {
          if (i != 0) {
            out << ", ";
          }
          auto susbt_names = type->Select(var_names, i);
          WriteName(out, type->GetSubstitution(i), susbt_names);
        }
        out << ">";
      }
      break;
    }
    case typesystem::PrototypeType::ENUM: {
      const auto enum_proto = dynamic_cast<const typesystem::EnumeratedPrototype*>(type->GetPrototype());
      if (!enum_proto->GetNamespaceName().empty()) {
        out << enum_proto->GetNamespaceName() << "::";
      }
      out << enum_proto->GetEnumName();
      break;
    }
    case typesystem::PrototypeType::FUNCTION: {
      break;
    }
  }
}

void CppCodegen::WriteName(std::ostream& out,
                           const typesystem::ElaboratedType& elaborated_type,
                           std::span<std::string> var_names) {

  if (elaborated_type.is_const) {
    out << "const ";
  }
  WriteName(out, elaborated_type.type, var_names);
  if (elaborated_type.is_ref) {
    out << "&";
  }
}

void CppCodegen::WriteDeclaration(std::ostream& out, const typesystem::GeneralizedType* type) {
  MANTA_REQUIRE(type, "type must be valid to write declaration");

  switch (type->GetPrototypeType()) {
    case typesystem::PrototypeType::BASIC: {
      MANTA_FAIL("cannot write a declaration for a basic type");
    }
    case typesystem::PrototypeType::OBJECT: {
      MANTA_ASSERT(!type->HasSubstitutions(),
                   "C++ type declarations cannot be written for a partial specialization");

      const auto basic = dynamic_cast<const typesystem::ObjectPrototype*>(type->GetPrototype());
      if (0 < basic->GetNumTypeVars()) {
        out << "template <";
        for (std::size_t i = 0; i < basic->GetNumTypeVars(); ++i) {
          if (i != 0) {
            out << ", ";
          }
          out << "typename T" << i << "_t";
        }
        out << ">\n";
      }
      out << "class " << basic->GetObjectName() << ";";
      break;
    }
    case typesystem::PrototypeType::ENUM: {
      const auto enum_proto = dynamic_cast<const typesystem::EnumeratedPrototype*>(type->GetPrototype());
      out << "enum " << enum_proto->GetEnumName() << ";";
      break;
    }
    case typesystem::PrototypeType::FUNCTION: {
      break;
    }
  }
}

void CppCodegen::WriteDefinition(std::ostream& out, const typesystem::GeneralizedType* type) {
  MANTA_REQUIRE(type, "type must be valid to write definition");
  MANTA_REQUIRE(!type->HasSubstitutions(), "cannot define a partial specialization");

  switch (type->GetPrototypeType()) {
    case typesystem::PrototypeType::BASIC: {
      MANTA_FAIL("cannot write a definition for a basic type");
    }
    case typesystem::PrototypeType::OBJECT: {
      const auto basic = dynamic_cast<const typesystem::ObjectPrototype*>(type->GetPrototype());
      writeObjectDefinition(out, type, basic);
      break;
    }
    case typesystem::PrototypeType::ENUM: {
      const auto enum_proto = dynamic_cast<const typesystem::EnumeratedPrototype*>(type->GetPrototype());
      writeEnumDefinition(out, type, enum_proto);
      break;
    }
    case typesystem::PrototypeType::FUNCTION: {
      break;
    }
  }
}

void CppCodegen::writeEnumDefinition(std::ostream& out,
                                     const typesystem::GeneralizedType* type,
                                     const typesystem::EnumeratedPrototype* enumeration) {
  MANTA_REQUIRE(type->GetPrototype() == enumeration, "type's prototype must be the prototype");

  // C++ only supports enums where all the enum possibilities have the same type. Check whether this
  // is possible. Otherwise, we have to do something more complex.

  auto options = enumeration->GetOptions();
  if (options.empty()) {
    out << "enum class " << enumeration->GetEnumName() << "{};\n";
    return;
  }

  // Check whether the types are all the same.
  auto first_type = options.begin()->second;
  auto all_same   = [&]() {
    for (auto [name, type] : options) {
      if (type != first_type) {
        return false;
      }
    }
    return true;
  }();

  if (all_same) {
    out << "enum class " << enumeration->GetEnumName() << " : ";
    WriteName(out, first_type);
    out << " {\n";
    for (auto [name, type] : options) {
      out << "  " << name << ",\n";
    }
    out << "};\n";
    return;
  }

  MANTA_FAIL("TODO: implement");
  out << "struct " << enumeration->GetEnumName() << " {\n";
}

void CppCodegen::writeObjectDefinition(std::ostream& out,
                                       const typesystem::GeneralizedType* type,
                                       const typesystem::ObjectPrototype* object) {
  MANTA_REQUIRE(type->GetPrototype() == object, "type's prototype must be the prototype");

  std::vector<std::string> free_var_names;
  std::vector<std::string> parent_free_var_names;
  if (0 < type->GetNumFreeVars()) {
    out << "template <";
    for (std::size_t i = 0; i < type->GetNumFreeVars(); ++i) {
      if (i != 0) {
        out << ", ";
      }
      free_var_names.push_back(std::format("T{}_t", i));
      out << free_var_names.back();
    }
    out << ">\n";
  }
  out << "class " << object->GetObjectName();
  // Parent classes.
  auto& parent_types = object->GetParentTypes();
  if (!parent_types.empty()) {
    out << " : ";
    for (auto& [type, bindings] : parent_types) {
      out << " public ";
      // Bind free variables in the parents to the free variables in the object.
      parent_free_var_names.clear();
      for (std::size_t i = 0; i < bindings.size(); ++i) {
        parent_free_var_names.push_back(free_var_names[bindings[i]]);
      }
      WriteName(out, type, parent_free_var_names);
      out << ", ";
    }
  }
  out << " {\n";

  // Write constructor(s).

  // Write functions.

  // Write fields.
  for (auto& field : object->GetFields()) {
    out << "  ";
    WriteName(out, field.type, free_var_names);
    out << " " << field.field_name << ";\n";
  }

  out << "};\n";
}


}  // namespace manta::codegen
