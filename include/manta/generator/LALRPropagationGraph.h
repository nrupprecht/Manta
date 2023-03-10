#pragma once

#include "manta/utility/ParserUtility.hpp"
#include <deque>
#include <set>

namespace manta {

//! \brief LALR uses "state items" that are pairs of states and items.
using StateItem = std::pair<StateID, Item>;

//! \brief Typedef for an LALR item follow set.
using ItemFollowSet = std::map<StateItem, std::set<int>>;


//! \brief A structure to represent an LALR propagation graph, which is used to construct LALR parsers.
class LALRPropagationGraph {
 public:
  void AddVertex(const StateItem& v) {
    vertices_.insert(v);
  }

  void AddEdge(const StateItem& v, const StateItem& w) {
    auto [_, did_insert] = edges_[v].insert(w);
    if (did_insert) {
      ++num_edges_;
    }

    vertices_.insert(v);
    vertices_.insert(w);
  }

  NO_DISCARD bool HasVertex(const StateItem& v) const {
    return vertices_.contains(v);
  }

  NO_DISCARD const std::map<StateItem, std::set<StateItem>>& Edges() const {
    return edges_;
  }

  NO_DISCARD const std::set<StateItem>& Vertices() const {
    return vertices_;
  }

  NO_DISCARD std::size_t NumVertices() const {
    return vertices_.size();
  }

  NO_DISCARD std::size_t NumEdges() const {
    return num_edges_;
  }

  void Clear() {
    edges_.clear();
  }

 private:
  std::set<StateItem> vertices_;
  std::map<StateItem, std::set<StateItem>> edges_;
  std::size_t num_edges_ = 0;
};

} // namespace manta::generator
