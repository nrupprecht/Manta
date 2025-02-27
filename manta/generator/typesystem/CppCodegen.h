#pragma once

#include "manta/generator/typesystem/CodeGen.h"

namespace manta {

//! \brief Class that generates C++ code
class CppCodeGen : public CodeGen {
public:
  //! \brief Write any header for a file, e.g. #pragma once.
  void WriteHeader(std::ostream& out) const override;

  void WriteImports(std::ostream& out) const override;

  void WriteDefinition(std::ostream& out, const TypeDescriptionStructure* structure) const override;

  void WriteDefinition(std::ostream& out, const TypeDescriptionEnum* enumeration) const override;

  using CodeGen::WriteDefinition;

  void GenerateEnumToStringFunction(std::ostream& out, const TypeDescriptionEnum* enumeration) const override;

  std::string WriteName(const TypeDescription* type) const override;
  std::string WriteName(const ElaboratedType& type) const override;

  void WriteImport(std::ostream& out, const std::string& name, bool is_external) const override;

  void AddComment(std::ostream& out, const std::string& comment, bool newline = true) const override;
  void AddComment(std::ostream& out,
                  int tab_indents,
                  const std::string& comment,
                  bool newline = true) const override;

  void AddBreak(std::ostream& out) const override;

  // C++ specific configs

  void SetMarkParamsAsMaybeUnused(bool flag) {
    mark_params_as_maybe_unused_ = flag;
  }

protected:
  // =========================================================================================================
  // Protected implementations.
  // =========================================================================================================

  void writeFreeFunctionDefinition(std::ostream& out, const FunctionValue* function) const override;
  void writeStructureFunctionDefinition(std::ostream& out, const StructureFunction* function) const override;

private:
  // =========================================================================================================
  // Private helper functions.
  // =========================================================================================================

  //! \brief Helper function to write either a free or bound function.
  //!
  //! Whether the function is free or bound is determined by whether the structure name is empty or not.
  void writeFunction(std::ostream& out,
                     const FunctionValue* function,
                     std::string_view structure_name = {},
                     bool is_const                   = false) const;

  void writeBody(std::ostream& out, std::string_view body) const;

  bool mark_params_as_maybe_unused_ = true;
};

}  // namespace manta
