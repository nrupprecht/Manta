//
// Created by Nathaniel Rupprecht on 5/14/23.
//

#pragma once

#include "manta/generator/typesystem/TypeSystem.h"

namespace manta {

class CodeGen {
public:
  virtual ~CodeGen() = default;

  //! \brief Write any header for a file, e.g. #pragma once.
  //!
  //! TODO: This should probably be separated into some other class, codegen just knows how to generate code,
  //!       not what to include for the purpose of parsing.
  virtual void WriteHeader(std::ostream& out) const = 0;

  //! \brief Write standard imports to the file.
  //!
  //! TODO: This should probably be separated into some other class, codegen just knows how to generate code,
  //!       not what to include for the purpose of parsing.
  virtual void WriteImports(std::ostream& out) const = 0;

  //! \brief Write the definition of a type.
  virtual void WriteDefinition(std::ostream& out, const TypeDescriptionStructure* structure) const = 0;

  virtual void WriteDefinition(std::ostream& out, const TypeDescriptionEnum* enumeration) const = 0;

  void WriteDefinition(std::ostream& out, const FunctionValue* function) const {
    if (auto structure_function = dynamic_cast<const StructureFunction*>(function)) {
      writeStructureFunctionDefinition(out, structure_function);
    }
    else {
      writeFreeFunctionDefinition(out, function);
    }
  }

  void WriteDefinition(std::ostream& out, const StructureFunction* function) const {
    writeStructureFunctionDefinition(out, function);
  }

  //! \brief Create and write a function that serializes enum values to the enum option name.
  virtual void GenerateEnumToStringFunction(std::ostream& out,
                                            const TypeDescriptionEnum* enumeration) const = 0;

  //! \brief  Write the name of a type.
  virtual std::string WriteName(const TypeDescription* type) const = 0;

  //! \brief Write the name of an elaborated type.
  virtual std::string WriteName(const ElaboratedType& type) const = 0;

  //! \brief Write an import or include statement for a file or module.
  virtual void WriteImport(std::ostream& out, const std::string& name, bool is_external) const = 0;

  //! \brief Add a comment to the output.
  virtual void AddComment(std::ostream& out, const std::string& comment, bool newline = true) const = 0;

  //! \brief More advanced comment with tab indents.
  virtual void AddComment(std::ostream& out,
                          int tab_indents,
                          const std::string& comment,
                          bool newline = true) const = 0;

  //! \brief Add a vertical break, e.g. one or more newlines.
  virtual void AddBreak(std::ostream& out) const = 0;

protected:
  virtual void writeFreeFunctionDefinition(std::ostream& out, const FunctionValue* function) const = 0;
  virtual void writeStructureFunctionDefinition(std::ostream& out, const StructureFunction* function) const = 0;
};

}  // namespace manta
