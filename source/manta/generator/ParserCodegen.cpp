#include "manta/generator/ParserCodegen.h"
// Other files.
#include "manta/generator/ASTNodeDescription.h"

using namespace manta;

namespace {

enum class CheckType {
  Field, Insert, Push
};

struct TypeRelationship {
  //! \brief The nonterminal type of the $N
  NonterminalID nonterminal_id;

  //! \brief If a dependent name was referenced, it is stored here.
  std::optional<std::string> source_field_name;

  //! \brief The type which requested the data. Items for different productions of the same non-terminal
  //! may or may not have the same type, so we use the name of the actual type, not item ID or the
  //! non terminal ID.
  [[maybe_unused]] std::string referencing_type;

  //! \brief The field name in the node for the reducing item.
  std::string target_field_name;

  //! \brief In what context the name was being used.
  //!
  //! For all checks, it must be checked that all references to this name for the node sub-types of nonterminal_id must be of the same type,
  //! and any source_field_name must exist in each non-terminal sub-type.
  //!
  //! Field check:    <no additional checks>
  //! Insert check:   The type must be a vector type.
  //! Push check:     The type of target_field_name must be a vector type, whose vector type matches that of the pushed node or node field.
  [[maybe_unused]] CheckType check_type;
};

//! \brief Split a string into segments by splitting on a deliminator.
std::vector<std::string> split(const std::string& str, char deliminator) {
  std::vector<std::string> segments;
  auto begin = str.begin(), end = begin;
  while (begin != str.end()) {
    end = std::find(begin, str.end(), deliminator);
    segments.emplace_back(begin, end);
    begin = end == str.end() ? end : std::next(end);
  }
  return segments;
}

std::pair<NonterminalID, std::optional<std::string>> GetSouceData(const std::string& argument_string, const Item& item) {
  auto segments = split(argument_string, '.');
  MANTA_ASSERT(segments.size() == 1 || segments.size() == 2, "argument name must be in one of the forms '$N' or '$N.<field-name>'");
  auto referenced_type = item.rhs.at(std::stoi(segments[0]));
  if (segments.size() == 1) {
    return {referenced_type, {} /* No field name */ };
  }
  else { // Specifies field name.
    return {referenced_type, segments[1]};
  }
}

void processFieldCommand(
    std::map<std::string, std::vector<TypeRelationship>>& relationships,
    const std::vector<std::shared_ptr<ParseNode>>& arguments,
    const Item& item,
    unsigned item_number,
    const ProductionRulesData& production_rules_data,
    ASTNodeDescription* node_type_description,
    ASTNodeManager& node_manager) {
  auto nonterminal_id = item.production;
  auto& instructions = item.instructions;
  auto&& nonterminal_name = production_rules_data.GetNonterminalName(nonterminal_id);

  auto get_arg = [&](auto i) -> std::string& { return arguments[i]->designator; };

  auto num_args = arguments.size();
  MANTA_ASSERT(num_args == 1 || num_args == 2, "field function needs one or two arguments, not " << num_args);

  bool needs_type_check = true;
  std::string target_field_name{};
  auto[referenced_type, source_field_name] = GetSouceData(get_arg(0), item);

  // Type is a non-terminal. A field name does not have to be specified.
  if (production_rules_data.NumTerminals() <= referenced_type) {
    if (num_args == 1) { // One argument: ($N[.field_name])
      if (source_field_name) {
        // Referenced a field of the node, $N.field_name.
        target_field_name = *source_field_name;
      }
      else {
        // References the node itself, $N.
        target_field_name = production_rules_data.GetNonterminalName(referenced_type);
      }
    }
    else { // Two arguments: ($N[.src_field_name], target_field_name)
      target_field_name = get_arg(1);
    }
  }
    // Type is a terminal. Field name must be specified. Type check not required.
  else {
    MANTA_ASSERT(num_args == 2, "field name must be specified to create a field for a terminal, non-terminal type is '"
        << production_rules_data.GetNonterminalName(nonterminal_id) << "', item number " << item_number);
    target_field_name = get_arg(1);
    needs_type_check = false;

    // Directly add the field, which is known to be a string.
    // TODO: Allow for other types of fields? The problem would be that, like llvm, we'd need arbitrary precision numbers
    //  so we didn't lose information. Strings do not have this problem.
    node_type_description->AddField(target_field_name, node_manager.GetStringType());
  }

  if (needs_type_check) {
    TypeRelationship relationship{
        static_cast<NonterminalID>(referenced_type),
        source_field_name,
        node_type_description->node_type_name,
        target_field_name,
        CheckType::Field,
    };
    relationships[node_type_description->node_type_name].push_back(relationship);
  }
}

void processInsertCommand(
    std::map<std::string, std::vector<TypeRelationship>>& relationships,
    const std::vector<std::shared_ptr<ParseNode>>& arguments,
    const Item& item,
    unsigned item_number,
    const ProductionRulesData& production_rules_data,
    ASTNodeDescription* node_type_description) {
  auto nonterminal_id = item.production;
  auto& instructions = item.instructions;
  auto&& nonterminal_name = production_rules_data.GetNonterminalName(nonterminal_id);

  auto get_arg = [&](auto i) -> std::string& { return arguments[i]->designator; };

  MANTA_ASSERT(arguments.size() == 2, "insert function needs two arguments, not " << arguments.size());

  std::string target_field_name{};
  auto[referenced_type, source_field_name] = GetSouceData(get_arg(0), item);
  MANTA_ASSERT(source_field_name,
               "insert function's first argument must reference a field, item was " << item_number << " for non-terminal " << nonterminal_name);

  TypeRelationship relationship{
      static_cast<NonterminalID>(referenced_type),
      source_field_name,
      node_type_description->node_type_name,
      get_arg(1),
      CheckType::Insert,
  };
  relationships[node_type_description->node_type_name].push_back(relationship);
}

void processPushCommand(
    std::map<std::string, std::vector<TypeRelationship>>& relationships,
    const std::vector<std::shared_ptr<ParseNode>>& arguments,
    const Item& item,
    unsigned, // item_number
    const ProductionRulesData& production_rules_data,
    ASTNodeDescription* node_type_description) {
  auto nonterminal_id = item.production;
  auto& instructions = item.instructions;
  auto&& nonterminal_name = production_rules_data.GetNonterminalName(nonterminal_id);

  auto get_arg = [&](auto i) -> std::string& { return arguments[i]->designator; };

  MANTA_REQUIRE(arguments.size() == 2, "push function needs two arguments, not " << arguments.size());

  std::string target_field_name{};
  auto[referenced_type, source_field_name] = GetSouceData(get_arg(0), item);

  TypeRelationship relationship{
      static_cast<NonterminalID>(referenced_type),
      source_field_name,
      node_type_description->node_type_name,
      get_arg(1),
      CheckType::Push,
  };
  relationships[node_type_description->node_type_name].push_back(relationship);
}

class TypeDeduction {
 public:
  struct NonterminalsTypes {
    //! \brief All field names from any type for the non-terminal.
    std::set<std::string> all_fields;

