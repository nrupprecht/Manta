//
// Created by Nathaniel Rupprecht on 4/12/23.
//

#pragma once

#include "manta/generator/typesystem/TypeDeduction.h"
#include "manta/generator/typesystem/TypeRelationship.h"
#include "manta/generator/DescriptionParser.h"
#include "manta/generator/ParserGenerator.h"


namespace manta::typesystem {

std::tuple<
    ASTNodeManager,
    std::map<std::string, std::vector<TypeRelationship>>,
    std::map<std::string, NonterminalID>,
    std::map<unsigned, std::string>
>
CreateRelationships(const std::shared_ptr<const ParserData>& parser_data,
                    bool tag_generated_field_names,
                    bool generated_nodes_have_node_in_name);


TypeDeduction DeduceTypes(
    ASTNodeManager& node_manager,
    const std::map<std::string, std::vector<TypeRelationship>>& relationships,
    const std::map<std::string, NonterminalID>& nonterminals_for_type,
    const ProductionRulesData& production_rules_data);

} // namespace manta::typesystem