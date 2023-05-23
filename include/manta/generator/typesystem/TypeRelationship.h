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
  //! \brief The terminal or non-terminal type of the $N
  NonterminalID referenced_id{};

  //! \brief Whether the item referenced by referenced_id is actually a non-terminal.
  bool target_is_nonterminal = true;

  //! \brief If a dependent name was referenced, it is stored here.
  std::optional<std::string> source_field_name{};

  //! \brief The type which requested the data.
  //!
  //! Items for different productions of the same non-terminal may or may not have the same type,
  //! so we use the name of the actual type, not item ID or the non terminal ID.
  std::string referencing_type{};

  //! \brief The field name in the node for the reducing item.
  std::string target_field_name{};

  //! \brief In what context the name was being used, or, what assignment must be done.
  //!
  //! For all checks, it must be checked that all references to this name for the node sub-types of referenced_id must be of the same type,
  //! and any source_field_name must exist in each non-terminal sub-type.
  //!
  //! Field check:    <no additional checks>
  //! Append check:   The type must be a vector type.
  //! Push check:     The type of target_field_name must be a vector type, whose vector type matches that of the pushed node or node field.
  CheckType check_type = CheckType::Field;

  //! \brief What position is the target in the RHS of the item that created this relationship.
  int position{};
};

}