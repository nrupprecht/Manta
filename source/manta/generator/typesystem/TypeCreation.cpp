//
// Created by Nathaniel Rupprecht on 4/12/23.
//

#include "manta/generator/typesystem/TypeCreation.h"
// Other files.
#include "manta/generator/typesystem/TypeDeduction.h"
#include "manta/generator/typesystem/TypeRelationship.h"
#include "manta/utility/Formatting.h"

namespace {

using namespace manta;
using namespace manta::typesystem;

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

}  // namespace

using namespace manta::typesystem;

ParserDataToTypeManager::ParserDataToTypeManager(bool tag_generated_field_names,
                                                 bool generated_nodes_have_node_in_name) noexcept
    : tag_generated_field_names_(tag_generated_field_names)
    , generated_nodes_have_node_in_name_(generated_nodes_have_node_in_name) {
  std::set<std::string> reserved_words {"alignas",
                                        "alignof",
                                        "and",
                                        "and_eq",
                                        "asm",
                                        "atomic_cancel",
                                        "atomic_commit",
                                        "atomic_noexcept",
                                        "auto",
                                        "bitand",
                                        "bitor",
                                        "bool",
                                        "break",
                                        "case",
                                        "catch",
                                        "char",
                                        "char16_t",
                                        "char32_t",
                                        "class",
                                        "compl",
                                        "concept",
                                        "const",
                                        "constexpr",
                                        "const_cast",
                                        "continue",
                                        "co_await",
                                        "co_return",
                                        "co_yield",
                                        "decltype",
                                        "default",
                                        "delete",
                                        "do",
                                        "double",
                                        "dynamic_cast",
                                        "else",
                                        "enum",
                                        "explicit",
                                        "export",
                                        "extern",
                                        "false",
                                        "float",
                                        "for",
                                        "friend",
                                        "goto",
                                        "if",
                                        "import",
                                        "inline",
                                        "int",
                                        "long",
                                        "modules",
                                        "mutable",
                                        "namespace",
                                        "new",
                                        "noexcept",
                                        "not",
                                        "not_eq",
                                        "nullptr",
                                        "operator",
                                        "or",
                                        "or_eq",
                                        "private",
                                        "protected",
                                        "public",
                                        "register",
                                        "reinterpret_cast",
                                        "requires",
                                        "return",
                                        "short",
                                        "signed",
                                        "sizeof",
                                        "static",
                                        "static_assert",
                                        "static_cast",
                                        "struct",
                                        "switch",
                                        "thread_local",
                                        "throw",
                                        "true",
                                        "try",
                                        "typedef",
                                        "typeid",
                                        "typename",
                                        "union",
                                        "unsigned",
                                        "using",
                                        "virtual",
                                        "void",
                                        "volatile",
                                        "wchar_t",
                                        "while",
                                        "xor",
                                        "xor_eq"};

  field_name_sanitizer_ = [reserved_words](std::string name) -> std::string {
    MANTA_REQUIRE(!name.empty(), "cannot sanitize an empty name");

    if (reserved_words.contains(name)) {
      return name + "_";
    }
    // Replace any bad characters.
    std::replace_if(
        name.begin(), name.end(), [](char c) { return !isalpha(c) || isdigit(c) || c == '_'; }, '_');
    if (std::isdigit(name[0])) {
      name = "_" + name;
    }
    return name;
  };
}

