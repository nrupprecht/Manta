//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#include "manta/parser/ParseNode.h"

using namespace manta;

ParseNode::ParseNode(const std::string& d) {
  for (auto c: d) {
    if (c == '\n') designator.append("\\n");
    else if (c == '\t') designator.append("\\t");
    else designator.push_back(c);
  }
};

ParseNode &ParseNode::operator=(const ParseNode &node) {
  designator = node.designator;
  for (const auto &child: node.children) {
    auto new_child = std::make_shared<ParseNode>("");
    *new_child = *child;
    children.push_back(new_child);
  }
  return *this;
}

void ParseNode::Add(const std::string &str) {
  children.push_back(std::make_shared<ParseNode>(str));
}

void ParseNode::Add(const std::shared_ptr<ParseNode> &node) {
  children.push_back(node);
}

std::ostream &manta::operator<<(std::ostream &out, const ParseNode &node) {
  // Make sure we don't print actual newlines or things like that.
  std::string alias = node.designator;
  if (alias == "\n") alias = "\\n";
  else if (alias == "\t") alias = "\\t";

  // Print out node.
  out << "[ " << alias;
  int size = static_cast<int>(node.children.size());
  if (0 < size) {
    out << ": {";
    for (int i = 0; i < size; ++i) {
      out << *node.children[i];
      if (i != 0) {
        out << ", ";
      }
    }
    out << "}";
  }
  out << " ]";
  // Return the stream.
  return out;
}

std::string ParseNode::printTerminals() const {
  // If this is a terminal.
  if (children.empty()) {
    std::string alias{};
    for (auto c: designator) {
      if (c == '\n') alias.append("\\n");
      else if (c == '\t') alias.append("\\t");
      else alias.push_back(c);
    }
    return alias + " ";
  }
  // If this is a production.
  std::string str;
  //for (auto it = children.rbegin(); it!=children.rend(); ++it)
  for (auto &it: children) {
    str += it->printTerminals();
  }
  // Return.
  return str;
}

std::string ParseNode::printTree(int level) const {
  std::string str;
  std::string alias = designator;
  if (alias == "\n") alias = "\\n";
  else if (alias == "\t") alias = "\\t";
  str += repeat('|', level) + alias;
  if (!children.empty()) {
    str += '\n';
    //for (int i=children.size()-1; 0<=i; --i) {
    for (std::size_t i = 0; i < children.size(); ++i) {
      str += children[i]->printTree(level + 1);
      if (i != children.size() - 1) str += "\n";
    }
  }
  // Return the string.
  return str;
}