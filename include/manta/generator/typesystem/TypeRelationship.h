//
// Created by Nathaniel Rupprecht on 3/17/23.
//

#pragma once

#include <optional>
#include <string>

namespace manta {
using NonterminalID = int;
}

namespace manta::typesystem {

enum class CheckType {
  Field, Append, Push
};

struct TypeRelationship {
  //! \brief The nonterminal type of the $N
  NonterminalID nonterminal_id{};

  //! \brief If a dependent name was referenced, it is stored here.
  std::optional<std::string> source_field_name{};

  //! \brief The type which requested the data. Items for different productions of the same non-terminal
  //! may or may not have the same type, so we use the name of the actual type, not item ID or the
  //! non terminal ID.
  std::string referencing_type{};

  //! \brief The field name in the node for the reducing item.
  std::string target_field_name{};

  //! \brief In what context the name was being used.
  //!
  //! For all checks, it must be checked that all references to this name for the node sub-types of nonterminal_id must be of the same type,
  //! and any source_field_name must exist in each non-terminal sub-type.
  //!
  //! Field check:    <no additional checks>
  //! Append check:   The type must be a vector type.
  //! Push check:     The type of target_field_name must be a vector type, whose vector type matches that of the pushed node or node field.
  CheckType check_type = CheckType::Field;
};

}