ParserTypeData& ParserDataToTypeManager::CreateRelationships(
    const std::shared_ptr<const ParserData>& parser_data) {
  // Look at all reduction rules to figure out the types of AST nodes that are required.

  production_rules_data_ = parser_data->production_rules_data;

  unsigned item_number = 0, generated_nodes = 0;
  auto& all_productions = parser_data->production_rules_data->all_productions;
  for (auto& item : all_productions) {
    LOG_SEV(Info) << "Processing item number " << item_number << " (" << item_number + 1 << " / "
                  << all_productions.size() << ").";
    // Check for instructions upon a reduction by this rule.
    auto nonterminal_id = item.production;
    auto& instructions = item.instructions;
    auto& nonterminal_name = parser_data->production_rules_data->GetNonterminalName(nonterminal_id);
    LOG_SEV(Info) << "Non-terminal's name is '" << nonterminal_name << ".";

    if (instructions) {
      // There are specific instructions on how to create the AST node from the parsed
      // items.

      // Bucket instructions, we should look for a "node" instruction first, so we know
      // the name of the node type (if it was user-defined, otherwise, we create a node
      // name).
      std::map<std::string, std::vector<std::shared_ptr<ParseNode>>> instructions_by_name;
      // NOTE(Nate): Keywords of the allowed instructions are hard-coded.
      static std::set<std::string> allowed_instructions {"node", "field", "append", "push"};
      for (auto& command : instructions->children) {
        auto& fn_name = command->designator;
        MANTA_ASSERT(allowed_instructions.contains(fn_name), "unrecognized function '" << fn_name << "'");
        instructions_by_name[fn_name].push_back(command);
      }

      // This will by the type of node that is created by a reduction of this type.
      std::string type_name {};

      // Look for the node instruction.
      if (auto node_it = instructions_by_name.find("node"); node_it != instructions_by_name.end()) {
        MANTA_ASSERT(node_it->second.size() == 1,
                     "there can be at most one 'node' instruction per production rule");
        MANTA_ASSERT(node_it->second.size() == 1, "'node' function takes one argument");
        // TODO: Check if the name follows the correct format, e.g. does not contain ':'
        // or '-' or any other illegal characters.
        type_name = "ASTNode_" + node_it->second[0]->children[0]->designator;
      }
      else {
        // Create a node type name from the production name. Every item potentially needs its own node type.
        //  TODO: Merge duplicate nodes at the end. Eliminate nodes with no members.
        type_name = "ASTNode_" + std::to_string(generated_nodes) + "_" + nonterminal_name;
        ++generated_nodes;
      }
      // Register that this item (production) will be represented by a node of this type
      // name.
      node_types_for_item()[item_number] = type_name;

      LOG_SEV(Info) << "Node type for item " << item_number << " will be " << formatting::CLBB(type_name)
                    << ".";

      if (auto it = nonterminals_for_type().find(type_name); it != nonterminals_for_type().end()) {
        // Make sure that the non-terminal IDs match, i.e. a type only corresponds to one
        // non-terminal.
        // TODO: Relax this, it could make sense for multiple non-terminal reductions to
        // make the same type.
        MANTA_ASSERT(it->second == nonterminal_id,
                     "type '" << type_name << "' cannot belong to multiple non-terminals");
      }
      else {
        // Add for the first time.
        nonterminals_for_type()[type_name] = nonterminal_id;
      }

      // Create the type info for this node if it does not already exist.
      auto node_type_description = node_manager().GetNodeDescription(type_name, nonterminal_id);

      LOG_SEV(Debug) << "Processing all 'field', 'append', and 'push' commands for node type named "
                     << formatting::CLBB(type_name) << " for nonterminal ID " << nonterminal_id << ".";

      if (!instructions_by_name["field"].empty() || !instructions_by_name["append"].empty()
          || !instructions_by_name["push"].empty())
      {
        // Process all the 'field' commands.
        for (auto& cmd : instructions_by_name["field"]) {
          processFieldCommand(cmd->children, item, item_number, node_type_description);
        }

        // Process all the 'append' commands.
        for (auto& cmd : instructions_by_name["append"]) {
          processAppendCommand(cmd->children, item, item_number, node_type_description);
        }

        // Process all the 'push' commands.
        for (auto& cmd : instructions_by_name["push"]) {
          processPushCommand(cmd->children, item, item_number, node_type_description);
        }
      }
      else {
        LOG_SEV(Debug) << "There are no FIELD, APPEND, or PUSH commands for item " << item_number << ".";

        // Update this so we determine the node name beforehand.
        createGeneralNode(
            type_name, nonterminal_id, nonterminal_name, item, nonterminals_for_type(), item_number);
      }
    }
    // There are no instructions. Default to including every non-literal in the node
    // definition. For example P -> A "+" B would not have a field for the "+", but would
    // for A and B.
    else {
      auto type_name = "ASTNodeGeneral_" + std::to_string(generated_nodes) + "_" + nonterminal_name;
      node_types_for_item()[item_number] = type_name;
      ++generated_nodes;

      LOG_SEV(Debug) << "No instructions for item number " << item_number << " (production: " << item
                     << ") creating node type named " << formatting::CLBB(type_name) << ".";

      createGeneralNode(
          type_name, nonterminal_id, nonterminal_name, item, nonterminals_for_type(), item_number);
    }

    ++item_number;
  }

  LOG_SEV(Info) << "Done creating relationships";
  return parser_type_data_;
}

