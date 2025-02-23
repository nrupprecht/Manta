//
// Created by Nathaniel Rupprecht on 8/20/23.
//

#include "manta/generatedparsers/GeneratedDescriptionParser.h"
// Other files.
#include "manta/generatedparsers/ParserRepresentation.h"

namespace manta::generator::parser {

std::shared_ptr<ProductionRulesData> ParsingDescriptionParser::ParseDescription(std::istream& stream) {
  // Create a new production rules object.
#ifdef MANTA_PARSER_GENERATED
  Parser parser;
  parser.SetLogger(lightning::Global::GetLogger());

  parser.SetInput(utility::IStreamContainer::FromStream(stream));
  auto parsed_ast = parser.ParseInput();

  MANTA_REQUIRE(parsed_ast, "failed to parse the description");

  LOG_SEV(Info) << "Running ParserGenerator visitor.";
  ParserGenerator visitor {};
  parsed_ast->Accept(visitor);

  return visitor.GetProductionRulesData();
#else
  return nullptr;
#endif
}

}  // namespace manta::generator::parser
