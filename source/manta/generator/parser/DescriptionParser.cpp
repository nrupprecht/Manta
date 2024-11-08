//
// Created by Nathaniel Rupprecht on 8/20/23.
//

#include "manta/generator/parser/DescriptionParser.h"
// Other files.
// #include "manta/generator/parser/GeneratedDescriptionParser.h"
#include "manta/generatedparsers/ParserRepresentation.h"

namespace manta::generator::parser {

std::shared_ptr<ProductionRulesData> ParsingDescriptionParser::ParseDescription(std::istream& stream) {
  // Create a new production rules object.

  Parser parser;
  parser.SetLogger(lightning::Global::GetLogger());

  parser.SetInput(utility::IStreamContainer::FromStream(stream));
  auto parsed_ast = parser.ParseInput();

  MANTA_REQUIRE(parsed_ast, "failed to parse the description");

  ParserGenerator visitor {};
  parsed_ast->Accept(visitor);

  return visitor.GetProductionRulesData();
  // return nullptr;
}

}  // namespace manta::generator::parser
