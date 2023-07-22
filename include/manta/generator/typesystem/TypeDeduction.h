//
// Created by Nathaniel Rupprecht on 3/17/23.
//

#pragma once

#include "manta/generator/typesystem/ASTNodeDescription.h"

namespace manta::typesystem {

class TypeDeduction {
 public:

  struct NonterminalsTypes {
    //! \brief All field names from any type for the non-terminal.
    std::set<std::string> all_fields{};

    //! \brief Map from a type associated with a non-terminal to the set of all field names for the type.
    std::map<std::string, std::set<std::string>> fields_for_type;

    //! \brief The fields that all types for this non-terminal have in common.
    std::set<std::string> common_fields{};

    //! \brief Map from a field for a type associated with a non-terminal to a description of that field's type type.
    //!
    //! This map's index contains all fields.
    //!
    std::map<std::string, const TypeDescription*> field_type_descriptions{};

    //! \brief The type descriptions of the sub-types.
    std::map<std::string, TypeDescriptionStructure*> sub_types{};

    //! \brief If there are multiple subtypes for this nonterminal, we have them derive from a base type.
    //! Otherwise, there is only one node type, so that is the base node type.
    //!
    std::string base_type_name{};

    void AddType(const std::string& type_name) {
      fields_for_type[type_name]; // Will initialize an empty set if the entry is not already there.
      sub_types[type_name];
    }

    void AddField(const std::string& type_name, const std::string& field_name, const TypeDescription* field_type = nullptr) {
      all_fields.insert(field_name);
      fields_for_type[type_name].insert(field_name);
      field_type_descriptions[field_name] = field_type;
    }

    NO_DISCARD const std::set<std::string>& GetFields(const std::string& type_name) const {
      return fields_for_type.at(type_name);
    }

    NO_DISCARD const TypeDescription* GetFieldType(const std::string& field_name) const {
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

  void AddField(NonterminalID nonterminal, const std::string& type_name, const std::string& field_name, const TypeDescription* field_type = nullptr) {
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

  //! \brief Get the type of a common field.
  //!
  NO_DISCARD const TypeDescription* GetFieldType(NonterminalID nonterminal_id, const std::string& field_name) const {
    return types_data.at(nonterminal_id).field_type_descriptions.at(field_name);
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
  std::set<std::string> all_type_names{};
  //! \brief The names of all types that were additionally generated as base types for nodes.
  std::set<std::string> base_type_names{};
};

} // namespace manta::typesystem
