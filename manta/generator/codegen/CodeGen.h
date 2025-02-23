//
// Created by Nathaniel Rupprecht on 12/28/24.
//

#pragma once

#include <span>
#include "manta/generator/typesystem/HigherTypeSystem.h"

namespace manta::codegen {

class CppCodegen {
public:
  void WriteName(std::ostream& out,
                 const typesystem::GeneralizedType* type,
                 std::span<std::string> var_names = {});

  void WriteName(std::ostream& out,
                 const typesystem::ElaboratedType& elaborated_type,
                 std::span<std::string> var_names = {});

  void WriteDeclaration(std::ostream& out, const typesystem::GeneralizedType* type);

  void WriteDefinition(std::ostream& out, const typesystem::GeneralizedType* type);

private:
  void writeEnumDefinition(std::ostream& out,
                           const typesystem::GeneralizedType* type,
                           const typesystem::EnumeratedPrototype* enumeration);

  void writeObjectDefinition(std::ostream& out,
                             const typesystem::GeneralizedType* type,
                             const typesystem::ObjectPrototype* object);
};

}  // namespace manta::codegen