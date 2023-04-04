#include "manta/generator/ParserCodegen.h"
// Other files.


using namespace manta;
using namespace manta::typesystem;

using FieldTracker = std::pair<NonterminalID, std::string>;

namespace {

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

std::tuple<int, NonterminalID, std::optional<std::string>> getSourceData(const std::string& argument_string, const Item& item) {
  auto segments = split(argument_string, '.');
  MANTA_ASSERT(segments.size() == 1 || segments.size() == 2, "argument name must be in one of the forms '$N' or '$N.<field-name>'");
  auto position = std::stoi(segments[0]);
  auto referenced_type = item.rhs.at(position);
  if (segments.size() == 1) {
    return {position, referenced_type, {} /* No field name */ };
  }
  else { // Specifies field name.
    return {position, referenced_type, segments[1]};
  }
}

void createGeneralNode(
    const std::string& node_type_name,
    NonterminalID nonterminal_id,
    ASTNodeManager& node_manager,
    const ProductionRulesData& production_rules_data,
    const std::string& nonterminal_name,
    const Item& item,
    std::map<std::string, std::vector<TypeRelationship>>& relationships,
    std::map<std::string, NonterminalID>& nonterminals_for_type,
    bool tag_generated_field_names,
    bool generated_nodes_have_node_in_name) {
  // There are no instructions. Add everything that isn't a "literal" as a field.
  auto node_type_description = node_manager.GetNodeDescription(node_type_name, nonterminal_id);
  nonterminals_for_type[node_type_name] = nonterminal_id;

  auto count = 0u;
  for (auto referenced_type: item.rhs) {
    auto& name = production_rules_data.GetName(referenced_type);

    // TODO: Change lexer generator so there is a better way of tagging which lexemes are literals.
    if (name.substr(0, 4) == "RES:") {
      continue; // Literal
    }
    auto target_field_name = name;
    if (tag_generated_field_names) {
      target_field_name += "__" + std::to_string(count);
    }

    if (production_rules_data.IsNonTerminal(referenced_type)) {
      // Non-terminal
      if (generated_nodes_have_node_in_name) {
        target_field_name += "__node";
      }
    }
    else {
      // Directly add the field, which is known to be a string.
      // TODO: Allow for other types of fields? The problem would be that, like llvm, we'd need arbitrary precision numbers
      //  so we didn't lose information. Strings do not have this problem.
      node_type_description->AddField(target_field_name, node_manager.GetStringType());
    }

    TypeRelationship relationship{
        static_cast<NonterminalID>(referenced_type),
        production_rules_data.IsNonTerminal(referenced_type),
        {}, // No field access.
        node_type_description->node_type_name,
        target_field_name,
        CheckType::Field,
    };
    relationships[node_type_description->node_type_name].push_back(relationship);

    ++count;
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

  std::string target_field_name{};
  auto[position, referenced_type, source_field_name] = getSourceData(get_arg(0), item);

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

    // Directly add the field, which is known to be a string.
    // TODO: Allow for other types of fields? The problem would be that, like llvm, we'd need arbitrary precision numbers
    //  so we didn't lose information. Strings do not have this problem.
    node_type_description->AddField(target_field_name, node_manager.GetStringType());

    std::cout << "Adding field '" << target_field_name << "' to type " << node_type_description->node_type_name << ".\n";
  }

  TypeRelationship relationship{
      static_cast<NonterminalID>(referenced_type),
      production_rules_data.IsNonTerminal(referenced_type),
      source_field_name,
      node_type_description->node_type_name,
      target_field_name,
      CheckType::Field,
      position,
  };
  relationships[node_type_description->node_type_name].push_back(relationship);
}

void processAppendCommand(
    std::map<std::string, std::vector<TypeRelationship>>& relationships,
    const std::vector<std::shared_ptr<ParseNode>>& arguments,
    const Item& item,
    unsigned item_number,
    const ProductionRulesData& production_rules_data,
    ASTNodeDescription* node_type_description) {
  // Form: append($N.field_name_1, field_name_2)

  auto nonterminal_id = item.production;
  auto& instructions = item.instructions;
  auto&& nonterminal_name = production_rules_data.GetNonterminalName(nonterminal_id);

  auto get_arg = [&](auto i) -> std::string& { return arguments[i]->designator; };

  MANTA_ASSERT(arguments.size() == 2, "append function needs two arguments, not " << arguments.size());

  std::string target_field_name{};
  auto[position, referenced_type, source_field_name] = getSourceData(get_arg(0), item);
  MANTA_ASSERT(source_field_name,
               "append function's first argument must reference a field, item was " << item_number << " for non-terminal " << nonterminal_name);

  TypeRelationship relationship{
      static_cast<NonterminalID>(referenced_type),
      production_rules_data.IsNonTerminal(referenced_type),
      source_field_name,
      node_type_description->node_type_name,
      get_arg(1),
      CheckType::Append,
      position,
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
  auto[position, referenced_type, source_field_name] = getSourceData(get_arg(0), item);

  TypeRelationship relationship{
      static_cast<NonterminalID>(referenced_type),
      true,
      source_field_name,
      node_type_description->node_type_name,
      get_arg(1),
      CheckType::Push,
      position,
  };
  relationships[node_type_description->node_type_name].push_back(relationship);
}

const ASTType* makeType(CheckType check_type, const ASTType* source_type, ASTNodeManager& node_manager) {
  if (check_type == CheckType::Push) {
    return node_manager.MakeVector(source_type);
  }
  else if (check_type == CheckType::Append) {
    MANTA_ASSERT(source_type->general_type == ASTGeneralType::Vector, "cannot append a non-vector to a vector");
    return source_type;
  }
  else if (check_type == CheckType::Field) {
    // Types are the same.
    return source_type;
  }
  else {
    MANTA_FAIL("unrecognized relationship type between fields");
  }
}

const ASTType* deduceTypesDFS(FieldTracker& target_field,
                              ASTNodeManager& node_manager,
                              TypeDeduction& deduction,
                              const std::map<FieldTracker, std::vector<TypeRelationship*>>& unsolved_relationships,
                              std::set<FieldTracker>& referenced_fields) {
  // Depth first search to deduce the type needed for each remaining relationship.
  const ASTType* type = deduction.GetFieldType(target_field.first, target_field.second);
  if (type) {
    return type;
  }

  // Type not deduced yet.
  if (auto it = unsolved_relationships.find(target_field); it != unsolved_relationships.end()) {
    // Try each relationship whose source field has not already been referenced (to prevent infinite loops).
    for (auto& rel: it->second) {
      std::pair reference(rel->referenced_id, *rel->source_field_name);
      if (!referenced_fields.contains(reference)) {
        referenced_fields.insert(reference);
        type = deduceTypesDFS(reference, node_manager, deduction, unsolved_relationships, referenced_fields);
      }
      if (type) {
        // Found the type of a field referenced by this relationship. Determine the type of the target field based on this relationship.
        type = makeType(rel->check_type, type, node_manager);
        break;
      }
    }
  }

  MANTA_ASSERT(type, "could not deduce type for field " << target_field.first << ", " << target_field.second);

  // Set the type in all the different data structures.
  auto& base_type_name = deduction.GetBaseTypeName(target_field.first);
  node_manager.GetNodeDescription(base_type_name)->AddField(target_field.second, type);
  deduction.At(target_field.first).field_type_descriptions[target_field.second] = type;

  return type;
}

// TODO: Separate this into multiple functions.
TypeDeduction deduceTypes(
    ASTNodeManager& node_manager,
    const std::map<std::string, std::vector<TypeRelationship>>& relationships,
    const std::map<std::string, NonterminalID>& nonterminals_for_type,
    const ProductionRulesData& production_rules_data) {

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
      std::cout << "  * Adding field '" << field_name << "' for type " << type_name << ", non terminal is " << nonterminal_id
                << ". Type already determined: " << type->Write() << ".\n";
    }

    // Add from relationships.
    for (auto& rel: relationships_for_type) {
      deduction.AddField(nonterminal_id, type_name, rel.target_field_name);
      std::cout << "  * Adding field '" << rel.target_field_name << "' for type " << type_name << ", non terminal is " << nonterminal_id << ".\n";
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

  std::cout << "Done determining all base types for non-terminals.\n" << std::endl;

  // We will fill in all relationships that depend on the *fields* of other types whose types are not
  // determined at the time we make our first check once we have filled in all the types
  // we can immediately fill in.
  std::vector<TypeRelationship> field_type_relationships;

  auto set_field_type = [node_manager](
      const std::string& type_name, const std::string& field_name, ASTType* type, auto& nonterminals_types) {
    // Make sure that if the type already exists, it is consistent.
    node_manager.GetNodeDescription(type_name)->AddField(field_name, type);
    nonterminals_types.field_type_descriptions[field_name] = type;
  };

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
      // Don't have to do a check if the referenced field is a terminal, instead of a non-terminal.
      if (production_rules_data.IsTerminal(rel.referenced_id)) {
        // Type is (right now) always string for terminals.
        set_field_type(type_name, rel.target_field_name, node_manager.GetStringType(), nonterminals_types);
        continue;
      }

      // Only need to deduce types when we reference a field of another type.
      if (!rel.source_field_name) {
        // The type will be the node type. We can fill this in right away, now that every base class has been created.
        auto& base_type_name = deduction.GetBaseTypeName(rel.referenced_id);

        auto base_type = node_manager.GetNodeDescription(base_type_name);
        if (rel.check_type == CheckType::Field) {
          auto shared_ptr_type = node_manager.MakeShared(base_type);
          // Note that the AddField function in set_field_type will make sure that if the field is added again, the types match.
          set_field_type(type_name, rel.target_field_name, shared_ptr_type, nonterminals_types);
        }
        else if (rel.check_type == CheckType::Push) {
          // The type is std::vector<std::shared_ptr<T>>
          auto vector_type = node_manager.MakeVector(node_manager.MakeShared(base_type));
          // Note that the AddField function in set_field_type will make sure that if the field is added again, the types match.
          set_field_type(type_name, rel.target_field_name, vector_type, nonterminals_types);
        }
        else if (rel.check_type == CheckType::Append) {
          MANTA_FAIL("append must reference a source field since it must reference some vector type, and an AST node is not a vector type");
        }
      }
      else {
        auto referenced_nonterminal = rel.referenced_id;

        // Make sure the referenced field is common
        MANTA_ASSERT(
            deduction.IsCommonField(referenced_nonterminal, *rel.source_field_name),
            "can only reference common fields from the types associated with non-terminals, "
            "the field named '" << *rel.source_field_name << "' is not common. Target type is " << type_name
                                << " source type (which lacks the common field) is " << deduction.GetBaseTypeName(referenced_nonterminal));

        // See if the type of the referenced field has already been filled in. If so, we can process this relationship.
        auto& base_type_name = deduction.GetBaseTypeName(referenced_nonterminal);
        auto base_type = node_manager.GetNodeDescription(base_type_name);
        if (auto ty = base_type->members_.find(*rel.source_field_name); ty != base_type->members_.end() && ty->second) {
          // The field has already been typed and defined. We can handle this relationship right now.
          // It must be that the target type is some vector type.
          auto type = ty->second;

          if (rel.check_type == CheckType::Append) {
            MANTA_ASSERT(type->general_type == ASTGeneralType::Vector, "for append, the target field must be a vector type");
          }
          else if (rel.check_type == CheckType::Field) {
            // Nothing to check. The source and target fields will have the same types.
          }
          else if (rel.check_type == CheckType::Push) {
            // Type is a vector of <type>. Note that we don't have to worry about circular dependencies here (pushing a type into itself).
            type = node_manager.MakeVector(type);
          }

          std::cout << "  * Source field has already been typed, can fill in immediately." << std::endl;

          // TODO: Make sure that if the field has already been added, the types are the same.

          node_manager.GetNodeDescription(type_name)->AddField(rel.target_field_name, type);
          nonterminals_types.field_type_descriptions[rel.target_field_name] = type;
        }
        else {
          // There are circumstances under which the relationship is useless:
          //    * Field references itself.
          //    * Field appends itself.
          // The following is not allowed
          //    * Field pushes itself (there are more complicated versions of this that are disallowed, like cycles, but this is the
          //      simplest and easiest to check quickly).
          // Note that all these cases only happen when the referenced non-terminal and non-terminal that the target type belongs
          // to are the same.
          if (referenced_nonterminal == nonterminals_for_type.at(rel.referencing_type)) {
            bool is_self_reference = rel.target_field_name == *rel.source_field_name;
            if (is_self_reference) {
              MANTA_ASSERT(rel.check_type != CheckType::Push, "no type T is equal to vector<T>, cannot push a field into itself");
              std::cout << "  * Found (valid) self-referential relationship, this is consistent, but not useful. Dropping." << std::endl;
            }
            else {
              field_type_relationships.push_back(rel);
            }
          }
          else {
            std::cout << "  * Source field has NOT been typed yet, delaying typing until all base types have been deduced." << std::endl;
            field_type_relationships.push_back(rel);
          }
        }
      }
    }
  }

