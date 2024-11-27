//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#include "manta/generator/ParserGenerator.h"
// Other files
#include "manta/generator/DescriptionParser.h"
#include "manta/parser/LALRParser.hpp"

using namespace lightning;

namespace manta {

struct WritableItem {
  const Item& item;
  ParserGenerator* generator;
};

void format_logstream(const WritableItem& item, RefBundle& handler) {
  handler << item.generator->WriteItem(item.item);
}

// ====================================================================================
//  ParserGenerator
// ====================================================================================

ParserGenerator::ParserGenerator(ParserType type) : parser_type_(type) {}

void ParserGenerator::SetDescriptionParser(std::shared_ptr<DescriptionParser> description_parser) {
  description_parser_ = std::move(description_parser);
}

std::shared_ptr<ParserData> ParserGenerator::CreateParserData(std::istream& stream) {
  // Parse the stream to get description of the lexer_generator and the parser.
  production_rules_data_ = description_parser_->ParseDescription(stream);
  MANTA_ASSERT(production_rules_data_, "failed to parse the description");

  // Now, create the parser from its description.

  // Compute which productions can derive empty.
  createStateDerivesEmpty();

  // Generate the parse table.
  computeLR0();

  // Complete the table
  completeTable();

  auto parser_data                   = std::make_shared<ParserData>();
  parser_data->production_rules_data = production_rules_data_;
  parser_data->parse_table           = parse_table_;
  parser_data->all_states            = all_states_;

  return parser_data;
}

std::shared_ptr<LALRParser> ParserGenerator::CreateParserFromFile(const std::string& filename) {
  std::ifstream fin(filename);
  if (fin.fail()) {
    return nullptr;
  }
  auto parser = CreateParserFromStream(fin);
  fin.close();
  return parser;
}

std::shared_ptr<LALRParser> ParserGenerator::CreateParserFromString(const std::string& description) {
  std::stringstream stream(description);
  return CreateParserFromStream(stream);
}

std::shared_ptr<LALRParser> ParserGenerator::CreateParserFromStream(std::istream& stream) {
  auto parser_data = CreateParserData(stream);
  // NOTE: this uses a private constructor.
  return std::shared_ptr<LALRParser>(new LALRParser(parser_data));
}

int ParserGenerator::NumNonTerminals() const {
  return production_rules_data_->NumNonTerminals();
}

int ParserGenerator::NumTerminals() const {
  return production_rules_data_->NumTerminals();
}

int ParserGenerator::GetNonterminalID(const std::string& non_terminal) const {
  return production_rules_data_->nonterminal_map.at(non_terminal);
}

std::set<std::string> ParserGenerator::FirstSet(const std::string& symbol) {
  auto idx       = production_rules_data_->nonterminal_map[symbol];
  auto first_set = FirstSet(std::span(&idx, 1));
  std::set<std::string> output;
  std::for_each(first_set.begin(), first_set.end(), [&](int x) {
    output.insert(production_rules_data_->lexer_generator->LexemeName(x));
  });
  return output;
}

std::set<int> ParserGenerator::FirstSet(std::span<const int> symbols) {
  std::vector visited(NumNonTerminals(), false);
  return internalFirst(symbols, visited);
}


std::set<int> ParserGenerator::FollowSet(int symbol) {
  // See p. 135 of "Crafting a Compiler"

  // The follow set is the set of terminals that can follow a non-terminal
  // Formally, the follow set of A is defined to be
  //    Follow(A) = { b ∈ Σ | S ⇒+ α A b β }.
  //  for any (potentially empty) strings α, β (and Σ being the set of terminal symbols)

  // NOTE: It is assumed that the TERMINAL character for EOF is an element of the grammar, and that we
  // only look at the follow set of non-terminals. Therefore, every non-terminal must be followed by some
  // terminal.

  // Keep track of which terminals have already had their follow sets computed.
  std::vector visited(NumNonTerminals(), false);
  return internalFollow(symbol, visited);
}

std::set<std::string> ParserGenerator::FollowSet(const std::string& symbol) {
  auto follow_set = FollowSet(production_rules_data_->nonterminal_map[symbol]);
  std::set<std::string> output;
  std::for_each(follow_set.begin(), follow_set.end(), [&](int x) {
    output.insert(production_rules_data_->lexer_generator->LexemeName(x));
  });
  return output;
}

void ParserGenerator::WriteStates(std::ostream& out) const {
  int it = 0;
  for (const auto& state : all_states_) {
    auto augmented_state = closure(it);

    std::set<Item> augmented_set;  // Part of the state that is not in the kernel.
    std::set_difference(augmented_state.item_set.begin(),
                        augmented_state.item_set.end(),
                        state.item_set.begin(),
                        state.item_set.end(),
                        std::inserter(augmented_set, augmented_set.begin()));

    out << "---- State " << it << " -----------\n";
    // Write kernel
    for (auto& item : state) {
      out << "  " << WriteItem(item) << "\n";
    }
    if (!augmented_set.empty()) {
      out << "---------------------------\n";
      for (auto& item : augmented_set) {
        out << "  " << WriteItem(item) << "\n";
      }
    }
    out << "---------------------------\n\n";
    ++it;
  }
}

const std::vector<State>& ParserGenerator::GetStates() const {
  return all_states_;
}

const std::vector<std::vector<Entry>>& ParserGenerator::GetParseTable() const {
  return parse_table_;
}

std::string ParserGenerator::NameOf(int id) const {
  if (isTerminal(id)) {
    auto lex_name = production_rules_data_->lexer_generator->LexemeName(id);
    if (lex_name.find("RES:") == 0) {
      std::copy(lex_name.begin() + 4, lex_name.end(), lex_name.begin() + 1);
      lex_name[0] = '"';
      lex_name.resize(lex_name.size() - 3);
      return lex_name + '"';
    }
    return lex_name;
  }
  return production_rules_data_->inverse_nonterminal_map.at(id);
}

std::string ParserGenerator::WriteItem(const Item& item) const {
  // This may be a null production, just a placeholder for a shift.
  if (item.produced_nonterminal < 0) {
    return "";
  }
  std::string output = NameOf(item.produced_nonterminal) + " -> ";
  int j              = 0;
  for (auto symbol : item.rhs) {
    if (j == item.bookmark) {
      output += "* ";
    }
    output += NameOf(symbol) + " ";
    ++j;
  }
  if (j == item.bookmark) {
    output += "*";
  }
  return output;
}

void ParserGenerator::createStateDerivesEmpty() {
  // See p. 128 - 130 of "Crafting a Compiler"
  utility::WorkDeque<NonterminalID> work_deque;
  std::map<Item, bool> rule_derives_empty;
  // Count the symbols on the RHS of each production that do not derive empty.
  //  This is updated throughout the algorithm.
  std::map<Item, int> counts;

  // Start everything as false.
  production_rules_data_->nonterminal_derives_empty.assign(production_rules_data_->nonterminal_map.size(),
                                                           false);

  auto check_for_empty = [&](const Item& item, int count) {
    if (count == 0) {
      rule_derives_empty[item] = true;
      auto index               = nonTerminalIndex(item.produced_nonterminal);
      if (!production_rules_data_->nonterminal_derives_empty[index]) {
        production_rules_data_->nonterminal_derives_empty[index] = true;
        work_deque.Add(item.produced_nonterminal);
      }
    }
  };

  // Record all the production rules that contain instances of a symbol
  // TODO: Are these production (nonterminal) IDs, or item IDs?
  std::map<NonterminalID, std::set<int>> productions_containing_symbol;

  int i = 0;
  for (auto& production : production_rules_data_->all_productions) {
    // Update the map.
    for (auto r : production.rhs) {
      productions_containing_symbol[r].insert(i);
    }

    rule_derives_empty[production] = false;
    auto count                     = static_cast<int>(production.rhs.size());
    counts[production]             = count;
    check_for_empty(production, count);

    ++i;
  }

  while (!work_deque.Empty()) {
    const auto next = work_deque.PopNext();

    // Iterate through all productions that include [next]. It is ok if we create an empty entry.
    for (auto production_id : productions_containing_symbol[next]) {
      auto& production = production_rules_data_->all_productions[production_id];
      --counts[production];
      check_for_empty(production, counts[production]);
    }
  }

  // Log which rules derive empty.
  for (auto& [item, derives_empty] : rule_derives_empty) {
    if (derives_empty) {
      LOG_SEV_TO(logger_, Info) << "Rule (" << WritableItem {item, this} << ") derives empty.";
    }
  }
}

bool ParserGenerator::isTerminal(int id) const {
  return id < NumTerminals();
}

bool ParserGenerator::isNonTerminal(int id) const {
  return !isTerminal(id);
}

int ParserGenerator::nonTerminalIndex(int id) const {
  return id - NumTerminals();
}

int ParserGenerator::getProductionIndex(const Item& item) const {
  auto it = std::find(
      production_rules_data_->all_productions.begin(), production_rules_data_->all_productions.end(), item);
  if (it == production_rules_data_->all_productions.end()) {
    throw std::runtime_error("could not find the item in the productions");
  }
  return static_cast<int>(std::distance(production_rules_data_->all_productions.begin(), it));
}

bool ParserGenerator::computeLR0() {
  status_ = true;

  // Find productions for the starting non-terminal.
  const auto start_nonterminal =
      production_rules_data_->productions_for.find(production_rules_data_->start_nonterminal);
  if (start_nonterminal == production_rules_data_->productions_for.end()) {
    LOG_SEV_TO(logger_, Error) << "Could not find productions for the start state.";
    status_ = false;
    return status_;
  }

  State start_items = start_nonterminal->second;  // Copy, since we will zero the bookmark.
  start_items.ZeroBookmarks();

  // Add the start state.
  std::deque<int> work_list;
  addState(start_items, work_list);
  // Go through the work list until it is empty.
  while (!work_list.empty() && status_) {
    int s = work_list.front();
    work_list.pop_front();
    computeGoto(s, work_list);
  }

  // Return success.
  return status_;
}

int ParserGenerator::addState(const State& items, std::deque<int>& work_list) {
  // Try to find a state that is the given collection of items.
  int state = findState(items);
  // If the items are not a state, create a state for them.
  if (state == -1) {
    all_states_.push_back(items);
    state = static_cast<int>(all_states_.size()) - 1;
    // Initialize entries to Error.
    parse_table_.emplace_back(production_rules_data_->total_symbols, Entry());
    work_list.push_back(state);
  }
  // Return the state number.
  return state;
}

void ParserGenerator::computeGoto(int s, std::deque<int>& work_list) {
  // Find the closure of state s.
  const State closed = closure(s);

  // Try advancing the dot for every symbol.
  for (int x = 0; x < production_rules_data_->total_symbols; ++x) {
    const State relevantItems = advanceDot(closed, x);
    if (!relevantItems.empty()) {
      // Get the resolution info for the shift.
      ResolutionInfo res_info {};
      bool found_res_info     = false;
      bool differing_res_info = false;
      for (const auto& item : relevantItems) {
        if (item.res_info != NullResolutionInfo) {
          if (found_res_info && res_info != item.res_info) {
            differing_res_info = true;
          }
          res_info       = item.res_info;
          found_res_info = true;
        }
      }

      const int state = addState(relevantItems, work_list);

      // Add shift entry, possibly with resolution info.
      if (found_res_info && !differing_res_info) {
        parse_table_[s][x] = Entry(state, res_info);
      }
      else {
        parse_table_[s][x] = Entry(state);
      }
    }
  }
}

State ParserGenerator::closure(int s) const {
  // Initialize ans.
  auto ans      = all_states_[s];
  int prev_size = 0;

  // While ans is still growing.
  while (ans.size() != prev_size) {
    // Update size.
    prev_size = ans.size();
    // For all productions in ans.
    for (auto A : ans) {
      const int bookmark = A.bookmark;
      const int next     = -1 < bookmark && bookmark < A.rhs.size() ? A.rhs.at(bookmark) : -1;

      // If the bookmark was behind a non-terminal, we need to add that non-terminal to the closure.
      if (production_rules_data_->lexer_generator->GetNumLexemes() < next) {
        // Find the production for next.
        auto it = production_rules_data_->productions_for.find(next);
        if (it == production_rules_data_->productions_for.end()) {
          continue;
        }

        // Productions for next.
        auto state = it->second;
        // Set productions' bookmarks so they are like next -> * RHS(next)
        state.ZeroBookmarks();
        for (const auto& st : state) {
          if (ans.find(st) == ans.end()) {
            ans.insert(st);
          }
        }
      }
    }
  }
  return ans;
}

State ParserGenerator::advanceDot(const State& state, int symbol) {
  // The symbol may be terminal or non-terminal.
  State advance_set;
  // Create set: { A -> a X * b | A -> a * X b in state}
  for (const auto& item : state) {
    const int bookmark = item.bookmark;
    if (-1 < bookmark && bookmark < item.Size() && item.At(bookmark) == symbol) {
      Item new_item = item;
      // Increment the bookmark
      ++new_item.bookmark;
      advance_set.insert(new_item);
    }
  }
  return advance_set;
}

void ParserGenerator::completeTable() {
  if (parser_type_ == ParserType::LALR) {
    // Used by LALR(k) parser.
    const auto item_follow = computeLookahead();
    for (int state_index = 0; state_index < all_states_.size(); ++state_index) {
      for (const auto& rule : production_rules_data_->all_productions) {
        tryRuleInStateLALR(state_index, rule, item_follow);
      }
    }
  }
  else {
    for (int state_index = 0; state_index < all_states_.size(); ++state_index) {
      for (const auto& rule : production_rules_data_->all_productions) {
        tryRuleInState(state_index, rule);
      }
    }
  }

  // Assert the accept state for the starting production.
  assertEntry(0, production_rules_data_->start_nonterminal, Entry(true));
}

void ParserGenerator::assertEntry(int state, int symbol, const Entry& action) {
  auto& current_entry = parse_table_[state][symbol];
  // If the current entry is unfilled (Error), fill it with the entry.
  if (current_entry.IsError()) {
    current_entry = action;
  }
  else {
    // Resolve shift/reduce conflicts with precedence. I found a good summary of this scheme at
    // https://softwareengineering.stackexchange.com/questions/178187/how-to-add-precedence-to-lalr-parser-like-in-yacc
    // though this is in other places, like the dragon book.

    const auto& current_res_info = current_entry.GetResInfo();
    const auto& res_info         = action.GetResInfo();

    // Record the potential conflict.
    std::string bf(20, ' ');
    LOG_SEV_TO(logger_, Warning)
        << "Conflict for state " << state << ", symbol " << NameOf(symbol) << NewLineIndent
        << "  > Current entry:  " << ToString(current_entry.GetAction()) << " " << current_entry.GetState()
        << (current_entry.IsReduce() ? "\n" + bf + "Reduction" + WriteItem(current_entry.GetRule()) : "")
        << NewLineIndent << "Prec: " << current_res_info.precedence
        << ", Assoc: " << to_string(current_res_info.assoc) << NewLineIndent
        << "  > Proposed entry: " << ToString(action.GetAction()) << " " << action.GetState()
        << (action.IsReduce() ? "\n" + bf + "Reduction: " + WriteItem(action.GetRule()) : "") << NewLineIndent
        << "Prec: " << res_info.precedence << ", Assoc: " << to_string(res_info.assoc);

    // This is resolution is a generalization of comparing the precedence of first and second operators, e.g.
    // things like A + A * A. We compare the precedence of the first and second operator. The lookahead is the
    // second operator, and the proposed reduction will contain the first operator, e.g. X -> A "+" A.
    // In other words, the second operator's precedence is in the current_res_info, the first operator's
    // precedence is in the incoming action.

    const auto first_prec   = res_info.precedence;
    const auto second_prec  = current_res_info.precedence;
    const auto first_assoc  = res_info.assoc;
    const auto second_assoc = current_res_info.assoc;

    // If the first operator has lower precedence, shift.
    if (first_prec < second_prec) {
      bool current_shift = current_entry.IsShift();
      current_entry      = (current_shift ? current_entry : action);
      LOG_SEV_TO(logger_, Info) << "  @Res: Current entry is lower precedence. Using Shift ("
                                << (current_shift ? "current entry" : "replacing entry") << ").";
    }
    // If the first operator has higher precedence, reduce.
    else if (second_prec < first_prec) {
      bool current_reduce = current_entry.IsReduce();
      current_entry       = (current_reduce ? current_entry : action);
      LOG_SEV_TO(logger_, Info) << "  @Res: Current entry is higher precedence. Using Reduce ("
                                << (current_reduce ? "current entry" : "replacing entry") << ").";
    }
    // If they are the same, check the associativity.
    else {
      // Shift
      if (first_assoc == Associativity::RIGHT && second_assoc == Associativity::RIGHT) {
        bool current_shift = current_entry.IsShift();
        current_entry      = (current_shift ? current_entry : action);
        LOG_SEV_TO(logger_, Info) << "  @Res: Both entries are Right associative. Using Shift ("
                                  << (current_shift ? "current entry" : "replacing entry") << ").";
      }
      // Reduce
      else if (first_assoc == Associativity::LEFT && second_assoc == Associativity::LEFT) {
        bool current_reduce = current_entry.IsReduce();
        current_entry       = (current_reduce ? current_entry : action);
        LOG_SEV_TO(logger_, Info) << "  @Res: Both entries are Left associative. Using Reduce ("
                                  << (current_reduce ? "current entry" : "replacing entry") << ").";
      }
      // Otherwise, error.
      else {
        LOG_SEV_TO(logger_, Error) << "Error - Entry already exists!!!" << lightning::NewLineIndent
                                   << "State: " << state << ", Symbol: " << symbol
                                   << ". Old entry: " << parse_table_[state][symbol] << ", "
                                   << " New entry: " << action;
        // Set status_ to false.
        status_ = false;
      }
    }
  }
}

ItemFollowSet ParserGenerator::computeLookahead() {
  // Used in LALR(1) parser. This setup comes from "Crafting a Compiler" p. 213
  auto item_follow = buildItemForPropGraph();
  evalItemForPropGraph(item_follow);
  return item_follow;
}

ItemFollowSet ParserGenerator::buildItemForPropGraph() {
  propagation_graph_.Clear();
  ItemFollowSet item_follow;

  // Initialize item follow set for all vertices.
  auto num_items = 0;
  for (auto state_id = 0u; state_id < all_states_.size(); ++state_id) {
    const auto augmented_state = closure(state_id);

    for (const auto& item : augmented_state) {
      StateItem vertex(state_id, item.WithoutInstructions() /* Just in case... */);
      propagation_graph_.AddVertex(vertex);
      item_follow[vertex] = {};  // Initialize to empty, the "= {}" is not really needed.
      ++num_items;
    }
  }

  // Initialize start state items so EOF follows each of them.
  for (const auto& item : closure(0)) {
    item_follow.at(StateItem(0, item)).insert(0 /* EOF */);
  }

  // Place edges of the propagation graph between *items*
  auto added_edges = 0;
  for (auto state_id = 0u; state_id < all_states_.size(); ++state_id) {
    // Need to get the *complete* state, not just the kernel.
    const auto augmented_state = closure(state_id);

    for (const auto& item : augmented_state) {
      // Item: A -> alpha * B gamma  : for any (possibly empty) strings alpha, gamma
      auto cleaned_item = item.WithoutInstructions();

      auto el = item.GetElementFollowingBookmark();
      if (!el) {
        continue;
      }
      auto next_element = *el;  // B in the above notation.

      auto [start_vertex, _] = addEdge(state_id, next_element, cleaned_item);
      ++added_edges;

      // The 'gamma' part of the item that we are looking at (A -> alpha * B gamma)
      auto gamma               = item.Tail(item.bookmark + 1);
      std::set<int> first_set  = FirstSet(gamma);  // Will be empty if gamma is empty.
      bool gamma_derives_empty = allDeriveEmpty(gamma);

      // Find items of the form (B -> * gamma) in the same state.
      for (auto& other_item : augmented_state) {
        // Only forms of the form (B -> * gamma) are relevant.
        if (other_item.produced_nonterminal != next_element || !other_item.IsBookmarkAtBeginning()) {
          continue;
        }

        // Create vertex {state_id, B -> * gamma}
        StateItem vertex(state_id, other_item.WithoutInstructions());
        MANTA_ASSERT(propagation_graph_.HasVertex(vertex), "vertex not in the graph");

        item_follow.at(vertex).insert(first_set.begin(), first_set.end());
        if (gamma_derives_empty) {
          propagation_graph_.AddEdge(start_vertex, vertex);
          ++added_edges;
        }
      }
    }
  }

  LOG_SEV_TO(logger_, Info) << "Added " << added_edges << " edges to LALR prop graph.";
  return item_follow;
}

void ParserGenerator::evalItemForPropGraph(ItemFollowSet& item_follow) const {
  bool changed       = false;
  unsigned num_iters = 0;
  do {
    changed = false;
    // Iterate through all the edges in the LALR propagation graph.
    for (const auto& [v, endpoints] : propagation_graph_.Edges()) {
      // Starting vertex's follow set.
      auto& follow_v = item_follow.at(v);
      for (const auto& w : endpoints) {
        auto& follow_w      = item_follow.at(w);
        const auto old_size = follow_w.size();

        follow_w.insert(follow_v.begin(), follow_v.end());
        if (follow_w.size() != old_size) {
          changed = true;
        }
      }
    }
    ++num_iters;
  } while (changed);

  LOG_SEV_TO(logger_, Info) << "LALR item follow set has " << item_follow.size() << " elements.";
}

std::pair<StateItem, StateItem> ParserGenerator::addEdge(int state_id,
                                                         int next_element,
                                                         const Item& cleaned_item) {
  StateItem start_vertex(state_id, cleaned_item);
  MANTA_ASSERT(propagation_graph_.HasVertex(start_vertex), "start vertex not in the graph");

  // Safe to unwrap item.AdvanceDot() since element following bookmark exists.
  StateItem end_vertex(parse_table_.at(state_id).at(next_element).GetState(), *cleaned_item.AdvanceDot());
  MANTA_ASSERT(propagation_graph_.HasVertex(end_vertex), "end vertex not in the graph");

  // Add a new edge to the graph.
  propagation_graph_.AddEdge(start_vertex, end_vertex);

  return {start_vertex, end_vertex};
}

void ParserGenerator::tryRuleInState(int state, const Item& rule) {
  // Make rule into LHS(rule) -> RHS(rule) *
  auto rule_reduce = rule.MakeReducibleForm();

  const auto& state_set = all_states_[state];

  switch (parser_type_) {
    case ParserType::LR0: {
      // === LR(0) ===
      if (state_set.Contains(rule_reduce)) {  // If LHS(rule) -> RHS(rule) * is in State(state)
        for (int sym = 0; sym < production_rules_data_->total_symbols; ++sym) {
          assertEntry(state, sym, Entry(rule));
        }
      }
      break;
    }
    case ParserType::SLR: {
      // === SLR ===
      if (state_set.Contains(rule_reduce)) {  // If LHS(rule) -> RHS(rule) * is in State(state)
        auto follow_set = FollowSet(rule.produced_nonterminal);
        for (int sym : follow_set) {
          assertEntry(state, sym, Entry(rule));
        }
      }
      break;
    }
    case ParserType::LALR: {
      MANTA_FAIL("for LALR, use the tryRuleInStateLALR function");
    }
    default: {
      MANTA_FAIL("unrecognized ParserType");
    }
  }
}

void ParserGenerator::tryRuleInStateLALR(int state_index,
                                         const Item& rule,
                                         const ItemFollowSet& item_follow) {
  // Make rule into LHS(rule) -> RHS(rule) *
  auto rule_reduce = rule.MakeReducibleForm();

  // We must consider the full state, not just the kernel.
  auto augmented_state = closure(state_index);
  if (augmented_state.Contains(rule_reduce)) {  // i.e. if LHS(rule) -> RHS(rule) * is in State(state)
    auto& follow_set = item_follow.at(StateItem(state_index, rule_reduce));
    for (int sym = 0; sym < NumTerminals(); ++sym) {
      if (auto it = follow_set.find(sym) != follow_set.end()) {
        assertEntry(state_index, sym, Entry(rule));
      }
    }
  }
}

int ParserGenerator::findState(const State& items) const {
  auto it = std::find(all_states_.begin(), all_states_.end(), items);
  if (it != all_states_.end()) {
    return static_cast<int>(std::distance(all_states_.begin(), it));
  }
  return -1;
}

std::set<int> ParserGenerator::internalFirst(int symbol, std::vector<bool>& visited) {
  return internalFirst(std::span(&symbol, 1), visited);
}

std::set<int> ParserGenerator::internalFirst(std::span<const int> symbols, std::vector<bool>& visited) {
  // See p. 130 of "Crafting a Compiler"
  if (symbols.empty()) {
    return {};
  }
  auto initial_symbol = symbols[0];
  if (isTerminal(initial_symbol)) {
    return {initial_symbol};
  }
  auto nonterminal_index = nonTerminalIndex(initial_symbol);

  std::set<int> potential_first_terminals {};
  // If already visited, we don't have to redo the work.
  if (!visited.at(nonterminal_index)) {
    visited[nonterminal_index]          = true;
    const auto& productions_for_initial = production_rules_data_->productions_for.at(initial_symbol);
    for (const auto& production : productions_for_initial) {
      if (production.Size() == 0) {  // Lambda production.
        continue;
      }

      auto new_set = internalFirst(production.rhs, visited);
      potential_first_terminals.insert(new_set.begin(), new_set.end());
    }
  }

  // If the first symbol in a sentence derives empty, the remainder of the sentence needs to be checked.
  if (derivesEmpty(initial_symbol)) {
    auto first_set = internalFirst(symbols.subspan(1), visited);
    potential_first_terminals.insert(first_set.begin(), first_set.end());
  }
  return potential_first_terminals;
}

// TODO: Rewrite, non-recursively.
std::set<int> ParserGenerator::internalFollow(int non_terminal, std::vector<bool>& visited) {
  // See p. 135 of "Crafting a Compiler"

  if (non_terminal == production_rules_data_->start_nonterminal) {
    // We always assume that EOF follows the starting symbol.
    return {0};  // @eof
  }

  int index = nonTerminalIndex(non_terminal);
  if (visited[index]) {
    return {};
  }
  visited[index] = true;

  std::set<int> output;
  auto occurences = productionsIncluding(non_terminal);
  for (auto [prod_index, i] : occurences) {
    const auto& production = production_rules_data_->all_productions[prod_index];
    const std::span tail(production.rhs.data() + i + 1, production.rhs.size() - i - 1);
    if (!tail.empty()) {
      const auto first_set = FirstSet(tail);
      output.insert(first_set.begin(), first_set.end());
    }
    if (allDeriveEmpty(tail)) {
      auto follow_set = internalFollow(production.produced_nonterminal, visited);
      output.insert(follow_set.begin(), follow_set.end());
    }
  }

  return output;
}

std::vector<std::pair<int, int>> ParserGenerator::productionsIncluding(int non_terminal) const {
  std::vector<std::pair<int, int>> output;

  std::size_t production_counter = 0;
  for (const auto& production : production_rules_data_->all_productions) {
    // Look through the production for instances of non_terminal.
    for (std::size_t i = 0; i < production.rhs.size(); ++i) {
      // If this is an occurrence of symbol add it.
      if (production.rhs[i] == non_terminal) {
        output.emplace_back(production_counter, i);
      }
    }
    ++production_counter;
  }
  return output;
}

bool ParserGenerator::allDeriveEmpty(std::span<const int> sentence) const {
  // See p. 135 of "Crafting a Compiler"

  // If tail is empty, then by definition, the remainder of the tail is empty.
  if (sentence.empty()) {
    return true;
  }
  return std::ranges::all_of(sentence, [this](auto symbol) { return derivesEmpty(symbol); });
}

bool ParserGenerator::derivesEmpty(int symbol) const {
  if (isTerminal(symbol)) {
    return false;
  }
  return production_rules_data_->nonterminal_derives_empty[nonTerminalIndex(symbol)];
}

bool ParserGenerator::stateDerivesEmpty(const State& state) const {
  return production_rules_data_->nonterminal_derives_empty[findState(state)];
}

}  // namespace manta