//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#pragma once

#include "manta/utility/ParserUtility.h"

namespace manta {

struct ParseNode {
  explicit ParseNode(const std::string& designator);

  ParseNode &operator=(const ParseNode &node);

  void Add(const std::string &str);

  void Add(const std::shared_ptr<ParseNode> &node);

  std::string printTerminals() const;

  std::string printTree(int level = 0) const;

  //! Node label.
  std::string designator;

  //! \brief All the children of the Node.
  std::vector<std::shared_ptr<ParseNode>> children;
};

//! \brief Stream operator to write a ParseNode.
std::ostream &operator<<(std::ostream &out, const ParseNode &node);

} // namespace manta