  // Deduce the more complicated type relationships.
  if (field_type_relationships.empty()) {
    std::cout << std::endl << "Utilized all relationships in the first pass, DFS pass needed." << std::endl;
  }
  else {
    std::cout << std::endl << "After filling in immediate types and eliminating non-useful relationships, there are "
              << field_type_relationships.size() << " relationships to consider." << std::endl;

    // Have to organize types mapping fields to relationships having this field as a target.
    // Note that we only have to do this for the remaining relationships, not all the relationships that were passed to
    // this function. This is because any relationship that was processed above connected two (now) already deduced types,
    // so we do not need to follow this relationship. To follow a relationship like this, we would have to start at the
    // target field, but the target field's type will have been deduced, so we wouldn't need to follow any of its relationships.
    std::map<FieldTracker, std::vector<TypeRelationship*>> unsolved_relationships;
    for (auto& relationship: field_type_relationships) {
      std::pair reference(relationship.referenced_id, *relationship.source_field_name);
      unsolved_relationships[reference].push_back(&relationship);
    }

    // Depth first search to deduce the type needed for each remaining relationship.
    for (auto& relationship: field_type_relationships) {
      if (!relationship.target_is_nonterminal) {
        continue;
      }
      // Check to see if the target field's type has already been deduced.
      auto nonterminal_id = nonterminals_for_type.at(relationship.referencing_type);
      auto type = deduction.GetFieldType(nonterminal_id, relationship.target_field_name);
      if (type) {
        continue;
      }

      std::set<FieldTracker> referenced_fields;

      std::pair reference(relationship.referenced_id, *relationship.source_field_name);
      type = deduceTypesDFS(reference, node_manager, deduction, unsolved_relationships, referenced_fields);
      type = makeType(relationship.check_type, type, node_manager);

      // TODO: Figure out the right way to handle const vs mutable types.
      auto& nonterminals_types = deduction.At(nonterminal_id);
      set_field_type(relationship.referencing_type, relationship.target_field_name, const_cast<ASTType*>(type), nonterminals_types);
    }
  }
  return deduction;
}

} // namespace <unnamed>