// TODO: Separate this into multiple functions.
TypeDeduction ParserDataToTypeManager::DeduceTypes() {
  LOG_SEV(Info) << "Deducing types.";

  TypeDeduction deduction;

  // Initialize from non-terminals_for_type. This is because there may be types that have
  // no relationships, so we can't depend solely on the next loop (through relationships)
  // to initialize all types.
  LOG_SEV(Debug) << "Processing " << nonterminals_for_type().size()
                 << " non-terminals to initialize all needed types.";
  for (auto& [type_name, nonterminal_id] : nonterminals_for_type()) {
    LOG_SEV(Debug) << "Adding type named " << formatting::CLBB(type_name) << " for non-terminal ID "
                   << nonterminal_id << ".";
    deduction.AddType(nonterminal_id, type_name);
  }

  LOG_SEV(Info) << "Processing all relationships for each type.";
  for (auto& [type_name, relationships_for_type] : relationships()) {
    LOG_SEV(Debug) << "  * Looking for type named " << formatting::CLBB(type_name)
                   << " in the non-terminals.";

    auto nonterminal_id = nonterminals_for_type().at(type_name);
    auto type_ptr = node_manager().GetNodeDescription(type_name);

    // Add already determined types. Note that we also add the type description.
    LOG_SEV(Debug) << "  * Adding all fields whose types have already been determined.";
    for (auto& [field_name, type] : type_ptr->fields) {
      deduction.AddField(nonterminal_id, type_name, field_name, type);
      LOG_SEV(Debug) << "    >> Adding field '" << field_name << "' for type " << formatting::CLBB(type_name)
                     << ", non terminal is " << nonterminal_id << ". Type already determined: " << *type
                     << ".";
    }

    // Add from relationships.
    LOG_SEV(Debug) << "  * Adding slots for fields whose types have not been determined yet.";
    for (auto& rel : relationships_for_type) {
      auto& name = rel.target_field_name;
      if (!type_ptr->fields.contains(name)) {  // If the type has not been added yet...
        deduction.AddField(nonterminal_id, type_name, rel.target_field_name);
        LOG_SEV(Debug) << "    >> Adding field '" << rel.target_field_name << "' for type "
                       << formatting::CLBB(type_name) << ", non terminal is " << nonterminal_id << ".";
      }
    }
  }

  // Initialize all common fields records.
  LOG_SEV(Info) << "Looking for common fields for each non-terminal.";
  deduction.FindCommonFields();

  // Add all type descriptions for sub-types.
  LOG_SEV(Info) << "Finding all type descriptions.";
  deduction.GetTypeDescriptions(node_manager());

  LOG_SEV(Info) << "Determining all base types for non-terminals.";
  determineBaseTypes(deduction);
  LOG_SEV(Info) << "Done determining all base types for non-terminals.\n";

  // We will fill in all relationships that depend on the *fields* of other types whose
  // types are not determined at the time we make our first check once we have filled in
  // all the types we can immediately fill in.
  std::vector<TypeRelationship> field_type_relationships;

  auto set_field_type =
      [this](
          const std::string& type_name, const std::string& field_name, auto* type, auto& nonterminals_types) {
        // Make sure that if the type already exists, it is consistent.
        this->node_manager().GetNodeDescription(type_name)->AddField(field_name, type);
        nonterminals_types.field_type_descriptions[field_name] = type;
      };

  LOG_SEV(Info) << "Deducing all field types for all node types.";
  for (const auto& type_name : deduction.all_type_names) {
    // Deduce the types of fields for this type.
    LOG_SEV(Debug) << "  * Deducing field types for type " << formatting::CLBB(type_name) << ".";

    auto nonterminal_id = nonterminals_for_type().at(type_name);
    auto& nonterminals_types = deduction.At(nonterminal_id);

    auto it = relationships().find(type_name);
    if (it == relationships().end()) {
      LOG_SEV(Debug) << "  * Type named '" << type_name << "' had no relationships. Skipping.";
      continue;
    }
    auto& relationships_for_type = it->second;

    for (auto& rel : relationships_for_type) {
      // Don't have to do a check if the referenced field is a terminal, instead of a
      // non-terminal.
      if (production_rules_data_->IsTerminal(rel.referenced_id)) {
        // Type is (right now) always string for terminals.
        set_field_type(type_name, rel.target_field_name, node_manager().GetStringType(), nonterminals_types);
        LOG_SEV(Debug) << "    >> Field '" << rel.target_field_name << "' is a terminal so it has type "
                       << formatting::CLBB("String") << ".";
        continue;
      }

      // Only need to deduce types when we reference a *field* of another type. Otherwise,
      // we are just storing another node, so the type is a shared pointer to the type of
      // the node, or a container of pointers.
      //
      // First, check if we are in this simple case - where type is a shared pointer to or
      // container of shared pointers of the base class for the other node.
      if (!rel.source_field_name) {
        LOG_SEV(Trace) << "    >> Relationship references another node, but does not "
                          "reference a field of that node. Type will be a shared pointer. Referenced ID is "
                       << rel.referenced_id << ".";
        // The type will be the node type. We can fill this in right away, now that every
        // base class has been created.
        auto& base_type_name = deduction.GetBaseTypeName(rel.referenced_id);

        auto base_type = node_manager().GetNodeDescription(base_type_name);
        if (rel.check_type == CheckType::Field) {
          LOG_SEV(Trace) << "    >> Check type is FIELD.";
          auto shared_ptr_type = node_manager().MakeShared(base_type);
          // Note that the AddField function in set_field_type will make sure that if the
          // field is added again, the types match.
          set_field_type(type_name, rel.target_field_name, shared_ptr_type, nonterminals_types);

          LOG_SEV(Debug) << "    >> Relationship references another node, relation type is "
                            "'Field', field type will be "
                         << to_string(*shared_ptr_type) << ".";
        }
        else if (rel.check_type == CheckType::Push) {
          LOG_SEV(Trace) << "    >> Check type is PUSH.";
          // The type is std::vector<std::shared_ptr<T>>
          auto vector_type = node_manager().MakeVector(node_manager().MakeShared(base_type));
          // Note that the AddField function in set_field_type will make sure that if the
          // field is added again, the types match.
          set_field_type(type_name, rel.target_field_name, vector_type, nonterminals_types);

          LOG_SEV(Debug) << "    >> Relationship references another node, relation type is "
                            "'Push', field type will be "
                         << *vector_type << ".";
        }
        else if (rel.check_type == CheckType::Append) {
          MANTA_FAIL(
              "append must reference a source field since it must reference some vector "
              "type, and an AST node is not a vector type");
        }
      }
      // In this case, the relationship references a field of another node. Since the node
      // could have been formed by any production for the non-terminal, only common fields
      // for the non-terminal's base node type can be referenced.
      else {
        LOG_SEV(Trace) << "    >> There is no source field name.";

        auto referenced_nonterminal = rel.referenced_id;

        // Make sure the referenced field is common
        MANTA_ASSERT(deduction.IsCommonField(referenced_nonterminal, *rel.source_field_name),
                     "can only reference common fields from the types associated with "
                     "non-terminals, "
                     "the field named '"
                         << *rel.source_field_name << "' is not common. Target type is " << type_name
                         << " source type (which lacks the common field) is "
                         << deduction.GetBaseTypeName(referenced_nonterminal));

        // See if the type of the referenced field has already been filled in. If so, we
        // can process this relationship.
        auto& base_type_name = deduction.GetBaseTypeName(referenced_nonterminal);
        auto base_type = node_manager().GetNodeDescription(base_type_name);
        if (auto ty = base_type->fields.find(*rel.source_field_name);
            ty != base_type->fields.end() && ty->second)
        {
          // The field has already been typed and defined. We can handle this relationship
          // right now. It must be that the target type is some vector type.
          auto type = ty->second;

          if (rel.check_type == CheckType::Append) {
            MANTA_ASSERT(type->general_type == TSGeneralType::Vector,
                         "for append, the target field must be a vector type");
          }
          else if (rel.check_type == CheckType::Field) {
            // Nothing to check. The source and target fields will have the same types.
          }
          else if (rel.check_type == CheckType::Push) {
            // Type is a vector of <type>. Note that we don't have to worry about circular
            // dependencies here (pushing a type into itself).
            type = node_manager().MakeVector(type);
          }

          LOG_SEV(Info) << "    >> Source field has already been typed, can fill in "
                           "immediately. Type for field '"
                        << rel.target_field_name << "' is " << *type << ".";

          // TODO: Make sure that if the field has already been added, the types are the
          //  same.

          node_manager().GetNodeDescription(type_name)->AddField(rel.target_field_name, type);
          nonterminals_types.field_type_descriptions[rel.target_field_name] = type;
        }
        else {
          // There are circumstances under which the relationship is useless:
          //    * Field references itself.
          //    * Field appends itself.
          // The following is not allowed
          //    * Field pushes itself (there are more complicated versions of this that
          //    are disallowed, like cycles, but this is the simplest and easiest to check
          //    quickly).
          // Note that all these cases only happen when the referenced non-terminal and
          // non-terminal that the target type belongs to are the same.
          if (referenced_nonterminal == nonterminals_for_type().at(rel.referencing_type)) {
            bool is_self_reference = rel.target_field_name == *rel.source_field_name;
            if (is_self_reference) {
              MANTA_ASSERT(rel.check_type != CheckType::Push,
                           "no type T is equal to vector<T>, cannot push a field into itself");
              LOG_SEV(Debug) << "    << Found (valid) self-referential relationship, this "
                                "is consistent, but not useful. Dropping.";
            }
            else {
              LOG_SEV(Debug) << "    >> Found relationship for which we cannot immediately deduce "
                                "type, delaying type deduction for field '"
                             << rel.target_field_name << "'.";
              field_type_relationships.push_back(rel);
            }
          }
          else {
            LOG_SEV(Debug) << "    >> Source field has NOT been typed yet, delaying typing "
                              "until all base types have been deduced.";
            field_type_relationships.push_back(rel);
          }
        }
      }
    }
  }

  // Deduce the more complicated type relationships.
  if (field_type_relationships.empty()) {
    LOG_SEV(Info) << "Utilized all relationships in the first pass, no DFS pass needed.";
  }
  else {
    LOG_SEV(Info) << "After filling in immediate types and eliminating useless "
                     "relationships, there are "
                  << field_type_relationships.size() << " relationships to consider.";

    // Have to organize types mapping fields to relationships having this field as a
    // target. Note that we only have to do this for the remaining relationships, not all
    // the relationships that were passed to this function. This is because any
    // relationship that was processed above connected two (now) already deduced types, so
    // we do not need to follow this relationship. To follow a relationship like this, we
    // would have to start at the target field, but the target field's type will have been
    // deduced, so we wouldn't need to follow any of its relationships.
    std::map<FieldTracker, std::vector<TypeRelationship*>> unsolved_relationships;
    for (auto& relationship : field_type_relationships) {
      std::pair reference(relationship.referenced_id, *relationship.source_field_name);
      unsolved_relationships[reference].push_back(&relationship);
    }

    // Depth first search to deduce the type needed for each remaining relationship.
    for (auto& relationship : field_type_relationships) {
      if (!relationship.target_is_nonterminal) {
        continue;
      }
      // Check to see if the target field's type has already been deduced.
      auto nonterminal_id = nonterminals_for_type().at(relationship.referencing_type);
      auto type = deduction.GetFieldType(nonterminal_id, relationship.target_field_name);
      if (type) {
        continue;
      }

      std::set<FieldTracker> referenced_fields;  // Set for the DFS.

      std::pair reference(relationship.referenced_id, *relationship.source_field_name);
      type = deduceTypesDFS(reference, deduction, unsolved_relationships, referenced_fields);
      type = makeType(relationship.check_type, type);

      // TODO: Figure out the right way to handle const vs mutable types.
      auto& nonterminals_types = deduction.At(nonterminal_id);
      set_field_type(relationship.referencing_type,
                     relationship.target_field_name,
                     const_cast<TypeDescription*>(type),
                     nonterminals_types);
    }
  }

  // Remove fields from child nodes that are common, and should be only in the base node.
  LOG_SEV(Info) << "Removing fields from child nodes that should only be in parent node.";
  for (auto& [_, types] : deduction.types_data) {
    auto& base_type = types.base_type_name;
    for (auto& common_field : types.common_fields) {
      LOG_SEV(Debug) << "Searching sub-types for field " << formatting::CLW(common_field) << " to remove.";
      for (auto& [type_name, fields] : types.fields_for_type) {
        // Leave the common fields in the base type (only).
        if (type_name == base_type) {
          continue;
        }
        // Remove common fields from all other types.
        auto type_ptr = types.sub_types.at(type_name);

        LOG_SEV(Debug) << ">> Checking type " << formatting::CLBB(type_name) << ", has "
                       << type_ptr->fields.size() << " fields.";

        if (types.RemoveField(type_name, common_field)) {
          LOG_SEV(Debug) << "  * Removing the field '" << formatting::CLW(common_field) << "' from type "
                         << formatting::CLBB(type_name) << " since it is a common field.";
        }
      }
    }
  }

  return deduction;
}

