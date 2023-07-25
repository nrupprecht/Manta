#pragma once

#include "manta/generator/typesystem/CodeGen.h"

namespace manta {

//! \brief Class that generates C++ code
class CppCodeGen : public CodeGen {
public:
  void WriteDefinition(std::ostream& out,
                       const TypeDescriptionStructure* structure) const override;

  void WriteDefinition(std::ostream& out,
                       const TypeDescriptionEnum* enumeration) const override;

  std::string WriteName(const TypeDescription* type) const override;
};

}  // namespace manta