    //! \brief Map from a type associated with a non-terminal to the set of all field names for the type.
    std::map<std::string, std::set<std::string>> fields_for_type;

    //! \brief The fields that all types for this non-terminal have in common.
    std::set<std::string> common_fields;

    //! \brief Map from a field for a type associated with a non-terminal to a description of that field's type type.
    //!
    //! This map's index contains all fields.
    //!
    std::map<std::string, const ASTType*> field_type_descriptions;

    //! \brief The type descriptions of the sub-types.
    std::map<std::string, ASTNodeDescription*> sub_types;

    //! \brief If there are multiple subtypes for this nonterminal, we have them derive from a base type.
    //! Otherwise, there is only one node type, so that is the base node type.
    //!
    std::string base_type_name{};

    void AddType(const std::string& type_name) {
      fields_for_type[type_name]; // Will initialize an empty set if the entry is not already there.
      sub_types[type_name];
    }

    void AddField(const std::string& type_name, const std::string& field_name, const ASTType* field_type = nullptr) {
      all_fields.insert(field_name);
      fields_for_type[type_name].insert(field_name);
      field_type_descriptions[field_name] = field_type;
    }

    const std::set<std::string>& GetFields(const std::string& type_name) {
      return fields_for_type.at(type_name);
    }

    const ASTType* GetFieldType(const std::string& field_name) {
      return field_type_descriptions.at(field_name);
    }

    //! \brief Find the common fields. All field information must have been added.
    std::size_t FindCommonFields() {
      common_fields.clear(); // Make sure this is empty.

      // For every field, check if it occurs in every type.
      for (auto& field: all_fields) {
        bool is_common = true;
        for (auto&[_, type_fields]: fields_for_type) {
          if (!type_fields.contains(field)) {
            is_common = false;
            break;
          }
        }
        if (is_common) {
          common_fields.insert(field);
        }
      }
      return common_fields.size();
    }