void ParserDataToTypeManager::determineBaseTypes(TypeDeduction& deduction) {
  LOG_SEV(Info) << "Determining base types for all types in the TypeDeduction.";

  // For every non-terminal that needs it, create the base type.
  //
  // Right now the algorithm is that we create a base type for every non-terminal that has more than one
  // production (item) associated with it.
  for (auto& [nonterminal_id, nonterminals_types] : deduction.types_data) {
    LOG_SEV(Debug) << "Determining base type for non-terminal ID " << nonterminal_id << ".";
    MANTA_ASSERT(nonterminals_types.NumSubTypes() != 0,
                 "there must be at least one type for non-terminal " << nonterminal_id);
    auto& nonterminal_name = production_rules_data_->GetNonterminalName(nonterminal_id);

    // Add a non-terminal type enum.
    auto nonterminal_enum = node_manager().GetNonterminalEnum();
    auto nonterminal_enum_name = field_name_sanitizer_(nonterminal_name);  // Sanitize name.
    nonterminal_enum->AddOption(nonterminal_enum_name);

    // TODO: Consolidate "subclasses" that have no additional data.
    if (1 < nonterminals_types.NumSubTypes()) {
      auto base_name = "ASTNodeBase_" + nonterminal_name;
      // Add the new type name to the set of all type names.
      deduction.base_type_names.insert(base_name);

      LOG_SEV(Info) << "  * Non-terminal " << nonterminal_id << " has " << nonterminals_types.NumSubTypes()
                    << " subtypes. Creating base type named '" << base_name << "'.";

      nonterminals_types.base_type_name = base_name;

      // Create a type description for the base class.
      auto base_class_description = node_manager().GetNodeDescription(base_name, nonterminal_id);
      base_class_description->AddParent(node_manager().GetASTNodeBase());
      base_class_description->AddConstructor(StructureConstructor {
          // Arguments,
          {{node_manager().GetASTNodeType(), "node_type"}, {node_manager().GetItemID(), "item_id"}},
          // Call ASTNodeBase's constructor
          {{node_manager().GetASTNodeBase(),
            {"node_type", nonterminal_enum->GetName() + "::" + nonterminal_enum_name, "item_id"}}}});

      node_manager().GetNonterminalTypes(nonterminal_id).base_type = base_class_description;
      LOG_SEV(Info) << "  * Setting base type for non-terminal ID " << nonterminal_id << " (" << base_name
                    << ").";

      // Add the base class as the base class of all other classes for the non-terminal.
      auto& types = deduction.At(nonterminal_id);
      LOG_SEV(Info) << "  * There are " << types.sub_types.size() << " child types.";
      for (auto& [name, description] : types.sub_types) {
        LOG_SEV(Info) << "    >> Setting the parent class of " << formatting::CLBB(name) << " to be "
                      << formatting::CLBB(base_class_description->type_name) << ".";
        description->AddParent(base_class_description);
        description->AddConstructor(StructureConstructor {
            // Arguments
            {{node_manager().GetItemID(), "item_id"}},
            // Call base class's constructor
            {{base_class_description, {StructureConstructor::Value {"ASTNodeType::Type_" + name}, "item_id"}}}
        });
      }

      // Add the type.
      types.sub_types[base_name] = base_class_description;

      // The fields of the base type will be the common fields.
      types.fields_for_type[base_name].insert(types.common_fields.begin(), types.common_fields.end());
    }
    else {
      LOG_SEV(Info) << "  * Non-terminal " << nonterminal_id << " has no subtypes.";

      // Get the name of the only type. This type will already be in the sub-types, since
      // we didn't newly create it to be a base class type.
      auto type_name = nonterminals_types.fields_for_type.begin()->first;
      nonterminals_types.base_type_name = type_name;
      auto type = node_manager().GetNodeDescription(type_name);
      LOG_SEV(Info) << "  * Setting the parent class of type " << formatting::CLBB(type_name) << " to be "
                    << formatting::CLBB("ASTNodeBase") << ".";
      type->AddParent(node_manager().GetASTNodeBase());
      type->AddConstructor(StructureConstructor {
          // Arguments,
          {{node_manager().GetItemID(), "item_id"}},
          // Call ASTNodeBase's constructor
          {{node_manager().GetASTNodeBase(),
            {StructureConstructor::Value {"ASTNodeType::Type_" + type_name},
             StructureConstructor::Value {nonterminal_enum->GetName() + "::" + nonterminal_enum_name},
            "item_id"}}}});
    }

    LOG_SEV(Info) << "  * The base type name for non-terminal " << nonterminal_id << "'s types will be "
                  << formatting::CLBB(nonterminals_types.base_type_name) << ".";
  }
}

