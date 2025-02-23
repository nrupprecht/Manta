//
// Created by Nathaniel Rupprecht on 8/7/21.
//

#pragma once

#include "manta/parser/ParseNode.h"

namespace manta {

class Display {
 public:
  static std::string RenderParseTree(const std::shared_ptr<ParseNode> &node);

 private:

  //! \brief Render a node into the buffer.
  //!
  //! \param node The node to render.
  //! \param buffer The string buffer to write to.
  //! \param w The total width of the string buffer.
  //! \param h The total height of the string buffer
  //! \param x0 The starting x for this node in the buffer.
  //! \param y0 The starting y for this node in the buffer.
  //! \return The y (relative / sub-buffer) position at which the name was written.
  static std::size_t renderNode(const std::shared_ptr<ParseNode> &node,
                                std::string &buffer,
                                std::size_t w,
                                std::size_t h,
                                std::size_t x0,
                                std::size_t y0);

  static std::size_t width(const std::shared_ptr<ParseNode> &node);
  static std::size_t height(const std::shared_ptr<ParseNode> &node);
  static std::size_t treeLength(const std::shared_ptr<ParseNode> &node);
  static std::size_t maxTerminal(const std::shared_ptr<ParseNode> &node);
};

} // namespace manta