    NO_DISCARD std::size_t NumSubTypes() const {
      return fields_for_type.size();
    }

  };

  void AddType(NonterminalID nonterminal_id, const std::string& type_name) {
    types_data[nonterminal_id].AddType(type_name);
    all_type_names.insert(type_name);
  }

  void AddField(NonterminalID nonterminal, const std::string& type_name, const std::string& field_name, const ASTType* field_type = nullptr) {
    types_data[nonterminal].AddField(type_name, field_name, field_type);
  }

  //! \brief Find common fields for all non-terminals' types.
  std::size_t FindCommonFields() {
    std::size_t num_common_fields = 0;
    for (auto&[_, nonterminal_types]: types_data) {
      num_common_fields += nonterminal_types.FindCommonFields();
    }
    return num_common_fields;
  }

  NO_DISCARD bool IsCommonField(NonterminalID nonterminal_id, const std::string& field_name) const {
    auto& common_fields = types_data.at(nonterminal_id).common_fields;
    return common_fields.find(field_name) != common_fields.end();
  }

  NO_DISCARD const std::string& GetBaseTypeName(NonterminalID nonterminal_id) const {
    return types_data.at(nonterminal_id).base_type_name;
  }

  NO_DISCARD NonterminalsTypes& At(NonterminalID nonterminal_id) {
    return types_data.at(nonterminal_id);
  }

  //! \brief Fill the sub-types map for every set of nonterminals' types.
  void GetTypeDescriptions(ASTNodeManager& node_manager) {
    for (auto&[_, nonterminals_types]: types_data) {
      for (auto&[type_name, _2]: nonterminals_types.fields_for_type) {
        nonterminals_types.sub_types[type_name] = node_manager.GetNodeDescription(type_name);
      }
    }
  }

  NO_DISCARD const std::map<NonterminalID, NonterminalsTypes>& GetTypesData() const {
    return types_data;
  }

