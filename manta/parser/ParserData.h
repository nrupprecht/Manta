//
// Created by Nathaniel Rupprecht on 4/4/23.
//

#pragma once

#include "manta/generator/DescriptionParser.h"

namespace manta {

//! \brief All the data needed to create a full parser, e.g. transition table, meta data, lexer_generator,
//! etc.
struct ParserData {
  //! \brief The production rules data that was used to create the parse table and states.
  std::shared_ptr<ProductionRulesData> production_rules_data{};

  //! \brief The full parse table used by the Parser.
  std::vector<std::vector<Entry>> parse_table{};

  //! \brief All the different states.
  std::vector<State> all_states{};

  //! \brief Get the lexer generator from the production rules data.
  LexerGenerator& GetLexerGenerator() { return *production_rules_data->lexer_generator; }

  const LexerGenerator& GetLexerGenerator() const { return *production_rules_data->lexer_generator; }
};

//! \brief Write the parser data to a stream, as a pretty printed parse table.
void WriteToStream(std::ostream& out, const ParserData& parser_data);

}  // namespace manta