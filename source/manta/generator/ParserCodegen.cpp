#include "manta/generator/ParserCodegen.h"

#include <manta/generator/typesystem/CppCodegen.h>
// Other files.
#include <Lightning/Lightning.h>

#include "manta/generator/typesystem/TypeCreation.h"

using namespace manta;
using namespace manta::typesystem;

namespace {

template<typename T>
auto CLBB(const T& data) {
  return lightning::AnsiColor8Bit(data,
                                  lightning::formatting::AnsiForegroundColor::BrightBlue);
}

template<typename T>
auto CLBG(const T& data) {
  return lightning::AnsiColor8Bit(
      data, lightning::formatting::AnsiForegroundColor::BrightGreen);
}

}  // namespace

namespace manta::typesystem {

void format_logstream(const TypeDescription& type_description,
                      lightning::RefBundle& handler) {
  handler << lightning::AnsiColor8Bit(
      type_description.Write(), lightning::formatting::AnsiForegroundColor::BrightBlue);
}

}  // namespace manta::typesystem

void ParserCodegen::GenerateParserCode(
    std::ostream& code_out, const std::shared_ptr<const ParserData>& parser_data) const {
  ParserDataToTypeManager manager(false, true);
  // Find all relationships between nodes.
  auto& [node_manager, relationships, nonterminals_for_type, node_types_for_item] =
      manager.CreateRelationships(parser_data);
  auto deduced_types = manager.DeduceTypes();

  LOG_SEV(Info)
      << "Done deducing types. Filling in type descriptions for all nonterminals' types.";

  // Fill in all type descriptions from the deduced types.
  for (const auto& [nonterminal_id, nonterminals_types] : deduced_types.GetTypesData()) {
    LOG_SEV(Debug) << "  * Filling type descriptions for "
                   << nonterminals_types.NumSubTypes() << " types for non-terminal "
                   << nonterminal_id;
    for (const auto& [type_name, description] : nonterminals_types.sub_types) {
      LOG_SEV(Debug) << "    >> Filling in type description for " << type_name
                     << ", getting fields for type '" << type_name << "'.";
      const auto& field_names = nonterminals_types.GetFields(type_name);
      LOG_SEV(Debug) << "    >> There are " << field_names.size() << " fields for '"
                     << type_name << "'.";
      for (const auto& field_name : field_names) {
        LOG_SEV(Debug) << "     - Looking for type of field '" << field_name << "'.";
        const auto* type = nonterminals_types.GetFieldType(field_name);

        MANTA_ASSERT(type,
                     "could not deduce the type of '" << type_name << "::" << field_name);
        description->fields[field_name] = type;
        LOG_SEV(Debug) << "     - Field " << type_name << "::" << field_name
                       << " has type " << *type << ".";
      }
    }
  }
  LOG_SEV(Info) << "Done filling in all type descriptions. Generating code.";

  // Write the guard and includes.
  code_out << "#pragma once\n\n#include <vector>\n#include <string>\n\n";
  code_out << "// Include the support for the parser.\n";
  code_out << "#include \"manta/generator/ParserDriver.h\"\n\n";

  // Create the node class definitions.
  LOG_SEV(Info) << "Generating code for all AST node definitions.";

  CppCodeGen codegen;

  node_manager.CreateAllDefinitions(code_out, codegen);

  std::string parser_class_name = "Parser";

  // Write parser definitions
  code_out << std::endl;
  code_out << "// "
              "================================================================"
              "========\n";
  code_out << "//  LALR Parser.\n";
  code_out << "// "
              "================================================================"
              "========\n";
  code_out << std::endl;
  code_out << "class Parser : public manta::ParserDriverBase<ASTNodeBase, "
              "Parser> {\n";
  code_out << "public:\n";
  code_out << "  //! \\brief Constructor, initializes the parser.\n  //!\n";
  code_out << "  Parser();\n\n";
  code_out << "  //! \\brief Function to parse the input.\n  //!\n";
  code_out << "  void ParseInput();\n\n";
  code_out << "protected:\n";
  code_out << "  //! \\brief Function that sets up the lexer.\n//!\n";
  code_out << "  void createLexer();\n\n";
  code_out << "  //! \\brief The reduce function, which allows this parser to "
              "call the reduction functions.\n";
  code_out << "  std::shared_ptr<ASTNodeBase> reduce(unsigned reduction_id, const "
              "std::vector<std::shared_ptr<ASTNodeBase>>& collected_nodes);\n\n";
  code_out << "  //! \\brief The parser table.\n  //!\n";
  code_out << "  std::vector<std::vector<manta::Entry>> parse_table_;\n";

  LOG_SEV(Info) << "Generating declarations of all reduce functions.";
  auto item_number = 0u;
  for (auto& item : parser_data->production_rules_data->all_productions) {
    // TODO: Sanitize names.

    LOG_SEV(Debug) << "Looking for node type name for item " << item_number << ".";
    auto& node_type_name = node_types_for_item.at(item_number);
    LOG_SEV(Debug) << "Node type name for item " << item_number << " is "
                   << node_type_name;

    code_out << "  std::shared_ptr<" << node_type_name << ">\n";
    code_out << "  ReduceTo_" + node_type_name << "_ViaItem_" << item_number << "(";
    auto i = 0;
    LOG_SEV(Debug) << "Writing declaration for item " << item_number << ": "
                   << CLBG(to_string(item, false));
    for (auto id : item.rhs) {
      if (i != 0)
        code_out << ",";
      if (parser_data->production_rules_data->IsNonTerminal(id)) {
        // Get the base type for this non-terminal.
        auto& base_type = deduced_types.GetBaseTypeName(id);
        code_out << "\n      const std::shared_ptr<" << base_type << ">& argument_" << i;
      }
      else {
        code_out << "\n      const std::string& argument_" << i;
      }
      ++i;
    }
    code_out << ");\n\n";

    ++item_number;
  }
  code_out << "};\n\n";

  LOG_SEV(Info) << "Generating definitions of Parser's functions.";

  code_out << "Parser::Parser() {\n";
  code_out << "  using namespace manta;\n\n";
  code_out << "  start_nonterminal_ = "
           << parser_data->production_rules_data->start_nonterminal << ";\n";
  code_out << "  // Allocate space for the parser table.\n";
  code_out << "  parse_table_.assign(" << parser_data->parse_table.size()
           << ", std::vector<Entry>(" << parser_data->parse_table[0].size() << ","
           << "Entry()"
           << "));\n\n";
  code_out << "  // Create the table. There are better, though more difficult, "
              "ways to serialize this information.";
  auto row_it = 0u;
  for (auto& row : parser_data->parse_table) {
    for (auto col_it = 0u; col_it < row.size(); ++col_it) {
      auto& entry = row[col_it];
      if (!entry.IsError()) {
        code_out << "  parse_table_[" << row_it << "][" << col_it << "] = ";
        if (entry.IsReduce()) {
          auto& item = entry.GetRule();
          code_out << "Entry(Item(" << item.production << ", " << item.production_label
                   << ", 0, {";
          for (auto i = 0u; i < item.rhs.size(); ++i) {
            if (i != 0)
              code_out << ", ";
            code_out << item.rhs[i];
          }
          code_out << "}));  // Reduce\n";
        }
        else if (entry.IsShift()) {
          code_out << "Entry(" << entry.GetState() << ");  // Shift\n";
        }
        else if (entry.IsAccept()) {
          code_out << "Entry(true);  // Accept\n";
        }
      }
    }
    ++row_it;
  }

  code_out << "  createLexer();\n";
  code_out << "}\n\n";

  code_out << "void Parser::ParseInput() {\n  parse();\n}\n\n";

  // Generate the code to create the lexer.
  code_out << "void Parser::createLexer() {\n";
  // TODO: Create lexer.
  code_out << "}\n\n";

  code_out << "std::shared_ptr<ASTNodeBase> Parser::reduce(unsigned "
              "reduction_id, const std::vector<std::shared_ptr<ASTNodeBase>>& "
              "collected_nodes) {\n";

  item_number = 0u;

  code_out << "  switch (reduction_id) {\n";
  for (auto& item : parser_data->production_rules_data->all_productions) {
    // TODO: Sanitize names.

    auto& node_type_name = node_types_for_item.at(item_number);
    code_out << "    case " << item_number << ": {\n";
    code_out << "      return ReduceTo_" + node_type_name << "_ViaItem_" << item_number
             << "(";
    auto i = 0;
    for (auto id : item.rhs) {
      if (i != 0)
        code_out << ",";
      if (parser_data->production_rules_data->IsNonTerminal(id)) {
        // Get the base type for this non-terminal.
        code_out << "\n          std::reinterpret_pointer_cast<";

        auto& base_type = deduced_types.GetBaseTypeName(id);
        code_out << base_type << ">(collected_nodes[" << i << "])";
      }
      else {
        code_out << "\n          reinterpret_cast<ASTLexeme*>(collected_nodes[" << i
                 << "].get())->literal";
      }
      ++i;
    }
    code_out << ");\n";
    code_out << "    }\n";

    ++item_number;
  }
  code_out << "    default: {\n";
  code_out << "      MANTA_FAIL(\"unrecognized production\" << reduction_id << "
              "\", cannot reduce\");\n";
  code_out << "    }\n";
  code_out << "  }\n";

  code_out << "}\n\n";

  item_number = 0u;
  for (auto& item : parser_data->production_rules_data->all_productions) {
    // TODO: Sanitize names.

    LOG_SEV(Debug) << "Creating reduction function for item " << item_number << ".";
    auto& node_type_name = node_types_for_item.at(item_number);
    LOG_SEV(Debug) << "Node type name is '" << node_type_name << "'.";

    code_out << "std::shared_ptr<" << node_type_name << ">\n";
    code_out << parser_class_name << "::ReduceTo_" + node_type_name << "_ViaItem_"
             << item_number;
    code_out << "(";

    // Arguments.
    // std::map<int, int> count_duplicates;
    auto i = 0;
    for (auto id : item.rhs) {
      // auto count = count_duplicates[id]++;
      if (i != 0)
        code_out << ",";
      if (parser_data->production_rules_data->IsNonTerminal(id)) {
        // Get the base type for this non-terminal.
        auto& base_type = deduced_types.GetBaseTypeName(id);
        code_out << "\n    const std::shared_ptr<" << base_type
                 << ">& "
                 // parser_data->production_rules_data->GetName(id)
                 << "argument_" << i;
      }
      else {
        code_out << "\n    const std::string& argument_" << i;
      }
      ++i;
    }
    code_out << ") {\n";
    code_out << "  auto new_node = std::make_shared<" << node_type_name << ">();\n\n";
    code_out << "  // Set fields in the new node.\n";
    // Get relationships for this node.
    auto& relationships_for_node = relationships.at(node_type_name);
    std::sort(relationships_for_node.end(),
              relationships_for_node.end(),
              [](auto& l, auto& r) { return l.position < r.position; });
    for (auto& rel : relationships_for_node) {
      auto&& field_name = rel.target_field_name;
      switch (rel.check_type) {
        case CheckType::Push: {
          code_out << "  new_node->" << field_name << ".push_back(argument_"
                   << rel.position << ");\n";
          break;
        }
        case CheckType::Append: {
          const std::string arg_name = "argument_" + std::to_string(rel.position);

          code_out << "  new_node->" << field_name << ".insert("
                   << "new_node->" << field_name << ".end(), " << arg_name << "->"
                   << *rel.source_field_name << ".cbegin(), " << arg_name << "->"
                   << *rel.source_field_name << ".cend());\n";
          break;
        }
        case CheckType::Field: {
          code_out << "  new_node->" << field_name << " = argument_" << rel.position
                   << ";\n";
          break;
        }
      }
    }
    code_out << "\n";
    code_out << "  return new_node;\n";
    code_out << "}\n" << std::endl;

    LOG_SEV(Info) << "Done writing code for reduction of item " << item_number << ".";
    ++item_number;
  }
}

void ParserCodegen::GenerateParserCode(std::ostream& code_out,
                                       std::istream& parser_description,
                                       ParserType parser_type) const {
  ParserGenerator generator(parser_type);
  GenerateParserCode(code_out, generator.CreateParserData(parser_description));
}

std::string ParserCodegen::fieldNameFromTarget(const std::string& target_name) {
  // Check if the target name is a reserved keyword or other problematic word.
  // TODO: Fill in all keywords.
  static std::set<std::string> keywords = {
      "class", "char", "int", "float", "double", "or", "and", "std"};

  if (keywords.contains(target_name)) {
    return "var_" + target_name;
  }
  return target_name;
}