std::tuple<int, NonterminalID, std::optional<std::string>> ParserDataToTypeManager::getSourceData(
    const std::string& argument_string, const Item& item) {
  auto segments = split(argument_string, '.');
  MANTA_ASSERT(segments.size() == 1 || segments.size() == 2,
               "argument name must be in one of the forms '$N' or '$N.<field-name>'");
  auto position = std::stoi(segments[0]);
  auto referenced_type = item.rhs.at(position);
  if (segments.size() == 1) {
    return {position, referenced_type, {} /* No field name */};
  }
  else {  // Specifies field name.
    return {position, referenced_type, field_name_sanitizer_(segments[1])};
  }
}

void ParserDataToTypeManager::createGeneralNode(const std::string& type_name,
                                                NonterminalID nonterminal_id,
                                                const std::string& nonterminal_name,
                                                const Item& item,
                                                std::map<std::string, NonterminalID>& nonterminals_for_type,
                                                unsigned item_number) {
  // There are no instructions. Add everything that isn't a "literal" as a field.
  auto node_type_description = node_manager().GetNodeDescription(type_name, nonterminal_id);
  nonterminals_for_type[type_name] = nonterminal_id;

  LOG_SEV(Debug) << "  * Creating general node for non terminal ID " << nonterminal_id << ", type name "
                 << formatting::CLBB(type_name) << ".";

  int count = -1;
  for (auto referenced_type : item.rhs) {
    // Increment here so if we continue, we still always increment count.
    ++count;

    auto& name = production_rules_data_->GetName(referenced_type);

    if (production_rules_data_->lexer_generator->IsReserved(name)) {
      LOG_SEV(Debug) << "    >> Not creating a field for '" << name << "' since it is a literal.";

      continue;  // Literal
    }
    auto target_field_name = field_name_sanitizer_(name);
    if (tag_generated_field_names_) {
      target_field_name += "_" + std::to_string(count);
    }

    if (production_rules_data_->IsNonTerminal(referenced_type)) {
      // Non-terminal
      if (generated_nodes_have_node_in_name_) {
        target_field_name += "_node";
      }
    }
    else {
      // Directly add the field, which is known to be a string.
      // TODO: Allow for other types of fields? The problem would be that, like llvm, we'd
      // need arbitrary precision numbers
      //  so we didn't lose information. Strings do not have this problem.
      node_type_description->AddField(target_field_name, node_manager().GetStringType());
    }

    LOG_SEV(Debug) << "    >> Target field will be named '" << target_field_name << "'.";

    TypeRelationship relationship {static_cast<NonterminalID>(referenced_type),
                                   production_rules_data_->IsNonTerminal(referenced_type),
                                   {},  // No field access needed.
                                   node_type_description->type_name,
                                   target_field_name,
                                   CheckType::Field,
                                   static_cast<int>(count),  // Position in the reduction.
                                   item_number};
    relationships()[node_type_description->type_name].push_back(relationship);
  }
}

