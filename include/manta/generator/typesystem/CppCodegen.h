#pragma once

#include "manta/generator/typesystem/CodeGen.h"

namespace manta {

//! \brief Class that generates C++ code
class CppCodeGen : public CodeGen {
public:
  void WriteImports(std::ostream& out) const override;

  void WriteDefinition(std::ostream& out, const TypeDescriptionStructure* structure) const override;

  void WriteDefinition(std::ostream& out, const TypeDescriptionEnum* enumeration) const override;

  void GenerateEnumToStringFunction(std::ostream& out, const TypeDescriptionEnum* enumeration) const override;

  std::string WriteName(const TypeDescription* type) const override;
  std::string WriteName(const ElaboratedType& type) const override;

  void AddComment(std::ostream& out, const std::string& comment, bool newline = true) const override;
  void AddComment(std::ostream& out,
                  int tab_indents,
                  const std::string& comment,
                  bool newline = true) const override;

  void AddBreak(std::ostream& out) const override;
};

}  // namespace manta