void ParserCodegen::GenerateParserCode(std::ostream& code_out, const std::shared_ptr<const ParserData>& parser_data) const {

  auto[node_manager,
  relationships,
  nonterminals_for_type,
  node_types_for_item] = createRelationships(parser_data);

  auto deduced_types = deduceTypes(node_manager,
                                   relationships,
                                   nonterminals_for_type,
                                   *parser_data->production_rules_data);

  std::cout << "\nDone deducing types. Filling in type descriptions.\n" << std::endl;

  // Fill in all type descriptions from the deduced types.
  for (auto[nonterminal_id, nonterminals_types]: deduced_types.GetTypesData()) {
    for (auto&[type_name, description]: nonterminals_types.sub_types) {
      std::cout << "Filling in type description for " << type_name << "." << std::endl;
      for (auto& field_name: nonterminals_types.GetFields(type_name)) {
        auto sanitized_field_name = field_name; // fieldNameFromTarget(field_name);
        auto type = nonterminals_types.GetFieldType(sanitized_field_name);

        MANTA_ASSERT(type, "could not deduce the type of '" << type_name << "::" << sanitized_field_name);
        description->members_[sanitized_field_name] = type;
        std::cout << "  * Got type of " << type_name << "::" << sanitized_field_name << ": " << type->Write() << "\n";
      }
    }
  }

  // Write the guard and includes.
  code_out << "#pragma once\n\n#include <vector>\n#include <string>\n\n";
  code_out << "// Include the support for the parser.\n";
  code_out << "#include \"manta/generator/ParserDriver.h\"\n\n";

  // Create the node class definitions.
  node_manager.CreateAllDefinitions(code_out);

  // Create the parser and the functions that handle creating new nodes through reduction of items.
  // ... TODO ...

  // Write parser declarations.
  std::string parser_class_name = "Parser";
  code_out << "\n//! \\brief The main parser class.\n//!\n";
  code_out << "class " << parser_class_name << " {\n";
  code_out << "public:\n";

  code_out << "private:\n";

  auto item_number = 0u;
  for (auto& item: parser_data->production_rules_data->all_productions) {
    // TODO: Sanitize names.

    auto& node_type_name = node_types_for_item.at(item_number);

    code_out << "  std::shared_ptr<" << node_type_name << ">\n";
    code_out << "  ReduceTo_" + node_type_name << "_ViaItem_" << item_number << "(";
    auto i = 0;
    for (auto id: item.rhs) {
      if (i != 0) code_out << ",";
      if (parser_data->production_rules_data->IsNonTerminal(id)) {
        // Get the base type for this non-terminal.
        auto& base_type = deduced_types.GetBaseTypeName(id);
        code_out << "\n      const std::shared_ptr<" << base_type << ">& "
                 // parser_data->production_rules_data->GetName(id)
                 << "argument_" << i;
      }
      else {
        code_out << "\n      const std::string& argument_" << i;
      }
      ++i;
    }
    code_out << ");\n\n";

    ++item_number;
  }

  ////

  code_out << "};\n" << std::endl;

  code_out << std::endl;
  code_out << "// ========================================================================\n";
  code_out << "//  Parser reduction functions.\n";
  code_out << "// ========================================================================\n";
  code_out << std::endl;

  item_number = 0u;
  for (auto& item: parser_data->production_rules_data->all_productions) {
    // TODO: Sanitize names.

    std::cout << "Creating reduction function for item " << item_number << "." << std::endl;
    auto& node_type_name = node_types_for_item.at(item_number);
    std::cout << "Node type name is '" << node_type_name << "'." << std::endl;

    code_out << "std::shared_ptr<" << node_type_name << ">\n";
    code_out << parser_class_name << "::ReduceTo_" + node_type_name << "_ViaItem_" << item_number;
    code_out << "(";

    // Arguments.
    // std::map<int, int> count_duplicates;
    auto i = 0;
    for (auto id: item.rhs) {
      // auto count = count_duplicates[id]++;
      if (i != 0) code_out << ",";
      if (parser_data->production_rules_data->IsNonTerminal(id)) {
        // Get the base type for this non-terminal.
        auto& base_type = deduced_types.GetBaseTypeName(id);
        code_out << "\n    const std::shared_ptr<" << base_type << ">& "
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
    std::sort(relationships_for_node.end(), relationships_for_node.end(), [](auto& l, auto& r) {
      return l.position < r.position;
    });
    for (auto& rel: relationships_for_node) {
      auto field_name = fieldNameFromTarget(rel.target_field_name);
      switch (rel.check_type) {
        case CheckType::Push: {
          code_out << "  new_node->" << field_name << ".push_back(argument_" << rel.position << ");\n";
          break;
        }
        case CheckType::Append: {
          const std::string arg_name = "argument_" + std::to_string(rel.position);
          code_out << "  new_node->" << field_name << ".insert("
                   << "new_node->" << field_name << ".end(), "
                   << arg_name << ".cbegin(), " << arg_name << ".cend());\n";
          break;
        }
        case CheckType::Field: {
          code_out << "  new_node->" << field_name << " = argument_" << rel.position << ";\n";
          break;
        }
      }

    }
    code_out << "\n";
    code_out << "  return new_node;\n";
    code_out << "}\n" << std::endl;

    std::cout << "Done writing code for reduction of item " << item_number << "." << std::endl;
    ++item_number;
  }

  // Write parser definitions
  code_out << std::endl;
  code_out << "// ========================================================================\n";
  code_out << "//  LALR Parser.\n";
  code_out << "// ========================================================================\n";
  code_out << std::endl;
  code_out << "class Parser : public ParserDriverBase<ASTNodeBase, Parser> {\n";
  code_out << "public:\n";
  code_out << "  //! \\brief Constructor, initializes the parser.\n  //!\n";
  code_out << "  void Parser();\n\n";
  code_out << "  //! \\brief Function to parse the input.\n  //!\n";
  code_out << "  void ParseInput();\n\n";
  code_out << "protected:\n";
  code_out << "  //! \\brief The reduce function, which allows this parser to call the reduction functions.\n";
  code_out << "  std::shared_ptr<ASTNodeBase> reduce(unsigned reduction_id, const std::vector<ASTNodeBase>& collected_nodes);\n\n";
  code_out << "  //! \\brief The parser table.\n  //!\n";
  code_out << "  std::vector<std::vector<Entry>> parse_table_;\n";

  code_out << "};\n\n";

  code_out << "Parser::Parser() {\n";

  code_out << "  start_nonterminal_ = " << parser_data->production_rules_data->start_nonterminal << ";\n";

  code_out << "  // Allocate space for the parser table.\n";
  code_out << "  parse_table_.assign("
           << parser_data->parse_table.size() << ", std::vector<Entry>("
           << parser_data->parse_table[0].size() << "," << "Entry()" << "));\n\n";
  code_out << "  // Create the table. There are better, though more difficult, ways to serialize this information.";
  auto row_it = 0u;
  for (auto& row: parser_data->parse_table) {
    for (auto col_it = 0u; col_it < row.size(); ++col_it) {
      auto& entry = row[col_it];
      if (!entry.IsError()) {
        code_out << "  parse_table_[" << row_it << "][" << col_it << "] = ";
        if (entry.IsReduce()) {
          auto& item = entry.GetRule();
          code_out << "Entry(Item(" << item.production << ", " << item.production_label << ", 0, {";
          for (auto i = 0u; i < item.rhs.size(); ++i) {
            if (i != 0) code_out << ", ";
            code_out << item.rhs[i];
          }
          code_out << "});  // Reduce\n";
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

  code_out << "}\n\n";

  code_out << "void Parser::ParseInput() {\n  parse();\n}\n\n";

  code_out << "std::shared_ptr<ASTNodeBase> reduce(unsigned reduction_id, const std::vector<ASTNodeBase>& collected_nodes) {\n";

  item_number = 0u;

  code_out << "  switch (reduction_id) {\n";
  for (auto& item: parser_data->production_rules_data->all_productions) {
    // TODO: Sanitize names.

    auto& node_type_name = node_types_for_item.at(item_number);
    code_out << "    case " << item_number << ": {\n";
    code_out << "      return ReduceTo_" + node_type_name << "_ViaItem_" << item_number << "(";
    auto i = 0;
    for (auto id: item.rhs) {
      if (i != 0) code_out << ",";
      if (parser_data->production_rules_data->IsNonTerminal(id)) {
        // Get the base type for this non-terminal.
        code_out << "\n          std::reinterpret_pointer_cast<";

        auto& base_type = deduced_types.GetBaseTypeName(id);
        code_out << base_type << ">(collect[" << i << "])";
      }
      else {
        code_out << "\n          reinterpret_cast<ASTLexeme>(collect[" << i << "].get())->literal";
      }
      ++i;
    }
    code_out << ");\n";
    code_out << "    }\n";

    ++item_number;
  }
  code_out << "    default: {\n";
  code_out << "      MANTA_FAIL(\"unrecognized production\" << reduction_id << \", cannot reduce\");\n";
  code_out << "    }\n";
  code_out << "  }\n";

  code_out << "}\n\n";
}

void ParserCodegen::GenerateParserCode(std::ostream& code_out, std::istream& parser_description, ParserType parser_type) const {
  ParserGenerator generator(parser_type);
  GenerateParserCode(code_out, generator.CreateParserData(parser_description));
}

std::tuple<
    ASTNodeManager,
    std::map<std::string, std::vector<TypeRelationship>>,
    std::map<std::string, NonterminalID>,
    std::map<unsigned, std::string>
>
ParserCodegen::createRelationships(const std::shared_ptr<const ParserData>& parser_data) const {
  // Look at all reduction rules to figure out the types of AST nodes that are required.

  ASTNodeManager node_manager;

  std::map<std::string, std::vector<TypeRelationship>> relationships;
  std::map<std::string, NonterminalID> nonterminals_for_type;
  std::map<unsigned, std::string> node_types_for_item;

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
      static std::set<std::string> allowed_instructions{"node", "field", "append", "push"};
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
        // TODO: Check if the name follows the correct format, e.g. does not contain ':' or '-' or any other illegal characters.
        node_type_name = "ASTNode_" + node_it->second[0]->children[0]->designator;
      }
      else {
        // Create a node type name from the production name.
        node_type_name = "ASTNode_" + nonterminal_name;
      }
      node_types_for_item[item_number] = node_type_name;

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

      if (!instructions_by_name["field"].empty() || !instructions_by_name["append"].empty() || !instructions_by_name["push"].empty()) {
        // Process all the 'field' commands.
        for (auto& cmd: instructions_by_name["field"]) {
          processFieldCommand(relationships, cmd->children, item, item_number,
                              *parser_data->production_rules_data, node_type_description,
                              node_manager);
        }

        // Process all the 'append' commands.
        for (auto& cmd: instructions_by_name["append"]) {
          processAppendCommand(relationships, cmd->children, item, item_number, *parser_data->production_rules_data, node_type_description);
        }

        // Process all the 'push' commands.
        for (auto& cmd: instructions_by_name["push"]) {
          processPushCommand(relationships, cmd->children, item, item_number, *parser_data->production_rules_data, node_type_description);
        }
      }
      else {
        // Update this so we determine the node name beforehand.
        createGeneralNode(
            node_type_name,
            nonterminal_id,
            node_manager,
            *parser_data->production_rules_data,
            nonterminal_name,
            item,
            relationships,
            nonterminals_for_type,
            tag_generated_field_names,
            generated_nodes_have_node_in_name);
      }
    }
    else {
      auto node_type_name = "ASTNodeGeneral_" + std::to_string(generated_nodes) + "_" + nonterminal_name;
      node_types_for_item[item_number] = node_type_name;
      ++generated_nodes;

      createGeneralNode(
          node_type_name,
          nonterminal_id,
          node_manager,
          *parser_data->production_rules_data,
          nonterminal_name,
          item,
          relationships,
          nonterminals_for_type,
          tag_generated_field_names,
          generated_nodes_have_node_in_name);
    }

    ++item_number;
  }

  return {node_manager, relationships, nonterminals_for_type, node_types_for_item};
}

std::string ParserCodegen::fieldNameFromTarget(const std::string& target_name) {
  // Check if the target name is a reserved keyword or other problematic word.
  // TODO: Fill in all keywords.
  static std::set<std::string> keywords = {
      "class", "char", "int", "float", "double",
      "or", "and", "std"
  };

  if (keywords.contains(target_name)) {
    return "var_" + target_name;
  }
  return target_name;
}
