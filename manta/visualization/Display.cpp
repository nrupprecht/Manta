//
// Created by Nathaniel Rupprecht on 8/7/21.
//

#include "manta/visualization/Display.h"
// Other files.
#include <cmath>

namespace manta {

std::string Display::RenderParseTree(const std::shared_ptr<ParseNode>& node) {
  auto w = width(node);
  auto h = height(node);

  std::string output((w + 1) * h, ' ');
  for (std::size_t i = 1; i <= h; ++i) {
    output[i * (w + 1) - 1] = '\n';
  }

  renderNode(node, output, w, h, 0, 0);

  return output;
}

std::size_t Display::renderNode(const std::shared_ptr<ParseNode>& node,
                                std::string& buffer,
                                std::size_t w,
                                std::size_t h,
                                std::size_t x0,
                                std::size_t y0) {
  // Turn a relative coordinate in 2d sub-buffer space to a position in the buffer string.
  auto addr = [=](int x, int y) { return (y0 + y) * (w + 1) + x + x0; };
  // If this is a terminal, write yourself and return.
  if (node->children.empty()) {
    std::copy(node->designator.begin(), node->designator.end(), buffer.begin() + addr(0, 0));
    return 0;
  }

  // Have all children write themselves
  std::size_t sum_heights = 0, first_y = 0, last_y = 0, count = 0;
  for (const auto& child : node->children) {
    auto x = treeLength(node) - treeLength(child);
    auto y_write = renderNode(child, buffer, w, h, x0 + x, y0 + sum_heights);

    for (std::size_t i = node->designator.length() + 4; i < x; ++i) {
      buffer[addr(i, y_write + sum_heights)] = '-';
    }

    if (count == 0) {
      first_y = sum_heights + y_write;  // Get in this sub-buffer's coordinates.
    }
    ++count;
    if (count == node->children.size()) {
      last_y = sum_heights + y_write;  // Get in this sub-buffer's coordinates.
    }

    sum_heights += height(child) + 1;
  }
  sum_heights -= 1;  // Should now be equal to height.

  auto write_y = static_cast<int>(std::ceil(0.5 * (last_y + first_y)));

  auto start = addr(0, write_y);
  std::copy(node->designator.begin(), node->designator.end(), buffer.begin() + start);
  // Write the " --"
  buffer[start + node->designator.length() + 1] = '-';
  buffer[start + node->designator.length() + 2] = '-';

  for (std::size_t i = first_y; i <= last_y; ++i) {
    buffer[addr(node->designator.length() + 3, i)] = '|';
  }

  return write_y;
}

std::size_t Display::width(const std::shared_ptr<ParseNode>& node) {
  return treeLength(node) + maxTerminal(node);
}

std::size_t Display::height(const std::shared_ptr<ParseNode>& node) {
  if (node->children.empty()) {
    return 1;
  }

  std::size_t sum_heights = 0;
  for (const auto& child : node->children) {
    sum_heights += height(child);
  }
  return node->children.size() - 1 + sum_heights;
}

std::size_t Display::treeLength(const std::shared_ptr<ParseNode>& node) {
  if (node->children.empty()) {
    return 0;
  }

  std::size_t max_tree_length = 0;
  for (const auto& child : node->children) {
    auto tree_length = treeLength(child);
    max_tree_length = max_tree_length < tree_length ? tree_length : max_tree_length;
  }

  return node->designator.size() + 6 + max_tree_length;
}

std::size_t Display::maxTerminal(const std::shared_ptr<ParseNode>& node) {
  if (node->children.empty()) {
    return node->designator.size();
  }

  std::size_t max_terminal = 0;
  for (const auto& child : node->children) {
    auto terminal = maxTerminal(child);
    max_terminal = max_terminal < terminal ? terminal : max_terminal;
  }

  return max_terminal;
}

}