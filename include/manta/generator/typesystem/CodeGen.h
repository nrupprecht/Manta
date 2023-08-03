//
// Created by Nathaniel Rupprecht on 5/14/23.
//

#pragma once

#include "manta/generator/typesystem/TypeSystem.h"

namespace manta {

class CodeGen {
public:
  //! \brief Write the definition of a type.
  virtual void WriteDefinition(std::ostream& out,
                               const TypeDescriptionStructure* structure) const = 0;

  virtual void WriteDefinition(std::ostream& out,
                               const TypeDescriptionEnum* enumeration) const = 0;

  //! \brief  Write the name of a type.
  virtual std::string WriteName(const TypeDescription* type) const = 0;
  //! \brief Write the name of an elaborated type.
  virtual std::string WriteName(const ElaboratedType& type) const = 0;
};

}  // namespace manta
