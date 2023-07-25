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

//! \brief Enumerate what types of relationships between node structures must be checked.
enum class CheckType
{
  Field,
  Append,
  Push
};

//! \brief To-string function for CheckType.
inline std::string to_string(CheckType type) {
  switch (type) {
    case CheckType::Field:
      return "CheckType::Field";
    case CheckType::Append:
      return "CheckType::Append";
    case CheckType::Push:
      return "CheckType::Push";
    default:
      MANTA_FAIL("unrecognized CheckType");
  }
}

struct TypeRelationship {
  //! \brief The terminal or non-terminal type of the $N
  NonterminalID referenced_id {};

  //! \brief Whether the item referenced by referenced_id is actually a non-terminal.
  bool target_is_nonterminal = true;

  //! \brief If a dependent name was referenced, it is stored here.
  std::optional<std::string> source_field_name {};

  //! \brief The type which requested the data.
  //!
  //! Items for different productions of the same non-terminal may or may not have the
  //! same type, so we use the name of the actual type, not item ID or the non terminal
  //! ID.
  std::string referencing_type {};

  //! \brief The field name in the node for the reducing item.
  std::string target_field_name {};

  //! \brief In what context the name was being used, or, what assignment must be done.
  //!
  //! For all checks, it must be checked that all references to this name for the node
  //! sub-types of referenced_id must be of the same type, and any source_field_name must
  //! exist in each non-terminal sub-type.
  //!
  //! Field check:    <no additional checks>
  //! Append check:   The type must be a vector type.
  //! Push check:     The type of target_field_name must be a vector type, whose vector
  //! type matches that of the pushed node or node field.
  CheckType check_type = CheckType::Field;

  //! \brief What position is the target in the RHS of the item that created this
  //! relationship.
  int position {};
};

}  // namespace manta::typesystem