  std::map<NonterminalID, NonterminalsTypes> types_data;
  std::set<std::string> all_type_names;
};

TypeDeduction deduceTypes(
    ASTNodeManager& node_manager,
    const std::map<std::string, std::vector<TypeRelationship>>& relationships,
    const std::map<std::string, NonterminalID>& nonterminals_for_type,
    const ProductionRulesData& production_rules_data
) {

  TypeDeduction deduction;

  // Initialize from non-terminals_for_type. This is because there may be types that have no relationships,
  // so we can't depend solely on the next loop (through relationships) to initialize all types.
  for (auto&[type_name, nonterminal_id]: nonterminals_for_type) {
    deduction.AddType(nonterminal_id, type_name);
  }

  for (auto&[type_name, relationships_for_type]: relationships) {
    auto nonterminal_id = nonterminals_for_type.at(type_name);
    auto type_ptr = node_manager.GetNodeDescription(type_name);

    // Add already determined types. Note that we also add the type description.
    for (auto&[field_name, type]: type_ptr->members_) {
      deduction.AddField(nonterminal_id, type_name, field_name, type);
    }

    // Add from relationships.
    for (auto& rel: relationships_for_type) {
      deduction.AddField(nonterminal_id, type_name, rel.target_field_name);
    }
  }

  // Initialize all common fields records.
  deduction.FindCommonFields();

  // Add all type descriptions for sub-types.
  deduction.GetTypeDescriptions(node_manager);

  std::cout << "Determining all base types for non-terminals." << std::endl;

  // For every non-terminal that needs it, create the base type.
  for (auto&[nonterminal_id, nonterminals_types]: deduction.types_data) {
    MANTA_ASSERT(nonterminals_types.NumSubTypes() != 0, "there must be at least one type for non-terminal " << nonterminal_id);
    auto& nonterminal_name = production_rules_data.GetNonterminalName(nonterminal_id);
    if (1 < nonterminals_types.NumSubTypes()) {
      auto base_name = "ASTNodeBase_" + nonterminal_name;
      nonterminals_types.base_type_name = base_name;
      // Create a type description for the base class.
      auto base_class_description = node_manager.GetNodeDescription(base_name, nonterminal_id);

      // Add the base class as the base class of all other classes for the non-terminal.
      auto& types = deduction.At(nonterminal_id);
      for (auto&[name, description]: types.sub_types) {
        description->parent_classes.insert(base_class_description);
      }

      // Add the type.
      types.sub_types[base_name] = base_class_description;

      // The fields of the base type will be the common fields.
      types.fields_for_type[base_name].insert(types.common_fields.begin(), types.common_fields.end());
    }
    else {
      // Get the name of the only type. This type will already be in the sub-types, since we didn't newly create it to be a base class type.
      nonterminals_types.base_type_name = nonterminals_types.fields_for_type.begin()->first;
    }

    std::cout << "The base type name for non-terminal " << nonterminal_id << "'s types will be " << nonterminals_types.base_type_name << ".\n";
  }

  std::cout << "Done determining all base types for non-terminals." << std::endl;

  for (const auto& type_name: deduction.all_type_names) {
    // Deduce the types of fields for this type.
    std::cout << "Deducing field types for type '" << type_name << "'" << std::endl;

    auto nonterminal_id = nonterminals_for_type.at(type_name);
    auto& nonterminals_types = deduction.At(nonterminal_id);

    auto it = relationships.find(type_name);
    if (it == relationships.end()) {
      std::cout << "Type named '" << type_name << "' had no relationships. Skipping.\n";
      continue;
    }
    auto& relationships_for_type = it->second;

    for (auto& rel: relationships_for_type) {
      // Only need to deduce types when we reference a field of another type.
      if (!rel.source_field_name) {
        // The type will be the node type. We can fill this in right away, now that every base class has been created.
        auto& base_type_name = deduction.GetBaseTypeName(rel.nonterminal_id);

        auto base_type = node_manager.GetNodeDescription(base_type_name);
        auto shared_ptr_type = node_manager.MakeShared(base_type);
        node_manager.GetNodeDescription(type_name)->AddField(rel.target_field_name, shared_ptr_type);
        nonterminals_types.field_type_descriptions[rel.target_field_name] = shared_ptr_type;
      }
      else {
        auto referenced_nonterminal = rel.nonterminal_id;

        // Make sure the referenced field is common
        MANTA_ASSERT(deduction.IsCommonField(referenced_nonterminal, *rel.source_field_name),
                     "can only reference common fields from the types associated with non-terminals, "
                     "the field named '" << *rel.source_field_name << "' is not common. Type is " << type_name);
      }
    }
  }

  return deduction;
}

} // namespace <unnamed>