void ParserDataToTypeManager::processFieldCommand(const std::vector<std::shared_ptr<ParseNode>>& arguments,
                                                  const Item& item,
                                                  unsigned item_number,
                                                  TypeDescriptionStructure* node_type_description) {
  auto nonterminal_id = item.production;
  auto& instructions = item.instructions;
  auto&& nonterminal_name = production_rules_data_->GetNonterminalName(nonterminal_id);

  LOG_SEV(Debug) << "  * Processing 'field' command for non-terminal '" << nonterminal_name << "'.";

  auto get_arg = [&](auto i) -> std::string& { return arguments[i]->designator; };

  auto num_args = arguments.size();
  MANTA_ASSERT(num_args == 1 || num_args == 2, "field function needs one or two arguments, not " << num_args);

  std::string target_field_name {};
  auto [position, referenced_type, source_field_name] = getSourceData(get_arg(0), item);
  LOG_SEV(Debug) << "    >> Extracted field command: [pos = " << position
                 << "] [ref type = " << referenced_type
                 << "] [src field = " << (source_field_name ? source_field_name.value() : "{}") << "]";

  // Type is a non-terminal. A field name does not have to be specified.
  if (production_rules_data_->IsNonTerminal(referenced_type)) {
    if (num_args == 1) {  // One argument: ($N[.field_name])
      if (source_field_name) {
        // Referenced a field of the node, $N.field_name.
        target_field_name = *source_field_name;
        LOG_SEV(Debug) << "    >> Single argument field command, target field is the field '"
                       << target_field_name << "' of target node.";
      }
      else {
        // References the node itself, $N (i.e. not a field of the other node, but the
        // entire other node).
        target_field_name = production_rules_data_->GetNonterminalName(referenced_type);
        LOG_SEV(Debug) << "    >> Single argument field command, target is the other node.";
      }
    }
    else {  // Two arguments: ($N[.src_field_name], target_field_name)
      target_field_name = get_arg(1);
      LOG_SEV(Debug) << "    >> Two argument field command targets, target field name '" << target_field_name
                     << "'.";
    }
  }
  // Type is a terminal. Field name must be specified. Type check not required.
  else {
    MANTA_ASSERT(num_args == 2,
                 "field name must be specified to create a field for a terminal, "
                 "non-terminal type is '"
                     << production_rules_data_->GetNonterminalName(nonterminal_id) << "', item number "
                     << item_number);
    target_field_name = get_arg(1);

    // Directly add the field, which is known to be a string.
    // TODO: Allow for other types of fields? The problem would be that, like llvm, we'd
    //  need arbitrary precision numbers so we didn't lose information. Strings do not
    //  have this problem.
    node_type_description->AddField(target_field_name, node_manager().GetStringType());

    LOG_SEV(Info) << "    >> Adding field referring to a terminal, '" << target_field_name << "', to type "
                  << formatting::CLBB(node_type_description->type_name) << ".";
  }

  TypeRelationship relationship {
      static_cast<NonterminalID>(referenced_type),
      production_rules_data_->IsNonTerminal(referenced_type),
      source_field_name,
      node_type_description->type_name,
      target_field_name,
      CheckType::Field,
      position,
      item_number,
  };
  relationships()[node_type_description->type_name].push_back(relationship);
}

