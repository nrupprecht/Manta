//
// Created by Nathaniel Rupprecht on 5/14/23.
//

#pragma once

#include "manta/generator/typesystem/TypeSystem.h"

namespace manta {

class CodeGen {
public:
  virtual void WriteImports(std::ostream& out) const = 0;

  //! \brief Write the definition of a type.
  virtual void WriteDefinition(std::ostream& out, const TypeDescriptionStructure* structure) const = 0;

  virtual void WriteDefinition(std::ostream& out, const TypeDescriptionEnum* enumeration) const = 0;

  //! \brief Create and write a function that serializes enum values to the enum option name.
  virtual void GenerateEnumToStringFunction(std::ostream& out,
                                            const TypeDescriptionEnum* enumeration) const = 0;

  //! \brief  Write the name of a type.
  virtual std::string WriteName(const TypeDescription* type) const = 0;
  //! \brief Write the name of an elaborated type.
  virtual std::string WriteName(const ElaboratedType& type) const = 0;

  virtual void AddComment(std::ostream& out, const std::string& comment, bool newline = true) const = 0;
  virtual void AddComment(std::ostream& out,
                          int tab_indents,
                          const std::string& comment,
                          bool newline = true) const = 0;

  //! \brief Add a vertical break, e.g. one or more newlines.
  virtual void AddBreak(std::ostream& out) const = 0;
};

}  // namespace manta