void ParserCodegen::GenerateParserCode(std::ostream& code_out, const std::shared_ptr<const ParserData>& parser_data) const {
  // Write the guard and includes.
  code_out << "#pragma once\n\n#include <vector>\n#include <string>\n\n\n";

  // Look at all reduction rules to figure out the types of AST nodes that are required.

  ASTNodeManager node_manager;

  std::map<std::string, std::vector<TypeRelationship>> relationships;
  std::map<std::string, NonterminalID> nonterminals_for_type;

  unsigned item_number = 0, generated_nodes = 0;
  for (auto& item: parser_data->production_rules_data->all_productions) {
    // Check for instructions upon a reduce by this rule.
    auto nonterminal_id = item.production;
    auto& instructions = item.instructions;
    auto& nonterminal_name = parser_data->production_rules_data->GetNonterminalName(nonterminal_id);

    if (instructions) {
      // There are specific instructions on how to create the AST node from the parsed items.

      // Bucket instructions, we should look for a "node" instruction first, so we know the name of the node type.
      std::map<std::string, std::vector<std::shared_ptr<ParseNode>>> instructions_by_name;
      static std::set<std::string> allowed_instructions{"node", "field", "insert", "push"};
      for (auto& command: instructions->children) {
        auto& fn_name = command->designator;
        if (!allowed_instructions.contains(fn_name)) {
          MANTA_FAIL("unrecognized function '" << fn_name << "'");
        }
        instructions_by_name[fn_name].push_back(command);
      }

      // This will by the type of node that is created by a reduction of this type.
      std::string node_type_name{};

      // Look for the node instruction.
      if (auto node_it = instructions_by_name.find("node"); node_it != instructions_by_name.end()) {
        MANTA_ASSERT(node_it->second.size() == 1, "there can be at most one 'node' instruction per production rule");
        MANTA_ASSERT(node_it->second.size() == 1, "'node' function takes one argument");
        // TODO: Check if the name follows the correct format.
        node_type_name = "ASTNode_" + node_it->second[0]->children[0]->designator;
      }
      else {
        // Create a node type name from the production name.
        node_type_name = "ASTNode_" + nonterminal_name;
      }

      if (auto it = nonterminals_for_type.find(node_type_name); it != nonterminals_for_type.end()) {
        // Make sure that the non-terminal IDs match, i.e. a type only corresponds to one non-terminal.
        // TODO: Relax this, it could make sense for multiple non-terminal reductions to make the same type.
        MANTA_ASSERT(it->second == nonterminal_id, "type '" << node_type_name << "' cannot belong to multiple non-terminals");
      }
      else {
        // Add for the first time.
        nonterminals_for_type[node_type_name] = nonterminal_id;
      }

      // Create the type info for this node if it does not already exist.
      auto node_type_description = node_manager.GetNodeDescription(node_type_name, nonterminal_id);

      // Process all the 'field' commands.
      for (auto& cmd: instructions_by_name["field"]) {
        processFieldCommand(relationships, cmd->children, item, item_number,
                            *parser_data->production_rules_data, node_type_description,
                            node_manager);
      }

      // Process all the 'insert' commands.
      for (auto& cmd: instructions_by_name["insert"]) {
        processInsertCommand(relationships, cmd->children, item, item_number, *parser_data->production_rules_data, node_type_description);
      }

      // Process all the 'push' commands.
      for (auto& cmd: instructions_by_name["push"]) {
        processPushCommand(relationships, cmd->children, item, item_number, *parser_data->production_rules_data, node_type_description);
      }
    }
    else {
      // There are no instructions. Add everything that isn't a "literal."

      // Just create a type for now. This type will be empty.
      auto node_type_name = "ASTNodeGeneral_" + std::to_string(generated_nodes) + "__" + nonterminal_name;
      ++generated_nodes;

      auto node_type_description = node_manager.GetNodeDescription(node_type_name, nonterminal_id);
      nonterminals_for_type[node_type_name] = nonterminal_id;

      auto count = 0u;
      for (auto referenced_type: item.rhs) {
        auto name = parser_data->production_rules_data->GetName(referenced_type);

        // TODO: Change lexer generator so there is a better way of tagging which lexemes are literals.
        if (name.substr(0, 4) == "RES:") {
          continue; // Literal
        }
        auto target_field_name = name;
        if (tag_generated_field_names) {
          target_field_name += "__" + std::to_string(count);
        }

        // Non-terminal
        if (parser_data->production_rules_data->IsNonTerminal(referenced_type)) {
          if (generated_nodes_have_node_in_name) {
            target_field_name += "__node";
          }

          TypeRelationship relationship{
              static_cast<NonterminalID>(referenced_type),
              {}, // No field access.
              node_type_description->node_type_name,
              target_field_name,
              CheckType::Field,
          };
          relationships[node_type_description->node_type_name].push_back(relationship);
        }
          // Terminal
        else {
          // Directly add the field, which is known to be a string.
          // TODO: Allow for other types of fields? The problem would be that, like llvm, we'd need arbitrary precision numbers
          //  so we didn't lose information. Strings do not have this problem.
          node_type_description->AddField(target_field_name, node_manager.GetStringType());
        }

        ++count;
      }
    }

    ++item_number;
  }

  auto deduced_types = deduceTypes(node_manager, relationships, nonterminals_for_type, *parser_data->production_rules_data);

  std::cout << "\nDone deducing types. Filling in type descriptions.\n" << std::endl;

  // Fill in all type descriptions from the deduced types.
  for (auto[nonterminal_id, nonterminals_types]: deduced_types.GetTypesData()) {
    for (auto&[type_name, description]: nonterminals_types.sub_types) {
      std::cout << "Filling in type description for " << type_name << "." << std::endl;
      for (auto& field_name: nonterminals_types.GetFields(type_name)) {
        auto type = nonterminals_types.GetFieldType(field_name);

        MANTA_ASSERT(type, "could not deduce the type of '" << type_name << "::" << field_name);
        description->members_[field_name] = type;
        std::cout << "  * Got type of " << type_name << "::" << field_name << "\n";
      }
    }
  }

  node_manager.CreateAllDefinitions(code_out);
}

void ParserCodegen::GenerateParserCode(std::ostream& code_out, std::istream& parser_description, ParserType parser_type) const {
  ParserGenerator generator(parser_type);
  GenerateParserCode(code_out, generator.CreateParserData(parser_description));
}