void ParserDataToTypeManager::processAppendCommand(const std::vector<std::shared_ptr<ParseNode>>& arguments,
                                                   const Item& item,
                                                   unsigned item_number,
                                                   TypeDescriptionStructure* node_type_description) {
  // Form: append($N.field_name_1, field_name_2)

  auto nonterminal_id = item.production;
  auto& instructions = item.instructions;
  auto&& nonterminal_name = production_rules_data_->GetNonterminalName(nonterminal_id);

  LOG_SEV(Debug) << "  * Processing 'append' command for non-terminal '" << nonterminal_name << "'.";

  auto get_arg = [&](auto i) -> std::string& { return arguments[i]->designator; };

  MANTA_ASSERT(arguments.size() == 2, "append function needs two arguments, not " << arguments.size());

  std::string target_field_name {};
  auto [position, referenced_type, source_field_name] = getSourceData(get_arg(0), item);
  MANTA_ASSERT(source_field_name,
               "append function's first argument must reference a field, item was "
                   << item_number << " for non-terminal " << nonterminal_name);

  LOG_SEV(Debug) << "    >> Extracted field command: [pos = " << position
                 << "] [ref type = " << referenced_type
                 << "] [src field = " << (source_field_name ? source_field_name.value() : "{}") << "]";

  TypeRelationship relationship {
      static_cast<NonterminalID>(referenced_type),
      production_rules_data_->IsNonTerminal(referenced_type),
      source_field_name,
      node_type_description->type_name,
      get_arg(1),
      CheckType::Append,
      position,
      item_number,
  };
  relationships()[node_type_description->type_name].push_back(relationship);
}

