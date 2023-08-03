//
// Created by Nathaniel Rupprecht on 7/24/23.
//

#include "manta/generator/typesystem/CppCodegen.h"
// Other files.

namespace manta {

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
      out << "public " << WriteName(parent);
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
      if (i != 0)
        out << ", ";
      out << "const " << WriteName(type) << "& " << arg_name;
      ++i;
    }
    out << ")\n";
    bool has_list_init = false;
    if (!constructor.parent_constructors.empty()) {
      has_list_init = true;
      out << "    : ";
      i = 0;
      for (auto& pconst : constructor.parent_constructors) {
        if (i != 0) {
          out << ", ";
        }
        out << WriteName(pconst.first) << "(";
        for (auto& arg : pconst.second) {
          if (arg.index() == 0) {
            out << std::get<0>(arg);
          }
          else {
            // TODO: Way to change values into code in target language other than
            // "printing" them.
            out << std::get<1>(arg).literal;
          }
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
      i = 0;
      for (auto& [arg_name, field_name] : constructor.list_initialized_args) {
        if (i != 0)
          out << ", ";
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
        if (i != 0)
          out << ", ";
        // TODO: Way to change values into code in target language other than "printing"
        // them.
        out << field_name << "(" << value.literal << ")";
        ++i;
      }
    }

    out << "\n  {}\n\n";
  }

  // Write all fields.
  for (auto& [field, type] : structure->fields) {
    out << "  " << WriteName(type) << " " << field << ";\n";
  }
  out << "\n";

  // Write all functions.
  for (auto& function : structure->functions) {
    out << "  ";
    if (function.IsVirtual()) {
      out << "virtual ";
    }
    if (function.function_signature.return_type) {
      out << WriteName(*function.function_signature.return_type);
    }
    else {
      out << "void";
    }
    out << " " << function.function_name << "(";
    int count = 0;
    for (auto& arg : function.function_signature.arguments) {
      if (count != 0) {
        out << ", ";
      }
      out << WriteName(arg.arg_type) << " " << arg.argument_name;
      ++count;
    }
    out << ")" ;
    if (function.function_signature.is_const) {
      out << " const";
    }
    if (function.is_override) {
      out << " override";
    }
    if (function.IsVirtual()) {
      out << " = 0;\n\n";
    }
    else {
      out << " {\n";
      // Write function body.
      out << *function.function_body;
      // Write closing }
      out << "\n  }\n\n";
    }
  }

  out << "};\n";
}

void CppCodeGen::WriteDefinition(std::ostream& out, const TypeDescriptionEnum* enumeration) const {
  out << "enum class " << enumeration->GetName() << "{\n";
  for (auto& option : enumeration->GetOptions()) {
    out << "  " << option << ",\n";
  }
  out << "};\n";
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
    case TSGeneralType::Structure: {
      auto stype = dynamic_cast<const TypeDescriptionStructure*>(type);
      return stype->type_name;
    }
    case TSGeneralType::Enumeration: {
      auto etype = dynamic_cast<const TypeDescriptionEnum*>(type);
      return etype->GetName();
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

}  // namespace manta