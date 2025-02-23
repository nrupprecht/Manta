//
// Created by Nathaniel Rupprecht on 3/17/23.
//

#pragma once

#include "manta/generator/typesystem/ASTNodeDescription.h"

namespace manta::typesystem {

//! \brief Structure that handles all the types associated with a non-terminal.
//!
//! In general, there will be many types per non-terminal, since there are many productions per non-terminal.
struct NonterminalsTypes {
  //! \brief All field names from any type for the non-terminal.
  std::set<std::string> all_fields {};

  //! \brief Map from a type associated with a non-terminal to the set of all field names for the type.
  std::map<std::string, std::set<std::string>> fields_for_type;

  //! \brief The fields that all types for this non-terminal have in common.
  std::set<std::string> common_fields {};

  //! \brief Map from a field name for a type associated with a non-terminal to a description of that field's
  //!        type type.
  //!
  //! This map's index contains all fields.
  std::map<std::string, const TypeDescription*> field_type_descriptions {};

  //! \brief The type descriptions of the sub-types.
  std::map<std::string, TypeDescriptionStructure*> sub_types {};

  //! \brief If there are multiple subtypes for this nonterminal, we have them derive from a base type.
  //!        Otherwise, there is only one node type, so that is the base node type.
  std::string base_type_name {};

  //! \brief Register that a type name should exists for a type for the non-terminal.
  void AddType(const std::string& type_name) {
    // Will initialize empty sets if the entry is not already there.
    fields_for_type[type_name];
    sub_types[type_name];
  }

  void AddField(const std::string& type_name,
                const std::string& field_name,
                const TypeDescription* field_type = nullptr) {
    all_fields.insert(field_name);
    fields_for_type[type_name].insert(field_name);
    if (auto& ptr = field_type_descriptions[field_name]) {
      ptr = field_type;
    }
  }

  bool RemoveField(const std::string& type_name, const std::string& field_name) {
    auto type_ptr = sub_types.at(type_name);
    type_ptr->RemoveField(field_name);
    // Remove also from the types deduction data structure.
    fields_for_type.at(type_name).erase(field_name);
    return false;
  }

  NO_DISCARD const std::set<std::string>& GetFields(const std::string& type_name) const {
    return fields_for_type.at(type_name);
  }

  NO_DISCARD const TypeDescription* GetFieldType(const std::string& field_name) const {
    return field_type_descriptions.at(field_name);
  }

  //! \brief Find the common fields. All field information must have been added.
  std::size_t FindCommonFields() {
    common_fields.clear();  // Make sure this is empty.

    // For every field, check if it occurs in every type.
    for (auto& field : all_fields) {
      bool is_common = true;
      for (const auto& [_, type_fields] : fields_for_type) {
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

  //! \brief Get the number of sub-types for the non-terminal.
  NO_DISCARD std::size_t NumSubTypes() const { return fields_for_type.size(); }

};  // struct NonterminalsTypes

//! \brief A structure that handles all the data about all node types needed for every production and
//!        non-terminal.
class TypeDeduction {
public:
  void AddType(NonterminalID nonterminal_id, const std::string& type_name) {
    types_data[nonterminal_id].AddType(type_name);
    all_type_names.insert(type_name);
  }

  void AddField(NonterminalID nonterminal,
                const std::string& type_name,
                const std::string& field_name,
                const TypeDescription* field_type = nullptr) {
    types_data[nonterminal].AddField(type_name, field_name, field_type);
  }

  //! \brief Find common fields for all non-terminals' types.
  //!
  //! \return The number of common fields across all types.
  std::size_t FindCommonFields() {
    std::size_t num_common_fields = 0;
    for (auto& [_, nonterminal_types] : types_data) {
      num_common_fields += nonterminal_types.FindCommonFields();
    }
    return num_common_fields;
  }

  NO_DISCARD bool IsCommonField(NonterminalID nonterminal_id, const std::string& field_name) const {
    auto& common_fields = types_data.at(nonterminal_id).common_fields;
    return common_fields.contains(field_name);
  }

  //! \brief Get the type of a common field.
  NO_DISCARD const TypeDescription* GetFieldType(NonterminalID nonterminal_id,
                                                 const std::string& field_name) const {
    auto it = types_data.find(nonterminal_id);
    MANTA_ASSERT(it != types_data.end(), "Could not find type data for non-terminal " << nonterminal_id);
    return it->second.field_type_descriptions.at(field_name);
  }

  NO_DISCARD const std::string& GetBaseTypeName(NonterminalID nonterminal_id) const {
    auto it = types_data.find(nonterminal_id);
    MANTA_ASSERT(it != types_data.end(), "Could not find type data for non-terminal " << nonterminal_id);
    return it->second.base_type_name;
  }

  NO_DISCARD NonterminalsTypes& At(NonterminalID nonterminal_id) { return types_data.at(nonterminal_id); }

  //! \brief Fill the sub-types map for every set of non-terminals' types.
  void GetTypeDescriptions(ASTNodeManager& node_manager) {
    for (auto& [_, nonterminals_types] : types_data) {
      for (const auto& [type_name, _2] : nonterminals_types.fields_for_type) {
        nonterminals_types.sub_types[type_name] = node_manager.GetNodeDescription(type_name);
      }
    }
  }

  NO_DISCARD const std::map<NonterminalID, NonterminalsTypes>& GetTypesData() const { return types_data; }

  //! \brief The data about all type associated with each non-terminal.
  std::map<NonterminalID, NonterminalsTypes> types_data;

  //! \brief The set of all type names for all non-terminals.
  std::set<std::string> all_type_names {};

  //! \brief The names of all types that were additionally generated as base types for nodes.
  std::set<std::string> base_type_names {};
};

}  // namespace manta::typesystem
