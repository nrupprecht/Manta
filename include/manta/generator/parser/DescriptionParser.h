//
// Created by Nathaniel Rupprecht on 8/20/23.
//

#pragma once

#include <iostream>

#include "manta/generator/DescriptionParser.h"

namespace manta::generator::parser {

//! \brief A description parser that uses a manta generated parser to parse the DSL describing a parser,
//!        and extracts the ProductionRulesData describing that parser.
class ParsingDescriptionParser : public DescriptionParser, public ProductionRulesBuilder {
public:
  //! \brief Parse an istream, creating a ProductionRulesData from the parser described by the string.
  std::shared_ptr<ProductionRulesData> ParseDescription(std::istream& stream) override;
};

}  // namespace manta::generator::parser