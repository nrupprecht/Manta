//
// Created by Nathaniel Rupprecht on 11/30/24.
//

#pragma once

#include <map>
#include <string_view>
#include <string>

#include "manta/utility/Formatting.h"
#include "manta/utility/Exceptions.h"
#include "manta/utility/ParserUtility.h"

namespace manta {

template<typename LexerType>
struct NamingUtility {
  std::size_t NumTerminals() const {
    MANTA_REQUIRE(lexer, "lexer has not been set in NamingUtility");
    return lexer->GetNumLexemes();
  }

  const std::string& GetNonterminalName(int id) const {
    MANTA_REQUIRE(inverse_nonterminal_map, "inverse_nonterminal_map has not been set in NamingUtility");
    return inverse_nonterminal_map->at(id);
  }

  NO_DISCARD const std::string& GetName(int id) const {
    if (id < NumTerminals()) {
      return lexer->LexemeName(id);
    }
    return GetNonterminalName(id);
  }

  NO_DISCARD std::string GetPrettyName(int id) const {
    if (id < NumTerminals()) {
      using namespace std::string_literals;
      using namespace std::string_view_literals;
      using namespace lightning::formatting;

      auto&& lexeme_name = lexer->LexemeName(id);
      if (std::string_view(lexeme_name).substr(0, 4) == "RES:"sv) {
        // TODO: Update Lightning to get Format() to work with string_view.
        return Format("\"{}\"", std::string(std::string_view(lexeme_name).substr(4)));
      }
      return "@"s + lexeme_name;
    }
    return GetNonterminalName(id);
  }

  std::string Write(const Item& item) const {
    // This may be a null production, just a placeholder for a shift.
    if (item.produced_nonterminal < 0) {
      return "";
    }
    std::string output = GetPrettyName(item.produced_nonterminal) + " -> ";
    int j              = 0;
    for (auto symbol : item.rhs) {
      if (j == item.bookmark) {
        output += "* ";
      }
      output += GetPrettyName(symbol) + " ";
      ++j;
    }
    if (j == item.bookmark) {
      output += "*";
    }
    return output;
  }

  std::string Write(const ProductionRule& item) const {
    // This may be a null production, just a placeholder for a shift.
    if (item.produced_nonterminal < 0) {
      return "";
    }
    std::string output = GetPrettyName(item.produced_nonterminal) + " -> ";
    for (auto symbol : item.rhs) {
      output += GetPrettyName(symbol) + " ";
    }
    return output;
  }

  std::map<int, std::string>* inverse_nonterminal_map;
  LexerType* lexer;
};

}  // namespace manta