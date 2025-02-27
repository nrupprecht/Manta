//
// Created by Nathaniel Rupprecht on 7/24/23.
//

#include "manta/generator/typesystem/CppCodegen.h"
// Other files.
#include <iterator>

#include "manta/utility/Formatting.h"

using namespace std::literals;

namespace manta {

namespace {

bool bodyIsShort(std::string_view body) {
  while (!body.empty() && std::isspace(body.front())) {
    body.remove_prefix(1);
  }
  while (!body.empty() && std::isspace(body.back())) {
    body.remove_suffix(1);
  }
  // Count number of newlines within the body.
  const auto num_newlines = std::ranges::count(body, '\n');
  return body.size() < 80 && num_newlines == 0;
}

void writeNoSpaces(std::ostream& out, std::string_view body) {
  while (!body.empty() && std::isspace(body.front())) {
    body.remove_prefix(1);
  }
  while (!body.empty() && std::isspace(body.back())) {
    body.remove_suffix(1);
  }
  out << body;
}

}  // namespace

void CppCodeGen::WriteHeader(std::ostream& out) const {
  out << "#pragma once\n\n";
}

void CppCodeGen::WriteImports(std::ostream& out) const {
  // Write the guard and includes.
  WriteImport(out, "vector", true);
  WriteImport(out, "string", true);

  AddComment(out, 0, " Include the support for the parser."s);
  WriteImport(out, "manta/generator/ParserDriver.h", false);
  WriteImport(out, "manta/generator/LexerGenerator.h", false);
  WriteImport(out, "Lightning/Lightning.h", true);
  WriteImport(out, "manta/utility/Formatting.h", false);
  out << "\n\n";
}

void CppCodeGen::WriteDefinition(std::ostream& out, const TypeDescriptionStructure* structure) const {
  MANTA_REQUIRE(structure, "cannot write the declaration for a null type structure description pointer");

  out << "struct " << structure->type_name;
  // Write any base classes.
  if (!structure->parent_classes.empty()) {
    int count = 0;
    for (auto& parent : structure->parent_classes) {
      if (structure->parent_classes.size() != 1) {
        out << "\n  ";
      }
      else {
        out << " ";
      }
      out << (count == 0 ? ": " : ", ");
      out << WriteName(parent);
      ++count;
    }
  }
  out << " {\n";

  // Write constructor(s).
  for (auto& constructor : structure->constructors) {
    out << "  ";
    // Making the choice to make all single argument constructors explicit.
    if (constructor.arguments.size() == 1) {
      out << "explicit ";
    }
    out << structure->type_name << "(";
    auto i = 0u;
    for (auto& [type, arg_name] : constructor.arguments) {
      if (i != 0) out << ", ";
      out << "const " << WriteName(type) << "& " << arg_name;
      ++i;
    }
    out << ")\n";
    bool has_list_init = false;
    // Write out any calls to parent class constructors.
    if (!constructor.parent_constructors.empty()) {
      has_list_init = true;
      out << "    : ";
      i = 0;
      for (auto& [name, arguments] : constructor.parent_constructors) {
        if (i != 0) {
          out << ", ";
        }
        out << WriteName(name) << "(";
        int j = 0;
        for (auto& arg : arguments) {
          if (j != 0) {
            out << ", ";
          }
          if (arg.index() == 0) {
            out << std::get<0>(arg);
          }
          else {
            // TODO: Way to change values into code in target language other than "printing" them.
            out << std::get<1>(arg).literal;
          }
          ++j;
        }
        out << ")";
        ++i;
      }
    }
    if (!constructor.list_initialized_args.empty()) {
      if (!has_list_init)
        out << "    : ";
      else
        out << ", ";
      has_list_init = true;
      i             = 0;
      for (auto& [arg_name, field_name] : constructor.list_initialized_args) {
        if (i != 0) out << ", ";
        out << field_name << "(" << arg_name << ")";
        ++i;
      }
    }
    if (!constructor.additional_initializations.empty()) {
      if (!has_list_init)
        out << "    : ";
      else
        out << ", ";
      i = 0;
      for (auto& [field_name, value] : constructor.additional_initializations) {
        if (i != 0) out << ", ";
        // TODO: Way to change values into code in target language other than "printing"
        // them.
        out << field_name << "(" << value.literal << ")";
        ++i;
      }
    }

    out << " {}\n";
  }

  // Add a virtual destructor. For now, adding to all structs that do not inherit from other structs.
  if (structure->parent_classes.empty()) {
    out << "  virtual ~" << structure->type_name << "() = default;\n";
  }

  if (!structure->constructors.empty()) {
    AddBreak(out);
  }

  // Write all fields.
  for (auto& [field, type] : structure->fields) {
    out << "  " << WriteName(type) << " " << field << "{};\n";
  }
  if (!structure->fields.empty()) {
    AddBreak(out);
  }

  // Write all functions.
  for (auto& function : structure->functions) {
    out << "  ";
    if (function.IsVirtual()) {
      out << "virtual ";
    }
    if (function.function_type.return_type) {
      out << WriteName(*function.function_type.return_type);
    }
    else {
      out << "void";
    }
    out << " " << function.function_name << "(";
    int count = 0;
    for (auto& argument_name : function.argument_names) {
      if (count != 0) {
        out << ", ";
      }
      if (mark_params_as_maybe_unused_) {
        out << "[[maybe_unused]] ";
      }
      out << WriteName(function.function_type.arguments[count]) << " " << argument_name;
      // out << WriteName(function.function_type.arguments[count].arg_type) << " " << argument_name;
      ++count;
    }
    out << ")";
    if (function.is_const_function) {
      out << " const";
    }
    if (function.is_override) {
      out << " override";
    }
    if (function.IsVirtual()) {
      out << " = 0;\n";
    }
    else {
      if (function.function_body->empty()) {
        out << " { }\n";
      }
      else {
        out << " { ";

        if (bodyIsShort(*function.function_body)) {
          writeNoSpaces(out, *function.function_body);
          out << " ";
        }
        else {
          out << "\n";
          // Write function body. Indent after every newline. function_body is not nullopt, since the function
          // is not virtual.
          out << "    ";  // Indent.
          for (auto c : *function.function_body) {
            out << c;
            if (c == '\n') {
              out << "    ";
            }
          }
          // Newline and indent for the closing '}'
          out << "\n  ";
        }
        out << "}\n";
      }
    }
  }

  if (!structure->adhoc_code.empty()) {
    out << "\n  // Ad-hoc code.\n";
    // Format the code so it is indented correctly.
    writeBody(out, structure->adhoc_code);
    out << "\n  // End ad-hoc code.\n";
  }

  out << "};";
  AddBreak(out);
}

void CppCodeGen::WriteDefinition(std::ostream& out, const TypeDescriptionEnum* enumeration) const {
  out << "enum class " << enumeration->GetName() << "{\n";
  for (auto& option : enumeration->GetOptions()) {
    out << "  " << option << ",\n";
  }
  out << "};\n";
}

void CppCodeGen::GenerateEnumToStringFunction(std::ostream& out,
                                              const TypeDescriptionEnum* enumeration) const {
  // TODO: Could express this as an actual function objects, is that better?
  out << "inline const char* to_string(" << enumeration->GetName() << " type) {\n";
  out << "  switch (type) {\n";
  for (auto& option : enumeration->GetOptions()) {
    out << "  case " << enumeration->GetName() << "::" << option << ":\n";
    out << "    return \"" << option << "\";\n";
  }
  // Default case - failure.
  AddComment(out, 1, " Default case for unrecognized enums.");
  out << "  default:\n     MANTA_FAIL(\"unrecognized enumeration\");\n";
  out << "  }\n}\n";
}

std::string CppCodeGen::WriteName(const TypeDescription* type) const {
  MANTA_REQUIRE(type, "cannot write the name of a null type description pointer");
  switch (type->general_type) {
    case TSGeneralType::Vector: {
      auto vtype = dynamic_cast<const TypeDescriptionVector*>(type);
      return "std::vector<" + WriteName(vtype->element_type) + ">";
    }
    case TSGeneralType::SharedPointer: {
      auto ptype = dynamic_cast<const TypeDescriptionSharedPointer*>(type);
      return "std::shared_ptr<" + WriteName(ptype->pointed_type) + ">";
    }
    case TSGeneralType::String: {
      return "std::string";
    }
    case TSGeneralType::Integer: {
      return "int";
    }
    case TSGeneralType::Float: {
      return "double";
    }
    case TSGeneralType::Structure: {
      auto stype = dynamic_cast<const TypeDescriptionStructure*>(type);
      return stype->type_name;
    }
    case TSGeneralType::Enumeration: {
      auto etype = dynamic_cast<const TypeDescriptionEnum*>(type);
      return etype->GetName();
    }
    case TSGeneralType::Custom: {
      auto ctype = dynamic_cast<const TypeDescriptionCustom*>(type);
      return ctype->type_declaration;
    }
    default: {
      MANTA_FAIL("unrecognized type description general type");
    }
  }
}

std::string CppCodeGen::WriteName(const ElaboratedType& type) const {
  std::string output;
  if (type.is_const) {
    output = "const ";
  }
  output += WriteName(type.arg_type);
  if (type.is_ref) {
    output += "&";
  }
  return output;
}

void CppCodeGen::WriteImport(std::ostream& out, const std::string& name, bool is_external) const {
  out << "#include " << (is_external ? "<"s : "\""s) << name << (is_external ? ">"s : "\""s) << "\n";
}

void CppCodeGen::AddComment(std::ostream& out, const std::string& comment, bool newline) const {
  out << "//" << comment;
  if (newline) {
    out << "\n";
  }
}

void CppCodeGen::AddComment(std::ostream& out,
                            int tab_indents,
                            const std::string& comment,
                            bool newline) const {
  // Pad with 2 spaces per indent.
  std::fill_n(std::ostream_iterator<char>(out), 2 * tab_indents, ' ');
  AddComment(out, comment, newline);
}

void CppCodeGen::AddBreak(std::ostream& out) const {
  out << "\n";
}

void CppCodeGen::writeFreeFunctionDefinition(std::ostream& out, const FunctionValue* function) const {
  writeFunction(out, function);
}

void CppCodeGen::writeStructureFunctionDefinition(std::ostream& out,
                                                  const StructureFunction* function) const {
  writeFunction(out, function, function->binding_structure->type_name, function->is_const_function);
}

void CppCodeGen::writeFunction(std::ostream& out,
                               const FunctionValue* function,
                               std::string_view structure_name,
                               bool is_const) const {
  function->Validate();

  if (auto type = function->function_type.return_type) {
    out << WriteName(*function->function_type.return_type);
  }
  else {
    out << "void";
  }
  out << " ";
  if (!structure_name.empty()) {
    // Qualify name by class.
    out << structure_name << "::";
  }
  out << function->function_name << "(";
  for (std::size_t i = 0; i < function->GetArity(); ++i) {
    if (i != 0) {
      out << ", ";
    }
    if (mark_params_as_maybe_unused_) {
      out << "[[maybe_unused]] ";
    }
    out << WriteName(function->function_type.arguments[i]) << " " << function->argument_names[i];
  }
  out << ")";
  if (!structure_name.empty() && is_const) {
    out << " const";
  }
  out << " {";
  if (function->function_body && !function->function_body->empty()) {
    // Check if the function should be written in a single line

    if (bodyIsShort(*function->function_body)) {
      out << " ";
      writeNoSpaces(out, *function->function_body);
      out << " ";
    }
    else {
      out << "\n";
      writeBody(out, *function->function_body);
      out << "\n";
    }
  }
  out << "}";
}

void CppCodeGen::writeBody(std::ostream& out, std::string_view body) const {
  formatting::HandleIndentation(out, body, 2, true, true, true, true);
}

}  // namespace manta