void ParserDataToTypeManager::processPushCommand(const std::vector<std::shared_ptr<ParseNode>>& arguments,
                                                 const Item& item,
                                                 unsigned item_number,  // item_number
                                                 TypeDescriptionStructure* node_type_description) {
  auto nonterminal_id = item.production;
  auto& instructions = item.instructions;
  auto&& nonterminal_name = production_rules_data_->GetNonterminalName(nonterminal_id);

  LOG_SEV(Debug) << "  * Processing 'push' command for non-terminal '" << nonterminal_name << "'.";

  auto get_arg = [&](auto i) -> std::string& { return arguments[i]->designator; };

  MANTA_REQUIRE(arguments.size() == 2, "push function needs two arguments, not " << arguments.size());

  std::string target_field_name {};
  auto [position, referenced_type, source_field_name] = getSourceData(get_arg(0), item);

  LOG_SEV(Debug) << "    >> Extracted field command: [pos = " << position
                 << "] [ref type = " << referenced_type
                 << "] [src field = " << (source_field_name ? source_field_name.value() : "{}") << "]";

  TypeRelationship relationship {
      static_cast<NonterminalID>(referenced_type),
      true,
      source_field_name,
      node_type_description->type_name,
      get_arg(1),
      CheckType::Push,
      position,
      item_number,
  };
  relationships()[node_type_description->type_name].push_back(relationship);
}

const TypeDescription* ParserDataToTypeManager::makeType(CheckType check_type,
                                                         const TypeDescription* source_type) {
  if (check_type == CheckType::Push) {
    return node_manager().MakeVector(source_type);
  }
  else if (check_type == CheckType::Append) {
    MANTA_ASSERT(source_type->general_type == TSGeneralType::Vector,
                 "cannot append a non-vector to a vector");
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

const TypeDescription* ParserDataToTypeManager::deduceTypesDFS(
    FieldTracker& target_field,
    TypeDeduction& deduction,
    const std::map<FieldTracker, std::vector<TypeRelationship*>>& unsolved_relationships,
    std::set<FieldTracker>& referenced_fields) {
  // Depth first search to deduce the type needed for each remaining relationship.
  const TypeDescription* type = deduction.GetFieldType(target_field.first, target_field.second);
  if (type) {
    return type;
  }

  // Type not deduced yet.
  if (auto it = unsolved_relationships.find(target_field); it != unsolved_relationships.end()) {
    // Try each relationship whose source field has not already been referenced (to
    // prevent infinite loops).
    for (auto& rel : it->second) {
      std::pair reference(rel->referenced_id, *rel->source_field_name);
      if (!referenced_fields.contains(reference)) {
        referenced_fields.insert(reference);
        type = deduceTypesDFS(reference, deduction, unsolved_relationships, referenced_fields);
      }
      if (type) {
        // Found the type of a field referenced by this relationship. Determine the type
        // of the target field based on this relationship.
        type = makeType(rel->check_type, type);
        break;
      }
    }
  }

  MANTA_ASSERT(type, "could not deduce type for field " << target_field.first << ", " << target_field.second);

  // Set the type in all the different data structures.
  auto& base_type_name = deduction.GetBaseTypeName(target_field.first);
  node_manager().GetNodeDescription(base_type_name)->AddField(target_field.second, type);
  deduction.At(target_field.first).field_type_descriptions[target_field.second] = type;

  return type;
}