//
// Created by Nathaniel Rupprecht on 4/4/23.
//

#pragma once

#include "manta/generator/DescriptionParser.h"

namespace manta {

//! \brief All the data needed to create a full parser, e.g. transition table, meta data, lexer_generator, etc.
struct ParserData {
  std::shared_ptr<ProductionRulesData> production_rules_data{};

  //! \brief The full parse table used by the Parser.
  std::vector<std::vector<Entry>> parse_table{};

  //! \brief All the different states.
  std::vector<State> all_states{};

  std::shared_ptr<LexerGenerator> lexer_generator{};
};

} // namespace manta