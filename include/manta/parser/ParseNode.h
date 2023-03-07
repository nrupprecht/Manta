//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#ifndef MANTACLION_PARSENODE_H
#define MANTACLION_PARSENODE_H

#include <utility>

#include "ParserUtility.hpp"

namespace manta {

struct ParseNode : public std::enable_shared_from_this<ParseNode> {
  explicit ParseNode(const std::string& d);

  ParseNode(std::string designator, const std::shared_ptr<ParseNode>& p);

  ParseNode &operator=(const ParseNode &node);

  void Add(const std::string &str);

  void Add(const std::shared_ptr<ParseNode> &node);

  std::string printTerminals() const;

  std::string printTree(int level = 0) const;

  //! Node label.
  std::string designator;

  //! \brief The node may have a parent. Use a weak_ptr since the parent has shared pointers to its children.
  std::optional<std::weak_ptr<ParseNode>> parent{};

  //! \brief All the children of the Node.
  std::vector<std::shared_ptr<ParseNode>> children;
};

//! \brief Stream operator to write a ParseNode.
std::ostream &operator<<(std::ostream &out, const ParseNode &node);

}
#endif //MANTACLION_PARSENODE_H
