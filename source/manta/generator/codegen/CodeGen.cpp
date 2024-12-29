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
      const auto basic = dynamic_cast<const typesystem::ObjectPrototype*>(type->GetPrototype());
      out << basic->GetObjectName();
      if (0 < basic->GetNumTypeVars()) {
        out << "<";
        for (std::size_t i = 0; i < basic->GetNumTypeVars(); ++i) {
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
      out << enum_proto->GetEnumName();
      break;
    }
    case typesystem::PrototypeType::FUNCTION: {
      break;
    }
  }
}

void CppCodegen::WriteDeclaration(std::ostream& out, const typesystem::GeneralizedType* type) {
  MANTA_REQUIRE(type, "type must be valid to write declaration");

  switch (type->GetPrototypeType()) {
    case typesystem::PrototypeType::BASIC: {
      MANTA_FAIL("cannot write a declaration for a basic type");
    }
    case typesystem::PrototypeType::OBJECT: {
      MANTA_ASSERT(!type->HasSubstitutions(), "C++ type declarations cannot be written for a partial specialization");

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
      out << "class " << basic->GetObjectName()  << ";";
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

}  // namespace manta::codegen
