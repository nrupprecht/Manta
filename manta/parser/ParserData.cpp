//
// Created by Nathaniel Rupprecht on 12/28/23.
//

#include "manta/parser/ParserData.h"

namespace manta {

void WriteToStream(std::ostream& out, const ParserData& parser_data) {
  std::string str;

  auto& prd = *parser_data.production_rules_data;
  auto& lg = parser_data.GetLexerGenerator();
  auto total_symbols = prd.total_symbols;
  auto num_lexemes = lg.GetNumLexemes();

  // Print out definitions.
  out << repeat('_', (total_symbols + 1) * 5) << "__";
  out << '\n';
  out << "Token and production definitions:\n";
  out << repeat('-', (total_symbols + 1) * 5) << "--";
  out << '\n';
  out << "   -- (Lexemes) -- \n";
  int i = 0;
  for (; i < num_lexemes; ++i) {
    out << buffered(i, 4) << ": " << lg.GetLexemeName(i) << "\n";
  }
  out << "   -- (Productions) -- \n";
  for (; i < total_symbols; ++i) {
    out << buffered(i, 4) << ": " << prd.inverse_nonterminal_map.find(i)->second << "\n";
  }

  // Print table header.
  out << repeat('_', 5 * (total_symbols + 2)) << "__";
  out << '\n';

  // Store the (space) buffered names. We need to find how long the longest name is,
  // so we know how many spaces we need to buffer with.
  std::vector<std::string> buffered_names;

  std::size_t max_size = 0;
  for (std::size_t j = 0; j < num_lexemes; ++j) {
    auto name = lg.GetLexemeName(static_cast<int>(j));
    if (name.substr(0, 4) == "RES:") {
      // if (name[0] == 'R' && name[1] == 'E' && name[2] == 'S' && name[3] == ':') {
      max_size = std::max(max_size, name.size() - 4);
    } else {
      max_size = std::max(max_size, name.size());
    }
  }
  for (auto& [id, name]: prd.inverse_nonterminal_map) {
    max_size = std::max(max_size, name.size());
  }

  for (std::size_t j = 0; j < num_lexemes; ++j) {
    auto name = lg.GetLexemeName(j);

    if (name.substr(0, 4) == "RES:") {
      //}
      // if (name[0] == 'R' && name[1] == 'E' && name[2] == 'S' && name[3] == ':') {
      std::copy(name.begin() + 4, name.end(), name.begin());
      name.resize(name.size() - 4);
    }
    buffered_names.push_back(repeat(' ', static_cast<int>(max_size - name.size())) + name);
  }
  for (auto& [id, name]: prd.inverse_nonterminal_map) {
    buffered_names.push_back(repeat(' ', static_cast<int>(max_size - name.size())) + name);
  }

  for (std::size_t j = 0; j < max_size; ++j) {
    out << "     |";
    for (std::size_t k = 0; k < total_symbols; ++k) {
      out << "    " << std::string{buffered_names[k][j]};
      if (k == num_lexemes - 1) {
        out << "  |";
      }
    }
    out << "  |\n";
  }

  auto& parse_table = parser_data.parse_table;

  out << repeat('-', 5 * (total_symbols + 2)) << "--";
  out << '\n';
  // Print transition table.
  for (int s = 0; s < parser_data.all_states.size(); ++s) {
    out << buffered(s, 4) << " | ";
    for (int j = 0; j < total_symbols; ++j) {
      out << parse_table[s][j].Write(4) << " ";

      if (j == num_lexemes - 1) {
        out << " | ";
      }
    }
    out << " |\n";
  }
  out << repeat('-', 5 * (total_symbols + 2)) << "--";
  out << '\n';
}

}