#pragma once

#include <vector>
#include <string>
// Include the support for the parser.
#include "manta/generator/ParserDriver.h"
#include "manta/generator/LexerGenerator.h"
#include <Lightning/Lightning.h>
#include "manta/utility/Formatting.h"


using namespace manta::formatting;
using manta::ItemID;

#define REDUCE_ASSERT(count, item, size) \
  MANTA_REQUIRE(count <= size, "in reduction "  #item  ", not enough nodes in the collect vector, needed at least " #count ", actual size was " << size)

// Forward declare AST node types.
struct ASTNodeGeneral_0_program;
struct ASTNodeBase_declList;
struct ASTNodeGeneral_1_declList;
struct ASTNodeGeneral_2_declList;
struct ASTNodeBase_decl;
struct ASTNodeGeneral_3_decl;
struct ASTNodeGeneral_4_decl;
struct ASTNodeGeneral_5_varDecl;
struct ASTNodeBase_funDecl;
struct ASTNodeGeneral_17_funDecl;
struct ASTNodeGeneral_18_funDecl;
struct ASTNodeBase_typeSpec;
struct ASTNodeGeneral_14_typeSpec;
struct ASTNodeGeneral_15_typeSpec;
struct ASTNodeGeneral_16_typeSpec;
struct ASTNodeBase_varDeclList;
struct ASTNodeGeneral_8_varDeclList;
struct ASTNodeGeneral_9_varDeclList;
struct ASTNodeBase_scopedVarDecl;
struct ASTNodeGeneral_6_scopedVarDecl;
struct ASTNodeGeneral_7_scopedVarDecl;
struct ASTNodeBase_varDeclInit;
struct ASTNodeGeneral_10_varDeclInit;
struct ASTNodeGeneral_11_varDeclInit;
struct ASTNodeBase_varDeclId;
struct ASTNodeGeneral_12_varDeclId;
struct ASTNodeGeneral_13_varDeclId;
struct ASTNodeBase_simpleExp;
struct ASTNodeGeneral_58_simpleExp;
struct ASTNodeGeneral_59_simpleExp;
struct ASTNodeBase_parms;
struct ASTNodeGeneral_19_parms;
struct ASTNodeGeneral_20_parms;
struct ASTNodeBase_stmt;
struct ASTNodeGeneral_28_stmt;
struct ASTNodeGeneral_29_stmt;
struct ASTNodeGeneral_30_stmt;
struct ASTNodeGeneral_31_stmt;
struct ASTNodeGeneral_32_stmt;
struct ASTNodeGeneral_33_stmt;
struct ASTNodeBase_parmList;
struct ASTNodeGeneral_21_parmList;
struct ASTNodeGeneral_22_parmList;
struct ASTNodeGeneral_23_parmTypeList;
struct ASTNodeBase_parmIdList;
struct ASTNodeGeneral_24_parmIdList;
struct ASTNodeGeneral_25_parmIdList;
struct ASTNodeBase_parmId;
struct ASTNodeGeneral_26_parmId;
struct ASTNodeGeneral_27_parmId;
struct ASTNodeBase_expStmt;
struct ASTNodeGeneral_34_expStmt;
struct ASTNodeGeneral_35_expStmt;
struct ASTNodeGeneral_36_compoundStmt;
struct ASTNodeBase_selectStmt;
struct ASTNodeGeneral_41_selectStmt;
struct ASTNodeGeneral_42_selectStmt;
struct ASTNodeBase_iterStmt;
struct ASTNodeGeneral_43_iterStmt;
struct ASTNodeGeneral_44_iterStmt;
struct ASTNodeBase_returnStmt;
struct ASTNodeGeneral_47_returnStmt;
struct ASTNodeGeneral_48_returnStmt;
struct ASTNodeGeneral_49_breakStmt;
struct ASTNodeBase_exp;
struct ASTNodeGeneral_50_exp;
struct ASTNodeGeneral_51_exp;
struct ASTNodeGeneral_52_exp;
struct ASTNodeGeneral_53_exp;
struct ASTNodeGeneral_54_exp;
struct ASTNodeGeneral_55_exp;
struct ASTNodeGeneral_56_exp;
struct ASTNodeGeneral_57_exp;
struct ASTNodeBase_localDecls;
struct ASTNodeGeneral_37_localDecls;
struct ASTNodeGeneral_38_localDecls;
struct ASTNodeBase_stmtList;
struct ASTNodeGeneral_39_stmtList;
struct ASTNodeGeneral_40_stmtList;
struct ASTNodeBase_iterRange;
struct ASTNodeGeneral_45_iterRange;
struct ASTNodeGeneral_46_iterRange;
struct ASTNodeBase_mutable;
struct ASTNodeGeneral_92_mutable;
struct ASTNodeGeneral_93_mutable;
struct ASTNodeBase_andExp;
struct ASTNodeGeneral_60_andExp;
struct ASTNodeGeneral_61_andExp;
struct ASTNodeBase_unaryRelExp;
struct ASTNodeGeneral_62_unaryRelExp;
struct ASTNodeGeneral_63_unaryRelExp;
struct ASTNodeBase_relExp;
struct ASTNodeGeneral_64_relExp;
struct ASTNodeGeneral_65_relExp;
struct ASTNodeBase_minmaxExp;
struct ASTNodeGeneral_72_minmaxExp;
struct ASTNodeGeneral_73_minmaxExp;
struct ASTNodeBase_relop;
struct ASTNodeGeneral_66_relop;
struct ASTNodeGeneral_67_relop;
struct ASTNodeGeneral_68_relop;
struct ASTNodeGeneral_69_relop;
struct ASTNodeGeneral_70_relop;
struct ASTNodeGeneral_71_relop;
struct ASTNodeBase_minmaxop;
struct ASTNodeGeneral_74_minmaxop;
struct ASTNodeGeneral_75_minmaxop;
struct ASTNodeBase_sumExp;
struct ASTNodeGeneral_76_sumExp;
struct ASTNodeGeneral_77_sumExp;
struct ASTNodeBase_sumop;
struct ASTNodeGeneral_78_sumop;
struct ASTNodeGeneral_79_sumop;
struct ASTNodeBase_mulExp;
struct ASTNodeGeneral_80_mulExp;
struct ASTNodeGeneral_81_mulExp;
struct ASTNodeBase_mulop;
struct ASTNodeGeneral_82_mulop;
struct ASTNodeGeneral_83_mulop;
struct ASTNodeGeneral_84_mulop;
struct ASTNodeBase_unaryExp;
struct ASTNodeGeneral_85_unaryExp;
struct ASTNodeGeneral_86_unaryExp;
struct ASTNodeBase_unaryop;
struct ASTNodeGeneral_87_unaryop;
struct ASTNodeGeneral_88_unaryop;
struct ASTNodeGeneral_89_unaryop;
struct ASTNodeBase_factor;
struct ASTNodeGeneral_90_factor;
struct ASTNodeGeneral_91_factor;
struct ASTNodeBase_immutable;
struct ASTNodeGeneral_94_immutable;
struct ASTNodeGeneral_95_immutable;
struct ASTNodeGeneral_96_immutable;
struct ASTNodeGeneral_97_call;
struct ASTNodeBase_constant;
struct ASTNodeGeneral_102_constant;
struct ASTNodeGeneral_103_constant;
struct ASTNodeGeneral_104_constant;
struct ASTNodeGeneral_105_constant;
struct ASTNodeGeneral_106_constant;
struct ASTNodeBase_args;
struct ASTNodeGeneral_98_args;
struct ASTNodeGeneral_99_args;
struct ASTNodeBase_argList;
struct ASTNodeGeneral_100_argList;
struct ASTNodeGeneral_101_argList;
// Lexeme type
struct ASTLexeme;

//! \brief Define the enums for each node type.
enum class ASTNodeType{
  Literal,
  Type_ASTNodeBase_andExp,
  Type_ASTNodeBase_argList,
  Type_ASTNodeBase_args,
  Type_ASTNodeBase_constant,
  Type_ASTNodeBase_decl,
  Type_ASTNodeBase_declList,
  Type_ASTNodeBase_exp,
  Type_ASTNodeBase_expStmt,
  Type_ASTNodeBase_factor,
  Type_ASTNodeBase_funDecl,
  Type_ASTNodeBase_immutable,
  Type_ASTNodeBase_iterRange,
  Type_ASTNodeBase_iterStmt,
  Type_ASTNodeBase_localDecls,
  Type_ASTNodeBase_minmaxExp,
  Type_ASTNodeBase_minmaxop,
  Type_ASTNodeBase_mulExp,
  Type_ASTNodeBase_mulop,
  Type_ASTNodeBase_mutable,
  Type_ASTNodeBase_parmId,
  Type_ASTNodeBase_parmIdList,
  Type_ASTNodeBase_parmList,
  Type_ASTNodeBase_parms,
  Type_ASTNodeBase_relExp,
  Type_ASTNodeBase_relop,
  Type_ASTNodeBase_returnStmt,
  Type_ASTNodeBase_scopedVarDecl,
  Type_ASTNodeBase_selectStmt,
  Type_ASTNodeBase_simpleExp,
  Type_ASTNodeBase_stmt,
  Type_ASTNodeBase_stmtList,
  Type_ASTNodeBase_sumExp,
  Type_ASTNodeBase_sumop,
  Type_ASTNodeBase_typeSpec,
  Type_ASTNodeBase_unaryExp,
  Type_ASTNodeBase_unaryRelExp,
  Type_ASTNodeBase_unaryop,
  Type_ASTNodeBase_varDeclId,
  Type_ASTNodeBase_varDeclInit,
  Type_ASTNodeBase_varDeclList,
  Type_ASTNodeGeneral_0_program,
  Type_ASTNodeGeneral_100_argList,
  Type_ASTNodeGeneral_101_argList,
  Type_ASTNodeGeneral_102_constant,
  Type_ASTNodeGeneral_103_constant,
  Type_ASTNodeGeneral_104_constant,
  Type_ASTNodeGeneral_105_constant,
  Type_ASTNodeGeneral_106_constant,
  Type_ASTNodeGeneral_10_varDeclInit,
  Type_ASTNodeGeneral_11_varDeclInit,
  Type_ASTNodeGeneral_12_varDeclId,
  Type_ASTNodeGeneral_13_varDeclId,
  Type_ASTNodeGeneral_14_typeSpec,
  Type_ASTNodeGeneral_15_typeSpec,
  Type_ASTNodeGeneral_16_typeSpec,
  Type_ASTNodeGeneral_17_funDecl,
  Type_ASTNodeGeneral_18_funDecl,
  Type_ASTNodeGeneral_19_parms,
  Type_ASTNodeGeneral_1_declList,
  Type_ASTNodeGeneral_20_parms,
  Type_ASTNodeGeneral_21_parmList,
  Type_ASTNodeGeneral_22_parmList,
  Type_ASTNodeGeneral_23_parmTypeList,
  Type_ASTNodeGeneral_24_parmIdList,
  Type_ASTNodeGeneral_25_parmIdList,
  Type_ASTNodeGeneral_26_parmId,
  Type_ASTNodeGeneral_27_parmId,
  Type_ASTNodeGeneral_28_stmt,
  Type_ASTNodeGeneral_29_stmt,
  Type_ASTNodeGeneral_2_declList,
  Type_ASTNodeGeneral_30_stmt,
  Type_ASTNodeGeneral_31_stmt,
  Type_ASTNodeGeneral_32_stmt,
  Type_ASTNodeGeneral_33_stmt,
  Type_ASTNodeGeneral_34_expStmt,
  Type_ASTNodeGeneral_35_expStmt,
  Type_ASTNodeGeneral_36_compoundStmt,
  Type_ASTNodeGeneral_37_localDecls,
  Type_ASTNodeGeneral_38_localDecls,
  Type_ASTNodeGeneral_39_stmtList,
  Type_ASTNodeGeneral_3_decl,
  Type_ASTNodeGeneral_40_stmtList,
  Type_ASTNodeGeneral_41_selectStmt,
  Type_ASTNodeGeneral_42_selectStmt,
  Type_ASTNodeGeneral_43_iterStmt,
  Type_ASTNodeGeneral_44_iterStmt,
  Type_ASTNodeGeneral_45_iterRange,
  Type_ASTNodeGeneral_46_iterRange,
  Type_ASTNodeGeneral_47_returnStmt,
  Type_ASTNodeGeneral_48_returnStmt,
  Type_ASTNodeGeneral_49_breakStmt,
  Type_ASTNodeGeneral_4_decl,
  Type_ASTNodeGeneral_50_exp,
  Type_ASTNodeGeneral_51_exp,
  Type_ASTNodeGeneral_52_exp,
  Type_ASTNodeGeneral_53_exp,
  Type_ASTNodeGeneral_54_exp,
  Type_ASTNodeGeneral_55_exp,
  Type_ASTNodeGeneral_56_exp,
  Type_ASTNodeGeneral_57_exp,
  Type_ASTNodeGeneral_58_simpleExp,
  Type_ASTNodeGeneral_59_simpleExp,
  Type_ASTNodeGeneral_5_varDecl,
  Type_ASTNodeGeneral_60_andExp,
  Type_ASTNodeGeneral_61_andExp,
  Type_ASTNodeGeneral_62_unaryRelExp,
  Type_ASTNodeGeneral_63_unaryRelExp,
  Type_ASTNodeGeneral_64_relExp,
  Type_ASTNodeGeneral_65_relExp,
  Type_ASTNodeGeneral_66_relop,
  Type_ASTNodeGeneral_67_relop,
  Type_ASTNodeGeneral_68_relop,
  Type_ASTNodeGeneral_69_relop,
  Type_ASTNodeGeneral_6_scopedVarDecl,
  Type_ASTNodeGeneral_70_relop,
  Type_ASTNodeGeneral_71_relop,
  Type_ASTNodeGeneral_72_minmaxExp,
  Type_ASTNodeGeneral_73_minmaxExp,
  Type_ASTNodeGeneral_74_minmaxop,
  Type_ASTNodeGeneral_75_minmaxop,
  Type_ASTNodeGeneral_76_sumExp,
  Type_ASTNodeGeneral_77_sumExp,
  Type_ASTNodeGeneral_78_sumop,
  Type_ASTNodeGeneral_79_sumop,
  Type_ASTNodeGeneral_7_scopedVarDecl,
  Type_ASTNodeGeneral_80_mulExp,
  Type_ASTNodeGeneral_81_mulExp,
  Type_ASTNodeGeneral_82_mulop,
  Type_ASTNodeGeneral_83_mulop,
  Type_ASTNodeGeneral_84_mulop,
  Type_ASTNodeGeneral_85_unaryExp,
  Type_ASTNodeGeneral_86_unaryExp,
  Type_ASTNodeGeneral_87_unaryop,
  Type_ASTNodeGeneral_88_unaryop,
  Type_ASTNodeGeneral_89_unaryop,
  Type_ASTNodeGeneral_8_varDeclList,
  Type_ASTNodeGeneral_90_factor,
  Type_ASTNodeGeneral_91_factor,
  Type_ASTNodeGeneral_92_mutable,
  Type_ASTNodeGeneral_93_mutable,
  Type_ASTNodeGeneral_94_immutable,
  Type_ASTNodeGeneral_95_immutable,
  Type_ASTNodeGeneral_96_immutable,
  Type_ASTNodeGeneral_97_call,
  Type_ASTNodeGeneral_98_args,
  Type_ASTNodeGeneral_99_args,
  Type_ASTNodeGeneral_9_varDeclList,
};

//! \brief Define function to write AST node type enums.
inline const char* to_string(ASTNodeType type) {
  switch (type) {
  case ASTNodeType::Literal:
    return "Literal";
  case ASTNodeType::Type_ASTNodeBase_andExp:
    return "Type_ASTNodeBase_andExp";
  case ASTNodeType::Type_ASTNodeBase_argList:
    return "Type_ASTNodeBase_argList";
  case ASTNodeType::Type_ASTNodeBase_args:
    return "Type_ASTNodeBase_args";
  case ASTNodeType::Type_ASTNodeBase_constant:
    return "Type_ASTNodeBase_constant";
  case ASTNodeType::Type_ASTNodeBase_decl:
    return "Type_ASTNodeBase_decl";
  case ASTNodeType::Type_ASTNodeBase_declList:
    return "Type_ASTNodeBase_declList";
  case ASTNodeType::Type_ASTNodeBase_exp:
    return "Type_ASTNodeBase_exp";
  case ASTNodeType::Type_ASTNodeBase_expStmt:
    return "Type_ASTNodeBase_expStmt";
  case ASTNodeType::Type_ASTNodeBase_factor:
    return "Type_ASTNodeBase_factor";
  case ASTNodeType::Type_ASTNodeBase_funDecl:
    return "Type_ASTNodeBase_funDecl";
  case ASTNodeType::Type_ASTNodeBase_immutable:
    return "Type_ASTNodeBase_immutable";
  case ASTNodeType::Type_ASTNodeBase_iterRange:
    return "Type_ASTNodeBase_iterRange";
  case ASTNodeType::Type_ASTNodeBase_iterStmt:
    return "Type_ASTNodeBase_iterStmt";
  case ASTNodeType::Type_ASTNodeBase_localDecls:
    return "Type_ASTNodeBase_localDecls";
  case ASTNodeType::Type_ASTNodeBase_minmaxExp:
    return "Type_ASTNodeBase_minmaxExp";
  case ASTNodeType::Type_ASTNodeBase_minmaxop:
    return "Type_ASTNodeBase_minmaxop";
  case ASTNodeType::Type_ASTNodeBase_mulExp:
    return "Type_ASTNodeBase_mulExp";
  case ASTNodeType::Type_ASTNodeBase_mulop:
    return "Type_ASTNodeBase_mulop";
  case ASTNodeType::Type_ASTNodeBase_mutable:
    return "Type_ASTNodeBase_mutable";
  case ASTNodeType::Type_ASTNodeBase_parmId:
    return "Type_ASTNodeBase_parmId";
  case ASTNodeType::Type_ASTNodeBase_parmIdList:
    return "Type_ASTNodeBase_parmIdList";
  case ASTNodeType::Type_ASTNodeBase_parmList:
    return "Type_ASTNodeBase_parmList";
  case ASTNodeType::Type_ASTNodeBase_parms:
    return "Type_ASTNodeBase_parms";
  case ASTNodeType::Type_ASTNodeBase_relExp:
    return "Type_ASTNodeBase_relExp";
  case ASTNodeType::Type_ASTNodeBase_relop:
    return "Type_ASTNodeBase_relop";
  case ASTNodeType::Type_ASTNodeBase_returnStmt:
    return "Type_ASTNodeBase_returnStmt";
  case ASTNodeType::Type_ASTNodeBase_scopedVarDecl:
    return "Type_ASTNodeBase_scopedVarDecl";
  case ASTNodeType::Type_ASTNodeBase_selectStmt:
    return "Type_ASTNodeBase_selectStmt";
  case ASTNodeType::Type_ASTNodeBase_simpleExp:
    return "Type_ASTNodeBase_simpleExp";
  case ASTNodeType::Type_ASTNodeBase_stmt:
    return "Type_ASTNodeBase_stmt";
  case ASTNodeType::Type_ASTNodeBase_stmtList:
    return "Type_ASTNodeBase_stmtList";
  case ASTNodeType::Type_ASTNodeBase_sumExp:
    return "Type_ASTNodeBase_sumExp";
  case ASTNodeType::Type_ASTNodeBase_sumop:
    return "Type_ASTNodeBase_sumop";
  case ASTNodeType::Type_ASTNodeBase_typeSpec:
    return "Type_ASTNodeBase_typeSpec";
  case ASTNodeType::Type_ASTNodeBase_unaryExp:
    return "Type_ASTNodeBase_unaryExp";
  case ASTNodeType::Type_ASTNodeBase_unaryRelExp:
    return "Type_ASTNodeBase_unaryRelExp";
  case ASTNodeType::Type_ASTNodeBase_unaryop:
    return "Type_ASTNodeBase_unaryop";
  case ASTNodeType::Type_ASTNodeBase_varDeclId:
    return "Type_ASTNodeBase_varDeclId";
  case ASTNodeType::Type_ASTNodeBase_varDeclInit:
    return "Type_ASTNodeBase_varDeclInit";
  case ASTNodeType::Type_ASTNodeBase_varDeclList:
    return "Type_ASTNodeBase_varDeclList";
  case ASTNodeType::Type_ASTNodeGeneral_0_program:
    return "Type_ASTNodeGeneral_0_program";
  case ASTNodeType::Type_ASTNodeGeneral_100_argList:
    return "Type_ASTNodeGeneral_100_argList";
  case ASTNodeType::Type_ASTNodeGeneral_101_argList:
    return "Type_ASTNodeGeneral_101_argList";
  case ASTNodeType::Type_ASTNodeGeneral_102_constant:
    return "Type_ASTNodeGeneral_102_constant";
  case ASTNodeType::Type_ASTNodeGeneral_103_constant:
    return "Type_ASTNodeGeneral_103_constant";
  case ASTNodeType::Type_ASTNodeGeneral_104_constant:
    return "Type_ASTNodeGeneral_104_constant";
  case ASTNodeType::Type_ASTNodeGeneral_105_constant:
    return "Type_ASTNodeGeneral_105_constant";
  case ASTNodeType::Type_ASTNodeGeneral_106_constant:
    return "Type_ASTNodeGeneral_106_constant";
  case ASTNodeType::Type_ASTNodeGeneral_10_varDeclInit:
    return "Type_ASTNodeGeneral_10_varDeclInit";
  case ASTNodeType::Type_ASTNodeGeneral_11_varDeclInit:
    return "Type_ASTNodeGeneral_11_varDeclInit";
  case ASTNodeType::Type_ASTNodeGeneral_12_varDeclId:
    return "Type_ASTNodeGeneral_12_varDeclId";
  case ASTNodeType::Type_ASTNodeGeneral_13_varDeclId:
    return "Type_ASTNodeGeneral_13_varDeclId";
  case ASTNodeType::Type_ASTNodeGeneral_14_typeSpec:
    return "Type_ASTNodeGeneral_14_typeSpec";
  case ASTNodeType::Type_ASTNodeGeneral_15_typeSpec:
    return "Type_ASTNodeGeneral_15_typeSpec";
  case ASTNodeType::Type_ASTNodeGeneral_16_typeSpec:
    return "Type_ASTNodeGeneral_16_typeSpec";
  case ASTNodeType::Type_ASTNodeGeneral_17_funDecl:
    return "Type_ASTNodeGeneral_17_funDecl";
  case ASTNodeType::Type_ASTNodeGeneral_18_funDecl:
    return "Type_ASTNodeGeneral_18_funDecl";
  case ASTNodeType::Type_ASTNodeGeneral_19_parms:
    return "Type_ASTNodeGeneral_19_parms";
  case ASTNodeType::Type_ASTNodeGeneral_1_declList:
    return "Type_ASTNodeGeneral_1_declList";
  case ASTNodeType::Type_ASTNodeGeneral_20_parms:
    return "Type_ASTNodeGeneral_20_parms";
  case ASTNodeType::Type_ASTNodeGeneral_21_parmList:
    return "Type_ASTNodeGeneral_21_parmList";
  case ASTNodeType::Type_ASTNodeGeneral_22_parmList:
    return "Type_ASTNodeGeneral_22_parmList";
  case ASTNodeType::Type_ASTNodeGeneral_23_parmTypeList:
    return "Type_ASTNodeGeneral_23_parmTypeList";
  case ASTNodeType::Type_ASTNodeGeneral_24_parmIdList:
    return "Type_ASTNodeGeneral_24_parmIdList";
  case ASTNodeType::Type_ASTNodeGeneral_25_parmIdList:
    return "Type_ASTNodeGeneral_25_parmIdList";
  case ASTNodeType::Type_ASTNodeGeneral_26_parmId:
    return "Type_ASTNodeGeneral_26_parmId";
  case ASTNodeType::Type_ASTNodeGeneral_27_parmId:
    return "Type_ASTNodeGeneral_27_parmId";
  case ASTNodeType::Type_ASTNodeGeneral_28_stmt:
    return "Type_ASTNodeGeneral_28_stmt";
  case ASTNodeType::Type_ASTNodeGeneral_29_stmt:
    return "Type_ASTNodeGeneral_29_stmt";
  case ASTNodeType::Type_ASTNodeGeneral_2_declList:
    return "Type_ASTNodeGeneral_2_declList";
  case ASTNodeType::Type_ASTNodeGeneral_30_stmt:
    return "Type_ASTNodeGeneral_30_stmt";
  case ASTNodeType::Type_ASTNodeGeneral_31_stmt:
    return "Type_ASTNodeGeneral_31_stmt";
  case ASTNodeType::Type_ASTNodeGeneral_32_stmt:
    return "Type_ASTNodeGeneral_32_stmt";
  case ASTNodeType::Type_ASTNodeGeneral_33_stmt:
    return "Type_ASTNodeGeneral_33_stmt";
  case ASTNodeType::Type_ASTNodeGeneral_34_expStmt:
    return "Type_ASTNodeGeneral_34_expStmt";
  case ASTNodeType::Type_ASTNodeGeneral_35_expStmt:
    return "Type_ASTNodeGeneral_35_expStmt";
  case ASTNodeType::Type_ASTNodeGeneral_36_compoundStmt:
    return "Type_ASTNodeGeneral_36_compoundStmt";
  case ASTNodeType::Type_ASTNodeGeneral_37_localDecls:
    return "Type_ASTNodeGeneral_37_localDecls";
  case ASTNodeType::Type_ASTNodeGeneral_38_localDecls:
    return "Type_ASTNodeGeneral_38_localDecls";
  case ASTNodeType::Type_ASTNodeGeneral_39_stmtList:
    return "Type_ASTNodeGeneral_39_stmtList";
  case ASTNodeType::Type_ASTNodeGeneral_3_decl:
    return "Type_ASTNodeGeneral_3_decl";
  case ASTNodeType::Type_ASTNodeGeneral_40_stmtList:
    return "Type_ASTNodeGeneral_40_stmtList";
  case ASTNodeType::Type_ASTNodeGeneral_41_selectStmt:
    return "Type_ASTNodeGeneral_41_selectStmt";
  case ASTNodeType::Type_ASTNodeGeneral_42_selectStmt:
    return "Type_ASTNodeGeneral_42_selectStmt";
  case ASTNodeType::Type_ASTNodeGeneral_43_iterStmt:
    return "Type_ASTNodeGeneral_43_iterStmt";
  case ASTNodeType::Type_ASTNodeGeneral_44_iterStmt:
    return "Type_ASTNodeGeneral_44_iterStmt";
  case ASTNodeType::Type_ASTNodeGeneral_45_iterRange:
    return "Type_ASTNodeGeneral_45_iterRange";
  case ASTNodeType::Type_ASTNodeGeneral_46_iterRange:
    return "Type_ASTNodeGeneral_46_iterRange";
  case ASTNodeType::Type_ASTNodeGeneral_47_returnStmt:
    return "Type_ASTNodeGeneral_47_returnStmt";
  case ASTNodeType::Type_ASTNodeGeneral_48_returnStmt:
    return "Type_ASTNodeGeneral_48_returnStmt";
  case ASTNodeType::Type_ASTNodeGeneral_49_breakStmt:
    return "Type_ASTNodeGeneral_49_breakStmt";
  case ASTNodeType::Type_ASTNodeGeneral_4_decl:
    return "Type_ASTNodeGeneral_4_decl";
  case ASTNodeType::Type_ASTNodeGeneral_50_exp:
    return "Type_ASTNodeGeneral_50_exp";
  case ASTNodeType::Type_ASTNodeGeneral_51_exp:
    return "Type_ASTNodeGeneral_51_exp";
  case ASTNodeType::Type_ASTNodeGeneral_52_exp:
    return "Type_ASTNodeGeneral_52_exp";
  case ASTNodeType::Type_ASTNodeGeneral_53_exp:
    return "Type_ASTNodeGeneral_53_exp";
  case ASTNodeType::Type_ASTNodeGeneral_54_exp:
    return "Type_ASTNodeGeneral_54_exp";
  case ASTNodeType::Type_ASTNodeGeneral_55_exp:
    return "Type_ASTNodeGeneral_55_exp";
  case ASTNodeType::Type_ASTNodeGeneral_56_exp:
    return "Type_ASTNodeGeneral_56_exp";
  case ASTNodeType::Type_ASTNodeGeneral_57_exp:
    return "Type_ASTNodeGeneral_57_exp";
  case ASTNodeType::Type_ASTNodeGeneral_58_simpleExp:
    return "Type_ASTNodeGeneral_58_simpleExp";
  case ASTNodeType::Type_ASTNodeGeneral_59_simpleExp:
    return "Type_ASTNodeGeneral_59_simpleExp";
  case ASTNodeType::Type_ASTNodeGeneral_5_varDecl:
    return "Type_ASTNodeGeneral_5_varDecl";
  case ASTNodeType::Type_ASTNodeGeneral_60_andExp:
    return "Type_ASTNodeGeneral_60_andExp";
  case ASTNodeType::Type_ASTNodeGeneral_61_andExp:
    return "Type_ASTNodeGeneral_61_andExp";
  case ASTNodeType::Type_ASTNodeGeneral_62_unaryRelExp:
    return "Type_ASTNodeGeneral_62_unaryRelExp";
  case ASTNodeType::Type_ASTNodeGeneral_63_unaryRelExp:
    return "Type_ASTNodeGeneral_63_unaryRelExp";
  case ASTNodeType::Type_ASTNodeGeneral_64_relExp:
    return "Type_ASTNodeGeneral_64_relExp";
  case ASTNodeType::Type_ASTNodeGeneral_65_relExp:
    return "Type_ASTNodeGeneral_65_relExp";
  case ASTNodeType::Type_ASTNodeGeneral_66_relop:
    return "Type_ASTNodeGeneral_66_relop";
  case ASTNodeType::Type_ASTNodeGeneral_67_relop:
    return "Type_ASTNodeGeneral_67_relop";
  case ASTNodeType::Type_ASTNodeGeneral_68_relop:
    return "Type_ASTNodeGeneral_68_relop";
  case ASTNodeType::Type_ASTNodeGeneral_69_relop:
    return "Type_ASTNodeGeneral_69_relop";
  case ASTNodeType::Type_ASTNodeGeneral_6_scopedVarDecl:
    return "Type_ASTNodeGeneral_6_scopedVarDecl";
  case ASTNodeType::Type_ASTNodeGeneral_70_relop:
    return "Type_ASTNodeGeneral_70_relop";
  case ASTNodeType::Type_ASTNodeGeneral_71_relop:
    return "Type_ASTNodeGeneral_71_relop";
  case ASTNodeType::Type_ASTNodeGeneral_72_minmaxExp:
    return "Type_ASTNodeGeneral_72_minmaxExp";
  case ASTNodeType::Type_ASTNodeGeneral_73_minmaxExp:
    return "Type_ASTNodeGeneral_73_minmaxExp";
  case ASTNodeType::Type_ASTNodeGeneral_74_minmaxop:
    return "Type_ASTNodeGeneral_74_minmaxop";
  case ASTNodeType::Type_ASTNodeGeneral_75_minmaxop:
    return "Type_ASTNodeGeneral_75_minmaxop";
  case ASTNodeType::Type_ASTNodeGeneral_76_sumExp:
    return "Type_ASTNodeGeneral_76_sumExp";
  case ASTNodeType::Type_ASTNodeGeneral_77_sumExp:
    return "Type_ASTNodeGeneral_77_sumExp";
  case ASTNodeType::Type_ASTNodeGeneral_78_sumop:
    return "Type_ASTNodeGeneral_78_sumop";
  case ASTNodeType::Type_ASTNodeGeneral_79_sumop:
    return "Type_ASTNodeGeneral_79_sumop";
  case ASTNodeType::Type_ASTNodeGeneral_7_scopedVarDecl:
    return "Type_ASTNodeGeneral_7_scopedVarDecl";
  case ASTNodeType::Type_ASTNodeGeneral_80_mulExp:
    return "Type_ASTNodeGeneral_80_mulExp";
  case ASTNodeType::Type_ASTNodeGeneral_81_mulExp:
    return "Type_ASTNodeGeneral_81_mulExp";
  case ASTNodeType::Type_ASTNodeGeneral_82_mulop:
    return "Type_ASTNodeGeneral_82_mulop";
  case ASTNodeType::Type_ASTNodeGeneral_83_mulop:
    return "Type_ASTNodeGeneral_83_mulop";
  case ASTNodeType::Type_ASTNodeGeneral_84_mulop:
    return "Type_ASTNodeGeneral_84_mulop";
  case ASTNodeType::Type_ASTNodeGeneral_85_unaryExp:
    return "Type_ASTNodeGeneral_85_unaryExp";
  case ASTNodeType::Type_ASTNodeGeneral_86_unaryExp:
    return "Type_ASTNodeGeneral_86_unaryExp";
  case ASTNodeType::Type_ASTNodeGeneral_87_unaryop:
    return "Type_ASTNodeGeneral_87_unaryop";
  case ASTNodeType::Type_ASTNodeGeneral_88_unaryop:
    return "Type_ASTNodeGeneral_88_unaryop";
  case ASTNodeType::Type_ASTNodeGeneral_89_unaryop:
    return "Type_ASTNodeGeneral_89_unaryop";
  case ASTNodeType::Type_ASTNodeGeneral_8_varDeclList:
    return "Type_ASTNodeGeneral_8_varDeclList";
  case ASTNodeType::Type_ASTNodeGeneral_90_factor:
    return "Type_ASTNodeGeneral_90_factor";
  case ASTNodeType::Type_ASTNodeGeneral_91_factor:
    return "Type_ASTNodeGeneral_91_factor";
  case ASTNodeType::Type_ASTNodeGeneral_92_mutable:
    return "Type_ASTNodeGeneral_92_mutable";
  case ASTNodeType::Type_ASTNodeGeneral_93_mutable:
    return "Type_ASTNodeGeneral_93_mutable";
  case ASTNodeType::Type_ASTNodeGeneral_94_immutable:
    return "Type_ASTNodeGeneral_94_immutable";
  case ASTNodeType::Type_ASTNodeGeneral_95_immutable:
    return "Type_ASTNodeGeneral_95_immutable";
  case ASTNodeType::Type_ASTNodeGeneral_96_immutable:
    return "Type_ASTNodeGeneral_96_immutable";
  case ASTNodeType::Type_ASTNodeGeneral_97_call:
    return "Type_ASTNodeGeneral_97_call";
  case ASTNodeType::Type_ASTNodeGeneral_98_args:
    return "Type_ASTNodeGeneral_98_args";
  case ASTNodeType::Type_ASTNodeGeneral_99_args:
    return "Type_ASTNodeGeneral_99_args";
  case ASTNodeType::Type_ASTNodeGeneral_9_varDeclList:
    return "Type_ASTNodeGeneral_9_varDeclList";
  // Default case for unrecognized enums.
  default:
     MANTA_FAIL("unrecognized enumeration");
  }
}

//! \brief Define the enums for each non-terminal type.
enum class NonterminalType{
  Terminal,
  andExp,
  argList,
  args,
  breakStmt,
  call,
  compoundStmt,
  constant,
  decl,
  declList,
  exp,
  expStmt,
  factor,
  funDecl,
  immutable,
  iterRange,
  iterStmt,
  localDecls,
  minmaxExp,
  minmaxop,
  mulExp,
  mulop,
  mutable_,
  parmId,
  parmIdList,
  parmList,
  parmTypeList,
  parms,
  program,
  relExp,
  relop,
  returnStmt,
  scopedVarDecl,
  selectStmt,
  simpleExp,
  stmt,
  stmtList,
  sumExp,
  sumop,
  typeSpec,
  unaryExp,
  unaryRelExp,
  unaryop,
  varDecl,
  varDeclId,
  varDeclInit,
  varDeclList,
};

//! \brief Define function to write the non-terminal type enum.
inline const char* to_string(NonterminalType type) {
  switch (type) {
  case NonterminalType::Terminal:
    return "Terminal";
  case NonterminalType::andExp:
    return "andExp";
  case NonterminalType::argList:
    return "argList";
  case NonterminalType::args:
    return "args";
  case NonterminalType::breakStmt:
    return "breakStmt";
  case NonterminalType::call:
    return "call";
  case NonterminalType::compoundStmt:
    return "compoundStmt";
  case NonterminalType::constant:
    return "constant";
  case NonterminalType::decl:
    return "decl";
  case NonterminalType::declList:
    return "declList";
  case NonterminalType::exp:
    return "exp";
  case NonterminalType::expStmt:
    return "expStmt";
  case NonterminalType::factor:
    return "factor";
  case NonterminalType::funDecl:
    return "funDecl";
  case NonterminalType::immutable:
    return "immutable";
  case NonterminalType::iterRange:
    return "iterRange";
  case NonterminalType::iterStmt:
    return "iterStmt";
  case NonterminalType::localDecls:
    return "localDecls";
  case NonterminalType::minmaxExp:
    return "minmaxExp";
  case NonterminalType::minmaxop:
    return "minmaxop";
  case NonterminalType::mulExp:
    return "mulExp";
  case NonterminalType::mulop:
    return "mulop";
  case NonterminalType::mutable_:
    return "mutable_";
  case NonterminalType::parmId:
    return "parmId";
  case NonterminalType::parmIdList:
    return "parmIdList";
  case NonterminalType::parmList:
    return "parmList";
  case NonterminalType::parmTypeList:
    return "parmTypeList";
  case NonterminalType::parms:
    return "parms";
  case NonterminalType::program:
    return "program";
  case NonterminalType::relExp:
    return "relExp";
  case NonterminalType::relop:
    return "relop";
  case NonterminalType::returnStmt:
    return "returnStmt";
  case NonterminalType::scopedVarDecl:
    return "scopedVarDecl";
  case NonterminalType::selectStmt:
    return "selectStmt";
  case NonterminalType::simpleExp:
    return "simpleExp";
  case NonterminalType::stmt:
    return "stmt";
  case NonterminalType::stmtList:
    return "stmtList";
  case NonterminalType::sumExp:
    return "sumExp";
  case NonterminalType::sumop:
    return "sumop";
  case NonterminalType::typeSpec:
    return "typeSpec";
  case NonterminalType::unaryExp:
    return "unaryExp";
  case NonterminalType::unaryRelExp:
    return "unaryRelExp";
  case NonterminalType::unaryop:
    return "unaryop";
  case NonterminalType::varDecl:
    return "varDecl";
  case NonterminalType::varDeclId:
    return "varDeclId";
  case NonterminalType::varDeclInit:
    return "varDeclInit";
  case NonterminalType::varDeclList:
    return "varDeclList";
  // Default case for unrecognized enums.
  default:
     MANTA_FAIL("unrecognized enumeration");
  }
}

//! \brief Base visitor class.
struct ASTNodeVisitor {
  virtual ~ASTNodeVisitor() = default;
  virtual void Visit(ASTNodeGeneral_0_program& object) = 0;
  virtual void Visit(ASTNodeBase_declList& object) = 0;
  virtual void Visit(ASTNodeGeneral_1_declList& object) = 0;
  virtual void Visit(ASTNodeGeneral_2_declList& object) = 0;
  virtual void Visit(ASTNodeBase_decl& object) = 0;
  virtual void Visit(ASTNodeGeneral_3_decl& object) = 0;
  virtual void Visit(ASTNodeGeneral_4_decl& object) = 0;
  virtual void Visit(ASTNodeGeneral_5_varDecl& object) = 0;
  virtual void Visit(ASTNodeBase_funDecl& object) = 0;
  virtual void Visit(ASTNodeGeneral_17_funDecl& object) = 0;
  virtual void Visit(ASTNodeGeneral_18_funDecl& object) = 0;
  virtual void Visit(ASTNodeBase_typeSpec& object) = 0;
  virtual void Visit(ASTNodeGeneral_14_typeSpec& object) = 0;
  virtual void Visit(ASTNodeGeneral_15_typeSpec& object) = 0;
  virtual void Visit(ASTNodeGeneral_16_typeSpec& object) = 0;
  virtual void Visit(ASTNodeBase_varDeclList& object) = 0;
  virtual void Visit(ASTNodeGeneral_8_varDeclList& object) = 0;
  virtual void Visit(ASTNodeGeneral_9_varDeclList& object) = 0;
  virtual void Visit(ASTNodeBase_scopedVarDecl& object) = 0;
  virtual void Visit(ASTNodeGeneral_6_scopedVarDecl& object) = 0;
  virtual void Visit(ASTNodeGeneral_7_scopedVarDecl& object) = 0;
  virtual void Visit(ASTNodeBase_varDeclInit& object) = 0;
  virtual void Visit(ASTNodeGeneral_10_varDeclInit& object) = 0;
  virtual void Visit(ASTNodeGeneral_11_varDeclInit& object) = 0;
  virtual void Visit(ASTNodeBase_varDeclId& object) = 0;
  virtual void Visit(ASTNodeGeneral_12_varDeclId& object) = 0;
  virtual void Visit(ASTNodeGeneral_13_varDeclId& object) = 0;
  virtual void Visit(ASTNodeBase_simpleExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_58_simpleExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_59_simpleExp& object) = 0;
  virtual void Visit(ASTNodeBase_parms& object) = 0;
  virtual void Visit(ASTNodeGeneral_19_parms& object) = 0;
  virtual void Visit(ASTNodeGeneral_20_parms& object) = 0;
  virtual void Visit(ASTNodeBase_stmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_28_stmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_29_stmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_30_stmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_31_stmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_32_stmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_33_stmt& object) = 0;
  virtual void Visit(ASTNodeBase_parmList& object) = 0;
  virtual void Visit(ASTNodeGeneral_21_parmList& object) = 0;
  virtual void Visit(ASTNodeGeneral_22_parmList& object) = 0;
  virtual void Visit(ASTNodeGeneral_23_parmTypeList& object) = 0;
  virtual void Visit(ASTNodeBase_parmIdList& object) = 0;
  virtual void Visit(ASTNodeGeneral_24_parmIdList& object) = 0;
  virtual void Visit(ASTNodeGeneral_25_parmIdList& object) = 0;
  virtual void Visit(ASTNodeBase_parmId& object) = 0;
  virtual void Visit(ASTNodeGeneral_26_parmId& object) = 0;
  virtual void Visit(ASTNodeGeneral_27_parmId& object) = 0;
  virtual void Visit(ASTNodeBase_expStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_34_expStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_35_expStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_36_compoundStmt& object) = 0;
  virtual void Visit(ASTNodeBase_selectStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_41_selectStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_42_selectStmt& object) = 0;
  virtual void Visit(ASTNodeBase_iterStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_43_iterStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_44_iterStmt& object) = 0;
  virtual void Visit(ASTNodeBase_returnStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_47_returnStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_48_returnStmt& object) = 0;
  virtual void Visit(ASTNodeGeneral_49_breakStmt& object) = 0;
  virtual void Visit(ASTNodeBase_exp& object) = 0;
  virtual void Visit(ASTNodeGeneral_50_exp& object) = 0;
  virtual void Visit(ASTNodeGeneral_51_exp& object) = 0;
  virtual void Visit(ASTNodeGeneral_52_exp& object) = 0;
  virtual void Visit(ASTNodeGeneral_53_exp& object) = 0;
  virtual void Visit(ASTNodeGeneral_54_exp& object) = 0;
  virtual void Visit(ASTNodeGeneral_55_exp& object) = 0;
  virtual void Visit(ASTNodeGeneral_56_exp& object) = 0;
  virtual void Visit(ASTNodeGeneral_57_exp& object) = 0;
  virtual void Visit(ASTNodeBase_localDecls& object) = 0;
  virtual void Visit(ASTNodeGeneral_37_localDecls& object) = 0;
  virtual void Visit(ASTNodeGeneral_38_localDecls& object) = 0;
  virtual void Visit(ASTNodeBase_stmtList& object) = 0;
  virtual void Visit(ASTNodeGeneral_39_stmtList& object) = 0;
  virtual void Visit(ASTNodeGeneral_40_stmtList& object) = 0;
  virtual void Visit(ASTNodeBase_iterRange& object) = 0;
  virtual void Visit(ASTNodeGeneral_45_iterRange& object) = 0;
  virtual void Visit(ASTNodeGeneral_46_iterRange& object) = 0;
  virtual void Visit(ASTNodeBase_mutable& object) = 0;
  virtual void Visit(ASTNodeGeneral_92_mutable& object) = 0;
  virtual void Visit(ASTNodeGeneral_93_mutable& object) = 0;
  virtual void Visit(ASTNodeBase_andExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_60_andExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_61_andExp& object) = 0;
  virtual void Visit(ASTNodeBase_unaryRelExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_62_unaryRelExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_63_unaryRelExp& object) = 0;
  virtual void Visit(ASTNodeBase_relExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_64_relExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_65_relExp& object) = 0;
  virtual void Visit(ASTNodeBase_minmaxExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_72_minmaxExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_73_minmaxExp& object) = 0;
  virtual void Visit(ASTNodeBase_relop& object) = 0;
  virtual void Visit(ASTNodeGeneral_66_relop& object) = 0;
  virtual void Visit(ASTNodeGeneral_67_relop& object) = 0;
  virtual void Visit(ASTNodeGeneral_68_relop& object) = 0;
  virtual void Visit(ASTNodeGeneral_69_relop& object) = 0;
  virtual void Visit(ASTNodeGeneral_70_relop& object) = 0;
  virtual void Visit(ASTNodeGeneral_71_relop& object) = 0;
  virtual void Visit(ASTNodeBase_minmaxop& object) = 0;
  virtual void Visit(ASTNodeGeneral_74_minmaxop& object) = 0;
  virtual void Visit(ASTNodeGeneral_75_minmaxop& object) = 0;
  virtual void Visit(ASTNodeBase_sumExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_76_sumExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_77_sumExp& object) = 0;
  virtual void Visit(ASTNodeBase_sumop& object) = 0;
  virtual void Visit(ASTNodeGeneral_78_sumop& object) = 0;
  virtual void Visit(ASTNodeGeneral_79_sumop& object) = 0;
  virtual void Visit(ASTNodeBase_mulExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_80_mulExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_81_mulExp& object) = 0;
  virtual void Visit(ASTNodeBase_mulop& object) = 0;
  virtual void Visit(ASTNodeGeneral_82_mulop& object) = 0;
  virtual void Visit(ASTNodeGeneral_83_mulop& object) = 0;
  virtual void Visit(ASTNodeGeneral_84_mulop& object) = 0;
  virtual void Visit(ASTNodeBase_unaryExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_85_unaryExp& object) = 0;
  virtual void Visit(ASTNodeGeneral_86_unaryExp& object) = 0;
  virtual void Visit(ASTNodeBase_unaryop& object) = 0;
  virtual void Visit(ASTNodeGeneral_87_unaryop& object) = 0;
  virtual void Visit(ASTNodeGeneral_88_unaryop& object) = 0;
  virtual void Visit(ASTNodeGeneral_89_unaryop& object) = 0;
  virtual void Visit(ASTNodeBase_factor& object) = 0;
  virtual void Visit(ASTNodeGeneral_90_factor& object) = 0;
  virtual void Visit(ASTNodeGeneral_91_factor& object) = 0;
  virtual void Visit(ASTNodeBase_immutable& object) = 0;
  virtual void Visit(ASTNodeGeneral_94_immutable& object) = 0;
  virtual void Visit(ASTNodeGeneral_95_immutable& object) = 0;
  virtual void Visit(ASTNodeGeneral_96_immutable& object) = 0;
  virtual void Visit(ASTNodeGeneral_97_call& object) = 0;
  virtual void Visit(ASTNodeBase_constant& object) = 0;
  virtual void Visit(ASTNodeGeneral_102_constant& object) = 0;
  virtual void Visit(ASTNodeGeneral_103_constant& object) = 0;
  virtual void Visit(ASTNodeGeneral_104_constant& object) = 0;
  virtual void Visit(ASTNodeGeneral_105_constant& object) = 0;
  virtual void Visit(ASTNodeGeneral_106_constant& object) = 0;
  virtual void Visit(ASTNodeBase_args& object) = 0;
  virtual void Visit(ASTNodeGeneral_98_args& object) = 0;
  virtual void Visit(ASTNodeGeneral_99_args& object) = 0;
  virtual void Visit(ASTNodeBase_argList& object) = 0;
  virtual void Visit(ASTNodeGeneral_100_argList& object) = 0;
  virtual void Visit(ASTNodeGeneral_101_argList& object) = 0;
  virtual void Visit(ASTLexeme& object) = 0;
};

//! \brief Define the base node type for all AST nodes.
struct ASTNodeBase {
  ASTNodeBase(const ASTNodeType& node_type, const NonterminalType& nonterminal_type, const ItemID& item_id)
    : node_type(node_type), nonterminal_type(nonterminal_type), item_id(item_id) {}
  virtual ~ASTNodeBase() = default;

  ASTNodeType node_type{};
  NonterminalType nonterminal_type{};
  ItemID item_id{};

  virtual void Accept(ASTNodeVisitor& visitor) = 0;
};

//! \brief Node for basic lexemes.
struct ASTLexeme : ASTNodeBase {
  explicit ASTLexeme(const std::string& literal)
    : ASTNodeBase(ASTNodeType::Literal, NonterminalType::Terminal, -1), literal(literal) {}

  std::string literal{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 57's productions.
// ============================================================

struct ASTNodeGeneral_0_program : ASTNodeBase {
  explicit ASTNodeGeneral_0_program(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNodeGeneral_0_program, NonterminalType::program, item_id) {}

  std::shared_ptr<ASTNodeBase_declList> declList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 58's productions.
// ============================================================

//! \brief Parent type for non-terminal type 58
struct ASTNodeBase_declList : ASTNodeBase {
  ASTNodeBase_declList(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::declList, item_id) {}

  std::shared_ptr<ASTNodeBase_decl> decl_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_1_declList : ASTNodeBase_declList {
  explicit ASTNodeGeneral_1_declList(const ItemID& item_id)
    : ASTNodeBase_declList(ASTNodeType::Type_ASTNodeGeneral_1_declList, item_id) {}

  std::shared_ptr<ASTNodeBase_declList> declList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_2_declList : ASTNodeBase_declList {
  explicit ASTNodeGeneral_2_declList(const ItemID& item_id)
    : ASTNodeBase_declList(ASTNodeType::Type_ASTNodeGeneral_2_declList, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 59's productions.
// ============================================================

//! \brief Parent type for non-terminal type 59
struct ASTNodeBase_decl : ASTNodeBase {
  ASTNodeBase_decl(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::decl, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_3_decl : ASTNodeBase_decl {
  explicit ASTNodeGeneral_3_decl(const ItemID& item_id)
    : ASTNodeBase_decl(ASTNodeType::Type_ASTNodeGeneral_3_decl, item_id) {}

  std::shared_ptr<ASTNodeGeneral_5_varDecl> varDecl_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_4_decl : ASTNodeBase_decl {
  explicit ASTNodeGeneral_4_decl(const ItemID& item_id)
    : ASTNodeBase_decl(ASTNodeType::Type_ASTNodeGeneral_4_decl, item_id) {}

  std::shared_ptr<ASTNodeBase_funDecl> funDecl_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 60's productions.
// ============================================================

struct ASTNodeGeneral_5_varDecl : ASTNodeBase {
  explicit ASTNodeGeneral_5_varDecl(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNodeGeneral_5_varDecl, NonterminalType::varDecl, item_id) {}

  std::shared_ptr<ASTNodeBase_typeSpec> typeSpec_node{};
  std::shared_ptr<ASTNodeBase_varDeclList> varDeclList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 61's productions.
// ============================================================

//! \brief Parent type for non-terminal type 61
struct ASTNodeBase_funDecl : ASTNodeBase {
  ASTNodeBase_funDecl(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::funDecl, item_id) {}

  std::string ID{};
  std::shared_ptr<ASTNodeBase_parms> parms_node{};
  std::shared_ptr<ASTNodeBase_stmt> stmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_17_funDecl : ASTNodeBase_funDecl {
  explicit ASTNodeGeneral_17_funDecl(const ItemID& item_id)
    : ASTNodeBase_funDecl(ASTNodeType::Type_ASTNodeGeneral_17_funDecl, item_id) {}

  std::shared_ptr<ASTNodeBase_typeSpec> typeSpec_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_18_funDecl : ASTNodeBase_funDecl {
  explicit ASTNodeGeneral_18_funDecl(const ItemID& item_id)
    : ASTNodeBase_funDecl(ASTNodeType::Type_ASTNodeGeneral_18_funDecl, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 62's productions.
// ============================================================

//! \brief Parent type for non-terminal type 62
struct ASTNodeBase_typeSpec : ASTNodeBase {
  ASTNodeBase_typeSpec(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::typeSpec, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_14_typeSpec : ASTNodeBase_typeSpec {
  explicit ASTNodeGeneral_14_typeSpec(const ItemID& item_id)
    : ASTNodeBase_typeSpec(ASTNodeType::Type_ASTNodeGeneral_14_typeSpec, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_15_typeSpec : ASTNodeBase_typeSpec {
  explicit ASTNodeGeneral_15_typeSpec(const ItemID& item_id)
    : ASTNodeBase_typeSpec(ASTNodeType::Type_ASTNodeGeneral_15_typeSpec, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_16_typeSpec : ASTNodeBase_typeSpec {
  explicit ASTNodeGeneral_16_typeSpec(const ItemID& item_id)
    : ASTNodeBase_typeSpec(ASTNodeType::Type_ASTNodeGeneral_16_typeSpec, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 63's productions.
// ============================================================

//! \brief Parent type for non-terminal type 63
struct ASTNodeBase_varDeclList : ASTNodeBase {
  ASTNodeBase_varDeclList(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::varDeclList, item_id) {}

  std::shared_ptr<ASTNodeBase_varDeclInit> varDeclInit_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_8_varDeclList : ASTNodeBase_varDeclList {
  explicit ASTNodeGeneral_8_varDeclList(const ItemID& item_id)
    : ASTNodeBase_varDeclList(ASTNodeType::Type_ASTNodeGeneral_8_varDeclList, item_id) {}

  std::shared_ptr<ASTNodeBase_varDeclList> varDeclList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_9_varDeclList : ASTNodeBase_varDeclList {
  explicit ASTNodeGeneral_9_varDeclList(const ItemID& item_id)
    : ASTNodeBase_varDeclList(ASTNodeType::Type_ASTNodeGeneral_9_varDeclList, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 64's productions.
// ============================================================

//! \brief Parent type for non-terminal type 64
struct ASTNodeBase_scopedVarDecl : ASTNodeBase {
  ASTNodeBase_scopedVarDecl(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::scopedVarDecl, item_id) {}

  std::shared_ptr<ASTNodeBase_typeSpec> typeSpec_node{};
  std::shared_ptr<ASTNodeBase_varDeclList> varDeclList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_6_scopedVarDecl : ASTNodeBase_scopedVarDecl {
  explicit ASTNodeGeneral_6_scopedVarDecl(const ItemID& item_id)
    : ASTNodeBase_scopedVarDecl(ASTNodeType::Type_ASTNodeGeneral_6_scopedVarDecl, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_7_scopedVarDecl : ASTNodeBase_scopedVarDecl {
  explicit ASTNodeGeneral_7_scopedVarDecl(const ItemID& item_id)
    : ASTNodeBase_scopedVarDecl(ASTNodeType::Type_ASTNodeGeneral_7_scopedVarDecl, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 65's productions.
// ============================================================

//! \brief Parent type for non-terminal type 65
struct ASTNodeBase_varDeclInit : ASTNodeBase {
  ASTNodeBase_varDeclInit(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::varDeclInit, item_id) {}

  std::shared_ptr<ASTNodeBase_varDeclId> varDeclId_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_10_varDeclInit : ASTNodeBase_varDeclInit {
  explicit ASTNodeGeneral_10_varDeclInit(const ItemID& item_id)
    : ASTNodeBase_varDeclInit(ASTNodeType::Type_ASTNodeGeneral_10_varDeclInit, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_11_varDeclInit : ASTNodeBase_varDeclInit {
  explicit ASTNodeGeneral_11_varDeclInit(const ItemID& item_id)
    : ASTNodeBase_varDeclInit(ASTNodeType::Type_ASTNodeGeneral_11_varDeclInit, item_id) {}

  std::shared_ptr<ASTNodeBase_simpleExp> simpleExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 66's productions.
// ============================================================

//! \brief Parent type for non-terminal type 66
struct ASTNodeBase_varDeclId : ASTNodeBase {
  ASTNodeBase_varDeclId(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::varDeclId, item_id) {}

  std::string ID{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_12_varDeclId : ASTNodeBase_varDeclId {
  explicit ASTNodeGeneral_12_varDeclId(const ItemID& item_id)
    : ASTNodeBase_varDeclId(ASTNodeType::Type_ASTNodeGeneral_12_varDeclId, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_13_varDeclId : ASTNodeBase_varDeclId {
  explicit ASTNodeGeneral_13_varDeclId(const ItemID& item_id)
    : ASTNodeBase_varDeclId(ASTNodeType::Type_ASTNodeGeneral_13_varDeclId, item_id) {}

  std::string NUMCONST{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 67's productions.
// ============================================================

//! \brief Parent type for non-terminal type 67
struct ASTNodeBase_simpleExp : ASTNodeBase {
  ASTNodeBase_simpleExp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::simpleExp, item_id) {}

  std::shared_ptr<ASTNodeBase_andExp> andExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_58_simpleExp : ASTNodeBase_simpleExp {
  explicit ASTNodeGeneral_58_simpleExp(const ItemID& item_id)
    : ASTNodeBase_simpleExp(ASTNodeType::Type_ASTNodeGeneral_58_simpleExp, item_id) {}

  std::shared_ptr<ASTNodeBase_simpleExp> simpleExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_59_simpleExp : ASTNodeBase_simpleExp {
  explicit ASTNodeGeneral_59_simpleExp(const ItemID& item_id)
    : ASTNodeBase_simpleExp(ASTNodeType::Type_ASTNodeGeneral_59_simpleExp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 68's productions.
// ============================================================

//! \brief Parent type for non-terminal type 68
struct ASTNodeBase_parms : ASTNodeBase {
  ASTNodeBase_parms(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::parms, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_19_parms : ASTNodeBase_parms {
  explicit ASTNodeGeneral_19_parms(const ItemID& item_id)
    : ASTNodeBase_parms(ASTNodeType::Type_ASTNodeGeneral_19_parms, item_id) {}

  std::shared_ptr<ASTNodeBase_parmList> parmList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_20_parms : ASTNodeBase_parms {
  explicit ASTNodeGeneral_20_parms(const ItemID& item_id)
    : ASTNodeBase_parms(ASTNodeType::Type_ASTNodeGeneral_20_parms, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 69's productions.
// ============================================================

//! \brief Parent type for non-terminal type 69
struct ASTNodeBase_stmt : ASTNodeBase {
  ASTNodeBase_stmt(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::stmt, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_28_stmt : ASTNodeBase_stmt {
  explicit ASTNodeGeneral_28_stmt(const ItemID& item_id)
    : ASTNodeBase_stmt(ASTNodeType::Type_ASTNodeGeneral_28_stmt, item_id) {}

  std::shared_ptr<ASTNodeBase_expStmt> expStmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_29_stmt : ASTNodeBase_stmt {
  explicit ASTNodeGeneral_29_stmt(const ItemID& item_id)
    : ASTNodeBase_stmt(ASTNodeType::Type_ASTNodeGeneral_29_stmt, item_id) {}

  std::shared_ptr<ASTNodeGeneral_36_compoundStmt> compoundStmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_30_stmt : ASTNodeBase_stmt {
  explicit ASTNodeGeneral_30_stmt(const ItemID& item_id)
    : ASTNodeBase_stmt(ASTNodeType::Type_ASTNodeGeneral_30_stmt, item_id) {}

  std::shared_ptr<ASTNodeBase_selectStmt> selectStmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_31_stmt : ASTNodeBase_stmt {
  explicit ASTNodeGeneral_31_stmt(const ItemID& item_id)
    : ASTNodeBase_stmt(ASTNodeType::Type_ASTNodeGeneral_31_stmt, item_id) {}

  std::shared_ptr<ASTNodeBase_iterStmt> iterStmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_32_stmt : ASTNodeBase_stmt {
  explicit ASTNodeGeneral_32_stmt(const ItemID& item_id)
    : ASTNodeBase_stmt(ASTNodeType::Type_ASTNodeGeneral_32_stmt, item_id) {}

  std::shared_ptr<ASTNodeBase_returnStmt> returnStmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_33_stmt : ASTNodeBase_stmt {
  explicit ASTNodeGeneral_33_stmt(const ItemID& item_id)
    : ASTNodeBase_stmt(ASTNodeType::Type_ASTNodeGeneral_33_stmt, item_id) {}

  std::shared_ptr<ASTNodeGeneral_49_breakStmt> breakStmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 70's productions.
// ============================================================

//! \brief Parent type for non-terminal type 70
struct ASTNodeBase_parmList : ASTNodeBase {
  ASTNodeBase_parmList(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::parmList, item_id) {}

  std::shared_ptr<ASTNodeGeneral_23_parmTypeList> parmTypeList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_21_parmList : ASTNodeBase_parmList {
  explicit ASTNodeGeneral_21_parmList(const ItemID& item_id)
    : ASTNodeBase_parmList(ASTNodeType::Type_ASTNodeGeneral_21_parmList, item_id) {}

  std::shared_ptr<ASTNodeBase_parmList> parmList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_22_parmList : ASTNodeBase_parmList {
  explicit ASTNodeGeneral_22_parmList(const ItemID& item_id)
    : ASTNodeBase_parmList(ASTNodeType::Type_ASTNodeGeneral_22_parmList, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 71's productions.
// ============================================================

struct ASTNodeGeneral_23_parmTypeList : ASTNodeBase {
  explicit ASTNodeGeneral_23_parmTypeList(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNodeGeneral_23_parmTypeList, NonterminalType::parmTypeList, item_id) {}

  std::shared_ptr<ASTNodeBase_typeSpec> typeSpec_node{};
  std::shared_ptr<ASTNodeBase_parmIdList> parmIdList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 72's productions.
// ============================================================

//! \brief Parent type for non-terminal type 72
struct ASTNodeBase_parmIdList : ASTNodeBase {
  ASTNodeBase_parmIdList(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::parmIdList, item_id) {}

  std::shared_ptr<ASTNodeBase_parmId> parmId_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_24_parmIdList : ASTNodeBase_parmIdList {
  explicit ASTNodeGeneral_24_parmIdList(const ItemID& item_id)
    : ASTNodeBase_parmIdList(ASTNodeType::Type_ASTNodeGeneral_24_parmIdList, item_id) {}

  std::shared_ptr<ASTNodeBase_parmIdList> parmIdList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_25_parmIdList : ASTNodeBase_parmIdList {
  explicit ASTNodeGeneral_25_parmIdList(const ItemID& item_id)
    : ASTNodeBase_parmIdList(ASTNodeType::Type_ASTNodeGeneral_25_parmIdList, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 73's productions.
// ============================================================

//! \brief Parent type for non-terminal type 73
struct ASTNodeBase_parmId : ASTNodeBase {
  ASTNodeBase_parmId(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::parmId, item_id) {}

  std::string ID{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_26_parmId : ASTNodeBase_parmId {
  explicit ASTNodeGeneral_26_parmId(const ItemID& item_id)
    : ASTNodeBase_parmId(ASTNodeType::Type_ASTNodeGeneral_26_parmId, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_27_parmId : ASTNodeBase_parmId {
  explicit ASTNodeGeneral_27_parmId(const ItemID& item_id)
    : ASTNodeBase_parmId(ASTNodeType::Type_ASTNodeGeneral_27_parmId, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 74's productions.
// ============================================================

//! \brief Parent type for non-terminal type 74
struct ASTNodeBase_expStmt : ASTNodeBase {
  ASTNodeBase_expStmt(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::expStmt, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_34_expStmt : ASTNodeBase_expStmt {
  explicit ASTNodeGeneral_34_expStmt(const ItemID& item_id)
    : ASTNodeBase_expStmt(ASTNodeType::Type_ASTNodeGeneral_34_expStmt, item_id) {}

  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_35_expStmt : ASTNodeBase_expStmt {
  explicit ASTNodeGeneral_35_expStmt(const ItemID& item_id)
    : ASTNodeBase_expStmt(ASTNodeType::Type_ASTNodeGeneral_35_expStmt, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 75's productions.
// ============================================================

struct ASTNodeGeneral_36_compoundStmt : ASTNodeBase {
  explicit ASTNodeGeneral_36_compoundStmt(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNodeGeneral_36_compoundStmt, NonterminalType::compoundStmt, item_id) {}

  std::shared_ptr<ASTNodeBase_localDecls> localDecls_node{};
  std::shared_ptr<ASTNodeBase_stmtList> stmtList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 76's productions.
// ============================================================

//! \brief Parent type for non-terminal type 76
struct ASTNodeBase_selectStmt : ASTNodeBase {
  ASTNodeBase_selectStmt(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::selectStmt, item_id) {}

  std::shared_ptr<ASTNodeBase_simpleExp> simpleExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_41_selectStmt : ASTNodeBase_selectStmt {
  explicit ASTNodeGeneral_41_selectStmt(const ItemID& item_id)
    : ASTNodeBase_selectStmt(ASTNodeType::Type_ASTNodeGeneral_41_selectStmt, item_id) {}

  std::shared_ptr<ASTNodeBase_stmt> stmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_42_selectStmt : ASTNodeBase_selectStmt {
  explicit ASTNodeGeneral_42_selectStmt(const ItemID& item_id)
    : ASTNodeBase_selectStmt(ASTNodeType::Type_ASTNodeGeneral_42_selectStmt, item_id) {}

  std::shared_ptr<ASTNodeBase_stmt> stmt_node_0{};
  std::shared_ptr<ASTNodeBase_stmt> stmt_node_1{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 77's productions.
// ============================================================

//! \brief Parent type for non-terminal type 77
struct ASTNodeBase_iterStmt : ASTNodeBase {
  ASTNodeBase_iterStmt(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::iterStmt, item_id) {}

  std::shared_ptr<ASTNodeBase_stmt> stmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_43_iterStmt : ASTNodeBase_iterStmt {
  explicit ASTNodeGeneral_43_iterStmt(const ItemID& item_id)
    : ASTNodeBase_iterStmt(ASTNodeType::Type_ASTNodeGeneral_43_iterStmt, item_id) {}

  std::shared_ptr<ASTNodeBase_simpleExp> simpleExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_44_iterStmt : ASTNodeBase_iterStmt {
  explicit ASTNodeGeneral_44_iterStmt(const ItemID& item_id)
    : ASTNodeBase_iterStmt(ASTNodeType::Type_ASTNodeGeneral_44_iterStmt, item_id) {}

  std::string ID{};
  std::shared_ptr<ASTNodeBase_iterRange> iterRange_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 78's productions.
// ============================================================

//! \brief Parent type for non-terminal type 78
struct ASTNodeBase_returnStmt : ASTNodeBase {
  ASTNodeBase_returnStmt(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::returnStmt, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_47_returnStmt : ASTNodeBase_returnStmt {
  explicit ASTNodeGeneral_47_returnStmt(const ItemID& item_id)
    : ASTNodeBase_returnStmt(ASTNodeType::Type_ASTNodeGeneral_47_returnStmt, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_48_returnStmt : ASTNodeBase_returnStmt {
  explicit ASTNodeGeneral_48_returnStmt(const ItemID& item_id)
    : ASTNodeBase_returnStmt(ASTNodeType::Type_ASTNodeGeneral_48_returnStmt, item_id) {}

  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 79's productions.
// ============================================================

struct ASTNodeGeneral_49_breakStmt : ASTNodeBase {
  explicit ASTNodeGeneral_49_breakStmt(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNodeGeneral_49_breakStmt, NonterminalType::breakStmt, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 80's productions.
// ============================================================

//! \brief Parent type for non-terminal type 80
struct ASTNodeBase_exp : ASTNodeBase {
  ASTNodeBase_exp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::exp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_50_exp : ASTNodeBase_exp {
  explicit ASTNodeGeneral_50_exp(const ItemID& item_id)
    : ASTNodeBase_exp(ASTNodeType::Type_ASTNodeGeneral_50_exp, item_id) {}

  std::shared_ptr<ASTNodeBase_mutable> mutable__node{};
  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_51_exp : ASTNodeBase_exp {
  explicit ASTNodeGeneral_51_exp(const ItemID& item_id)
    : ASTNodeBase_exp(ASTNodeType::Type_ASTNodeGeneral_51_exp, item_id) {}

  std::shared_ptr<ASTNodeBase_mutable> mutable__node{};
  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_52_exp : ASTNodeBase_exp {
  explicit ASTNodeGeneral_52_exp(const ItemID& item_id)
    : ASTNodeBase_exp(ASTNodeType::Type_ASTNodeGeneral_52_exp, item_id) {}

  std::shared_ptr<ASTNodeBase_mutable> mutable__node{};
  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_53_exp : ASTNodeBase_exp {
  explicit ASTNodeGeneral_53_exp(const ItemID& item_id)
    : ASTNodeBase_exp(ASTNodeType::Type_ASTNodeGeneral_53_exp, item_id) {}

  std::shared_ptr<ASTNodeBase_mutable> mutable__node{};
  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_54_exp : ASTNodeBase_exp {
  explicit ASTNodeGeneral_54_exp(const ItemID& item_id)
    : ASTNodeBase_exp(ASTNodeType::Type_ASTNodeGeneral_54_exp, item_id) {}

  std::shared_ptr<ASTNodeBase_mutable> mutable__node{};
  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_55_exp : ASTNodeBase_exp {
  explicit ASTNodeGeneral_55_exp(const ItemID& item_id)
    : ASTNodeBase_exp(ASTNodeType::Type_ASTNodeGeneral_55_exp, item_id) {}

  std::shared_ptr<ASTNodeBase_mutable> mutable__node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_56_exp : ASTNodeBase_exp {
  explicit ASTNodeGeneral_56_exp(const ItemID& item_id)
    : ASTNodeBase_exp(ASTNodeType::Type_ASTNodeGeneral_56_exp, item_id) {}

  std::shared_ptr<ASTNodeBase_mutable> mutable__node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_57_exp : ASTNodeBase_exp {
  explicit ASTNodeGeneral_57_exp(const ItemID& item_id)
    : ASTNodeBase_exp(ASTNodeType::Type_ASTNodeGeneral_57_exp, item_id) {}

  std::shared_ptr<ASTNodeBase_simpleExp> simpleExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 81's productions.
// ============================================================

//! \brief Parent type for non-terminal type 81
struct ASTNodeBase_localDecls : ASTNodeBase {
  ASTNodeBase_localDecls(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::localDecls, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_37_localDecls : ASTNodeBase_localDecls {
  explicit ASTNodeGeneral_37_localDecls(const ItemID& item_id)
    : ASTNodeBase_localDecls(ASTNodeType::Type_ASTNodeGeneral_37_localDecls, item_id) {}

  std::shared_ptr<ASTNodeBase_localDecls> localDecls_node{};
  std::shared_ptr<ASTNodeBase_scopedVarDecl> scopedVarDecl_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_38_localDecls : ASTNodeBase_localDecls {
  explicit ASTNodeGeneral_38_localDecls(const ItemID& item_id)
    : ASTNodeBase_localDecls(ASTNodeType::Type_ASTNodeGeneral_38_localDecls, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 82's productions.
// ============================================================

//! \brief Parent type for non-terminal type 82
struct ASTNodeBase_stmtList : ASTNodeBase {
  ASTNodeBase_stmtList(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::stmtList, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_39_stmtList : ASTNodeBase_stmtList {
  explicit ASTNodeGeneral_39_stmtList(const ItemID& item_id)
    : ASTNodeBase_stmtList(ASTNodeType::Type_ASTNodeGeneral_39_stmtList, item_id) {}

  std::shared_ptr<ASTNodeBase_stmtList> stmtList_node{};
  std::shared_ptr<ASTNodeBase_stmt> stmt_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_40_stmtList : ASTNodeBase_stmtList {
  explicit ASTNodeGeneral_40_stmtList(const ItemID& item_id)
    : ASTNodeBase_stmtList(ASTNodeType::Type_ASTNodeGeneral_40_stmtList, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 83's productions.
// ============================================================

//! \brief Parent type for non-terminal type 83
struct ASTNodeBase_iterRange : ASTNodeBase {
  ASTNodeBase_iterRange(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::iterRange, item_id) {}

  std::shared_ptr<ASTNodeBase_simpleExp> simpleExp_node_0{};
  std::shared_ptr<ASTNodeBase_simpleExp> simpleExp_node_1{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_45_iterRange : ASTNodeBase_iterRange {
  explicit ASTNodeGeneral_45_iterRange(const ItemID& item_id)
    : ASTNodeBase_iterRange(ASTNodeType::Type_ASTNodeGeneral_45_iterRange, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_46_iterRange : ASTNodeBase_iterRange {
  explicit ASTNodeGeneral_46_iterRange(const ItemID& item_id)
    : ASTNodeBase_iterRange(ASTNodeType::Type_ASTNodeGeneral_46_iterRange, item_id) {}

  std::shared_ptr<ASTNodeBase_simpleExp> simpleExp_node_2{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 84's productions.
// ============================================================

//! \brief Parent type for non-terminal type 84
struct ASTNodeBase_mutable : ASTNodeBase {
  ASTNodeBase_mutable(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::mutable_, item_id) {}

  std::string ID{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_92_mutable : ASTNodeBase_mutable {
  explicit ASTNodeGeneral_92_mutable(const ItemID& item_id)
    : ASTNodeBase_mutable(ASTNodeType::Type_ASTNodeGeneral_92_mutable, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_93_mutable : ASTNodeBase_mutable {
  explicit ASTNodeGeneral_93_mutable(const ItemID& item_id)
    : ASTNodeBase_mutable(ASTNodeType::Type_ASTNodeGeneral_93_mutable, item_id) {}

  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 85's productions.
// ============================================================

//! \brief Parent type for non-terminal type 85
struct ASTNodeBase_andExp : ASTNodeBase {
  ASTNodeBase_andExp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::andExp, item_id) {}

  std::shared_ptr<ASTNodeBase_unaryRelExp> unaryRelExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_60_andExp : ASTNodeBase_andExp {
  explicit ASTNodeGeneral_60_andExp(const ItemID& item_id)
    : ASTNodeBase_andExp(ASTNodeType::Type_ASTNodeGeneral_60_andExp, item_id) {}

  std::shared_ptr<ASTNodeBase_andExp> andExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_61_andExp : ASTNodeBase_andExp {
  explicit ASTNodeGeneral_61_andExp(const ItemID& item_id)
    : ASTNodeBase_andExp(ASTNodeType::Type_ASTNodeGeneral_61_andExp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 86's productions.
// ============================================================

//! \brief Parent type for non-terminal type 86
struct ASTNodeBase_unaryRelExp : ASTNodeBase {
  ASTNodeBase_unaryRelExp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::unaryRelExp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_62_unaryRelExp : ASTNodeBase_unaryRelExp {
  explicit ASTNodeGeneral_62_unaryRelExp(const ItemID& item_id)
    : ASTNodeBase_unaryRelExp(ASTNodeType::Type_ASTNodeGeneral_62_unaryRelExp, item_id) {}

  std::shared_ptr<ASTNodeBase_unaryRelExp> unaryRelExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_63_unaryRelExp : ASTNodeBase_unaryRelExp {
  explicit ASTNodeGeneral_63_unaryRelExp(const ItemID& item_id)
    : ASTNodeBase_unaryRelExp(ASTNodeType::Type_ASTNodeGeneral_63_unaryRelExp, item_id) {}

  std::shared_ptr<ASTNodeBase_relExp> relExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 87's productions.
// ============================================================

//! \brief Parent type for non-terminal type 87
struct ASTNodeBase_relExp : ASTNodeBase {
  ASTNodeBase_relExp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::relExp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_64_relExp : ASTNodeBase_relExp {
  explicit ASTNodeGeneral_64_relExp(const ItemID& item_id)
    : ASTNodeBase_relExp(ASTNodeType::Type_ASTNodeGeneral_64_relExp, item_id) {}

  std::shared_ptr<ASTNodeBase_minmaxExp> minmaxExp_node_0{};
  std::shared_ptr<ASTNodeBase_relop> relop_node{};
  std::shared_ptr<ASTNodeBase_minmaxExp> minmaxExp_node_1{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_65_relExp : ASTNodeBase_relExp {
  explicit ASTNodeGeneral_65_relExp(const ItemID& item_id)
    : ASTNodeBase_relExp(ASTNodeType::Type_ASTNodeGeneral_65_relExp, item_id) {}

  std::shared_ptr<ASTNodeBase_minmaxExp> minmaxExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 88's productions.
// ============================================================

//! \brief Parent type for non-terminal type 88
struct ASTNodeBase_minmaxExp : ASTNodeBase {
  ASTNodeBase_minmaxExp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::minmaxExp, item_id) {}

  std::shared_ptr<ASTNodeBase_sumExp> sumExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_72_minmaxExp : ASTNodeBase_minmaxExp {
  explicit ASTNodeGeneral_72_minmaxExp(const ItemID& item_id)
    : ASTNodeBase_minmaxExp(ASTNodeType::Type_ASTNodeGeneral_72_minmaxExp, item_id) {}

  std::shared_ptr<ASTNodeBase_minmaxExp> minmaxExp_node{};
  std::shared_ptr<ASTNodeBase_minmaxop> minmaxop_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_73_minmaxExp : ASTNodeBase_minmaxExp {
  explicit ASTNodeGeneral_73_minmaxExp(const ItemID& item_id)
    : ASTNodeBase_minmaxExp(ASTNodeType::Type_ASTNodeGeneral_73_minmaxExp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 89's productions.
// ============================================================

//! \brief Parent type for non-terminal type 89
struct ASTNodeBase_relop : ASTNodeBase {
  ASTNodeBase_relop(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::relop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_66_relop : ASTNodeBase_relop {
  explicit ASTNodeGeneral_66_relop(const ItemID& item_id)
    : ASTNodeBase_relop(ASTNodeType::Type_ASTNodeGeneral_66_relop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_67_relop : ASTNodeBase_relop {
  explicit ASTNodeGeneral_67_relop(const ItemID& item_id)
    : ASTNodeBase_relop(ASTNodeType::Type_ASTNodeGeneral_67_relop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_68_relop : ASTNodeBase_relop {
  explicit ASTNodeGeneral_68_relop(const ItemID& item_id)
    : ASTNodeBase_relop(ASTNodeType::Type_ASTNodeGeneral_68_relop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_69_relop : ASTNodeBase_relop {
  explicit ASTNodeGeneral_69_relop(const ItemID& item_id)
    : ASTNodeBase_relop(ASTNodeType::Type_ASTNodeGeneral_69_relop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_70_relop : ASTNodeBase_relop {
  explicit ASTNodeGeneral_70_relop(const ItemID& item_id)
    : ASTNodeBase_relop(ASTNodeType::Type_ASTNodeGeneral_70_relop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_71_relop : ASTNodeBase_relop {
  explicit ASTNodeGeneral_71_relop(const ItemID& item_id)
    : ASTNodeBase_relop(ASTNodeType::Type_ASTNodeGeneral_71_relop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 90's productions.
// ============================================================

//! \brief Parent type for non-terminal type 90
struct ASTNodeBase_minmaxop : ASTNodeBase {
  ASTNodeBase_minmaxop(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::minmaxop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_74_minmaxop : ASTNodeBase_minmaxop {
  explicit ASTNodeGeneral_74_minmaxop(const ItemID& item_id)
    : ASTNodeBase_minmaxop(ASTNodeType::Type_ASTNodeGeneral_74_minmaxop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_75_minmaxop : ASTNodeBase_minmaxop {
  explicit ASTNodeGeneral_75_minmaxop(const ItemID& item_id)
    : ASTNodeBase_minmaxop(ASTNodeType::Type_ASTNodeGeneral_75_minmaxop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 91's productions.
// ============================================================

//! \brief Parent type for non-terminal type 91
struct ASTNodeBase_sumExp : ASTNodeBase {
  ASTNodeBase_sumExp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::sumExp, item_id) {}

  std::shared_ptr<ASTNodeBase_mulExp> mulExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_76_sumExp : ASTNodeBase_sumExp {
  explicit ASTNodeGeneral_76_sumExp(const ItemID& item_id)
    : ASTNodeBase_sumExp(ASTNodeType::Type_ASTNodeGeneral_76_sumExp, item_id) {}

  std::shared_ptr<ASTNodeBase_sumExp> sumExp_node{};
  std::shared_ptr<ASTNodeBase_sumop> sumop_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_77_sumExp : ASTNodeBase_sumExp {
  explicit ASTNodeGeneral_77_sumExp(const ItemID& item_id)
    : ASTNodeBase_sumExp(ASTNodeType::Type_ASTNodeGeneral_77_sumExp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 92's productions.
// ============================================================

//! \brief Parent type for non-terminal type 92
struct ASTNodeBase_sumop : ASTNodeBase {
  ASTNodeBase_sumop(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::sumop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_78_sumop : ASTNodeBase_sumop {
  explicit ASTNodeGeneral_78_sumop(const ItemID& item_id)
    : ASTNodeBase_sumop(ASTNodeType::Type_ASTNodeGeneral_78_sumop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_79_sumop : ASTNodeBase_sumop {
  explicit ASTNodeGeneral_79_sumop(const ItemID& item_id)
    : ASTNodeBase_sumop(ASTNodeType::Type_ASTNodeGeneral_79_sumop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 93's productions.
// ============================================================

//! \brief Parent type for non-terminal type 93
struct ASTNodeBase_mulExp : ASTNodeBase {
  ASTNodeBase_mulExp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::mulExp, item_id) {}

  std::shared_ptr<ASTNodeBase_unaryExp> unaryExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_80_mulExp : ASTNodeBase_mulExp {
  explicit ASTNodeGeneral_80_mulExp(const ItemID& item_id)
    : ASTNodeBase_mulExp(ASTNodeType::Type_ASTNodeGeneral_80_mulExp, item_id) {}

  std::shared_ptr<ASTNodeBase_mulExp> mulExp_node{};
  std::shared_ptr<ASTNodeBase_mulop> mulop_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_81_mulExp : ASTNodeBase_mulExp {
  explicit ASTNodeGeneral_81_mulExp(const ItemID& item_id)
    : ASTNodeBase_mulExp(ASTNodeType::Type_ASTNodeGeneral_81_mulExp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 94's productions.
// ============================================================

//! \brief Parent type for non-terminal type 94
struct ASTNodeBase_mulop : ASTNodeBase {
  ASTNodeBase_mulop(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::mulop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_82_mulop : ASTNodeBase_mulop {
  explicit ASTNodeGeneral_82_mulop(const ItemID& item_id)
    : ASTNodeBase_mulop(ASTNodeType::Type_ASTNodeGeneral_82_mulop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_83_mulop : ASTNodeBase_mulop {
  explicit ASTNodeGeneral_83_mulop(const ItemID& item_id)
    : ASTNodeBase_mulop(ASTNodeType::Type_ASTNodeGeneral_83_mulop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_84_mulop : ASTNodeBase_mulop {
  explicit ASTNodeGeneral_84_mulop(const ItemID& item_id)
    : ASTNodeBase_mulop(ASTNodeType::Type_ASTNodeGeneral_84_mulop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 95's productions.
// ============================================================

//! \brief Parent type for non-terminal type 95
struct ASTNodeBase_unaryExp : ASTNodeBase {
  ASTNodeBase_unaryExp(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::unaryExp, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_85_unaryExp : ASTNodeBase_unaryExp {
  explicit ASTNodeGeneral_85_unaryExp(const ItemID& item_id)
    : ASTNodeBase_unaryExp(ASTNodeType::Type_ASTNodeGeneral_85_unaryExp, item_id) {}

  std::shared_ptr<ASTNodeBase_unaryop> unaryop_node{};
  std::shared_ptr<ASTNodeBase_unaryExp> unaryExp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_86_unaryExp : ASTNodeBase_unaryExp {
  explicit ASTNodeGeneral_86_unaryExp(const ItemID& item_id)
    : ASTNodeBase_unaryExp(ASTNodeType::Type_ASTNodeGeneral_86_unaryExp, item_id) {}

  std::shared_ptr<ASTNodeBase_factor> factor_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 96's productions.
// ============================================================

//! \brief Parent type for non-terminal type 96
struct ASTNodeBase_unaryop : ASTNodeBase {
  ASTNodeBase_unaryop(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::unaryop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_87_unaryop : ASTNodeBase_unaryop {
  explicit ASTNodeGeneral_87_unaryop(const ItemID& item_id)
    : ASTNodeBase_unaryop(ASTNodeType::Type_ASTNodeGeneral_87_unaryop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_88_unaryop : ASTNodeBase_unaryop {
  explicit ASTNodeGeneral_88_unaryop(const ItemID& item_id)
    : ASTNodeBase_unaryop(ASTNodeType::Type_ASTNodeGeneral_88_unaryop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_89_unaryop : ASTNodeBase_unaryop {
  explicit ASTNodeGeneral_89_unaryop(const ItemID& item_id)
    : ASTNodeBase_unaryop(ASTNodeType::Type_ASTNodeGeneral_89_unaryop, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 97's productions.
// ============================================================

//! \brief Parent type for non-terminal type 97
struct ASTNodeBase_factor : ASTNodeBase {
  ASTNodeBase_factor(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::factor, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_90_factor : ASTNodeBase_factor {
  explicit ASTNodeGeneral_90_factor(const ItemID& item_id)
    : ASTNodeBase_factor(ASTNodeType::Type_ASTNodeGeneral_90_factor, item_id) {}

  std::shared_ptr<ASTNodeBase_immutable> immutable_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_91_factor : ASTNodeBase_factor {
  explicit ASTNodeGeneral_91_factor(const ItemID& item_id)
    : ASTNodeBase_factor(ASTNodeType::Type_ASTNodeGeneral_91_factor, item_id) {}

  std::shared_ptr<ASTNodeBase_mutable> mutable__node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 98's productions.
// ============================================================

//! \brief Parent type for non-terminal type 98
struct ASTNodeBase_immutable : ASTNodeBase {
  ASTNodeBase_immutable(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::immutable, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_94_immutable : ASTNodeBase_immutable {
  explicit ASTNodeGeneral_94_immutable(const ItemID& item_id)
    : ASTNodeBase_immutable(ASTNodeType::Type_ASTNodeGeneral_94_immutable, item_id) {}

  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_95_immutable : ASTNodeBase_immutable {
  explicit ASTNodeGeneral_95_immutable(const ItemID& item_id)
    : ASTNodeBase_immutable(ASTNodeType::Type_ASTNodeGeneral_95_immutable, item_id) {}

  std::shared_ptr<ASTNodeGeneral_97_call> call_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_96_immutable : ASTNodeBase_immutable {
  explicit ASTNodeGeneral_96_immutable(const ItemID& item_id)
    : ASTNodeBase_immutable(ASTNodeType::Type_ASTNodeGeneral_96_immutable, item_id) {}

  std::shared_ptr<ASTNodeBase_constant> constant_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 99's productions.
// ============================================================

struct ASTNodeGeneral_97_call : ASTNodeBase {
  explicit ASTNodeGeneral_97_call(const ItemID& item_id)
    : ASTNodeBase(ASTNodeType::Type_ASTNodeGeneral_97_call, NonterminalType::call, item_id) {}

  std::string ID{};
  std::shared_ptr<ASTNodeBase_args> args_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 100's productions.
// ============================================================

//! \brief Parent type for non-terminal type 100
struct ASTNodeBase_constant : ASTNodeBase {
  ASTNodeBase_constant(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::constant, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_102_constant : ASTNodeBase_constant {
  explicit ASTNodeGeneral_102_constant(const ItemID& item_id)
    : ASTNodeBase_constant(ASTNodeType::Type_ASTNodeGeneral_102_constant, item_id) {}

  std::string NUMCONST{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_103_constant : ASTNodeBase_constant {
  explicit ASTNodeGeneral_103_constant(const ItemID& item_id)
    : ASTNodeBase_constant(ASTNodeType::Type_ASTNodeGeneral_103_constant, item_id) {}

  std::string CHARCONST{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_104_constant : ASTNodeBase_constant {
  explicit ASTNodeGeneral_104_constant(const ItemID& item_id)
    : ASTNodeBase_constant(ASTNodeType::Type_ASTNodeGeneral_104_constant, item_id) {}

  std::string STRINGCONST{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_105_constant : ASTNodeBase_constant {
  explicit ASTNodeGeneral_105_constant(const ItemID& item_id)
    : ASTNodeBase_constant(ASTNodeType::Type_ASTNodeGeneral_105_constant, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_106_constant : ASTNodeBase_constant {
  explicit ASTNodeGeneral_106_constant(const ItemID& item_id)
    : ASTNodeBase_constant(ASTNodeType::Type_ASTNodeGeneral_106_constant, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 101's productions.
// ============================================================

//! \brief Parent type for non-terminal type 101
struct ASTNodeBase_args : ASTNodeBase {
  ASTNodeBase_args(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::args, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_98_args : ASTNodeBase_args {
  explicit ASTNodeGeneral_98_args(const ItemID& item_id)
    : ASTNodeBase_args(ASTNodeType::Type_ASTNodeGeneral_98_args, item_id) {}

  std::shared_ptr<ASTNodeBase_argList> argList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_99_args : ASTNodeBase_args {
  explicit ASTNodeGeneral_99_args(const ItemID& item_id)
    : ASTNodeBase_args(ASTNodeType::Type_ASTNodeGeneral_99_args, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ============================================================
//  Nodes for non-terminal 102's productions.
// ============================================================

//! \brief Parent type for non-terminal type 102
struct ASTNodeBase_argList : ASTNodeBase {
  ASTNodeBase_argList(const ASTNodeType& node_type, const ItemID& item_id)
    : ASTNodeBase(node_type, NonterminalType::argList, item_id) {}

  std::shared_ptr<ASTNodeBase_exp> exp_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_100_argList : ASTNodeBase_argList {
  explicit ASTNodeGeneral_100_argList(const ItemID& item_id)
    : ASTNodeBase_argList(ASTNodeType::Type_ASTNodeGeneral_100_argList, item_id) {}

  std::shared_ptr<ASTNodeBase_argList> argList_node{};

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

struct ASTNodeGeneral_101_argList : ASTNodeBase_argList {
  explicit ASTNodeGeneral_101_argList(const ItemID& item_id)
    : ASTNodeBase_argList(ASTNodeType::Type_ASTNodeGeneral_101_argList, item_id) {}

  void Accept(ASTNodeVisitor& visitor) override { visitor.Visit(*this); }
};

// ========================================================================
//  LALR Parser.
// ========================================================================

class Parser : public manta::ParserDriverBase<ASTNodeBase, ASTLexeme,Parser> {
  friend class manta::ParserDriverBase<ASTNodeBase, ASTLexeme, Parser>;
public:
  //! \brief Constructor, initializes the parser.
  Parser();

  //! \brief Function to parse the input.
  std::shared_ptr<ASTNodeBase> ParseInput();

protected:
  //! \brief Function that sets up the lexer.
  void createLexer();

  //! \brief The reduce function, which allows this parser to call the reduction functions.
  std::shared_ptr<ASTNodeBase> reduce(unsigned reduction_id, const std::vector<std::shared_ptr<ASTNodeBase>>& collected_nodes);

  std::shared_ptr<ASTNodeGeneral_0_program>
  ReduceTo_ASTNodeGeneral_0_program_ViaItem_0(
      const std::shared_ptr<ASTNodeBase_declList>& argument_0);

  std::shared_ptr<ASTNodeGeneral_1_declList>
  ReduceTo_ASTNodeGeneral_1_declList_ViaItem_1(
      const std::shared_ptr<ASTNodeBase_declList>& argument_0,
      const std::shared_ptr<ASTNodeBase_decl>& argument_1);

  std::shared_ptr<ASTNodeGeneral_2_declList>
  ReduceTo_ASTNodeGeneral_2_declList_ViaItem_2(
      const std::shared_ptr<ASTNodeBase_decl>& argument_0);

  std::shared_ptr<ASTNodeGeneral_3_decl>
  ReduceTo_ASTNodeGeneral_3_decl_ViaItem_3(
      const std::shared_ptr<ASTNodeGeneral_5_varDecl>& argument_0);

  std::shared_ptr<ASTNodeGeneral_4_decl>
  ReduceTo_ASTNodeGeneral_4_decl_ViaItem_4(
      const std::shared_ptr<ASTNodeBase_funDecl>& argument_0);

  std::shared_ptr<ASTNodeGeneral_5_varDecl>
  ReduceTo_ASTNodeGeneral_5_varDecl_ViaItem_5(
      const std::shared_ptr<ASTNodeBase_typeSpec>& argument_0,
      const std::shared_ptr<ASTNodeBase_varDeclList>& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNodeGeneral_6_scopedVarDecl>
  ReduceTo_ASTNodeGeneral_6_scopedVarDecl_ViaItem_6(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_typeSpec>& argument_1,
      const std::shared_ptr<ASTNodeBase_varDeclList>& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNodeGeneral_7_scopedVarDecl>
  ReduceTo_ASTNodeGeneral_7_scopedVarDecl_ViaItem_7(
      const std::shared_ptr<ASTNodeBase_typeSpec>& argument_0,
      const std::shared_ptr<ASTNodeBase_varDeclList>& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNodeGeneral_8_varDeclList>
  ReduceTo_ASTNodeGeneral_8_varDeclList_ViaItem_8(
      const std::shared_ptr<ASTNodeBase_varDeclList>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_varDeclInit>& argument_2);

  std::shared_ptr<ASTNodeGeneral_9_varDeclList>
  ReduceTo_ASTNodeGeneral_9_varDeclList_ViaItem_9(
      const std::shared_ptr<ASTNodeBase_varDeclInit>& argument_0);

  std::shared_ptr<ASTNodeGeneral_10_varDeclInit>
  ReduceTo_ASTNodeGeneral_10_varDeclInit_ViaItem_10(
      const std::shared_ptr<ASTNodeBase_varDeclId>& argument_0);

  std::shared_ptr<ASTNodeGeneral_11_varDeclInit>
  ReduceTo_ASTNodeGeneral_11_varDeclInit_ViaItem_11(
      const std::shared_ptr<ASTNodeBase_varDeclId>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_12_varDeclId>
  ReduceTo_ASTNodeGeneral_12_varDeclId_ViaItem_12(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_13_varDeclId>
  ReduceTo_ASTNodeGeneral_13_varDeclId_ViaItem_13(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNodeGeneral_14_typeSpec>
  ReduceTo_ASTNodeGeneral_14_typeSpec_ViaItem_14(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_15_typeSpec>
  ReduceTo_ASTNodeGeneral_15_typeSpec_ViaItem_15(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_16_typeSpec>
  ReduceTo_ASTNodeGeneral_16_typeSpec_ViaItem_16(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_17_funDecl>
  ReduceTo_ASTNodeGeneral_17_funDecl_ViaItem_17(
      const std::shared_ptr<ASTNodeBase_typeSpec>& argument_0,
      const std::string& argument_1,
      const std::string& argument_2,
      const std::shared_ptr<ASTNodeBase_parms>& argument_3,
      const std::string& argument_4,
      const std::shared_ptr<ASTNodeBase_stmt>& argument_5);

  std::shared_ptr<ASTNodeGeneral_18_funDecl>
  ReduceTo_ASTNodeGeneral_18_funDecl_ViaItem_18(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_parms>& argument_2,
      const std::string& argument_3,
      const std::shared_ptr<ASTNodeBase_stmt>& argument_4);

  std::shared_ptr<ASTNodeGeneral_19_parms>
  ReduceTo_ASTNodeGeneral_19_parms_ViaItem_19(
      const std::shared_ptr<ASTNodeBase_parmList>& argument_0);

  std::shared_ptr<ASTNodeGeneral_20_parms>
  ReduceTo_ASTNodeGeneral_20_parms_ViaItem_20();

  std::shared_ptr<ASTNodeGeneral_21_parmList>
  ReduceTo_ASTNodeGeneral_21_parmList_ViaItem_21(
      const std::shared_ptr<ASTNodeBase_parmList>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeGeneral_23_parmTypeList>& argument_2);

  std::shared_ptr<ASTNodeGeneral_22_parmList>
  ReduceTo_ASTNodeGeneral_22_parmList_ViaItem_22(
      const std::shared_ptr<ASTNodeGeneral_23_parmTypeList>& argument_0);

  std::shared_ptr<ASTNodeGeneral_23_parmTypeList>
  ReduceTo_ASTNodeGeneral_23_parmTypeList_ViaItem_23(
      const std::shared_ptr<ASTNodeBase_typeSpec>& argument_0,
      const std::shared_ptr<ASTNodeBase_parmIdList>& argument_1);

  std::shared_ptr<ASTNodeGeneral_24_parmIdList>
  ReduceTo_ASTNodeGeneral_24_parmIdList_ViaItem_24(
      const std::shared_ptr<ASTNodeBase_parmIdList>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_parmId>& argument_2);

  std::shared_ptr<ASTNodeGeneral_25_parmIdList>
  ReduceTo_ASTNodeGeneral_25_parmIdList_ViaItem_25(
      const std::shared_ptr<ASTNodeBase_parmId>& argument_0);

  std::shared_ptr<ASTNodeGeneral_26_parmId>
  ReduceTo_ASTNodeGeneral_26_parmId_ViaItem_26(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_27_parmId>
  ReduceTo_ASTNodeGeneral_27_parmId_ViaItem_27(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNodeGeneral_28_stmt>
  ReduceTo_ASTNodeGeneral_28_stmt_ViaItem_28(
      const std::shared_ptr<ASTNodeBase_expStmt>& argument_0);

  std::shared_ptr<ASTNodeGeneral_29_stmt>
  ReduceTo_ASTNodeGeneral_29_stmt_ViaItem_29(
      const std::shared_ptr<ASTNodeGeneral_36_compoundStmt>& argument_0);

  std::shared_ptr<ASTNodeGeneral_30_stmt>
  ReduceTo_ASTNodeGeneral_30_stmt_ViaItem_30(
      const std::shared_ptr<ASTNodeBase_selectStmt>& argument_0);

  std::shared_ptr<ASTNodeGeneral_31_stmt>
  ReduceTo_ASTNodeGeneral_31_stmt_ViaItem_31(
      const std::shared_ptr<ASTNodeBase_iterStmt>& argument_0);

  std::shared_ptr<ASTNodeGeneral_32_stmt>
  ReduceTo_ASTNodeGeneral_32_stmt_ViaItem_32(
      const std::shared_ptr<ASTNodeBase_returnStmt>& argument_0);

  std::shared_ptr<ASTNodeGeneral_33_stmt>
  ReduceTo_ASTNodeGeneral_33_stmt_ViaItem_33(
      const std::shared_ptr<ASTNodeGeneral_49_breakStmt>& argument_0);

  std::shared_ptr<ASTNodeGeneral_34_expStmt>
  ReduceTo_ASTNodeGeneral_34_expStmt_ViaItem_34(
      const std::shared_ptr<ASTNodeBase_exp>& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNodeGeneral_35_expStmt>
  ReduceTo_ASTNodeGeneral_35_expStmt_ViaItem_35(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_36_compoundStmt>
  ReduceTo_ASTNodeGeneral_36_compoundStmt_ViaItem_36(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_localDecls>& argument_1,
      const std::shared_ptr<ASTNodeBase_stmtList>& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNodeGeneral_37_localDecls>
  ReduceTo_ASTNodeGeneral_37_localDecls_ViaItem_37(
      const std::shared_ptr<ASTNodeBase_localDecls>& argument_0,
      const std::shared_ptr<ASTNodeBase_scopedVarDecl>& argument_1);

  std::shared_ptr<ASTNodeGeneral_38_localDecls>
  ReduceTo_ASTNodeGeneral_38_localDecls_ViaItem_38();

  std::shared_ptr<ASTNodeGeneral_39_stmtList>
  ReduceTo_ASTNodeGeneral_39_stmtList_ViaItem_39(
      const std::shared_ptr<ASTNodeBase_stmtList>& argument_0,
      const std::shared_ptr<ASTNodeBase_stmt>& argument_1);

  std::shared_ptr<ASTNodeGeneral_40_stmtList>
  ReduceTo_ASTNodeGeneral_40_stmtList_ViaItem_40();

  std::shared_ptr<ASTNodeGeneral_41_selectStmt>
  ReduceTo_ASTNodeGeneral_41_selectStmt_ViaItem_41(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_1,
      const std::string& argument_2,
      const std::shared_ptr<ASTNodeBase_stmt>& argument_3);

  std::shared_ptr<ASTNodeGeneral_42_selectStmt>
  ReduceTo_ASTNodeGeneral_42_selectStmt_ViaItem_42(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_1,
      const std::string& argument_2,
      const std::shared_ptr<ASTNodeBase_stmt>& argument_3,
      const std::string& argument_4,
      const std::shared_ptr<ASTNodeBase_stmt>& argument_5);

  std::shared_ptr<ASTNodeGeneral_43_iterStmt>
  ReduceTo_ASTNodeGeneral_43_iterStmt_ViaItem_43(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_1,
      const std::string& argument_2,
      const std::shared_ptr<ASTNodeBase_stmt>& argument_3);

  std::shared_ptr<ASTNodeGeneral_44_iterStmt>
  ReduceTo_ASTNodeGeneral_44_iterStmt_ViaItem_44(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::string& argument_2,
      const std::shared_ptr<ASTNodeBase_iterRange>& argument_3,
      const std::string& argument_4,
      const std::shared_ptr<ASTNodeBase_stmt>& argument_5);

  std::shared_ptr<ASTNodeGeneral_45_iterRange>
  ReduceTo_ASTNodeGeneral_45_iterRange_ViaItem_45(
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_46_iterRange>
  ReduceTo_ASTNodeGeneral_46_iterRange_ViaItem_46(
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_2,
      const std::string& argument_3,
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_4);

  std::shared_ptr<ASTNodeGeneral_47_returnStmt>
  ReduceTo_ASTNodeGeneral_47_returnStmt_ViaItem_47(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNodeGeneral_48_returnStmt>
  ReduceTo_ASTNodeGeneral_48_returnStmt_ViaItem_48(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_exp>& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNodeGeneral_49_breakStmt>
  ReduceTo_ASTNodeGeneral_49_breakStmt_ViaItem_49(
      const std::string& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNodeGeneral_50_exp>
  ReduceTo_ASTNodeGeneral_50_exp_ViaItem_50(
      const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_exp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_51_exp>
  ReduceTo_ASTNodeGeneral_51_exp_ViaItem_51(
      const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_exp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_52_exp>
  ReduceTo_ASTNodeGeneral_52_exp_ViaItem_52(
      const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_exp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_53_exp>
  ReduceTo_ASTNodeGeneral_53_exp_ViaItem_53(
      const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_exp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_54_exp>
  ReduceTo_ASTNodeGeneral_54_exp_ViaItem_54(
      const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_exp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_55_exp>
  ReduceTo_ASTNodeGeneral_55_exp_ViaItem_55(
      const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNodeGeneral_56_exp>
  ReduceTo_ASTNodeGeneral_56_exp_ViaItem_56(
      const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
      const std::string& argument_1);

  std::shared_ptr<ASTNodeGeneral_57_exp>
  ReduceTo_ASTNodeGeneral_57_exp_ViaItem_57(
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_58_simpleExp>
  ReduceTo_ASTNodeGeneral_58_simpleExp_ViaItem_58(
      const std::shared_ptr<ASTNodeBase_simpleExp>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_andExp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_59_simpleExp>
  ReduceTo_ASTNodeGeneral_59_simpleExp_ViaItem_59(
      const std::shared_ptr<ASTNodeBase_andExp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_60_andExp>
  ReduceTo_ASTNodeGeneral_60_andExp_ViaItem_60(
      const std::shared_ptr<ASTNodeBase_andExp>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_unaryRelExp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_61_andExp>
  ReduceTo_ASTNodeGeneral_61_andExp_ViaItem_61(
      const std::shared_ptr<ASTNodeBase_unaryRelExp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_62_unaryRelExp>
  ReduceTo_ASTNodeGeneral_62_unaryRelExp_ViaItem_62(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_unaryRelExp>& argument_1);

  std::shared_ptr<ASTNodeGeneral_63_unaryRelExp>
  ReduceTo_ASTNodeGeneral_63_unaryRelExp_ViaItem_63(
      const std::shared_ptr<ASTNodeBase_relExp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_64_relExp>
  ReduceTo_ASTNodeGeneral_64_relExp_ViaItem_64(
      const std::shared_ptr<ASTNodeBase_minmaxExp>& argument_0,
      const std::shared_ptr<ASTNodeBase_relop>& argument_1,
      const std::shared_ptr<ASTNodeBase_minmaxExp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_65_relExp>
  ReduceTo_ASTNodeGeneral_65_relExp_ViaItem_65(
      const std::shared_ptr<ASTNodeBase_minmaxExp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_66_relop>
  ReduceTo_ASTNodeGeneral_66_relop_ViaItem_66(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_67_relop>
  ReduceTo_ASTNodeGeneral_67_relop_ViaItem_67(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_68_relop>
  ReduceTo_ASTNodeGeneral_68_relop_ViaItem_68(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_69_relop>
  ReduceTo_ASTNodeGeneral_69_relop_ViaItem_69(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_70_relop>
  ReduceTo_ASTNodeGeneral_70_relop_ViaItem_70(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_71_relop>
  ReduceTo_ASTNodeGeneral_71_relop_ViaItem_71(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_72_minmaxExp>
  ReduceTo_ASTNodeGeneral_72_minmaxExp_ViaItem_72(
      const std::shared_ptr<ASTNodeBase_minmaxExp>& argument_0,
      const std::shared_ptr<ASTNodeBase_minmaxop>& argument_1,
      const std::shared_ptr<ASTNodeBase_sumExp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_73_minmaxExp>
  ReduceTo_ASTNodeGeneral_73_minmaxExp_ViaItem_73(
      const std::shared_ptr<ASTNodeBase_sumExp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_74_minmaxop>
  ReduceTo_ASTNodeGeneral_74_minmaxop_ViaItem_74(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_75_minmaxop>
  ReduceTo_ASTNodeGeneral_75_minmaxop_ViaItem_75(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_76_sumExp>
  ReduceTo_ASTNodeGeneral_76_sumExp_ViaItem_76(
      const std::shared_ptr<ASTNodeBase_sumExp>& argument_0,
      const std::shared_ptr<ASTNodeBase_sumop>& argument_1,
      const std::shared_ptr<ASTNodeBase_mulExp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_77_sumExp>
  ReduceTo_ASTNodeGeneral_77_sumExp_ViaItem_77(
      const std::shared_ptr<ASTNodeBase_mulExp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_78_sumop>
  ReduceTo_ASTNodeGeneral_78_sumop_ViaItem_78(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_79_sumop>
  ReduceTo_ASTNodeGeneral_79_sumop_ViaItem_79(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_80_mulExp>
  ReduceTo_ASTNodeGeneral_80_mulExp_ViaItem_80(
      const std::shared_ptr<ASTNodeBase_mulExp>& argument_0,
      const std::shared_ptr<ASTNodeBase_mulop>& argument_1,
      const std::shared_ptr<ASTNodeBase_unaryExp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_81_mulExp>
  ReduceTo_ASTNodeGeneral_81_mulExp_ViaItem_81(
      const std::shared_ptr<ASTNodeBase_unaryExp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_82_mulop>
  ReduceTo_ASTNodeGeneral_82_mulop_ViaItem_82(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_83_mulop>
  ReduceTo_ASTNodeGeneral_83_mulop_ViaItem_83(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_84_mulop>
  ReduceTo_ASTNodeGeneral_84_mulop_ViaItem_84(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_85_unaryExp>
  ReduceTo_ASTNodeGeneral_85_unaryExp_ViaItem_85(
      const std::shared_ptr<ASTNodeBase_unaryop>& argument_0,
      const std::shared_ptr<ASTNodeBase_unaryExp>& argument_1);

  std::shared_ptr<ASTNodeGeneral_86_unaryExp>
  ReduceTo_ASTNodeGeneral_86_unaryExp_ViaItem_86(
      const std::shared_ptr<ASTNodeBase_factor>& argument_0);

  std::shared_ptr<ASTNodeGeneral_87_unaryop>
  ReduceTo_ASTNodeGeneral_87_unaryop_ViaItem_87(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_88_unaryop>
  ReduceTo_ASTNodeGeneral_88_unaryop_ViaItem_88(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_89_unaryop>
  ReduceTo_ASTNodeGeneral_89_unaryop_ViaItem_89(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_90_factor>
  ReduceTo_ASTNodeGeneral_90_factor_ViaItem_90(
      const std::shared_ptr<ASTNodeBase_immutable>& argument_0);

  std::shared_ptr<ASTNodeGeneral_91_factor>
  ReduceTo_ASTNodeGeneral_91_factor_ViaItem_91(
      const std::shared_ptr<ASTNodeBase_mutable>& argument_0);

  std::shared_ptr<ASTNodeGeneral_92_mutable>
  ReduceTo_ASTNodeGeneral_92_mutable_ViaItem_92(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_93_mutable>
  ReduceTo_ASTNodeGeneral_93_mutable_ViaItem_93(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_exp>& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNodeGeneral_94_immutable>
  ReduceTo_ASTNodeGeneral_94_immutable_ViaItem_94(
      const std::string& argument_0,
      const std::shared_ptr<ASTNodeBase_exp>& argument_1,
      const std::string& argument_2);

  std::shared_ptr<ASTNodeGeneral_95_immutable>
  ReduceTo_ASTNodeGeneral_95_immutable_ViaItem_95(
      const std::shared_ptr<ASTNodeGeneral_97_call>& argument_0);

  std::shared_ptr<ASTNodeGeneral_96_immutable>
  ReduceTo_ASTNodeGeneral_96_immutable_ViaItem_96(
      const std::shared_ptr<ASTNodeBase_constant>& argument_0);

  std::shared_ptr<ASTNodeGeneral_97_call>
  ReduceTo_ASTNodeGeneral_97_call_ViaItem_97(
      const std::string& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_args>& argument_2,
      const std::string& argument_3);

  std::shared_ptr<ASTNodeGeneral_98_args>
  ReduceTo_ASTNodeGeneral_98_args_ViaItem_98(
      const std::shared_ptr<ASTNodeBase_argList>& argument_0);

  std::shared_ptr<ASTNodeGeneral_99_args>
  ReduceTo_ASTNodeGeneral_99_args_ViaItem_99();

  std::shared_ptr<ASTNodeGeneral_100_argList>
  ReduceTo_ASTNodeGeneral_100_argList_ViaItem_100(
      const std::shared_ptr<ASTNodeBase_argList>& argument_0,
      const std::string& argument_1,
      const std::shared_ptr<ASTNodeBase_exp>& argument_2);

  std::shared_ptr<ASTNodeGeneral_101_argList>
  ReduceTo_ASTNodeGeneral_101_argList_ViaItem_101(
      const std::shared_ptr<ASTNodeBase_exp>& argument_0);

  std::shared_ptr<ASTNodeGeneral_102_constant>
  ReduceTo_ASTNodeGeneral_102_constant_ViaItem_102(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_103_constant>
  ReduceTo_ASTNodeGeneral_103_constant_ViaItem_103(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_104_constant>
  ReduceTo_ASTNodeGeneral_104_constant_ViaItem_104(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_105_constant>
  ReduceTo_ASTNodeGeneral_105_constant_ViaItem_105(
      const std::string& argument_0);

  std::shared_ptr<ASTNodeGeneral_106_constant>
  ReduceTo_ASTNodeGeneral_106_constant_ViaItem_106(
      const std::string& argument_0);

};

inline Parser::Parser() {
  using namespace manta;

  start_nonterminal_ = 57;
  // Allocate space for the parser table.
  parse_table_.assign(170, std::vector<Entry>(103,Entry()));

  // Create the table. There are better, though more difficult, ways to serialize this information.
  parse_table_[0][1] = Entry(1);  // Shift
  parse_table_[0][14] = Entry(2);  // Shift
  parse_table_[0][15] = Entry(3);  // Shift
  parse_table_[0][16] = Entry(4);  // Shift
  parse_table_[0][57] = Entry(true);  // Accept
  parse_table_[0][58] = Entry(5);  // Shift
  parse_table_[0][59] = Entry(6);  // Shift
  parse_table_[0][60] = Entry(7);  // Shift
  parse_table_[0][61] = Entry(8);  // Shift
  parse_table_[0][62] = Entry(9);  // Shift
  parse_table_[1][17] = Entry(10);  // Shift
  parse_table_[2][0] = Entry(ProductionRule(62, {14}), 14);  // Reduce
  parse_table_[2][1] = Entry(ProductionRule(62, {14}), 14);  // Reduce
  parse_table_[3][0] = Entry(ProductionRule(62, {15}), 15);  // Reduce
  parse_table_[3][1] = Entry(ProductionRule(62, {15}), 15);  // Reduce
  parse_table_[4][0] = Entry(ProductionRule(62, {16}), 16);  // Reduce
  parse_table_[4][1] = Entry(ProductionRule(62, {16}), 16);  // Reduce
  parse_table_[5][0] = Entry(ProductionRule(57, {58}), 0);  // Reduce
  parse_table_[5][1] = Entry(1);  // Shift
  parse_table_[5][14] = Entry(2);  // Shift
  parse_table_[5][15] = Entry(3);  // Shift
  parse_table_[5][16] = Entry(4);  // Shift
  parse_table_[5][59] = Entry(11);  // Shift
  parse_table_[5][60] = Entry(7);  // Shift
  parse_table_[5][61] = Entry(8);  // Shift
  parse_table_[5][62] = Entry(9);  // Shift
  parse_table_[6][0] = Entry(ProductionRule(58, {59}), 2);  // Reduce
  parse_table_[6][1] = Entry(ProductionRule(58, {59}), 2);  // Reduce
  parse_table_[6][14] = Entry(ProductionRule(58, {59}), 2);  // Reduce
  parse_table_[6][15] = Entry(ProductionRule(58, {59}), 2);  // Reduce
  parse_table_[6][16] = Entry(ProductionRule(58, {59}), 2);  // Reduce
  parse_table_[7][0] = Entry(ProductionRule(59, {60}), 3);  // Reduce
  parse_table_[7][1] = Entry(ProductionRule(59, {60}), 3);  // Reduce
  parse_table_[7][14] = Entry(ProductionRule(59, {60}), 3);  // Reduce
  parse_table_[7][15] = Entry(ProductionRule(59, {60}), 3);  // Reduce
  parse_table_[7][16] = Entry(ProductionRule(59, {60}), 3);  // Reduce
  parse_table_[8][0] = Entry(ProductionRule(59, {61}), 4);  // Reduce
  parse_table_[8][1] = Entry(ProductionRule(59, {61}), 4);  // Reduce
  parse_table_[8][14] = Entry(ProductionRule(59, {61}), 4);  // Reduce
  parse_table_[8][15] = Entry(ProductionRule(59, {61}), 4);  // Reduce
  parse_table_[8][16] = Entry(ProductionRule(59, {61}), 4);  // Reduce
  parse_table_[9][1] = Entry(12);  // Shift
  parse_table_[9][63] = Entry(13);  // Shift
  parse_table_[9][65] = Entry(14);  // Shift
  parse_table_[9][66] = Entry(15);  // Shift
  parse_table_[10][14] = Entry(2);  // Shift
  parse_table_[10][15] = Entry(3);  // Shift
  parse_table_[10][16] = Entry(4);  // Shift
  parse_table_[10][18] = Entry(ProductionRule(68, {}), 20);  // Reduce
  parse_table_[10][62] = Entry(16);  // Shift
  parse_table_[10][68] = Entry(17);  // Shift
  parse_table_[10][70] = Entry(18);  // Shift
  parse_table_[10][71] = Entry(19);  // Shift
  parse_table_[11][0] = Entry(ProductionRule(58, {58, 59}), 1);  // Reduce
  parse_table_[11][1] = Entry(ProductionRule(58, {58, 59}), 1);  // Reduce
  parse_table_[11][14] = Entry(ProductionRule(58, {58, 59}), 1);  // Reduce
  parse_table_[11][15] = Entry(ProductionRule(58, {58, 59}), 1);  // Reduce
  parse_table_[11][16] = Entry(ProductionRule(58, {58, 59}), 1);  // Reduce
  parse_table_[12][8] = Entry(ProductionRule(66, {1}), 12);  // Reduce
  parse_table_[12][10] = Entry(ProductionRule(66, {1}), 12);  // Reduce
  parse_table_[12][11] = Entry(ProductionRule(66, {1}), 12);  // Reduce
  parse_table_[12][12] = Entry(20);  // Shift
  parse_table_[12][17] = Entry(21);  // Shift
  parse_table_[13][8] = Entry(22);  // Shift
  parse_table_[13][10] = Entry(23);  // Shift
  parse_table_[14][8] = Entry(ProductionRule(63, {65}), 9);  // Reduce
  parse_table_[14][10] = Entry(ProductionRule(63, {65}), 9);  // Reduce
  parse_table_[15][8] = Entry(ProductionRule(65, {66}), 10);  // Reduce
  parse_table_[15][10] = Entry(ProductionRule(65, {66}), 10);  // Reduce
  parse_table_[15][11] = Entry(24);  // Shift
  parse_table_[16][1] = Entry(25);  // Shift
  parse_table_[16][72] = Entry(26);  // Shift
  parse_table_[16][73] = Entry(27);  // Shift
  parse_table_[17][18] = Entry(28);  // Shift
  parse_table_[18][8] = Entry(29);  // Shift
  parse_table_[18][18] = Entry(ProductionRule(68, {70}), 19);  // Reduce
  parse_table_[19][8] = Entry(ProductionRule(70, {71}), 22);  // Reduce
  parse_table_[19][18] = Entry(ProductionRule(70, {71}), 22);  // Reduce
  parse_table_[20][2] = Entry(30);  // Shift
  parse_table_[21][14] = Entry(2);  // Shift
  parse_table_[21][15] = Entry(3);  // Shift
  parse_table_[21][16] = Entry(4);  // Shift
  parse_table_[21][18] = Entry(ProductionRule(68, {}), 20);  // Reduce
  parse_table_[21][62] = Entry(16);  // Shift
  parse_table_[21][68] = Entry(31);  // Shift
  parse_table_[21][70] = Entry(18);  // Shift
  parse_table_[21][71] = Entry(19);  // Shift
  parse_table_[22][0] = Entry(ProductionRule(60, {62, 63, 8}), 5);  // Reduce
  parse_table_[22][1] = Entry(ProductionRule(60, {62, 63, 8}), 5);  // Reduce
  parse_table_[22][14] = Entry(ProductionRule(60, {62, 63, 8}), 5);  // Reduce
  parse_table_[22][15] = Entry(ProductionRule(60, {62, 63, 8}), 5);  // Reduce
  parse_table_[22][16] = Entry(ProductionRule(60, {62, 63, 8}), 5);  // Reduce
  parse_table_[23][1] = Entry(32);  // Shift
  parse_table_[23][65] = Entry(33);  // Shift
  parse_table_[23][66] = Entry(15);  // Shift
  parse_table_[24][1] = Entry(34);  // Shift
  parse_table_[24][2] = Entry(35);  // Shift
  parse_table_[24][3] = Entry(36);  // Shift
  parse_table_[24][4] = Entry(37);  // Shift
  parse_table_[24][17] = Entry(38);  // Shift
  parse_table_[24][40] = Entry(39);  // Shift
  parse_table_[24][50] = Entry(40);  // Shift
  parse_table_[24][51] = Entry(41);  // Shift
  parse_table_[24][54] = Entry(42);  // Shift
  parse_table_[24][55] = Entry(43);  // Shift
  parse_table_[24][56] = Entry(44);  // Shift
  parse_table_[24][67] = Entry(45);  // Shift
  parse_table_[24][84] = Entry(46);  // Shift
  parse_table_[24][85] = Entry(47);  // Shift
  parse_table_[24][86] = Entry(48);  // Shift
  parse_table_[24][87] = Entry(49);  // Shift
  parse_table_[24][88] = Entry(50);  // Shift
  parse_table_[24][91] = Entry(51);  // Shift
  parse_table_[24][93] = Entry(52);  // Shift
  parse_table_[24][95] = Entry(53);  // Shift
  parse_table_[24][96] = Entry(54);  // Shift
  parse_table_[24][97] = Entry(55);  // Shift
  parse_table_[24][98] = Entry(56);  // Shift
  parse_table_[24][99] = Entry(57);  // Shift
  parse_table_[24][100] = Entry(58);  // Shift
  parse_table_[25][8] = Entry(ProductionRule(73, {1}), 26);  // Reduce
  parse_table_[25][10] = Entry(ProductionRule(73, {1}), 26);  // Reduce
  parse_table_[25][12] = Entry(59);  // Shift
  parse_table_[25][18] = Entry(ProductionRule(73, {1}), 26);  // Reduce
  parse_table_[26][8] = Entry(ProductionRule(71, {62, 72}), 23);  // Reduce
  parse_table_[26][10] = Entry(60);  // Shift
  parse_table_[26][18] = Entry(ProductionRule(71, {62, 72}), 23);  // Reduce
  parse_table_[27][8] = Entry(ProductionRule(72, {73}), 25);  // Reduce
  parse_table_[27][10] = Entry(ProductionRule(72, {73}), 25);  // Reduce
  parse_table_[27][18] = Entry(ProductionRule(72, {73}), 25);  // Reduce
  parse_table_[28][1] = Entry(34);  // Shift
  parse_table_[28][2] = Entry(35);  // Shift
  parse_table_[28][3] = Entry(36);  // Shift
  parse_table_[28][4] = Entry(37);  // Shift
  parse_table_[28][8] = Entry(61);  // Shift
  parse_table_[28][17] = Entry(38);  // Shift
  parse_table_[28][19] = Entry(62);  // Shift
  parse_table_[28][21] = Entry(63);  // Shift
  parse_table_[28][24] = Entry(64);  // Shift
  parse_table_[28][26] = Entry(65);  // Shift
  parse_table_[28][30] = Entry(66);  // Shift
  parse_table_[28][31] = Entry(67);  // Shift
  parse_table_[28][40] = Entry(39);  // Shift
  parse_table_[28][50] = Entry(40);  // Shift
  parse_table_[28][51] = Entry(41);  // Shift
  parse_table_[28][54] = Entry(42);  // Shift
  parse_table_[28][55] = Entry(43);  // Shift
  parse_table_[28][56] = Entry(44);  // Shift
  parse_table_[28][67] = Entry(68);  // Shift
  parse_table_[28][69] = Entry(69);  // Shift
  parse_table_[28][74] = Entry(70);  // Shift
  parse_table_[28][75] = Entry(71);  // Shift
  parse_table_[28][76] = Entry(72);  // Shift
  parse_table_[28][77] = Entry(73);  // Shift
  parse_table_[28][78] = Entry(74);  // Shift
  parse_table_[28][79] = Entry(75);  // Shift
  parse_table_[28][80] = Entry(76);  // Shift
  parse_table_[28][84] = Entry(77);  // Shift
  parse_table_[28][85] = Entry(47);  // Shift
  parse_table_[28][86] = Entry(48);  // Shift
  parse_table_[28][87] = Entry(49);  // Shift
  parse_table_[28][88] = Entry(50);  // Shift
  parse_table_[28][91] = Entry(51);  // Shift
  parse_table_[28][93] = Entry(52);  // Shift
  parse_table_[28][95] = Entry(53);  // Shift
  parse_table_[28][96] = Entry(54);  // Shift
  parse_table_[28][97] = Entry(55);  // Shift
  parse_table_[28][98] = Entry(56);  // Shift
  parse_table_[28][99] = Entry(57);  // Shift
  parse_table_[28][100] = Entry(58);  // Shift
  parse_table_[29][14] = Entry(2);  // Shift
  parse_table_[29][15] = Entry(3);  // Shift
  parse_table_[29][16] = Entry(4);  // Shift
  parse_table_[29][62] = Entry(16);  // Shift
  parse_table_[29][71] = Entry(78);  // Shift
  parse_table_[30][13] = Entry(79);  // Shift
  parse_table_[31][18] = Entry(80);  // Shift
  parse_table_[32][8] = Entry(ProductionRule(66, {1}), 12);  // Reduce
  parse_table_[32][10] = Entry(ProductionRule(66, {1}), 12);  // Reduce
  parse_table_[32][11] = Entry(ProductionRule(66, {1}), 12);  // Reduce
  parse_table_[32][12] = Entry(20);  // Shift
  parse_table_[33][8] = Entry(ProductionRule(63, {63, 10, 65}), 8);  // Reduce
  parse_table_[33][10] = Entry(ProductionRule(63, {63, 10, 65}), 8);  // Reduce
  parse_table_[34][8] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][10] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][12] = Entry(81);  // Shift
  parse_table_[34][13] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][17] = Entry(82);  // Shift
  parse_table_[34][18] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][22] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][25] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][27] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][28] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][29] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][32] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][33] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][34] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][35] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][36] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][37] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][38] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][39] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][41] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][42] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][43] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][44] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][45] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][46] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][47] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][48] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][49] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][50] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][51] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][52] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[34][53] = Entry(ProductionRule(84, {1}), 92);  // Reduce
  parse_table_[35][8] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][10] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][13] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][18] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][22] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][25] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][28] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][29] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][38] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][39] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][41] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][42] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][43] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][44] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][45] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][46] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][47] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][48] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][49] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][50] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][51] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][52] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[35][53] = Entry(ProductionRule(100, {2}), 102);  // Reduce
  parse_table_[36][8] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][10] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][13] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][18] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][22] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][25] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][28] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][29] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][38] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][39] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][41] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][42] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][43] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][44] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][45] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][46] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][47] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][48] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][49] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][50] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][51] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][52] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[36][53] = Entry(ProductionRule(100, {3}), 103);  // Reduce
  parse_table_[37][8] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][10] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][13] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][18] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][22] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][25] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][28] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][29] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][38] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][39] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][41] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][42] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][43] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][44] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][45] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][46] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][47] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][48] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][49] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][50] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][51] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][52] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[37][53] = Entry(ProductionRule(100, {4}), 104);  // Reduce
  parse_table_[38][1] = Entry(34);  // Shift
  parse_table_[38][2] = Entry(35);  // Shift
  parse_table_[38][3] = Entry(36);  // Shift
  parse_table_[38][4] = Entry(37);  // Shift
  parse_table_[38][17] = Entry(38);  // Shift
  parse_table_[38][40] = Entry(39);  // Shift
  parse_table_[38][50] = Entry(40);  // Shift
  parse_table_[38][51] = Entry(41);  // Shift
  parse_table_[38][54] = Entry(42);  // Shift
  parse_table_[38][55] = Entry(43);  // Shift
  parse_table_[38][56] = Entry(44);  // Shift
  parse_table_[38][67] = Entry(68);  // Shift
  parse_table_[38][80] = Entry(83);  // Shift
  parse_table_[38][84] = Entry(77);  // Shift
  parse_table_[38][85] = Entry(47);  // Shift
  parse_table_[38][86] = Entry(48);  // Shift
  parse_table_[38][87] = Entry(49);  // Shift
  parse_table_[38][88] = Entry(50);  // Shift
  parse_table_[38][91] = Entry(51);  // Shift
  parse_table_[38][93] = Entry(52);  // Shift
  parse_table_[38][95] = Entry(53);  // Shift
  parse_table_[38][96] = Entry(54);  // Shift
  parse_table_[38][97] = Entry(55);  // Shift
  parse_table_[38][98] = Entry(56);  // Shift
  parse_table_[38][99] = Entry(57);  // Shift
  parse_table_[38][100] = Entry(58);  // Shift
  parse_table_[39][1] = Entry(34);  // Shift
  parse_table_[39][2] = Entry(35);  // Shift
  parse_table_[39][3] = Entry(36);  // Shift
  parse_table_[39][4] = Entry(37);  // Shift
  parse_table_[39][17] = Entry(38);  // Shift
  parse_table_[39][40] = Entry(39);  // Shift
  parse_table_[39][50] = Entry(40);  // Shift
  parse_table_[39][51] = Entry(41);  // Shift
  parse_table_[39][54] = Entry(42);  // Shift
  parse_table_[39][55] = Entry(43);  // Shift
  parse_table_[39][56] = Entry(44);  // Shift
  parse_table_[39][84] = Entry(46);  // Shift
  parse_table_[39][86] = Entry(84);  // Shift
  parse_table_[39][87] = Entry(49);  // Shift
  parse_table_[39][88] = Entry(50);  // Shift
  parse_table_[39][91] = Entry(51);  // Shift
  parse_table_[39][93] = Entry(52);  // Shift
  parse_table_[39][95] = Entry(53);  // Shift
  parse_table_[39][96] = Entry(54);  // Shift
  parse_table_[39][97] = Entry(55);  // Shift
  parse_table_[39][98] = Entry(56);  // Shift
  parse_table_[39][99] = Entry(57);  // Shift
  parse_table_[39][100] = Entry(58);  // Shift
  parse_table_[40][1] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][2] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][3] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][4] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][17] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][50] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][51] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][54] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][55] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[40][56] = Entry(ProductionRule(96, {50}), 87);  // Reduce
  parse_table_[41][1] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][2] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][3] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][4] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][17] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][50] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][51] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][54] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][55] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[41][56] = Entry(ProductionRule(96, {51}), 88);  // Reduce
  parse_table_[42][1] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][2] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][3] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][4] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][17] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][50] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][51] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][54] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][55] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[42][56] = Entry(ProductionRule(96, {54}), 89);  // Reduce
  parse_table_[43][8] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][10] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][13] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][18] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][22] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][25] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][28] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][29] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][38] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][39] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][41] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][42] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][43] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][44] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][45] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][46] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][47] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][48] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][49] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][50] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][51] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][52] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[43][53] = Entry(ProductionRule(100, {55}), 105);  // Reduce
  parse_table_[44][8] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][10] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][13] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][18] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][22] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][25] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][28] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][29] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][38] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][39] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][41] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][42] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][43] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][44] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][45] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][46] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][47] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][48] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][49] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][50] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][51] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][52] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[44][53] = Entry(ProductionRule(100, {56}), 106);  // Reduce
  parse_table_[45][8] = Entry(ProductionRule(65, {66, 11, 67}), 11);  // Reduce
  parse_table_[45][10] = Entry(ProductionRule(65, {66, 11, 67}), 11);  // Reduce
  parse_table_[45][38] = Entry(85);  // Shift
  parse_table_[46][8] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][10] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][13] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][18] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][22] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][25] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][28] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][29] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][38] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][39] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][41] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][42] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][43] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][44] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][45] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][46] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][47] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][48] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][49] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][50] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][51] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][52] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[46][53] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[47][8] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][10] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][13] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][18] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][22] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][25] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][28] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][29] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][38] = Entry(ProductionRule(67, {85}), 59);  // Reduce
  parse_table_[47][39] = Entry(86);  // Shift
  parse_table_[48][8] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][10] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][13] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][18] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][22] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][25] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][28] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][29] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][38] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[48][39] = Entry(ProductionRule(85, {86}), 61);  // Reduce
  parse_table_[49][8] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][10] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][13] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][18] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][22] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][25] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][28] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][29] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][38] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[49][39] = Entry(ProductionRule(86, {87}), 63);  // Reduce
  parse_table_[50][8] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][10] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][13] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][18] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][22] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][25] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][28] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][29] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][38] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][39] = Entry(ProductionRule(87, {88}), 65);  // Reduce
  parse_table_[50][41] = Entry(87);  // Shift
  parse_table_[50][42] = Entry(88);  // Shift
  parse_table_[50][43] = Entry(89);  // Shift
  parse_table_[50][44] = Entry(90);  // Shift
  parse_table_[50][45] = Entry(91);  // Shift
  parse_table_[50][46] = Entry(92);  // Shift
  parse_table_[50][47] = Entry(93);  // Shift
  parse_table_[50][48] = Entry(94);  // Shift
  parse_table_[50][89] = Entry(95);  // Shift
  parse_table_[50][90] = Entry(96);  // Shift
  parse_table_[51][8] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][10] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][13] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][18] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][22] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][25] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][28] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][29] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][38] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][39] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][41] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][42] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][43] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][44] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][45] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][46] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][47] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][48] = Entry(ProductionRule(88, {91}), 73);  // Reduce
  parse_table_[51][49] = Entry(97);  // Shift
  parse_table_[51][50] = Entry(98);  // Shift
  parse_table_[51][92] = Entry(99);  // Shift
  parse_table_[52][8] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][10] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][13] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][18] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][22] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][25] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][28] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][29] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][38] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][39] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][41] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][42] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][43] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][44] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][45] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][46] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][47] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][48] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][49] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][50] = Entry(ProductionRule(91, {93}), 77);  // Reduce
  parse_table_[52][51] = Entry(100);  // Shift
  parse_table_[52][52] = Entry(101);  // Shift
  parse_table_[52][53] = Entry(102);  // Shift
  parse_table_[52][94] = Entry(103);  // Shift
  parse_table_[53][8] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][10] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][13] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][18] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][22] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][25] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][28] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][29] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][38] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][39] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][41] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][42] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][43] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][44] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][45] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][46] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][47] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][48] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][49] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][50] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][51] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][52] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[53][53] = Entry(ProductionRule(93, {95}), 81);  // Reduce
  parse_table_[54][1] = Entry(34);  // Shift
  parse_table_[54][2] = Entry(35);  // Shift
  parse_table_[54][3] = Entry(36);  // Shift
  parse_table_[54][4] = Entry(37);  // Shift
  parse_table_[54][17] = Entry(38);  // Shift
  parse_table_[54][50] = Entry(40);  // Shift
  parse_table_[54][51] = Entry(41);  // Shift
  parse_table_[54][54] = Entry(42);  // Shift
  parse_table_[54][55] = Entry(43);  // Shift
  parse_table_[54][56] = Entry(44);  // Shift
  parse_table_[54][84] = Entry(46);  // Shift
  parse_table_[54][95] = Entry(104);  // Shift
  parse_table_[54][96] = Entry(54);  // Shift
  parse_table_[54][97] = Entry(55);  // Shift
  parse_table_[54][98] = Entry(56);  // Shift
  parse_table_[54][99] = Entry(57);  // Shift
  parse_table_[54][100] = Entry(58);  // Shift
  parse_table_[55][8] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][10] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][13] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][18] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][22] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][25] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][28] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][29] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][38] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][39] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][41] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][42] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][43] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][44] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][45] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][46] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][47] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][48] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][49] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][50] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][51] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][52] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[55][53] = Entry(ProductionRule(95, {97}), 86);  // Reduce
  parse_table_[56][8] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][10] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][13] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][18] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][22] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][25] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][28] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][29] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][38] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][39] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][41] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][42] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][43] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][44] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][45] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][46] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][47] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][48] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][49] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][50] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][51] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][52] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[56][53] = Entry(ProductionRule(97, {98}), 90);  // Reduce
  parse_table_[57][8] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][10] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][13] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][18] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][22] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][25] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][28] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][29] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][38] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][39] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][41] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][42] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][43] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][44] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][45] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][46] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][47] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][48] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][49] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][50] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][51] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][52] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[57][53] = Entry(ProductionRule(98, {99}), 95);  // Reduce
  parse_table_[58][8] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][10] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][13] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][18] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][22] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][25] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][28] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][29] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][38] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][39] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][41] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][42] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][43] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][44] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][45] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][46] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][47] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][48] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][49] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][50] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][51] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][52] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[58][53] = Entry(ProductionRule(98, {100}), 96);  // Reduce
  parse_table_[59][13] = Entry(105);  // Shift
  parse_table_[60][1] = Entry(25);  // Shift
  parse_table_[60][73] = Entry(106);  // Shift
  parse_table_[61][0] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][1] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][2] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][3] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][4] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][8] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][14] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][15] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][16] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][17] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][19] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][20] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][21] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][23] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][24] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][26] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][30] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][31] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][40] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][50] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][51] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][54] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][55] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[61][56] = Entry(ProductionRule(74, {8}), 35);  // Reduce
  parse_table_[62][1] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][2] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][3] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][4] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][8] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][9] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][14] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][15] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][16] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][17] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][19] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][20] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][21] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][24] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][26] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][30] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][31] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][40] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][50] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][51] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][54] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][55] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][56] = Entry(ProductionRule(81, {}), 38);  // Reduce
  parse_table_[62][81] = Entry(107);  // Shift
  parse_table_[63][1] = Entry(34);  // Shift
  parse_table_[63][2] = Entry(35);  // Shift
  parse_table_[63][3] = Entry(36);  // Shift
  parse_table_[63][4] = Entry(37);  // Shift
  parse_table_[63][17] = Entry(38);  // Shift
  parse_table_[63][40] = Entry(39);  // Shift
  parse_table_[63][50] = Entry(40);  // Shift
  parse_table_[63][51] = Entry(41);  // Shift
  parse_table_[63][54] = Entry(42);  // Shift
  parse_table_[63][55] = Entry(43);  // Shift
  parse_table_[63][56] = Entry(44);  // Shift
  parse_table_[63][67] = Entry(108);  // Shift
  parse_table_[63][84] = Entry(46);  // Shift
  parse_table_[63][85] = Entry(47);  // Shift
  parse_table_[63][86] = Entry(48);  // Shift
  parse_table_[63][87] = Entry(49);  // Shift
  parse_table_[63][88] = Entry(50);  // Shift
  parse_table_[63][91] = Entry(51);  // Shift
  parse_table_[63][93] = Entry(52);  // Shift
  parse_table_[63][95] = Entry(53);  // Shift
  parse_table_[63][96] = Entry(54);  // Shift
  parse_table_[63][97] = Entry(55);  // Shift
  parse_table_[63][98] = Entry(56);  // Shift
  parse_table_[63][99] = Entry(57);  // Shift
  parse_table_[63][100] = Entry(58);  // Shift
  parse_table_[64][1] = Entry(34);  // Shift
  parse_table_[64][2] = Entry(35);  // Shift
  parse_table_[64][3] = Entry(36);  // Shift
  parse_table_[64][4] = Entry(37);  // Shift
  parse_table_[64][17] = Entry(38);  // Shift
  parse_table_[64][40] = Entry(39);  // Shift
  parse_table_[64][50] = Entry(40);  // Shift
  parse_table_[64][51] = Entry(41);  // Shift
  parse_table_[64][54] = Entry(42);  // Shift
  parse_table_[64][55] = Entry(43);  // Shift
  parse_table_[64][56] = Entry(44);  // Shift
  parse_table_[64][67] = Entry(109);  // Shift
  parse_table_[64][84] = Entry(46);  // Shift
  parse_table_[64][85] = Entry(47);  // Shift
  parse_table_[64][86] = Entry(48);  // Shift
  parse_table_[64][87] = Entry(49);  // Shift
  parse_table_[64][88] = Entry(50);  // Shift
  parse_table_[64][91] = Entry(51);  // Shift
  parse_table_[64][93] = Entry(52);  // Shift
  parse_table_[64][95] = Entry(53);  // Shift
  parse_table_[64][96] = Entry(54);  // Shift
  parse_table_[64][97] = Entry(55);  // Shift
  parse_table_[64][98] = Entry(56);  // Shift
  parse_table_[64][99] = Entry(57);  // Shift
  parse_table_[64][100] = Entry(58);  // Shift
  parse_table_[65][1] = Entry(110);  // Shift
  parse_table_[66][1] = Entry(34);  // Shift
  parse_table_[66][2] = Entry(35);  // Shift
  parse_table_[66][3] = Entry(36);  // Shift
  parse_table_[66][4] = Entry(37);  // Shift
  parse_table_[66][8] = Entry(111);  // Shift
  parse_table_[66][17] = Entry(38);  // Shift
  parse_table_[66][40] = Entry(39);  // Shift
  parse_table_[66][50] = Entry(40);  // Shift
  parse_table_[66][51] = Entry(41);  // Shift
  parse_table_[66][54] = Entry(42);  // Shift
  parse_table_[66][55] = Entry(43);  // Shift
  parse_table_[66][56] = Entry(44);  // Shift
  parse_table_[66][67] = Entry(68);  // Shift
  parse_table_[66][80] = Entry(112);  // Shift
  parse_table_[66][84] = Entry(77);  // Shift
  parse_table_[66][85] = Entry(47);  // Shift
  parse_table_[66][86] = Entry(48);  // Shift
  parse_table_[66][87] = Entry(49);  // Shift
  parse_table_[66][88] = Entry(50);  // Shift
  parse_table_[66][91] = Entry(51);  // Shift
  parse_table_[66][93] = Entry(52);  // Shift
  parse_table_[66][95] = Entry(53);  // Shift
  parse_table_[66][96] = Entry(54);  // Shift
  parse_table_[66][97] = Entry(55);  // Shift
  parse_table_[66][98] = Entry(56);  // Shift
  parse_table_[66][99] = Entry(57);  // Shift
  parse_table_[66][100] = Entry(58);  // Shift
  parse_table_[67][8] = Entry(113);  // Shift
  parse_table_[68][8] = Entry(ProductionRule(80, {67}), 57);  // Reduce
  parse_table_[68][10] = Entry(ProductionRule(80, {67}), 57);  // Reduce
  parse_table_[68][13] = Entry(ProductionRule(80, {67}), 57);  // Reduce
  parse_table_[68][18] = Entry(ProductionRule(80, {67}), 57);  // Reduce
  parse_table_[68][38] = Entry(85);  // Shift
  parse_table_[69][0] = Entry(ProductionRule(61, {1, 17, 68, 18, 69}), 18);  // Reduce
  parse_table_[69][1] = Entry(ProductionRule(61, {1, 17, 68, 18, 69}), 18);  // Reduce
  parse_table_[69][14] = Entry(ProductionRule(61, {1, 17, 68, 18, 69}), 18);  // Reduce
  parse_table_[69][15] = Entry(ProductionRule(61, {1, 17, 68, 18, 69}), 18);  // Reduce
  parse_table_[69][16] = Entry(ProductionRule(61, {1, 17, 68, 18, 69}), 18);  // Reduce
  parse_table_[70][0] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][1] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][2] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][3] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][4] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][8] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][14] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][15] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][16] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][17] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][19] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][20] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][21] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][23] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][24] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][26] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][30] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][31] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][40] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][50] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][51] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][54] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][55] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[70][56] = Entry(ProductionRule(69, {74}), 28);  // Reduce
  parse_table_[71][0] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][1] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][2] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][3] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][4] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][8] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][14] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][15] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][16] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][17] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][19] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][20] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][21] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][23] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][24] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][26] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][30] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][31] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][40] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][50] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][51] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][54] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][55] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[71][56] = Entry(ProductionRule(69, {75}), 29);  // Reduce
  parse_table_[72][0] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][1] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][2] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][3] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][4] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][8] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][14] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][15] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][16] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][17] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][19] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][20] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][21] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][23] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][24] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][26] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][30] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][31] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][40] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][50] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][51] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][54] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][55] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[72][56] = Entry(ProductionRule(69, {76}), 30);  // Reduce
  parse_table_[73][0] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][1] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][2] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][3] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][4] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][8] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][14] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][15] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][16] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][17] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][19] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][20] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][21] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][23] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][24] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][26] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][30] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][31] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][40] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][50] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][51] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][54] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][55] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[73][56] = Entry(ProductionRule(69, {77}), 31);  // Reduce
  parse_table_[74][0] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][1] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][2] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][3] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][4] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][8] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][14] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][15] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][16] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][17] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][19] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][20] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][21] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][23] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][24] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][26] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][30] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][31] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][40] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][50] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][51] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][54] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][55] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[74][56] = Entry(ProductionRule(69, {78}), 32);  // Reduce
  parse_table_[75][0] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][1] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][2] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][3] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][4] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][8] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][14] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][15] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][16] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][17] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][19] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][20] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][21] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][23] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][24] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][26] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][30] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][31] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][40] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][50] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][51] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][54] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][55] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[75][56] = Entry(ProductionRule(69, {79}), 33);  // Reduce
  parse_table_[76][8] = Entry(114);  // Shift
  parse_table_[77][8] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][10] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][13] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][18] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][27] = Entry(115);  // Shift
  parse_table_[77][32] = Entry(116);  // Shift
  parse_table_[77][33] = Entry(117);  // Shift
  parse_table_[77][34] = Entry(118);  // Shift
  parse_table_[77][35] = Entry(119);  // Shift
  parse_table_[77][36] = Entry(120);  // Shift
  parse_table_[77][37] = Entry(121);  // Shift
  parse_table_[77][38] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][39] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][41] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][42] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][43] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][44] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][45] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][46] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][47] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][48] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][49] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][50] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][51] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][52] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[77][53] = Entry(ProductionRule(97, {84}), 91);  // Reduce
  parse_table_[78][8] = Entry(ProductionRule(70, {70, 8, 71}), 21);  // Reduce
  parse_table_[78][18] = Entry(ProductionRule(70, {70, 8, 71}), 21);  // Reduce
  parse_table_[79][8] = Entry(ProductionRule(66, {1, 12, 2, 13}), 13);  // Reduce
  parse_table_[79][10] = Entry(ProductionRule(66, {1, 12, 2, 13}), 13);  // Reduce
  parse_table_[79][11] = Entry(ProductionRule(66, {1, 12, 2, 13}), 13);  // Reduce
  parse_table_[80][1] = Entry(34);  // Shift
  parse_table_[80][2] = Entry(35);  // Shift
  parse_table_[80][3] = Entry(36);  // Shift
  parse_table_[80][4] = Entry(37);  // Shift
  parse_table_[80][8] = Entry(61);  // Shift
  parse_table_[80][17] = Entry(38);  // Shift
  parse_table_[80][19] = Entry(62);  // Shift
  parse_table_[80][21] = Entry(63);  // Shift
  parse_table_[80][24] = Entry(64);  // Shift
  parse_table_[80][26] = Entry(65);  // Shift
  parse_table_[80][30] = Entry(66);  // Shift
  parse_table_[80][31] = Entry(67);  // Shift
  parse_table_[80][40] = Entry(39);  // Shift
  parse_table_[80][50] = Entry(40);  // Shift
  parse_table_[80][51] = Entry(41);  // Shift
  parse_table_[80][54] = Entry(42);  // Shift
  parse_table_[80][55] = Entry(43);  // Shift
  parse_table_[80][56] = Entry(44);  // Shift
  parse_table_[80][67] = Entry(68);  // Shift
  parse_table_[80][69] = Entry(122);  // Shift
  parse_table_[80][74] = Entry(70);  // Shift
  parse_table_[80][75] = Entry(71);  // Shift
  parse_table_[80][76] = Entry(72);  // Shift
  parse_table_[80][77] = Entry(73);  // Shift
  parse_table_[80][78] = Entry(74);  // Shift
  parse_table_[80][79] = Entry(75);  // Shift
  parse_table_[80][80] = Entry(76);  // Shift
  parse_table_[80][84] = Entry(77);  // Shift
  parse_table_[80][85] = Entry(47);  // Shift
  parse_table_[80][86] = Entry(48);  // Shift
  parse_table_[80][87] = Entry(49);  // Shift
  parse_table_[80][88] = Entry(50);  // Shift
  parse_table_[80][91] = Entry(51);  // Shift
  parse_table_[80][93] = Entry(52);  // Shift
  parse_table_[80][95] = Entry(53);  // Shift
  parse_table_[80][96] = Entry(54);  // Shift
  parse_table_[80][97] = Entry(55);  // Shift
  parse_table_[80][98] = Entry(56);  // Shift
  parse_table_[80][99] = Entry(57);  // Shift
  parse_table_[80][100] = Entry(58);  // Shift
  parse_table_[81][1] = Entry(34);  // Shift
  parse_table_[81][2] = Entry(35);  // Shift
  parse_table_[81][3] = Entry(36);  // Shift
  parse_table_[81][4] = Entry(37);  // Shift
  parse_table_[81][17] = Entry(38);  // Shift
  parse_table_[81][40] = Entry(39);  // Shift
  parse_table_[81][50] = Entry(40);  // Shift
  parse_table_[81][51] = Entry(41);  // Shift
  parse_table_[81][54] = Entry(42);  // Shift
  parse_table_[81][55] = Entry(43);  // Shift
  parse_table_[81][56] = Entry(44);  // Shift
  parse_table_[81][67] = Entry(68);  // Shift
  parse_table_[81][80] = Entry(123);  // Shift
  parse_table_[81][84] = Entry(77);  // Shift
  parse_table_[81][85] = Entry(47);  // Shift
  parse_table_[81][86] = Entry(48);  // Shift
  parse_table_[81][87] = Entry(49);  // Shift
  parse_table_[81][88] = Entry(50);  // Shift
  parse_table_[81][91] = Entry(51);  // Shift
  parse_table_[81][93] = Entry(52);  // Shift
  parse_table_[81][95] = Entry(53);  // Shift
  parse_table_[81][96] = Entry(54);  // Shift
  parse_table_[81][97] = Entry(55);  // Shift
  parse_table_[81][98] = Entry(56);  // Shift
  parse_table_[81][99] = Entry(57);  // Shift
  parse_table_[81][100] = Entry(58);  // Shift
  parse_table_[82][1] = Entry(34);  // Shift
  parse_table_[82][2] = Entry(35);  // Shift
  parse_table_[82][3] = Entry(36);  // Shift
  parse_table_[82][4] = Entry(37);  // Shift
  parse_table_[82][17] = Entry(38);  // Shift
  parse_table_[82][18] = Entry(ProductionRule(101, {}), 99);  // Reduce
  parse_table_[82][40] = Entry(39);  // Shift
  parse_table_[82][50] = Entry(40);  // Shift
  parse_table_[82][51] = Entry(41);  // Shift
  parse_table_[82][54] = Entry(42);  // Shift
  parse_table_[82][55] = Entry(43);  // Shift
  parse_table_[82][56] = Entry(44);  // Shift
  parse_table_[82][67] = Entry(68);  // Shift
  parse_table_[82][80] = Entry(124);  // Shift
  parse_table_[82][84] = Entry(77);  // Shift
  parse_table_[82][85] = Entry(47);  // Shift
  parse_table_[82][86] = Entry(48);  // Shift
  parse_table_[82][87] = Entry(49);  // Shift
  parse_table_[82][88] = Entry(50);  // Shift
  parse_table_[82][91] = Entry(51);  // Shift
  parse_table_[82][93] = Entry(52);  // Shift
  parse_table_[82][95] = Entry(53);  // Shift
  parse_table_[82][96] = Entry(54);  // Shift
  parse_table_[82][97] = Entry(55);  // Shift
  parse_table_[82][98] = Entry(56);  // Shift
  parse_table_[82][99] = Entry(57);  // Shift
  parse_table_[82][100] = Entry(58);  // Shift
  parse_table_[82][101] = Entry(125);  // Shift
  parse_table_[82][102] = Entry(126);  // Shift
  parse_table_[83][18] = Entry(127);  // Shift
  parse_table_[84][8] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][10] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][13] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][18] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][22] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][25] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][28] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][29] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][38] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[84][39] = Entry(ProductionRule(86, {40, 86}), 62);  // Reduce
  parse_table_[85][1] = Entry(34);  // Shift
  parse_table_[85][2] = Entry(35);  // Shift
  parse_table_[85][3] = Entry(36);  // Shift
  parse_table_[85][4] = Entry(37);  // Shift
  parse_table_[85][17] = Entry(38);  // Shift
  parse_table_[85][40] = Entry(39);  // Shift
  parse_table_[85][50] = Entry(40);  // Shift
  parse_table_[85][51] = Entry(41);  // Shift
  parse_table_[85][54] = Entry(42);  // Shift
  parse_table_[85][55] = Entry(43);  // Shift
  parse_table_[85][56] = Entry(44);  // Shift
  parse_table_[85][84] = Entry(46);  // Shift
  parse_table_[85][85] = Entry(128);  // Shift
  parse_table_[85][86] = Entry(48);  // Shift
  parse_table_[85][87] = Entry(49);  // Shift
  parse_table_[85][88] = Entry(50);  // Shift
  parse_table_[85][91] = Entry(51);  // Shift
  parse_table_[85][93] = Entry(52);  // Shift
  parse_table_[85][95] = Entry(53);  // Shift
  parse_table_[85][96] = Entry(54);  // Shift
  parse_table_[85][97] = Entry(55);  // Shift
  parse_table_[85][98] = Entry(56);  // Shift
  parse_table_[85][99] = Entry(57);  // Shift
  parse_table_[85][100] = Entry(58);  // Shift
  parse_table_[86][1] = Entry(34);  // Shift
  parse_table_[86][2] = Entry(35);  // Shift
  parse_table_[86][3] = Entry(36);  // Shift
  parse_table_[86][4] = Entry(37);  // Shift
  parse_table_[86][17] = Entry(38);  // Shift
  parse_table_[86][40] = Entry(39);  // Shift
  parse_table_[86][50] = Entry(40);  // Shift
  parse_table_[86][51] = Entry(41);  // Shift
  parse_table_[86][54] = Entry(42);  // Shift
  parse_table_[86][55] = Entry(43);  // Shift
  parse_table_[86][56] = Entry(44);  // Shift
  parse_table_[86][84] = Entry(46);  // Shift
  parse_table_[86][86] = Entry(129);  // Shift
  parse_table_[86][87] = Entry(49);  // Shift
  parse_table_[86][88] = Entry(50);  // Shift
  parse_table_[86][91] = Entry(51);  // Shift
  parse_table_[86][93] = Entry(52);  // Shift
  parse_table_[86][95] = Entry(53);  // Shift
  parse_table_[86][96] = Entry(54);  // Shift
  parse_table_[86][97] = Entry(55);  // Shift
  parse_table_[86][98] = Entry(56);  // Shift
  parse_table_[86][99] = Entry(57);  // Shift
  parse_table_[86][100] = Entry(58);  // Shift
  parse_table_[87][1] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][2] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][3] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][4] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][17] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][50] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][51] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][54] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][55] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[87][56] = Entry(ProductionRule(89, {41}), 66);  // Reduce
  parse_table_[88][1] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][2] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][3] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][4] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][17] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][50] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][51] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][54] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][55] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[88][56] = Entry(ProductionRule(89, {42}), 67);  // Reduce
  parse_table_[89][1] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][2] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][3] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][4] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][17] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][50] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][51] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][54] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][55] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[89][56] = Entry(ProductionRule(89, {43}), 68);  // Reduce
  parse_table_[90][1] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][2] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][3] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][4] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][17] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][50] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][51] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][54] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][55] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[90][56] = Entry(ProductionRule(89, {44}), 69);  // Reduce
  parse_table_[91][1] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][2] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][3] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][4] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][17] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][50] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][51] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][54] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][55] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[91][56] = Entry(ProductionRule(89, {45}), 70);  // Reduce
  parse_table_[92][1] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][2] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][3] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][4] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][17] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][50] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][51] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][54] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][55] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[92][56] = Entry(ProductionRule(89, {46}), 71);  // Reduce
  parse_table_[93][1] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][2] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][3] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][4] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][17] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][50] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][51] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][54] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][55] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[93][56] = Entry(ProductionRule(90, {47}), 74);  // Reduce
  parse_table_[94][1] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][2] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][3] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][4] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][17] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][50] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][51] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][54] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][55] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[94][56] = Entry(ProductionRule(90, {48}), 75);  // Reduce
  parse_table_[95][1] = Entry(34);  // Shift
  parse_table_[95][2] = Entry(35);  // Shift
  parse_table_[95][3] = Entry(36);  // Shift
  parse_table_[95][4] = Entry(37);  // Shift
  parse_table_[95][17] = Entry(38);  // Shift
  parse_table_[95][50] = Entry(40);  // Shift
  parse_table_[95][51] = Entry(41);  // Shift
  parse_table_[95][54] = Entry(42);  // Shift
  parse_table_[95][55] = Entry(43);  // Shift
  parse_table_[95][56] = Entry(44);  // Shift
  parse_table_[95][84] = Entry(46);  // Shift
  parse_table_[95][88] = Entry(130);  // Shift
  parse_table_[95][91] = Entry(51);  // Shift
  parse_table_[95][93] = Entry(52);  // Shift
  parse_table_[95][95] = Entry(53);  // Shift
  parse_table_[95][96] = Entry(54);  // Shift
  parse_table_[95][97] = Entry(55);  // Shift
  parse_table_[95][98] = Entry(56);  // Shift
  parse_table_[95][99] = Entry(57);  // Shift
  parse_table_[95][100] = Entry(58);  // Shift
  parse_table_[96][1] = Entry(34);  // Shift
  parse_table_[96][2] = Entry(35);  // Shift
  parse_table_[96][3] = Entry(36);  // Shift
  parse_table_[96][4] = Entry(37);  // Shift
  parse_table_[96][17] = Entry(38);  // Shift
  parse_table_[96][50] = Entry(40);  // Shift
  parse_table_[96][51] = Entry(41);  // Shift
  parse_table_[96][54] = Entry(42);  // Shift
  parse_table_[96][55] = Entry(43);  // Shift
  parse_table_[96][56] = Entry(44);  // Shift
  parse_table_[96][84] = Entry(46);  // Shift
  parse_table_[96][91] = Entry(131);  // Shift
  parse_table_[96][93] = Entry(52);  // Shift
  parse_table_[96][95] = Entry(53);  // Shift
  parse_table_[96][96] = Entry(54);  // Shift
  parse_table_[96][97] = Entry(55);  // Shift
  parse_table_[96][98] = Entry(56);  // Shift
  parse_table_[96][99] = Entry(57);  // Shift
  parse_table_[96][100] = Entry(58);  // Shift
  parse_table_[97][1] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][2] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][3] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][4] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][17] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][50] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][51] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][54] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][55] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[97][56] = Entry(ProductionRule(92, {49}), 78);  // Reduce
  parse_table_[98][1] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][2] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][3] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][4] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][17] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][50] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][51] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][54] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][55] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[98][56] = Entry(ProductionRule(92, {50}), 79);  // Reduce
  parse_table_[99][1] = Entry(34);  // Shift
  parse_table_[99][2] = Entry(35);  // Shift
  parse_table_[99][3] = Entry(36);  // Shift
  parse_table_[99][4] = Entry(37);  // Shift
  parse_table_[99][17] = Entry(38);  // Shift
  parse_table_[99][50] = Entry(40);  // Shift
  parse_table_[99][51] = Entry(41);  // Shift
  parse_table_[99][54] = Entry(42);  // Shift
  parse_table_[99][55] = Entry(43);  // Shift
  parse_table_[99][56] = Entry(44);  // Shift
  parse_table_[99][84] = Entry(46);  // Shift
  parse_table_[99][93] = Entry(132);  // Shift
  parse_table_[99][95] = Entry(53);  // Shift
  parse_table_[99][96] = Entry(54);  // Shift
  parse_table_[99][97] = Entry(55);  // Shift
  parse_table_[99][98] = Entry(56);  // Shift
  parse_table_[99][99] = Entry(57);  // Shift
  parse_table_[99][100] = Entry(58);  // Shift
  parse_table_[100][1] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][2] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][3] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][4] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][17] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][50] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][51] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][54] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][55] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[100][56] = Entry(ProductionRule(94, {51}), 82);  // Reduce
  parse_table_[101][1] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][2] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][3] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][4] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][17] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][50] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][51] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][54] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][55] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[101][56] = Entry(ProductionRule(94, {52}), 83);  // Reduce
  parse_table_[102][1] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][2] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][3] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][4] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][17] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][50] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][51] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][54] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][55] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[102][56] = Entry(ProductionRule(94, {53}), 84);  // Reduce
  parse_table_[103][1] = Entry(34);  // Shift
  parse_table_[103][2] = Entry(35);  // Shift
  parse_table_[103][3] = Entry(36);  // Shift
  parse_table_[103][4] = Entry(37);  // Shift
  parse_table_[103][17] = Entry(38);  // Shift
  parse_table_[103][50] = Entry(40);  // Shift
  parse_table_[103][51] = Entry(41);  // Shift
  parse_table_[103][54] = Entry(42);  // Shift
  parse_table_[103][55] = Entry(43);  // Shift
  parse_table_[103][56] = Entry(44);  // Shift
  parse_table_[103][84] = Entry(46);  // Shift
  parse_table_[103][95] = Entry(133);  // Shift
  parse_table_[103][96] = Entry(54);  // Shift
  parse_table_[103][97] = Entry(55);  // Shift
  parse_table_[103][98] = Entry(56);  // Shift
  parse_table_[103][99] = Entry(57);  // Shift
  parse_table_[103][100] = Entry(58);  // Shift
  parse_table_[104][8] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][10] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][13] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][18] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][22] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][25] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][28] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][29] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][38] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][39] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][41] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][42] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][43] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][44] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][45] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][46] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][47] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][48] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][49] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][50] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][51] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][52] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[104][53] = Entry(ProductionRule(95, {96, 95}), 85);  // Reduce
  parse_table_[105][8] = Entry(ProductionRule(73, {1, 12, 13}), 27);  // Reduce
  parse_table_[105][10] = Entry(ProductionRule(73, {1, 12, 13}), 27);  // Reduce
  parse_table_[105][18] = Entry(ProductionRule(73, {1, 12, 13}), 27);  // Reduce
  parse_table_[106][8] = Entry(ProductionRule(72, {72, 10, 73}), 24);  // Reduce
  parse_table_[106][10] = Entry(ProductionRule(72, {72, 10, 73}), 24);  // Reduce
  parse_table_[106][18] = Entry(ProductionRule(72, {72, 10, 73}), 24);  // Reduce
  parse_table_[107][1] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][2] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][3] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][4] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][8] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][9] = Entry(134);  // Shift
  parse_table_[107][14] = Entry(2);  // Shift
  parse_table_[107][15] = Entry(3);  // Shift
  parse_table_[107][16] = Entry(4);  // Shift
  parse_table_[107][17] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][19] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][20] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][21] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][24] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][26] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][30] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][31] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][40] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][50] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][51] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][54] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][55] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][56] = Entry(ProductionRule(82, {}), 40);  // Reduce
  parse_table_[107][62] = Entry(135);  // Shift
  parse_table_[107][64] = Entry(136);  // Shift
  parse_table_[107][82] = Entry(137);  // Shift
  parse_table_[108][22] = Entry(138);  // Shift
  parse_table_[108][38] = Entry(85);  // Shift
  parse_table_[109][25] = Entry(139);  // Shift
  parse_table_[109][38] = Entry(85);  // Shift
  parse_table_[110][27] = Entry(140);  // Shift
  parse_table_[111][0] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][1] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][2] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][3] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][4] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][8] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][14] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][15] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][16] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][17] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][19] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][20] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][21] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][23] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][24] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][26] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][30] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][31] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][40] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][50] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][51] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][54] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][55] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[111][56] = Entry(ProductionRule(78, {30, 8}), 47);  // Reduce
  parse_table_[112][8] = Entry(141);  // Shift
  parse_table_[113][0] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][1] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][2] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][3] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][4] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][8] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][14] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][15] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][16] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][17] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][19] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][20] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][21] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][23] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][24] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][26] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][30] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][31] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][40] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][50] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][51] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][54] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][55] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[113][56] = Entry(ProductionRule(79, {31, 8}), 49);  // Reduce
  parse_table_[114][0] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][1] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][2] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][3] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][4] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][8] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][14] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][15] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][16] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][17] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][19] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][20] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][21] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][23] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][24] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][26] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][30] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][31] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][40] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][50] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][51] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][54] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][55] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[114][56] = Entry(ProductionRule(74, {80, 8}), 34);  // Reduce
  parse_table_[115][1] = Entry(34);  // Shift
  parse_table_[115][2] = Entry(35);  // Shift
  parse_table_[115][3] = Entry(36);  // Shift
  parse_table_[115][4] = Entry(37);  // Shift
  parse_table_[115][17] = Entry(38);  // Shift
  parse_table_[115][40] = Entry(39);  // Shift
  parse_table_[115][50] = Entry(40);  // Shift
  parse_table_[115][51] = Entry(41);  // Shift
  parse_table_[115][54] = Entry(42);  // Shift
  parse_table_[115][55] = Entry(43);  // Shift
  parse_table_[115][56] = Entry(44);  // Shift
  parse_table_[115][67] = Entry(68);  // Shift
  parse_table_[115][80] = Entry(142);  // Shift
  parse_table_[115][84] = Entry(77);  // Shift
  parse_table_[115][85] = Entry(47);  // Shift
  parse_table_[115][86] = Entry(48);  // Shift
  parse_table_[115][87] = Entry(49);  // Shift
  parse_table_[115][88] = Entry(50);  // Shift
  parse_table_[115][91] = Entry(51);  // Shift
  parse_table_[115][93] = Entry(52);  // Shift
  parse_table_[115][95] = Entry(53);  // Shift
  parse_table_[115][96] = Entry(54);  // Shift
  parse_table_[115][97] = Entry(55);  // Shift
  parse_table_[115][98] = Entry(56);  // Shift
  parse_table_[115][99] = Entry(57);  // Shift
  parse_table_[115][100] = Entry(58);  // Shift
  parse_table_[116][1] = Entry(34);  // Shift
  parse_table_[116][2] = Entry(35);  // Shift
  parse_table_[116][3] = Entry(36);  // Shift
  parse_table_[116][4] = Entry(37);  // Shift
  parse_table_[116][17] = Entry(38);  // Shift
  parse_table_[116][40] = Entry(39);  // Shift
  parse_table_[116][50] = Entry(40);  // Shift
  parse_table_[116][51] = Entry(41);  // Shift
  parse_table_[116][54] = Entry(42);  // Shift
  parse_table_[116][55] = Entry(43);  // Shift
  parse_table_[116][56] = Entry(44);  // Shift
  parse_table_[116][67] = Entry(68);  // Shift
  parse_table_[116][80] = Entry(143);  // Shift
  parse_table_[116][84] = Entry(77);  // Shift
  parse_table_[116][85] = Entry(47);  // Shift
  parse_table_[116][86] = Entry(48);  // Shift
  parse_table_[116][87] = Entry(49);  // Shift
  parse_table_[116][88] = Entry(50);  // Shift
  parse_table_[116][91] = Entry(51);  // Shift
  parse_table_[116][93] = Entry(52);  // Shift
  parse_table_[116][95] = Entry(53);  // Shift
  parse_table_[116][96] = Entry(54);  // Shift
  parse_table_[116][97] = Entry(55);  // Shift
  parse_table_[116][98] = Entry(56);  // Shift
  parse_table_[116][99] = Entry(57);  // Shift
  parse_table_[116][100] = Entry(58);  // Shift
  parse_table_[117][1] = Entry(34);  // Shift
  parse_table_[117][2] = Entry(35);  // Shift
  parse_table_[117][3] = Entry(36);  // Shift
  parse_table_[117][4] = Entry(37);  // Shift
  parse_table_[117][17] = Entry(38);  // Shift
  parse_table_[117][40] = Entry(39);  // Shift
  parse_table_[117][50] = Entry(40);  // Shift
  parse_table_[117][51] = Entry(41);  // Shift
  parse_table_[117][54] = Entry(42);  // Shift
  parse_table_[117][55] = Entry(43);  // Shift
  parse_table_[117][56] = Entry(44);  // Shift
  parse_table_[117][67] = Entry(68);  // Shift
  parse_table_[117][80] = Entry(144);  // Shift
  parse_table_[117][84] = Entry(77);  // Shift
  parse_table_[117][85] = Entry(47);  // Shift
  parse_table_[117][86] = Entry(48);  // Shift
  parse_table_[117][87] = Entry(49);  // Shift
  parse_table_[117][88] = Entry(50);  // Shift
  parse_table_[117][91] = Entry(51);  // Shift
  parse_table_[117][93] = Entry(52);  // Shift
  parse_table_[117][95] = Entry(53);  // Shift
  parse_table_[117][96] = Entry(54);  // Shift
  parse_table_[117][97] = Entry(55);  // Shift
  parse_table_[117][98] = Entry(56);  // Shift
  parse_table_[117][99] = Entry(57);  // Shift
  parse_table_[117][100] = Entry(58);  // Shift
  parse_table_[118][1] = Entry(34);  // Shift
  parse_table_[118][2] = Entry(35);  // Shift
  parse_table_[118][3] = Entry(36);  // Shift
  parse_table_[118][4] = Entry(37);  // Shift
  parse_table_[118][17] = Entry(38);  // Shift
  parse_table_[118][40] = Entry(39);  // Shift
  parse_table_[118][50] = Entry(40);  // Shift
  parse_table_[118][51] = Entry(41);  // Shift
  parse_table_[118][54] = Entry(42);  // Shift
  parse_table_[118][55] = Entry(43);  // Shift
  parse_table_[118][56] = Entry(44);  // Shift
  parse_table_[118][67] = Entry(68);  // Shift
  parse_table_[118][80] = Entry(145);  // Shift
  parse_table_[118][84] = Entry(77);  // Shift
  parse_table_[118][85] = Entry(47);  // Shift
  parse_table_[118][86] = Entry(48);  // Shift
  parse_table_[118][87] = Entry(49);  // Shift
  parse_table_[118][88] = Entry(50);  // Shift
  parse_table_[118][91] = Entry(51);  // Shift
  parse_table_[118][93] = Entry(52);  // Shift
  parse_table_[118][95] = Entry(53);  // Shift
  parse_table_[118][96] = Entry(54);  // Shift
  parse_table_[118][97] = Entry(55);  // Shift
  parse_table_[118][98] = Entry(56);  // Shift
  parse_table_[118][99] = Entry(57);  // Shift
  parse_table_[118][100] = Entry(58);  // Shift
  parse_table_[119][1] = Entry(34);  // Shift
  parse_table_[119][2] = Entry(35);  // Shift
  parse_table_[119][3] = Entry(36);  // Shift
  parse_table_[119][4] = Entry(37);  // Shift
  parse_table_[119][17] = Entry(38);  // Shift
  parse_table_[119][40] = Entry(39);  // Shift
  parse_table_[119][50] = Entry(40);  // Shift
  parse_table_[119][51] = Entry(41);  // Shift
  parse_table_[119][54] = Entry(42);  // Shift
  parse_table_[119][55] = Entry(43);  // Shift
  parse_table_[119][56] = Entry(44);  // Shift
  parse_table_[119][67] = Entry(68);  // Shift
  parse_table_[119][80] = Entry(146);  // Shift
  parse_table_[119][84] = Entry(77);  // Shift
  parse_table_[119][85] = Entry(47);  // Shift
  parse_table_[119][86] = Entry(48);  // Shift
  parse_table_[119][87] = Entry(49);  // Shift
  parse_table_[119][88] = Entry(50);  // Shift
  parse_table_[119][91] = Entry(51);  // Shift
  parse_table_[119][93] = Entry(52);  // Shift
  parse_table_[119][95] = Entry(53);  // Shift
  parse_table_[119][96] = Entry(54);  // Shift
  parse_table_[119][97] = Entry(55);  // Shift
  parse_table_[119][98] = Entry(56);  // Shift
  parse_table_[119][99] = Entry(57);  // Shift
  parse_table_[119][100] = Entry(58);  // Shift
  parse_table_[120][8] = Entry(ProductionRule(80, {84, 36}), 55);  // Reduce
  parse_table_[120][10] = Entry(ProductionRule(80, {84, 36}), 55);  // Reduce
  parse_table_[120][13] = Entry(ProductionRule(80, {84, 36}), 55);  // Reduce
  parse_table_[120][18] = Entry(ProductionRule(80, {84, 36}), 55);  // Reduce
  parse_table_[121][8] = Entry(ProductionRule(80, {84, 37}), 56);  // Reduce
  parse_table_[121][10] = Entry(ProductionRule(80, {84, 37}), 56);  // Reduce
  parse_table_[121][13] = Entry(ProductionRule(80, {84, 37}), 56);  // Reduce
  parse_table_[121][18] = Entry(ProductionRule(80, {84, 37}), 56);  // Reduce
  parse_table_[122][0] = Entry(ProductionRule(61, {62, 1, 17, 68, 18, 69}), 17);  // Reduce
  parse_table_[122][1] = Entry(ProductionRule(61, {62, 1, 17, 68, 18, 69}), 17);  // Reduce
  parse_table_[122][14] = Entry(ProductionRule(61, {62, 1, 17, 68, 18, 69}), 17);  // Reduce
  parse_table_[122][15] = Entry(ProductionRule(61, {62, 1, 17, 68, 18, 69}), 17);  // Reduce
  parse_table_[122][16] = Entry(ProductionRule(61, {62, 1, 17, 68, 18, 69}), 17);  // Reduce
  parse_table_[123][13] = Entry(147);  // Shift
  parse_table_[124][10] = Entry(ProductionRule(102, {80}), 101);  // Reduce
  parse_table_[124][18] = Entry(ProductionRule(102, {80}), 101);  // Reduce
  parse_table_[125][18] = Entry(148);  // Shift
  parse_table_[126][10] = Entry(149);  // Shift
  parse_table_[126][18] = Entry(ProductionRule(101, {102}), 98);  // Reduce
  parse_table_[127][8] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][10] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][13] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][18] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][22] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][25] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][28] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][29] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][38] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][39] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][41] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][42] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][43] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][44] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][45] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][46] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][47] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][48] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][49] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][50] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][51] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][52] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[127][53] = Entry(ProductionRule(98, {17, 80, 18}), 94);  // Reduce
  parse_table_[128][8] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][10] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][13] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][18] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][22] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][25] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][28] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][29] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][38] = Entry(ProductionRule(67, {67, 38, 85}), 58);  // Reduce
  parse_table_[128][39] = Entry(86);  // Shift
  parse_table_[129][8] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][10] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][13] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][18] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][22] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][25] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][28] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][29] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][38] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[129][39] = Entry(ProductionRule(85, {85, 39, 86}), 60);  // Reduce
  parse_table_[130][8] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][10] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][13] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][18] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][22] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][25] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][28] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][29] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][38] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][39] = Entry(ProductionRule(87, {88, 89, 88}), 64);  // Reduce
  parse_table_[130][47] = Entry(93);  // Shift
  parse_table_[130][48] = Entry(94);  // Shift
  parse_table_[130][90] = Entry(96);  // Shift
  parse_table_[131][8] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][10] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][13] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][18] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][22] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][25] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][28] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][29] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][38] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][39] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][41] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][42] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][43] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][44] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][45] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][46] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][47] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][48] = Entry(ProductionRule(88, {88, 90, 91}), 72);  // Reduce
  parse_table_[131][49] = Entry(97);  // Shift
  parse_table_[131][50] = Entry(98);  // Shift
  parse_table_[131][92] = Entry(99);  // Shift
  parse_table_[132][8] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][10] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][13] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][18] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][22] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][25] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][28] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][29] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][38] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][39] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][41] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][42] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][43] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][44] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][45] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][46] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][47] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][48] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][49] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][50] = Entry(ProductionRule(91, {91, 92, 93}), 76);  // Reduce
  parse_table_[132][51] = Entry(100);  // Shift
  parse_table_[132][52] = Entry(101);  // Shift
  parse_table_[132][53] = Entry(102);  // Shift
  parse_table_[132][94] = Entry(103);  // Shift
  parse_table_[133][8] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][10] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][13] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][18] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][22] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][25] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][28] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][29] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][38] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][39] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][41] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][42] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][43] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][44] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][45] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][46] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][47] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][48] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][49] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][50] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][51] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][52] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[133][53] = Entry(ProductionRule(93, {93, 94, 95}), 80);  // Reduce
  parse_table_[134][14] = Entry(2);  // Shift
  parse_table_[134][15] = Entry(3);  // Shift
  parse_table_[134][16] = Entry(4);  // Shift
  parse_table_[134][62] = Entry(150);  // Shift
  parse_table_[135][1] = Entry(32);  // Shift
  parse_table_[135][63] = Entry(151);  // Shift
  parse_table_[135][65] = Entry(14);  // Shift
  parse_table_[135][66] = Entry(15);  // Shift
  parse_table_[136][1] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][2] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][3] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][4] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][8] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][9] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][14] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][15] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][16] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][17] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][19] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][20] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][21] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][24] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][26] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][30] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][31] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][40] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][50] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][51] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][54] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][55] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[136][56] = Entry(ProductionRule(81, {81, 64}), 37);  // Reduce
  parse_table_[137][1] = Entry(34);  // Shift
  parse_table_[137][2] = Entry(35);  // Shift
  parse_table_[137][3] = Entry(36);  // Shift
  parse_table_[137][4] = Entry(37);  // Shift
  parse_table_[137][8] = Entry(61);  // Shift
  parse_table_[137][17] = Entry(38);  // Shift
  parse_table_[137][19] = Entry(62);  // Shift
  parse_table_[137][20] = Entry(152);  // Shift
  parse_table_[137][21] = Entry(63);  // Shift
  parse_table_[137][24] = Entry(64);  // Shift
  parse_table_[137][26] = Entry(65);  // Shift
  parse_table_[137][30] = Entry(66);  // Shift
  parse_table_[137][31] = Entry(67);  // Shift
  parse_table_[137][40] = Entry(39);  // Shift
  parse_table_[137][50] = Entry(40);  // Shift
  parse_table_[137][51] = Entry(41);  // Shift
  parse_table_[137][54] = Entry(42);  // Shift
  parse_table_[137][55] = Entry(43);  // Shift
  parse_table_[137][56] = Entry(44);  // Shift
  parse_table_[137][67] = Entry(68);  // Shift
  parse_table_[137][69] = Entry(153);  // Shift
  parse_table_[137][74] = Entry(70);  // Shift
  parse_table_[137][75] = Entry(71);  // Shift
  parse_table_[137][76] = Entry(72);  // Shift
  parse_table_[137][77] = Entry(73);  // Shift
  parse_table_[137][78] = Entry(74);  // Shift
  parse_table_[137][79] = Entry(75);  // Shift
  parse_table_[137][80] = Entry(76);  // Shift
  parse_table_[137][84] = Entry(77);  // Shift
  parse_table_[137][85] = Entry(47);  // Shift
  parse_table_[137][86] = Entry(48);  // Shift
  parse_table_[137][87] = Entry(49);  // Shift
  parse_table_[137][88] = Entry(50);  // Shift
  parse_table_[137][91] = Entry(51);  // Shift
  parse_table_[137][93] = Entry(52);  // Shift
  parse_table_[137][95] = Entry(53);  // Shift
  parse_table_[137][96] = Entry(54);  // Shift
  parse_table_[137][97] = Entry(55);  // Shift
  parse_table_[137][98] = Entry(56);  // Shift
  parse_table_[137][99] = Entry(57);  // Shift
  parse_table_[137][100] = Entry(58);  // Shift
  parse_table_[138][1] = Entry(34);  // Shift
  parse_table_[138][2] = Entry(35);  // Shift
  parse_table_[138][3] = Entry(36);  // Shift
  parse_table_[138][4] = Entry(37);  // Shift
  parse_table_[138][8] = Entry(61);  // Shift
  parse_table_[138][17] = Entry(38);  // Shift
  parse_table_[138][19] = Entry(62);  // Shift
  parse_table_[138][21] = Entry(63);  // Shift
  parse_table_[138][24] = Entry(64);  // Shift
  parse_table_[138][26] = Entry(65);  // Shift
  parse_table_[138][30] = Entry(66);  // Shift
  parse_table_[138][31] = Entry(67);  // Shift
  parse_table_[138][40] = Entry(39);  // Shift
  parse_table_[138][50] = Entry(40);  // Shift
  parse_table_[138][51] = Entry(41);  // Shift
  parse_table_[138][54] = Entry(42);  // Shift
  parse_table_[138][55] = Entry(43);  // Shift
  parse_table_[138][56] = Entry(44);  // Shift
  parse_table_[138][67] = Entry(68);  // Shift
  parse_table_[138][69] = Entry(154);  // Shift
  parse_table_[138][74] = Entry(70);  // Shift
  parse_table_[138][75] = Entry(71);  // Shift
  parse_table_[138][76] = Entry(72);  // Shift
  parse_table_[138][77] = Entry(73);  // Shift
  parse_table_[138][78] = Entry(74);  // Shift
  parse_table_[138][79] = Entry(75);  // Shift
  parse_table_[138][80] = Entry(76);  // Shift
  parse_table_[138][84] = Entry(77);  // Shift
  parse_table_[138][85] = Entry(47);  // Shift
  parse_table_[138][86] = Entry(48);  // Shift
  parse_table_[138][87] = Entry(49);  // Shift
  parse_table_[138][88] = Entry(50);  // Shift
  parse_table_[138][91] = Entry(51);  // Shift
  parse_table_[138][93] = Entry(52);  // Shift
  parse_table_[138][95] = Entry(53);  // Shift
  parse_table_[138][96] = Entry(54);  // Shift
  parse_table_[138][97] = Entry(55);  // Shift
  parse_table_[138][98] = Entry(56);  // Shift
  parse_table_[138][99] = Entry(57);  // Shift
  parse_table_[138][100] = Entry(58);  // Shift
  parse_table_[139][1] = Entry(34);  // Shift
  parse_table_[139][2] = Entry(35);  // Shift
  parse_table_[139][3] = Entry(36);  // Shift
  parse_table_[139][4] = Entry(37);  // Shift
  parse_table_[139][8] = Entry(61);  // Shift
  parse_table_[139][17] = Entry(38);  // Shift
  parse_table_[139][19] = Entry(62);  // Shift
  parse_table_[139][21] = Entry(63);  // Shift
  parse_table_[139][24] = Entry(64);  // Shift
  parse_table_[139][26] = Entry(65);  // Shift
  parse_table_[139][30] = Entry(66);  // Shift
  parse_table_[139][31] = Entry(67);  // Shift
  parse_table_[139][40] = Entry(39);  // Shift
  parse_table_[139][50] = Entry(40);  // Shift
  parse_table_[139][51] = Entry(41);  // Shift
  parse_table_[139][54] = Entry(42);  // Shift
  parse_table_[139][55] = Entry(43);  // Shift
  parse_table_[139][56] = Entry(44);  // Shift
  parse_table_[139][67] = Entry(68);  // Shift
  parse_table_[139][69] = Entry(155);  // Shift
  parse_table_[139][74] = Entry(70);  // Shift
  parse_table_[139][75] = Entry(71);  // Shift
  parse_table_[139][76] = Entry(72);  // Shift
  parse_table_[139][77] = Entry(73);  // Shift
  parse_table_[139][78] = Entry(74);  // Shift
  parse_table_[139][79] = Entry(75);  // Shift
  parse_table_[139][80] = Entry(76);  // Shift
  parse_table_[139][84] = Entry(77);  // Shift
  parse_table_[139][85] = Entry(47);  // Shift
  parse_table_[139][86] = Entry(48);  // Shift
  parse_table_[139][87] = Entry(49);  // Shift
  parse_table_[139][88] = Entry(50);  // Shift
  parse_table_[139][91] = Entry(51);  // Shift
  parse_table_[139][93] = Entry(52);  // Shift
  parse_table_[139][95] = Entry(53);  // Shift
  parse_table_[139][96] = Entry(54);  // Shift
  parse_table_[139][97] = Entry(55);  // Shift
  parse_table_[139][98] = Entry(56);  // Shift
  parse_table_[139][99] = Entry(57);  // Shift
  parse_table_[139][100] = Entry(58);  // Shift
  parse_table_[140][1] = Entry(34);  // Shift
  parse_table_[140][2] = Entry(35);  // Shift
  parse_table_[140][3] = Entry(36);  // Shift
  parse_table_[140][4] = Entry(37);  // Shift
  parse_table_[140][17] = Entry(38);  // Shift
  parse_table_[140][40] = Entry(39);  // Shift
  parse_table_[140][50] = Entry(40);  // Shift
  parse_table_[140][51] = Entry(41);  // Shift
  parse_table_[140][54] = Entry(42);  // Shift
  parse_table_[140][55] = Entry(43);  // Shift
  parse_table_[140][56] = Entry(44);  // Shift
  parse_table_[140][67] = Entry(156);  // Shift
  parse_table_[140][83] = Entry(157);  // Shift
  parse_table_[140][84] = Entry(46);  // Shift
  parse_table_[140][85] = Entry(47);  // Shift
  parse_table_[140][86] = Entry(48);  // Shift
  parse_table_[140][87] = Entry(49);  // Shift
  parse_table_[140][88] = Entry(50);  // Shift
  parse_table_[140][91] = Entry(51);  // Shift
  parse_table_[140][93] = Entry(52);  // Shift
  parse_table_[140][95] = Entry(53);  // Shift
  parse_table_[140][96] = Entry(54);  // Shift
  parse_table_[140][97] = Entry(55);  // Shift
  parse_table_[140][98] = Entry(56);  // Shift
  parse_table_[140][99] = Entry(57);  // Shift
  parse_table_[140][100] = Entry(58);  // Shift
  parse_table_[141][0] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][1] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][2] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][3] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][4] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][8] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][14] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][15] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][16] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][17] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][19] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][20] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][21] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][23] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][24] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][26] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][30] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][31] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][40] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][50] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][51] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][54] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][55] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[141][56] = Entry(ProductionRule(78, {30, 80, 8}), 48);  // Reduce
  parse_table_[142][8] = Entry(ProductionRule(80, {84, 27, 80}), 50);  // Reduce
  parse_table_[142][10] = Entry(ProductionRule(80, {84, 27, 80}), 50);  // Reduce
  parse_table_[142][13] = Entry(ProductionRule(80, {84, 27, 80}), 50);  // Reduce
  parse_table_[142][18] = Entry(ProductionRule(80, {84, 27, 80}), 50);  // Reduce
  parse_table_[143][8] = Entry(ProductionRule(80, {84, 32, 80}), 51);  // Reduce
  parse_table_[143][10] = Entry(ProductionRule(80, {84, 32, 80}), 51);  // Reduce
  parse_table_[143][13] = Entry(ProductionRule(80, {84, 32, 80}), 51);  // Reduce
  parse_table_[143][18] = Entry(ProductionRule(80, {84, 32, 80}), 51);  // Reduce
  parse_table_[144][8] = Entry(ProductionRule(80, {84, 33, 80}), 52);  // Reduce
  parse_table_[144][10] = Entry(ProductionRule(80, {84, 33, 80}), 52);  // Reduce
  parse_table_[144][13] = Entry(ProductionRule(80, {84, 33, 80}), 52);  // Reduce
  parse_table_[144][18] = Entry(ProductionRule(80, {84, 33, 80}), 52);  // Reduce
  parse_table_[145][8] = Entry(ProductionRule(80, {84, 34, 80}), 53);  // Reduce
  parse_table_[145][10] = Entry(ProductionRule(80, {84, 34, 80}), 53);  // Reduce
  parse_table_[145][13] = Entry(ProductionRule(80, {84, 34, 80}), 53);  // Reduce
  parse_table_[145][18] = Entry(ProductionRule(80, {84, 34, 80}), 53);  // Reduce
  parse_table_[146][8] = Entry(ProductionRule(80, {84, 35, 80}), 54);  // Reduce
  parse_table_[146][10] = Entry(ProductionRule(80, {84, 35, 80}), 54);  // Reduce
  parse_table_[146][13] = Entry(ProductionRule(80, {84, 35, 80}), 54);  // Reduce
  parse_table_[146][18] = Entry(ProductionRule(80, {84, 35, 80}), 54);  // Reduce
  parse_table_[147][8] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][10] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][13] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][18] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][22] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][25] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][27] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][28] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][29] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][32] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][33] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][34] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][35] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][36] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][37] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][38] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][39] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][41] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][42] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][43] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][44] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][45] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][46] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][47] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][48] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][49] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][50] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][51] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][52] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[147][53] = Entry(ProductionRule(84, {1, 12, 80, 13}), 93);  // Reduce
  parse_table_[148][8] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][10] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][13] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][18] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][22] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][25] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][28] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][29] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][38] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][39] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][41] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][42] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][43] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][44] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][45] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][46] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][47] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][48] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][49] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][50] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][51] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][52] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[148][53] = Entry(ProductionRule(99, {1, 17, 101, 18}), 97);  // Reduce
  parse_table_[149][1] = Entry(34);  // Shift
  parse_table_[149][2] = Entry(35);  // Shift
  parse_table_[149][3] = Entry(36);  // Shift
  parse_table_[149][4] = Entry(37);  // Shift
  parse_table_[149][17] = Entry(38);  // Shift
  parse_table_[149][40] = Entry(39);  // Shift
  parse_table_[149][50] = Entry(40);  // Shift
  parse_table_[149][51] = Entry(41);  // Shift
  parse_table_[149][54] = Entry(42);  // Shift
  parse_table_[149][55] = Entry(43);  // Shift
  parse_table_[149][56] = Entry(44);  // Shift
  parse_table_[149][67] = Entry(68);  // Shift
  parse_table_[149][80] = Entry(158);  // Shift
  parse_table_[149][84] = Entry(77);  // Shift
  parse_table_[149][85] = Entry(47);  // Shift
  parse_table_[149][86] = Entry(48);  // Shift
  parse_table_[149][87] = Entry(49);  // Shift
  parse_table_[149][88] = Entry(50);  // Shift
  parse_table_[149][91] = Entry(51);  // Shift
  parse_table_[149][93] = Entry(52);  // Shift
  parse_table_[149][95] = Entry(53);  // Shift
  parse_table_[149][96] = Entry(54);  // Shift
  parse_table_[149][97] = Entry(55);  // Shift
  parse_table_[149][98] = Entry(56);  // Shift
  parse_table_[149][99] = Entry(57);  // Shift
  parse_table_[149][100] = Entry(58);  // Shift
  parse_table_[150][1] = Entry(32);  // Shift
  parse_table_[150][63] = Entry(159);  // Shift
  parse_table_[150][65] = Entry(14);  // Shift
  parse_table_[150][66] = Entry(15);  // Shift
  parse_table_[151][8] = Entry(160);  // Shift
  parse_table_[151][10] = Entry(23);  // Shift
  parse_table_[152][0] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][1] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][2] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][3] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][4] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][8] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][14] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][15] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][16] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][17] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][19] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][20] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][21] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][23] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][24] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][26] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][30] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][31] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][40] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][50] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][51] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][54] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][55] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[152][56] = Entry(ProductionRule(75, {19, 81, 82, 20}), 36);  // Reduce
  parse_table_[153][1] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][2] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][3] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][4] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][8] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][17] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][19] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][20] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][21] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][24] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][26] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][30] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][31] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][40] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][50] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][51] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][54] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][55] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[153][56] = Entry(ProductionRule(82, {82, 69}), 39);  // Reduce
  parse_table_[154][0] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][1] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][2] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][3] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][4] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][8] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][14] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][15] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][16] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][17] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][19] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][20] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][21] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][23] = Entry(161);  // Shift
  parse_table_[154][24] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][26] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][30] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][31] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][40] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][50] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][51] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][54] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][55] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[154][56] = Entry(ProductionRule(76, {21, 67, 22, 69}), 41);  // Reduce
  parse_table_[155][0] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][1] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][2] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][3] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][4] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][8] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][14] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][15] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][16] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][17] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][19] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][20] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][21] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][23] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][24] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][26] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][30] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][31] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][40] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][50] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][51] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][54] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][55] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[155][56] = Entry(ProductionRule(77, {24, 67, 25, 69}), 43);  // Reduce
  parse_table_[156][28] = Entry(162);  // Shift
  parse_table_[156][38] = Entry(85);  // Shift
  parse_table_[157][25] = Entry(163);  // Shift
  parse_table_[158][10] = Entry(ProductionRule(102, {102, 10, 80}), 100);  // Reduce
  parse_table_[158][18] = Entry(ProductionRule(102, {102, 10, 80}), 100);  // Reduce
  parse_table_[159][8] = Entry(164);  // Shift
  parse_table_[159][10] = Entry(23);  // Shift
  parse_table_[160][1] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][2] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][3] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][4] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][8] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][9] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][14] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][15] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][16] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][17] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][19] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][20] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][21] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][24] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][26] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][30] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][31] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][40] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][50] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][51] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][54] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][55] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[160][56] = Entry(ProductionRule(64, {62, 63, 8}), 7);  // Reduce
  parse_table_[161][1] = Entry(34);  // Shift
  parse_table_[161][2] = Entry(35);  // Shift
  parse_table_[161][3] = Entry(36);  // Shift
  parse_table_[161][4] = Entry(37);  // Shift
  parse_table_[161][8] = Entry(61);  // Shift
  parse_table_[161][17] = Entry(38);  // Shift
  parse_table_[161][19] = Entry(62);  // Shift
  parse_table_[161][21] = Entry(63);  // Shift
  parse_table_[161][24] = Entry(64);  // Shift
  parse_table_[161][26] = Entry(65);  // Shift
  parse_table_[161][30] = Entry(66);  // Shift
  parse_table_[161][31] = Entry(67);  // Shift
  parse_table_[161][40] = Entry(39);  // Shift
  parse_table_[161][50] = Entry(40);  // Shift
  parse_table_[161][51] = Entry(41);  // Shift
  parse_table_[161][54] = Entry(42);  // Shift
  parse_table_[161][55] = Entry(43);  // Shift
  parse_table_[161][56] = Entry(44);  // Shift
  parse_table_[161][67] = Entry(68);  // Shift
  parse_table_[161][69] = Entry(165);  // Shift
  parse_table_[161][74] = Entry(70);  // Shift
  parse_table_[161][75] = Entry(71);  // Shift
  parse_table_[161][76] = Entry(72);  // Shift
  parse_table_[161][77] = Entry(73);  // Shift
  parse_table_[161][78] = Entry(74);  // Shift
  parse_table_[161][79] = Entry(75);  // Shift
  parse_table_[161][80] = Entry(76);  // Shift
  parse_table_[161][84] = Entry(77);  // Shift
  parse_table_[161][85] = Entry(47);  // Shift
  parse_table_[161][86] = Entry(48);  // Shift
  parse_table_[161][87] = Entry(49);  // Shift
  parse_table_[161][88] = Entry(50);  // Shift
  parse_table_[161][91] = Entry(51);  // Shift
  parse_table_[161][93] = Entry(52);  // Shift
  parse_table_[161][95] = Entry(53);  // Shift
  parse_table_[161][96] = Entry(54);  // Shift
  parse_table_[161][97] = Entry(55);  // Shift
  parse_table_[161][98] = Entry(56);  // Shift
  parse_table_[161][99] = Entry(57);  // Shift
  parse_table_[161][100] = Entry(58);  // Shift
  parse_table_[162][1] = Entry(34);  // Shift
  parse_table_[162][2] = Entry(35);  // Shift
  parse_table_[162][3] = Entry(36);  // Shift
  parse_table_[162][4] = Entry(37);  // Shift
  parse_table_[162][17] = Entry(38);  // Shift
  parse_table_[162][40] = Entry(39);  // Shift
  parse_table_[162][50] = Entry(40);  // Shift
  parse_table_[162][51] = Entry(41);  // Shift
  parse_table_[162][54] = Entry(42);  // Shift
  parse_table_[162][55] = Entry(43);  // Shift
  parse_table_[162][56] = Entry(44);  // Shift
  parse_table_[162][67] = Entry(166);  // Shift
  parse_table_[162][84] = Entry(46);  // Shift
  parse_table_[162][85] = Entry(47);  // Shift
  parse_table_[162][86] = Entry(48);  // Shift
  parse_table_[162][87] = Entry(49);  // Shift
  parse_table_[162][88] = Entry(50);  // Shift
  parse_table_[162][91] = Entry(51);  // Shift
  parse_table_[162][93] = Entry(52);  // Shift
  parse_table_[162][95] = Entry(53);  // Shift
  parse_table_[162][96] = Entry(54);  // Shift
  parse_table_[162][97] = Entry(55);  // Shift
  parse_table_[162][98] = Entry(56);  // Shift
  parse_table_[162][99] = Entry(57);  // Shift
  parse_table_[162][100] = Entry(58);  // Shift
  parse_table_[163][1] = Entry(34);  // Shift
  parse_table_[163][2] = Entry(35);  // Shift
  parse_table_[163][3] = Entry(36);  // Shift
  parse_table_[163][4] = Entry(37);  // Shift
  parse_table_[163][8] = Entry(61);  // Shift
  parse_table_[163][17] = Entry(38);  // Shift
  parse_table_[163][19] = Entry(62);  // Shift
  parse_table_[163][21] = Entry(63);  // Shift
  parse_table_[163][24] = Entry(64);  // Shift
  parse_table_[163][26] = Entry(65);  // Shift
  parse_table_[163][30] = Entry(66);  // Shift
  parse_table_[163][31] = Entry(67);  // Shift
  parse_table_[163][40] = Entry(39);  // Shift
  parse_table_[163][50] = Entry(40);  // Shift
  parse_table_[163][51] = Entry(41);  // Shift
  parse_table_[163][54] = Entry(42);  // Shift
  parse_table_[163][55] = Entry(43);  // Shift
  parse_table_[163][56] = Entry(44);  // Shift
  parse_table_[163][67] = Entry(68);  // Shift
  parse_table_[163][69] = Entry(167);  // Shift
  parse_table_[163][74] = Entry(70);  // Shift
  parse_table_[163][75] = Entry(71);  // Shift
  parse_table_[163][76] = Entry(72);  // Shift
  parse_table_[163][77] = Entry(73);  // Shift
  parse_table_[163][78] = Entry(74);  // Shift
  parse_table_[163][79] = Entry(75);  // Shift
  parse_table_[163][80] = Entry(76);  // Shift
  parse_table_[163][84] = Entry(77);  // Shift
  parse_table_[163][85] = Entry(47);  // Shift
  parse_table_[163][86] = Entry(48);  // Shift
  parse_table_[163][87] = Entry(49);  // Shift
  parse_table_[163][88] = Entry(50);  // Shift
  parse_table_[163][91] = Entry(51);  // Shift
  parse_table_[163][93] = Entry(52);  // Shift
  parse_table_[163][95] = Entry(53);  // Shift
  parse_table_[163][96] = Entry(54);  // Shift
  parse_table_[163][97] = Entry(55);  // Shift
  parse_table_[163][98] = Entry(56);  // Shift
  parse_table_[163][99] = Entry(57);  // Shift
  parse_table_[163][100] = Entry(58);  // Shift
  parse_table_[164][1] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][2] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][3] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][4] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][8] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][9] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][14] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][15] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][16] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][17] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][19] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][20] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][21] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][24] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][26] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][30] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][31] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][40] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][50] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][51] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][54] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][55] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[164][56] = Entry(ProductionRule(64, {9, 62, 63, 8}), 6);  // Reduce
  parse_table_[165][0] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][1] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][2] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][3] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][4] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][8] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][14] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][15] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][16] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][17] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][19] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][20] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][21] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][23] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][24] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][26] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][30] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][31] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][40] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][50] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][51] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][54] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][55] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[165][56] = Entry(ProductionRule(76, {21, 67, 22, 69, 23, 69}), 42);  // Reduce
  parse_table_[166][25] = Entry(ProductionRule(83, {67, 28, 67}), 45);  // Reduce
  parse_table_[166][29] = Entry(168);  // Shift
  parse_table_[166][38] = Entry(85);  // Shift
  parse_table_[167][0] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][1] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][2] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][3] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][4] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][8] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][14] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][15] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][16] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][17] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][19] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][20] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][21] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][23] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][24] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][26] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][30] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][31] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][40] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][50] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][51] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][54] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][55] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[167][56] = Entry(ProductionRule(77, {26, 1, 27, 83, 25, 69}), 44);  // Reduce
  parse_table_[168][1] = Entry(34);  // Shift
  parse_table_[168][2] = Entry(35);  // Shift
  parse_table_[168][3] = Entry(36);  // Shift
  parse_table_[168][4] = Entry(37);  // Shift
  parse_table_[168][17] = Entry(38);  // Shift
  parse_table_[168][40] = Entry(39);  // Shift
  parse_table_[168][50] = Entry(40);  // Shift
  parse_table_[168][51] = Entry(41);  // Shift
  parse_table_[168][54] = Entry(42);  // Shift
  parse_table_[168][55] = Entry(43);  // Shift
  parse_table_[168][56] = Entry(44);  // Shift
  parse_table_[168][67] = Entry(169);  // Shift
  parse_table_[168][84] = Entry(46);  // Shift
  parse_table_[168][85] = Entry(47);  // Shift
  parse_table_[168][86] = Entry(48);  // Shift
  parse_table_[168][87] = Entry(49);  // Shift
  parse_table_[168][88] = Entry(50);  // Shift
  parse_table_[168][91] = Entry(51);  // Shift
  parse_table_[168][93] = Entry(52);  // Shift
  parse_table_[168][95] = Entry(53);  // Shift
  parse_table_[168][96] = Entry(54);  // Shift
  parse_table_[168][97] = Entry(55);  // Shift
  parse_table_[168][98] = Entry(56);  // Shift
  parse_table_[168][99] = Entry(57);  // Shift
  parse_table_[168][100] = Entry(58);  // Shift
  parse_table_[169][25] = Entry(ProductionRule(83, {67, 28, 67, 29, 67}), 46);  // Reduce
  parse_table_[169][38] = Entry(85);  // Shift

  // Create inverse non-terminal map.
  inverse_nonterminal_map_.emplace(57, "program");
  inverse_nonterminal_map_.emplace(58, "declList");
  inverse_nonterminal_map_.emplace(59, "decl");
  inverse_nonterminal_map_.emplace(60, "varDecl");
  inverse_nonterminal_map_.emplace(61, "funDecl");
  inverse_nonterminal_map_.emplace(62, "typeSpec");
  inverse_nonterminal_map_.emplace(63, "varDeclList");
  inverse_nonterminal_map_.emplace(64, "scopedVarDecl");
  inverse_nonterminal_map_.emplace(65, "varDeclInit");
  inverse_nonterminal_map_.emplace(66, "varDeclId");
  inverse_nonterminal_map_.emplace(67, "simpleExp");
  inverse_nonterminal_map_.emplace(68, "parms");
  inverse_nonterminal_map_.emplace(69, "stmt");
  inverse_nonterminal_map_.emplace(70, "parmList");
  inverse_nonterminal_map_.emplace(71, "parmTypeList");
  inverse_nonterminal_map_.emplace(72, "parmIdList");
  inverse_nonterminal_map_.emplace(73, "parmId");
  inverse_nonterminal_map_.emplace(74, "expStmt");
  inverse_nonterminal_map_.emplace(75, "compoundStmt");
  inverse_nonterminal_map_.emplace(76, "selectStmt");
  inverse_nonterminal_map_.emplace(77, "iterStmt");
  inverse_nonterminal_map_.emplace(78, "returnStmt");
  inverse_nonterminal_map_.emplace(79, "breakStmt");
  inverse_nonterminal_map_.emplace(80, "exp");
  inverse_nonterminal_map_.emplace(81, "localDecls");
  inverse_nonterminal_map_.emplace(82, "stmtList");
  inverse_nonterminal_map_.emplace(83, "iterRange");
  inverse_nonterminal_map_.emplace(84, "mutable");
  inverse_nonterminal_map_.emplace(85, "andExp");
  inverse_nonterminal_map_.emplace(86, "unaryRelExp");
  inverse_nonterminal_map_.emplace(87, "relExp");
  inverse_nonterminal_map_.emplace(88, "minmaxExp");
  inverse_nonterminal_map_.emplace(89, "relop");
  inverse_nonterminal_map_.emplace(90, "minmaxop");
  inverse_nonterminal_map_.emplace(91, "sumExp");
  inverse_nonterminal_map_.emplace(92, "sumop");
  inverse_nonterminal_map_.emplace(93, "mulExp");
  inverse_nonterminal_map_.emplace(94, "mulop");
  inverse_nonterminal_map_.emplace(95, "unaryExp");
  inverse_nonterminal_map_.emplace(96, "unaryop");
  inverse_nonterminal_map_.emplace(97, "factor");
  inverse_nonterminal_map_.emplace(98, "immutable");
  inverse_nonterminal_map_.emplace(99, "call");
  inverse_nonterminal_map_.emplace(100, "constant");
  inverse_nonterminal_map_.emplace(101, "args");
  inverse_nonterminal_map_.emplace(102, "argList");

  createLexer();
}

inline std::shared_ptr<ASTNodeBase> Parser::ParseInput() {
  return parse();
}

inline void Parser::createLexer() {
  auto lexer_generator = std::make_shared<manta::LexerGenerator>();

  // Lexeme "eof" will be automatically added as the first (0-th) lexeme.
  lexer_generator->AddLexeme("ID", "\\@ (\\@ | \\d)*", 1);  // Lexeme #1
  lexer_generator->AddLexeme("NUMCONST", "\\d+", 1);  // Lexeme #2
  lexer_generator->AddLexeme("CHARCONST", "\\'(\\d | \\@)\\'", 1);  // Lexeme #3
  lexer_generator->AddLexeme("STRINGCONST", "\\\"(\\d | \\@)*\\\"", 1);  // Lexeme #4
  lexer_generator->AddLexeme("comment", "// [~\\n]* \\n", 1);  // Lexeme #5
  lexer_generator->AddLexeme("spaces", "\\s+", 1);  // Lexeme #6
  lexer_generator->AddLexeme("newlines", "\\n+", 1);  // Lexeme #7
  lexer_generator->AddReserved(";", 2);  // Lexeme #8
  lexer_generator->AddReserved("static", 2);  // Lexeme #9
  lexer_generator->AddReserved(",", 2);  // Lexeme #10
  lexer_generator->AddReserved(":", 2);  // Lexeme #11
  lexer_generator->AddReserved("[", 2);  // Lexeme #12
  lexer_generator->AddReserved("]", 2);  // Lexeme #13
  lexer_generator->AddReserved("int", 2);  // Lexeme #14
  lexer_generator->AddReserved("bool", 2);  // Lexeme #15
  lexer_generator->AddReserved("char", 2);  // Lexeme #16
  lexer_generator->AddReserved("(", 2);  // Lexeme #17
  lexer_generator->AddReserved(")", 2);  // Lexeme #18
  lexer_generator->AddReserved("{", 2);  // Lexeme #19
  lexer_generator->AddReserved("}", 2);  // Lexeme #20
  lexer_generator->AddReserved("if", 2);  // Lexeme #21
  lexer_generator->AddReserved("then", 2);  // Lexeme #22
  lexer_generator->AddReserved("else", 2);  // Lexeme #23
  lexer_generator->AddReserved("while", 2);  // Lexeme #24
  lexer_generator->AddReserved("do", 2);  // Lexeme #25
  lexer_generator->AddReserved("for", 2);  // Lexeme #26
  lexer_generator->AddReserved("=", 2);  // Lexeme #27
  lexer_generator->AddReserved("to", 2);  // Lexeme #28
  lexer_generator->AddReserved("by", 2);  // Lexeme #29
  lexer_generator->AddReserved("return", 2);  // Lexeme #30
  lexer_generator->AddReserved("break", 2);  // Lexeme #31
  lexer_generator->AddReserved("+=", 2);  // Lexeme #32
  lexer_generator->AddReserved("-=", 2);  // Lexeme #33
  lexer_generator->AddReserved("*=", 2);  // Lexeme #34
  lexer_generator->AddReserved("/=", 2);  // Lexeme #35
  lexer_generator->AddReserved("++", 2);  // Lexeme #36
  lexer_generator->AddReserved("−−", 2);  // Lexeme #37
  lexer_generator->AddReserved("or", 2);  // Lexeme #38
  lexer_generator->AddReserved("and", 2);  // Lexeme #39
  lexer_generator->AddReserved("not", 2);  // Lexeme #40
  lexer_generator->AddReserved("<=", 2);  // Lexeme #41
  lexer_generator->AddReserved("<", 2);  // Lexeme #42
  lexer_generator->AddReserved(">", 2);  // Lexeme #43
  lexer_generator->AddReserved(">=", 2);  // Lexeme #44
  lexer_generator->AddReserved("==", 2);  // Lexeme #45
  lexer_generator->AddReserved("!=", 2);  // Lexeme #46
  lexer_generator->AddReserved(":>:", 2);  // Lexeme #47
  lexer_generator->AddReserved(":<:", 2);  // Lexeme #48
  lexer_generator->AddReserved("+", 2);  // Lexeme #49
  lexer_generator->AddReserved("-", 2);  // Lexeme #50
  lexer_generator->AddReserved("*", 2);  // Lexeme #51
  lexer_generator->AddReserved("/", 2);  // Lexeme #52
  lexer_generator->AddReserved("%", 2);  // Lexeme #53
  lexer_generator->AddReserved("?", 2);  // Lexeme #54
  lexer_generator->AddReserved("true", 2);  // Lexeme #55
  lexer_generator->AddReserved("false", 2);  // Lexeme #56

  // Add the skip-lexemes (these will be lexed, but skipped, by the lexer).
  lexer_generator->AddSkip("comment");
  lexer_generator->AddSkip("spaces");
  lexer_generator->AddSkip("newlines");

  lexer_ = lexer_generator->CreateLexer();
}

inline std::shared_ptr<ASTNodeBase> Parser::reduce(unsigned reduction_id, const std::vector<std::shared_ptr<ASTNodeBase>>& collected_nodes) {
  switch (reduction_id) {
    case 0: {
      REDUCE_ASSERT(1, 0, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_0_program_ViaItem_0'.";
      return ReduceTo_ASTNodeGeneral_0_program_ViaItem_0(
          std::reinterpret_pointer_cast<ASTNodeBase_declList>(collected_nodes[0]));
    }
    case 1: {
      REDUCE_ASSERT(2, 1, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_1_declList_ViaItem_1'.";
      return ReduceTo_ASTNodeGeneral_1_declList_ViaItem_1(
          std::reinterpret_pointer_cast<ASTNodeBase_declList>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_decl>(collected_nodes[1]));
    }
    case 2: {
      REDUCE_ASSERT(1, 2, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_2_declList_ViaItem_2'.";
      return ReduceTo_ASTNodeGeneral_2_declList_ViaItem_2(
          std::reinterpret_pointer_cast<ASTNodeBase_decl>(collected_nodes[0]));
    }
    case 3: {
      REDUCE_ASSERT(1, 3, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_3_decl_ViaItem_3'.";
      return ReduceTo_ASTNodeGeneral_3_decl_ViaItem_3(
          std::reinterpret_pointer_cast<ASTNodeGeneral_5_varDecl>(collected_nodes[0]));
    }
    case 4: {
      REDUCE_ASSERT(1, 4, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_4_decl_ViaItem_4'.";
      return ReduceTo_ASTNodeGeneral_4_decl_ViaItem_4(
          std::reinterpret_pointer_cast<ASTNodeBase_funDecl>(collected_nodes[0]));
    }
    case 5: {
      REDUCE_ASSERT(3, 5, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_5_varDecl_ViaItem_5'.";
      return ReduceTo_ASTNodeGeneral_5_varDecl_ViaItem_5(
          std::reinterpret_pointer_cast<ASTNodeBase_typeSpec>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_varDeclList>(collected_nodes[1]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 6: {
      REDUCE_ASSERT(4, 6, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_6_scopedVarDecl_ViaItem_6'.";
      return ReduceTo_ASTNodeGeneral_6_scopedVarDecl_ViaItem_6(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_typeSpec>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_varDeclList>(collected_nodes[2]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 7: {
      REDUCE_ASSERT(3, 7, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_7_scopedVarDecl_ViaItem_7'.";
      return ReduceTo_ASTNodeGeneral_7_scopedVarDecl_ViaItem_7(
          std::reinterpret_pointer_cast<ASTNodeBase_typeSpec>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_varDeclList>(collected_nodes[1]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 8: {
      REDUCE_ASSERT(3, 8, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_8_varDeclList_ViaItem_8'.";
      return ReduceTo_ASTNodeGeneral_8_varDeclList_ViaItem_8(
          std::reinterpret_pointer_cast<ASTNodeBase_varDeclList>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_varDeclInit>(collected_nodes[2]));
    }
    case 9: {
      REDUCE_ASSERT(1, 9, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_9_varDeclList_ViaItem_9'.";
      return ReduceTo_ASTNodeGeneral_9_varDeclList_ViaItem_9(
          std::reinterpret_pointer_cast<ASTNodeBase_varDeclInit>(collected_nodes[0]));
    }
    case 10: {
      REDUCE_ASSERT(1, 10, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_10_varDeclInit_ViaItem_10'.";
      return ReduceTo_ASTNodeGeneral_10_varDeclInit_ViaItem_10(
          std::reinterpret_pointer_cast<ASTNodeBase_varDeclId>(collected_nodes[0]));
    }
    case 11: {
      REDUCE_ASSERT(3, 11, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_11_varDeclInit_ViaItem_11'.";
      return ReduceTo_ASTNodeGeneral_11_varDeclInit_ViaItem_11(
          std::reinterpret_pointer_cast<ASTNodeBase_varDeclId>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[2]));
    }
    case 12: {
      REDUCE_ASSERT(1, 12, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_12_varDeclId_ViaItem_12'.";
      return ReduceTo_ASTNodeGeneral_12_varDeclId_ViaItem_12(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 13: {
      REDUCE_ASSERT(4, 13, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_13_varDeclId_ViaItem_13'.";
      return ReduceTo_ASTNodeGeneral_13_varDeclId_ViaItem_13(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 14: {
      REDUCE_ASSERT(1, 14, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_14_typeSpec_ViaItem_14'.";
      return ReduceTo_ASTNodeGeneral_14_typeSpec_ViaItem_14(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 15: {
      REDUCE_ASSERT(1, 15, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_15_typeSpec_ViaItem_15'.";
      return ReduceTo_ASTNodeGeneral_15_typeSpec_ViaItem_15(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 16: {
      REDUCE_ASSERT(1, 16, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_16_typeSpec_ViaItem_16'.";
      return ReduceTo_ASTNodeGeneral_16_typeSpec_ViaItem_16(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 17: {
      REDUCE_ASSERT(6, 17, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_17_funDecl_ViaItem_17'.";
      return ReduceTo_ASTNodeGeneral_17_funDecl_ViaItem_17(
          std::reinterpret_pointer_cast<ASTNodeBase_typeSpec>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_parms>(collected_nodes[3]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[4].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_stmt>(collected_nodes[5]));
    }
    case 18: {
      REDUCE_ASSERT(5, 18, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_18_funDecl_ViaItem_18'.";
      return ReduceTo_ASTNodeGeneral_18_funDecl_ViaItem_18(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_parms>(collected_nodes[2]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_stmt>(collected_nodes[4]));
    }
    case 19: {
      REDUCE_ASSERT(1, 19, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_19_parms_ViaItem_19'.";
      return ReduceTo_ASTNodeGeneral_19_parms_ViaItem_19(
          std::reinterpret_pointer_cast<ASTNodeBase_parmList>(collected_nodes[0]));
    }
    case 20: {
      REDUCE_ASSERT(0, 20, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_20_parms_ViaItem_20'.";
      return ReduceTo_ASTNodeGeneral_20_parms_ViaItem_20();
    }
    case 21: {
      REDUCE_ASSERT(3, 21, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_21_parmList_ViaItem_21'.";
      return ReduceTo_ASTNodeGeneral_21_parmList_ViaItem_21(
          std::reinterpret_pointer_cast<ASTNodeBase_parmList>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeGeneral_23_parmTypeList>(collected_nodes[2]));
    }
    case 22: {
      REDUCE_ASSERT(1, 22, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_22_parmList_ViaItem_22'.";
      return ReduceTo_ASTNodeGeneral_22_parmList_ViaItem_22(
          std::reinterpret_pointer_cast<ASTNodeGeneral_23_parmTypeList>(collected_nodes[0]));
    }
    case 23: {
      REDUCE_ASSERT(2, 23, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_23_parmTypeList_ViaItem_23'.";
      return ReduceTo_ASTNodeGeneral_23_parmTypeList_ViaItem_23(
          std::reinterpret_pointer_cast<ASTNodeBase_typeSpec>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_parmIdList>(collected_nodes[1]));
    }
    case 24: {
      REDUCE_ASSERT(3, 24, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_24_parmIdList_ViaItem_24'.";
      return ReduceTo_ASTNodeGeneral_24_parmIdList_ViaItem_24(
          std::reinterpret_pointer_cast<ASTNodeBase_parmIdList>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_parmId>(collected_nodes[2]));
    }
    case 25: {
      REDUCE_ASSERT(1, 25, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_25_parmIdList_ViaItem_25'.";
      return ReduceTo_ASTNodeGeneral_25_parmIdList_ViaItem_25(
          std::reinterpret_pointer_cast<ASTNodeBase_parmId>(collected_nodes[0]));
    }
    case 26: {
      REDUCE_ASSERT(1, 26, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_26_parmId_ViaItem_26'.";
      return ReduceTo_ASTNodeGeneral_26_parmId_ViaItem_26(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 27: {
      REDUCE_ASSERT(3, 27, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_27_parmId_ViaItem_27'.";
      return ReduceTo_ASTNodeGeneral_27_parmId_ViaItem_27(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 28: {
      REDUCE_ASSERT(1, 28, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_28_stmt_ViaItem_28'.";
      return ReduceTo_ASTNodeGeneral_28_stmt_ViaItem_28(
          std::reinterpret_pointer_cast<ASTNodeBase_expStmt>(collected_nodes[0]));
    }
    case 29: {
      REDUCE_ASSERT(1, 29, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_29_stmt_ViaItem_29'.";
      return ReduceTo_ASTNodeGeneral_29_stmt_ViaItem_29(
          std::reinterpret_pointer_cast<ASTNodeGeneral_36_compoundStmt>(collected_nodes[0]));
    }
    case 30: {
      REDUCE_ASSERT(1, 30, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_30_stmt_ViaItem_30'.";
      return ReduceTo_ASTNodeGeneral_30_stmt_ViaItem_30(
          std::reinterpret_pointer_cast<ASTNodeBase_selectStmt>(collected_nodes[0]));
    }
    case 31: {
      REDUCE_ASSERT(1, 31, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_31_stmt_ViaItem_31'.";
      return ReduceTo_ASTNodeGeneral_31_stmt_ViaItem_31(
          std::reinterpret_pointer_cast<ASTNodeBase_iterStmt>(collected_nodes[0]));
    }
    case 32: {
      REDUCE_ASSERT(1, 32, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_32_stmt_ViaItem_32'.";
      return ReduceTo_ASTNodeGeneral_32_stmt_ViaItem_32(
          std::reinterpret_pointer_cast<ASTNodeBase_returnStmt>(collected_nodes[0]));
    }
    case 33: {
      REDUCE_ASSERT(1, 33, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_33_stmt_ViaItem_33'.";
      return ReduceTo_ASTNodeGeneral_33_stmt_ViaItem_33(
          std::reinterpret_pointer_cast<ASTNodeGeneral_49_breakStmt>(collected_nodes[0]));
    }
    case 34: {
      REDUCE_ASSERT(2, 34, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_34_expStmt_ViaItem_34'.";
      return ReduceTo_ASTNodeGeneral_34_expStmt_ViaItem_34(
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 35: {
      REDUCE_ASSERT(1, 35, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_35_expStmt_ViaItem_35'.";
      return ReduceTo_ASTNodeGeneral_35_expStmt_ViaItem_35(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 36: {
      REDUCE_ASSERT(4, 36, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_36_compoundStmt_ViaItem_36'.";
      return ReduceTo_ASTNodeGeneral_36_compoundStmt_ViaItem_36(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_localDecls>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_stmtList>(collected_nodes[2]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 37: {
      REDUCE_ASSERT(2, 37, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_37_localDecls_ViaItem_37'.";
      return ReduceTo_ASTNodeGeneral_37_localDecls_ViaItem_37(
          std::reinterpret_pointer_cast<ASTNodeBase_localDecls>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_scopedVarDecl>(collected_nodes[1]));
    }
    case 38: {
      REDUCE_ASSERT(0, 38, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_38_localDecls_ViaItem_38'.";
      return ReduceTo_ASTNodeGeneral_38_localDecls_ViaItem_38();
    }
    case 39: {
      REDUCE_ASSERT(2, 39, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_39_stmtList_ViaItem_39'.";
      return ReduceTo_ASTNodeGeneral_39_stmtList_ViaItem_39(
          std::reinterpret_pointer_cast<ASTNodeBase_stmtList>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_stmt>(collected_nodes[1]));
    }
    case 40: {
      REDUCE_ASSERT(0, 40, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_40_stmtList_ViaItem_40'.";
      return ReduceTo_ASTNodeGeneral_40_stmtList_ViaItem_40();
    }
    case 41: {
      REDUCE_ASSERT(4, 41, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_41_selectStmt_ViaItem_41'.";
      return ReduceTo_ASTNodeGeneral_41_selectStmt_ViaItem_41(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[1]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_stmt>(collected_nodes[3]));
    }
    case 42: {
      REDUCE_ASSERT(6, 42, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_42_selectStmt_ViaItem_42'.";
      return ReduceTo_ASTNodeGeneral_42_selectStmt_ViaItem_42(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[1]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_stmt>(collected_nodes[3]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[4].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_stmt>(collected_nodes[5]));
    }
    case 43: {
      REDUCE_ASSERT(4, 43, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_43_iterStmt_ViaItem_43'.";
      return ReduceTo_ASTNodeGeneral_43_iterStmt_ViaItem_43(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[1]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_stmt>(collected_nodes[3]));
    }
    case 44: {
      REDUCE_ASSERT(6, 44, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_44_iterStmt_ViaItem_44'.";
      return ReduceTo_ASTNodeGeneral_44_iterStmt_ViaItem_44(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_iterRange>(collected_nodes[3]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[4].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_stmt>(collected_nodes[5]));
    }
    case 45: {
      REDUCE_ASSERT(3, 45, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_45_iterRange_ViaItem_45'.";
      return ReduceTo_ASTNodeGeneral_45_iterRange_ViaItem_45(
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[2]));
    }
    case 46: {
      REDUCE_ASSERT(5, 46, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_46_iterRange_ViaItem_46'.";
      return ReduceTo_ASTNodeGeneral_46_iterRange_ViaItem_46(
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[2]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[4]));
    }
    case 47: {
      REDUCE_ASSERT(2, 47, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_47_returnStmt_ViaItem_47'.";
      return ReduceTo_ASTNodeGeneral_47_returnStmt_ViaItem_47(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 48: {
      REDUCE_ASSERT(3, 48, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_48_returnStmt_ViaItem_48'.";
      return ReduceTo_ASTNodeGeneral_48_returnStmt_ViaItem_48(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[1]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 49: {
      REDUCE_ASSERT(2, 49, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_49_breakStmt_ViaItem_49'.";
      return ReduceTo_ASTNodeGeneral_49_breakStmt_ViaItem_49(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 50: {
      REDUCE_ASSERT(3, 50, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_50_exp_ViaItem_50'.";
      return ReduceTo_ASTNodeGeneral_50_exp_ViaItem_50(
          std::reinterpret_pointer_cast<ASTNodeBase_mutable>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[2]));
    }
    case 51: {
      REDUCE_ASSERT(3, 51, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_51_exp_ViaItem_51'.";
      return ReduceTo_ASTNodeGeneral_51_exp_ViaItem_51(
          std::reinterpret_pointer_cast<ASTNodeBase_mutable>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[2]));
    }
    case 52: {
      REDUCE_ASSERT(3, 52, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_52_exp_ViaItem_52'.";
      return ReduceTo_ASTNodeGeneral_52_exp_ViaItem_52(
          std::reinterpret_pointer_cast<ASTNodeBase_mutable>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[2]));
    }
    case 53: {
      REDUCE_ASSERT(3, 53, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_53_exp_ViaItem_53'.";
      return ReduceTo_ASTNodeGeneral_53_exp_ViaItem_53(
          std::reinterpret_pointer_cast<ASTNodeBase_mutable>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[2]));
    }
    case 54: {
      REDUCE_ASSERT(3, 54, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_54_exp_ViaItem_54'.";
      return ReduceTo_ASTNodeGeneral_54_exp_ViaItem_54(
          std::reinterpret_pointer_cast<ASTNodeBase_mutable>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[2]));
    }
    case 55: {
      REDUCE_ASSERT(2, 55, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_55_exp_ViaItem_55'.";
      return ReduceTo_ASTNodeGeneral_55_exp_ViaItem_55(
          std::reinterpret_pointer_cast<ASTNodeBase_mutable>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 56: {
      REDUCE_ASSERT(2, 56, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_56_exp_ViaItem_56'.";
      return ReduceTo_ASTNodeGeneral_56_exp_ViaItem_56(
          std::reinterpret_pointer_cast<ASTNodeBase_mutable>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal);
    }
    case 57: {
      REDUCE_ASSERT(1, 57, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_57_exp_ViaItem_57'.";
      return ReduceTo_ASTNodeGeneral_57_exp_ViaItem_57(
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[0]));
    }
    case 58: {
      REDUCE_ASSERT(3, 58, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_58_simpleExp_ViaItem_58'.";
      return ReduceTo_ASTNodeGeneral_58_simpleExp_ViaItem_58(
          std::reinterpret_pointer_cast<ASTNodeBase_simpleExp>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_andExp>(collected_nodes[2]));
    }
    case 59: {
      REDUCE_ASSERT(1, 59, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_59_simpleExp_ViaItem_59'.";
      return ReduceTo_ASTNodeGeneral_59_simpleExp_ViaItem_59(
          std::reinterpret_pointer_cast<ASTNodeBase_andExp>(collected_nodes[0]));
    }
    case 60: {
      REDUCE_ASSERT(3, 60, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_60_andExp_ViaItem_60'.";
      return ReduceTo_ASTNodeGeneral_60_andExp_ViaItem_60(
          std::reinterpret_pointer_cast<ASTNodeBase_andExp>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_unaryRelExp>(collected_nodes[2]));
    }
    case 61: {
      REDUCE_ASSERT(1, 61, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_61_andExp_ViaItem_61'.";
      return ReduceTo_ASTNodeGeneral_61_andExp_ViaItem_61(
          std::reinterpret_pointer_cast<ASTNodeBase_unaryRelExp>(collected_nodes[0]));
    }
    case 62: {
      REDUCE_ASSERT(2, 62, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_62_unaryRelExp_ViaItem_62'.";
      return ReduceTo_ASTNodeGeneral_62_unaryRelExp_ViaItem_62(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_unaryRelExp>(collected_nodes[1]));
    }
    case 63: {
      REDUCE_ASSERT(1, 63, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_63_unaryRelExp_ViaItem_63'.";
      return ReduceTo_ASTNodeGeneral_63_unaryRelExp_ViaItem_63(
          std::reinterpret_pointer_cast<ASTNodeBase_relExp>(collected_nodes[0]));
    }
    case 64: {
      REDUCE_ASSERT(3, 64, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_64_relExp_ViaItem_64'.";
      return ReduceTo_ASTNodeGeneral_64_relExp_ViaItem_64(
          std::reinterpret_pointer_cast<ASTNodeBase_minmaxExp>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_relop>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_minmaxExp>(collected_nodes[2]));
    }
    case 65: {
      REDUCE_ASSERT(1, 65, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_65_relExp_ViaItem_65'.";
      return ReduceTo_ASTNodeGeneral_65_relExp_ViaItem_65(
          std::reinterpret_pointer_cast<ASTNodeBase_minmaxExp>(collected_nodes[0]));
    }
    case 66: {
      REDUCE_ASSERT(1, 66, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_66_relop_ViaItem_66'.";
      return ReduceTo_ASTNodeGeneral_66_relop_ViaItem_66(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 67: {
      REDUCE_ASSERT(1, 67, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_67_relop_ViaItem_67'.";
      return ReduceTo_ASTNodeGeneral_67_relop_ViaItem_67(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 68: {
      REDUCE_ASSERT(1, 68, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_68_relop_ViaItem_68'.";
      return ReduceTo_ASTNodeGeneral_68_relop_ViaItem_68(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 69: {
      REDUCE_ASSERT(1, 69, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_69_relop_ViaItem_69'.";
      return ReduceTo_ASTNodeGeneral_69_relop_ViaItem_69(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 70: {
      REDUCE_ASSERT(1, 70, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_70_relop_ViaItem_70'.";
      return ReduceTo_ASTNodeGeneral_70_relop_ViaItem_70(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 71: {
      REDUCE_ASSERT(1, 71, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_71_relop_ViaItem_71'.";
      return ReduceTo_ASTNodeGeneral_71_relop_ViaItem_71(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 72: {
      REDUCE_ASSERT(3, 72, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_72_minmaxExp_ViaItem_72'.";
      return ReduceTo_ASTNodeGeneral_72_minmaxExp_ViaItem_72(
          std::reinterpret_pointer_cast<ASTNodeBase_minmaxExp>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_minmaxop>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_sumExp>(collected_nodes[2]));
    }
    case 73: {
      REDUCE_ASSERT(1, 73, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_73_minmaxExp_ViaItem_73'.";
      return ReduceTo_ASTNodeGeneral_73_minmaxExp_ViaItem_73(
          std::reinterpret_pointer_cast<ASTNodeBase_sumExp>(collected_nodes[0]));
    }
    case 74: {
      REDUCE_ASSERT(1, 74, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_74_minmaxop_ViaItem_74'.";
      return ReduceTo_ASTNodeGeneral_74_minmaxop_ViaItem_74(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 75: {
      REDUCE_ASSERT(1, 75, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_75_minmaxop_ViaItem_75'.";
      return ReduceTo_ASTNodeGeneral_75_minmaxop_ViaItem_75(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 76: {
      REDUCE_ASSERT(3, 76, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_76_sumExp_ViaItem_76'.";
      return ReduceTo_ASTNodeGeneral_76_sumExp_ViaItem_76(
          std::reinterpret_pointer_cast<ASTNodeBase_sumExp>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_sumop>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_mulExp>(collected_nodes[2]));
    }
    case 77: {
      REDUCE_ASSERT(1, 77, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_77_sumExp_ViaItem_77'.";
      return ReduceTo_ASTNodeGeneral_77_sumExp_ViaItem_77(
          std::reinterpret_pointer_cast<ASTNodeBase_mulExp>(collected_nodes[0]));
    }
    case 78: {
      REDUCE_ASSERT(1, 78, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_78_sumop_ViaItem_78'.";
      return ReduceTo_ASTNodeGeneral_78_sumop_ViaItem_78(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 79: {
      REDUCE_ASSERT(1, 79, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_79_sumop_ViaItem_79'.";
      return ReduceTo_ASTNodeGeneral_79_sumop_ViaItem_79(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 80: {
      REDUCE_ASSERT(3, 80, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_80_mulExp_ViaItem_80'.";
      return ReduceTo_ASTNodeGeneral_80_mulExp_ViaItem_80(
          std::reinterpret_pointer_cast<ASTNodeBase_mulExp>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_mulop>(collected_nodes[1]),
          std::reinterpret_pointer_cast<ASTNodeBase_unaryExp>(collected_nodes[2]));
    }
    case 81: {
      REDUCE_ASSERT(1, 81, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_81_mulExp_ViaItem_81'.";
      return ReduceTo_ASTNodeGeneral_81_mulExp_ViaItem_81(
          std::reinterpret_pointer_cast<ASTNodeBase_unaryExp>(collected_nodes[0]));
    }
    case 82: {
      REDUCE_ASSERT(1, 82, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_82_mulop_ViaItem_82'.";
      return ReduceTo_ASTNodeGeneral_82_mulop_ViaItem_82(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 83: {
      REDUCE_ASSERT(1, 83, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_83_mulop_ViaItem_83'.";
      return ReduceTo_ASTNodeGeneral_83_mulop_ViaItem_83(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 84: {
      REDUCE_ASSERT(1, 84, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_84_mulop_ViaItem_84'.";
      return ReduceTo_ASTNodeGeneral_84_mulop_ViaItem_84(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 85: {
      REDUCE_ASSERT(2, 85, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_85_unaryExp_ViaItem_85'.";
      return ReduceTo_ASTNodeGeneral_85_unaryExp_ViaItem_85(
          std::reinterpret_pointer_cast<ASTNodeBase_unaryop>(collected_nodes[0]),
          std::reinterpret_pointer_cast<ASTNodeBase_unaryExp>(collected_nodes[1]));
    }
    case 86: {
      REDUCE_ASSERT(1, 86, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_86_unaryExp_ViaItem_86'.";
      return ReduceTo_ASTNodeGeneral_86_unaryExp_ViaItem_86(
          std::reinterpret_pointer_cast<ASTNodeBase_factor>(collected_nodes[0]));
    }
    case 87: {
      REDUCE_ASSERT(1, 87, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_87_unaryop_ViaItem_87'.";
      return ReduceTo_ASTNodeGeneral_87_unaryop_ViaItem_87(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 88: {
      REDUCE_ASSERT(1, 88, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_88_unaryop_ViaItem_88'.";
      return ReduceTo_ASTNodeGeneral_88_unaryop_ViaItem_88(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 89: {
      REDUCE_ASSERT(1, 89, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_89_unaryop_ViaItem_89'.";
      return ReduceTo_ASTNodeGeneral_89_unaryop_ViaItem_89(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 90: {
      REDUCE_ASSERT(1, 90, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_90_factor_ViaItem_90'.";
      return ReduceTo_ASTNodeGeneral_90_factor_ViaItem_90(
          std::reinterpret_pointer_cast<ASTNodeBase_immutable>(collected_nodes[0]));
    }
    case 91: {
      REDUCE_ASSERT(1, 91, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_91_factor_ViaItem_91'.";
      return ReduceTo_ASTNodeGeneral_91_factor_ViaItem_91(
          std::reinterpret_pointer_cast<ASTNodeBase_mutable>(collected_nodes[0]));
    }
    case 92: {
      REDUCE_ASSERT(1, 92, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_92_mutable_ViaItem_92'.";
      return ReduceTo_ASTNodeGeneral_92_mutable_ViaItem_92(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 93: {
      REDUCE_ASSERT(4, 93, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_93_mutable_ViaItem_93'.";
      return ReduceTo_ASTNodeGeneral_93_mutable_ViaItem_93(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[2]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 94: {
      REDUCE_ASSERT(3, 94, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_94_immutable_ViaItem_94'.";
      return ReduceTo_ASTNodeGeneral_94_immutable_ViaItem_94(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[1]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[2].get())->literal);
    }
    case 95: {
      REDUCE_ASSERT(1, 95, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_95_immutable_ViaItem_95'.";
      return ReduceTo_ASTNodeGeneral_95_immutable_ViaItem_95(
          std::reinterpret_pointer_cast<ASTNodeGeneral_97_call>(collected_nodes[0]));
    }
    case 96: {
      REDUCE_ASSERT(1, 96, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_96_immutable_ViaItem_96'.";
      return ReduceTo_ASTNodeGeneral_96_immutable_ViaItem_96(
          std::reinterpret_pointer_cast<ASTNodeBase_constant>(collected_nodes[0]));
    }
    case 97: {
      REDUCE_ASSERT(4, 97, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_97_call_ViaItem_97'.";
      return ReduceTo_ASTNodeGeneral_97_call_ViaItem_97(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal,
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_args>(collected_nodes[2]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[3].get())->literal);
    }
    case 98: {
      REDUCE_ASSERT(1, 98, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_98_args_ViaItem_98'.";
      return ReduceTo_ASTNodeGeneral_98_args_ViaItem_98(
          std::reinterpret_pointer_cast<ASTNodeBase_argList>(collected_nodes[0]));
    }
    case 99: {
      REDUCE_ASSERT(0, 99, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_99_args_ViaItem_99'.";
      return ReduceTo_ASTNodeGeneral_99_args_ViaItem_99();
    }
    case 100: {
      REDUCE_ASSERT(3, 100, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_100_argList_ViaItem_100'.";
      return ReduceTo_ASTNodeGeneral_100_argList_ViaItem_100(
          std::reinterpret_pointer_cast<ASTNodeBase_argList>(collected_nodes[0]),
          reinterpret_cast<ASTLexeme*>(collected_nodes[1].get())->literal,
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[2]));
    }
    case 101: {
      REDUCE_ASSERT(1, 101, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_101_argList_ViaItem_101'.";
      return ReduceTo_ASTNodeGeneral_101_argList_ViaItem_101(
          std::reinterpret_pointer_cast<ASTNodeBase_exp>(collected_nodes[0]));
    }
    case 102: {
      REDUCE_ASSERT(1, 102, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_102_constant_ViaItem_102'.";
      return ReduceTo_ASTNodeGeneral_102_constant_ViaItem_102(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 103: {
      REDUCE_ASSERT(1, 103, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_103_constant_ViaItem_103'.";
      return ReduceTo_ASTNodeGeneral_103_constant_ViaItem_103(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 104: {
      REDUCE_ASSERT(1, 104, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_104_constant_ViaItem_104'.";
      return ReduceTo_ASTNodeGeneral_104_constant_ViaItem_104(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 105: {
      REDUCE_ASSERT(1, 105, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_105_constant_ViaItem_105'.";
      return ReduceTo_ASTNodeGeneral_105_constant_ViaItem_105(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    case 106: {
      REDUCE_ASSERT(1, 106, collected_nodes.size());
      LOG_SEV_TO(logger_, Debug) << "Calling reduce function 'ReduceTo_ASTNodeGeneral_106_constant_ViaItem_106'.";
      return ReduceTo_ASTNodeGeneral_106_constant_ViaItem_106(
          reinterpret_cast<ASTLexeme*>(collected_nodes[0].get())->literal);
    }
    default: {
      MANTA_FAIL("unrecognized production" << reduction_id << ", cannot reduce");
    }
  }
}

inline std::shared_ptr<ASTNodeGeneral_0_program>
Parser::ReduceTo_ASTNodeGeneral_0_program_ViaItem_0(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_declList>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_0_program>(0);
  new_node->declList_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_1_declList>
Parser::ReduceTo_ASTNodeGeneral_1_declList_ViaItem_1(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_declList>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_decl>& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_1_declList>(1);
  new_node->declList_node = argument_0;
  new_node->decl_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_2_declList>
Parser::ReduceTo_ASTNodeGeneral_2_declList_ViaItem_2(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_decl>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_2_declList>(2);
  new_node->decl_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_3_decl>
Parser::ReduceTo_ASTNodeGeneral_3_decl_ViaItem_3(
    [[maybe_unused]] const std::shared_ptr<ASTNodeGeneral_5_varDecl>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_3_decl>(3);
  new_node->varDecl_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_4_decl>
Parser::ReduceTo_ASTNodeGeneral_4_decl_ViaItem_4(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_funDecl>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_4_decl>(4);
  new_node->funDecl_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_5_varDecl>
Parser::ReduceTo_ASTNodeGeneral_5_varDecl_ViaItem_5(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_typeSpec>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_varDeclList>& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_5_varDecl>(5);
  new_node->typeSpec_node = argument_0;
  new_node->varDeclList_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_6_scopedVarDecl>
Parser::ReduceTo_ASTNodeGeneral_6_scopedVarDecl_ViaItem_6(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_typeSpec>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_varDeclList>& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNodeGeneral_6_scopedVarDecl>(6);
  new_node->typeSpec_node = argument_1;
  new_node->varDeclList_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_7_scopedVarDecl>
Parser::ReduceTo_ASTNodeGeneral_7_scopedVarDecl_ViaItem_7(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_typeSpec>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_varDeclList>& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_7_scopedVarDecl>(7);
  new_node->typeSpec_node = argument_0;
  new_node->varDeclList_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_8_varDeclList>
Parser::ReduceTo_ASTNodeGeneral_8_varDeclList_ViaItem_8(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_varDeclList>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_varDeclInit>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_8_varDeclList>(8);
  new_node->varDeclList_node = argument_0;
  new_node->varDeclInit_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_9_varDeclList>
Parser::ReduceTo_ASTNodeGeneral_9_varDeclList_ViaItem_9(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_varDeclInit>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_9_varDeclList>(9);
  new_node->varDeclInit_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_10_varDeclInit>
Parser::ReduceTo_ASTNodeGeneral_10_varDeclInit_ViaItem_10(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_varDeclId>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_10_varDeclInit>(10);
  new_node->varDeclId_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_11_varDeclInit>
Parser::ReduceTo_ASTNodeGeneral_11_varDeclInit_ViaItem_11(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_varDeclId>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_11_varDeclInit>(11);
  new_node->varDeclId_node = argument_0;
  new_node->simpleExp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_12_varDeclId>
Parser::ReduceTo_ASTNodeGeneral_12_varDeclId_ViaItem_12(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_12_varDeclId>(12);
  new_node->ID = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_13_varDeclId>
Parser::ReduceTo_ASTNodeGeneral_13_varDeclId_ViaItem_13(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNodeGeneral_13_varDeclId>(13);
  new_node->ID = argument_0;
  new_node->NUMCONST = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_14_typeSpec>
Parser::ReduceTo_ASTNodeGeneral_14_typeSpec_ViaItem_14(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_14_typeSpec>(14);
}

inline std::shared_ptr<ASTNodeGeneral_15_typeSpec>
Parser::ReduceTo_ASTNodeGeneral_15_typeSpec_ViaItem_15(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_15_typeSpec>(15);
}

inline std::shared_ptr<ASTNodeGeneral_16_typeSpec>
Parser::ReduceTo_ASTNodeGeneral_16_typeSpec_ViaItem_16(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_16_typeSpec>(16);
}

inline std::shared_ptr<ASTNodeGeneral_17_funDecl>
Parser::ReduceTo_ASTNodeGeneral_17_funDecl_ViaItem_17(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_typeSpec>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parms>& argument_3,
    [[maybe_unused]] const std::string& argument_4,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmt>& argument_5) {
  auto new_node = std::make_shared<ASTNodeGeneral_17_funDecl>(17);
  new_node->typeSpec_node = argument_0;
  new_node->ID = argument_1;
  new_node->parms_node = argument_3;
  new_node->stmt_node = argument_5;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_18_funDecl>
Parser::ReduceTo_ASTNodeGeneral_18_funDecl_ViaItem_18(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parms>& argument_2,
    [[maybe_unused]] const std::string& argument_3,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmt>& argument_4) {
  auto new_node = std::make_shared<ASTNodeGeneral_18_funDecl>(18);
  new_node->ID = argument_0;
  new_node->parms_node = argument_2;
  new_node->stmt_node = argument_4;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_19_parms>
Parser::ReduceTo_ASTNodeGeneral_19_parms_ViaItem_19(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parmList>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_19_parms>(19);
  new_node->parmList_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_20_parms>
Parser::ReduceTo_ASTNodeGeneral_20_parms_ViaItem_20() {
  return std::make_shared<ASTNodeGeneral_20_parms>(20);
}

inline std::shared_ptr<ASTNodeGeneral_21_parmList>
Parser::ReduceTo_ASTNodeGeneral_21_parmList_ViaItem_21(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parmList>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeGeneral_23_parmTypeList>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_21_parmList>(21);
  new_node->parmList_node = argument_0;
  new_node->parmTypeList_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_22_parmList>
Parser::ReduceTo_ASTNodeGeneral_22_parmList_ViaItem_22(
    [[maybe_unused]] const std::shared_ptr<ASTNodeGeneral_23_parmTypeList>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_22_parmList>(22);
  new_node->parmTypeList_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_23_parmTypeList>
Parser::ReduceTo_ASTNodeGeneral_23_parmTypeList_ViaItem_23(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_typeSpec>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parmIdList>& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_23_parmTypeList>(23);
  new_node->typeSpec_node = argument_0;
  new_node->parmIdList_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_24_parmIdList>
Parser::ReduceTo_ASTNodeGeneral_24_parmIdList_ViaItem_24(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parmIdList>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parmId>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_24_parmIdList>(24);
  new_node->parmIdList_node = argument_0;
  new_node->parmId_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_25_parmIdList>
Parser::ReduceTo_ASTNodeGeneral_25_parmIdList_ViaItem_25(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_parmId>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_25_parmIdList>(25);
  new_node->parmId_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_26_parmId>
Parser::ReduceTo_ASTNodeGeneral_26_parmId_ViaItem_26(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_26_parmId>(26);
  new_node->ID = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_27_parmId>
Parser::ReduceTo_ASTNodeGeneral_27_parmId_ViaItem_27(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_27_parmId>(27);
  new_node->ID = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_28_stmt>
Parser::ReduceTo_ASTNodeGeneral_28_stmt_ViaItem_28(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_expStmt>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_28_stmt>(28);
  new_node->expStmt_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_29_stmt>
Parser::ReduceTo_ASTNodeGeneral_29_stmt_ViaItem_29(
    [[maybe_unused]] const std::shared_ptr<ASTNodeGeneral_36_compoundStmt>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_29_stmt>(29);
  new_node->compoundStmt_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_30_stmt>
Parser::ReduceTo_ASTNodeGeneral_30_stmt_ViaItem_30(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_selectStmt>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_30_stmt>(30);
  new_node->selectStmt_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_31_stmt>
Parser::ReduceTo_ASTNodeGeneral_31_stmt_ViaItem_31(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_iterStmt>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_31_stmt>(31);
  new_node->iterStmt_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_32_stmt>
Parser::ReduceTo_ASTNodeGeneral_32_stmt_ViaItem_32(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_returnStmt>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_32_stmt>(32);
  new_node->returnStmt_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_33_stmt>
Parser::ReduceTo_ASTNodeGeneral_33_stmt_ViaItem_33(
    [[maybe_unused]] const std::shared_ptr<ASTNodeGeneral_49_breakStmt>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_33_stmt>(33);
  new_node->breakStmt_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_34_expStmt>
Parser::ReduceTo_ASTNodeGeneral_34_expStmt_ViaItem_34(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_34_expStmt>(34);
  new_node->exp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_35_expStmt>
Parser::ReduceTo_ASTNodeGeneral_35_expStmt_ViaItem_35(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_35_expStmt>(35);
}

inline std::shared_ptr<ASTNodeGeneral_36_compoundStmt>
Parser::ReduceTo_ASTNodeGeneral_36_compoundStmt_ViaItem_36(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_localDecls>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmtList>& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNodeGeneral_36_compoundStmt>(36);
  new_node->localDecls_node = argument_1;
  new_node->stmtList_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_37_localDecls>
Parser::ReduceTo_ASTNodeGeneral_37_localDecls_ViaItem_37(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_localDecls>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_scopedVarDecl>& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_37_localDecls>(37);
  new_node->localDecls_node = argument_0;
  new_node->scopedVarDecl_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_38_localDecls>
Parser::ReduceTo_ASTNodeGeneral_38_localDecls_ViaItem_38() {
  return std::make_shared<ASTNodeGeneral_38_localDecls>(38);
}

inline std::shared_ptr<ASTNodeGeneral_39_stmtList>
Parser::ReduceTo_ASTNodeGeneral_39_stmtList_ViaItem_39(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmtList>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmt>& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_39_stmtList>(39);
  new_node->stmtList_node = argument_0;
  new_node->stmt_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_40_stmtList>
Parser::ReduceTo_ASTNodeGeneral_40_stmtList_ViaItem_40() {
  return std::make_shared<ASTNodeGeneral_40_stmtList>(40);
}

inline std::shared_ptr<ASTNodeGeneral_41_selectStmt>
Parser::ReduceTo_ASTNodeGeneral_41_selectStmt_ViaItem_41(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmt>& argument_3) {
  auto new_node = std::make_shared<ASTNodeGeneral_41_selectStmt>(41);
  new_node->simpleExp_node = argument_1;
  new_node->stmt_node = argument_3;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_42_selectStmt>
Parser::ReduceTo_ASTNodeGeneral_42_selectStmt_ViaItem_42(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmt>& argument_3,
    [[maybe_unused]] const std::string& argument_4,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmt>& argument_5) {
  auto new_node = std::make_shared<ASTNodeGeneral_42_selectStmt>(42);
  new_node->simpleExp_node = argument_1;
  new_node->stmt_node_0 = argument_3;
  new_node->stmt_node_1 = argument_5;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_43_iterStmt>
Parser::ReduceTo_ASTNodeGeneral_43_iterStmt_ViaItem_43(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmt>& argument_3) {
  auto new_node = std::make_shared<ASTNodeGeneral_43_iterStmt>(43);
  new_node->simpleExp_node = argument_1;
  new_node->stmt_node = argument_3;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_44_iterStmt>
Parser::ReduceTo_ASTNodeGeneral_44_iterStmt_ViaItem_44(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::string& argument_2,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_iterRange>& argument_3,
    [[maybe_unused]] const std::string& argument_4,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_stmt>& argument_5) {
  auto new_node = std::make_shared<ASTNodeGeneral_44_iterStmt>(44);
  new_node->ID = argument_1;
  new_node->iterRange_node = argument_3;
  new_node->stmt_node = argument_5;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_45_iterRange>
Parser::ReduceTo_ASTNodeGeneral_45_iterRange_ViaItem_45(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_45_iterRange>(45);
  new_node->simpleExp_node_0 = argument_0;
  new_node->simpleExp_node_1 = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_46_iterRange>
Parser::ReduceTo_ASTNodeGeneral_46_iterRange_ViaItem_46(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_2,
    [[maybe_unused]] const std::string& argument_3,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_4) {
  auto new_node = std::make_shared<ASTNodeGeneral_46_iterRange>(46);
  new_node->simpleExp_node_0 = argument_0;
  new_node->simpleExp_node_1 = argument_2;
  new_node->simpleExp_node_2 = argument_4;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_47_returnStmt>
Parser::ReduceTo_ASTNodeGeneral_47_returnStmt_ViaItem_47(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  return std::make_shared<ASTNodeGeneral_47_returnStmt>(47);
}

inline std::shared_ptr<ASTNodeGeneral_48_returnStmt>
Parser::ReduceTo_ASTNodeGeneral_48_returnStmt_ViaItem_48(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_48_returnStmt>(48);
  new_node->exp_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_49_breakStmt>
Parser::ReduceTo_ASTNodeGeneral_49_breakStmt_ViaItem_49(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  return std::make_shared<ASTNodeGeneral_49_breakStmt>(49);
}

inline std::shared_ptr<ASTNodeGeneral_50_exp>
Parser::ReduceTo_ASTNodeGeneral_50_exp_ViaItem_50(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_50_exp>(50);
  new_node->mutable__node = argument_0;
  new_node->exp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_51_exp>
Parser::ReduceTo_ASTNodeGeneral_51_exp_ViaItem_51(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_51_exp>(51);
  new_node->mutable__node = argument_0;
  new_node->exp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_52_exp>
Parser::ReduceTo_ASTNodeGeneral_52_exp_ViaItem_52(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_52_exp>(52);
  new_node->mutable__node = argument_0;
  new_node->exp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_53_exp>
Parser::ReduceTo_ASTNodeGeneral_53_exp_ViaItem_53(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_53_exp>(53);
  new_node->mutable__node = argument_0;
  new_node->exp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_54_exp>
Parser::ReduceTo_ASTNodeGeneral_54_exp_ViaItem_54(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_54_exp>(54);
  new_node->mutable__node = argument_0;
  new_node->exp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_55_exp>
Parser::ReduceTo_ASTNodeGeneral_55_exp_ViaItem_55(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_55_exp>(55);
  new_node->mutable__node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_56_exp>
Parser::ReduceTo_ASTNodeGeneral_56_exp_ViaItem_56(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mutable>& argument_0,
    [[maybe_unused]] const std::string& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_56_exp>(56);
  new_node->mutable__node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_57_exp>
Parser::ReduceTo_ASTNodeGeneral_57_exp_ViaItem_57(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_57_exp>(57);
  new_node->simpleExp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_58_simpleExp>
Parser::ReduceTo_ASTNodeGeneral_58_simpleExp_ViaItem_58(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_simpleExp>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_andExp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_58_simpleExp>(58);
  new_node->simpleExp_node = argument_0;
  new_node->andExp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_59_simpleExp>
Parser::ReduceTo_ASTNodeGeneral_59_simpleExp_ViaItem_59(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_andExp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_59_simpleExp>(59);
  new_node->andExp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_60_andExp>
Parser::ReduceTo_ASTNodeGeneral_60_andExp_ViaItem_60(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_andExp>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_unaryRelExp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_60_andExp>(60);
  new_node->andExp_node = argument_0;
  new_node->unaryRelExp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_61_andExp>
Parser::ReduceTo_ASTNodeGeneral_61_andExp_ViaItem_61(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_unaryRelExp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_61_andExp>(61);
  new_node->unaryRelExp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_62_unaryRelExp>
Parser::ReduceTo_ASTNodeGeneral_62_unaryRelExp_ViaItem_62(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_unaryRelExp>& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_62_unaryRelExp>(62);
  new_node->unaryRelExp_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_63_unaryRelExp>
Parser::ReduceTo_ASTNodeGeneral_63_unaryRelExp_ViaItem_63(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_relExp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_63_unaryRelExp>(63);
  new_node->relExp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_64_relExp>
Parser::ReduceTo_ASTNodeGeneral_64_relExp_ViaItem_64(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_minmaxExp>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_relop>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_minmaxExp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_64_relExp>(64);
  new_node->minmaxExp_node_0 = argument_0;
  new_node->relop_node = argument_1;
  new_node->minmaxExp_node_1 = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_65_relExp>
Parser::ReduceTo_ASTNodeGeneral_65_relExp_ViaItem_65(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_minmaxExp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_65_relExp>(65);
  new_node->minmaxExp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_66_relop>
Parser::ReduceTo_ASTNodeGeneral_66_relop_ViaItem_66(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_66_relop>(66);
}

inline std::shared_ptr<ASTNodeGeneral_67_relop>
Parser::ReduceTo_ASTNodeGeneral_67_relop_ViaItem_67(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_67_relop>(67);
}

inline std::shared_ptr<ASTNodeGeneral_68_relop>
Parser::ReduceTo_ASTNodeGeneral_68_relop_ViaItem_68(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_68_relop>(68);
}

inline std::shared_ptr<ASTNodeGeneral_69_relop>
Parser::ReduceTo_ASTNodeGeneral_69_relop_ViaItem_69(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_69_relop>(69);
}

inline std::shared_ptr<ASTNodeGeneral_70_relop>
Parser::ReduceTo_ASTNodeGeneral_70_relop_ViaItem_70(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_70_relop>(70);
}

inline std::shared_ptr<ASTNodeGeneral_71_relop>
Parser::ReduceTo_ASTNodeGeneral_71_relop_ViaItem_71(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_71_relop>(71);
}

inline std::shared_ptr<ASTNodeGeneral_72_minmaxExp>
Parser::ReduceTo_ASTNodeGeneral_72_minmaxExp_ViaItem_72(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_minmaxExp>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_minmaxop>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_sumExp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_72_minmaxExp>(72);
  new_node->minmaxExp_node = argument_0;
  new_node->minmaxop_node = argument_1;
  new_node->sumExp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_73_minmaxExp>
Parser::ReduceTo_ASTNodeGeneral_73_minmaxExp_ViaItem_73(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_sumExp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_73_minmaxExp>(73);
  new_node->sumExp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_74_minmaxop>
Parser::ReduceTo_ASTNodeGeneral_74_minmaxop_ViaItem_74(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_74_minmaxop>(74);
}

inline std::shared_ptr<ASTNodeGeneral_75_minmaxop>
Parser::ReduceTo_ASTNodeGeneral_75_minmaxop_ViaItem_75(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_75_minmaxop>(75);
}

inline std::shared_ptr<ASTNodeGeneral_76_sumExp>
Parser::ReduceTo_ASTNodeGeneral_76_sumExp_ViaItem_76(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_sumExp>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_sumop>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mulExp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_76_sumExp>(76);
  new_node->sumExp_node = argument_0;
  new_node->sumop_node = argument_1;
  new_node->mulExp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_77_sumExp>
Parser::ReduceTo_ASTNodeGeneral_77_sumExp_ViaItem_77(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mulExp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_77_sumExp>(77);
  new_node->mulExp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_78_sumop>
Parser::ReduceTo_ASTNodeGeneral_78_sumop_ViaItem_78(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_78_sumop>(78);
}

inline std::shared_ptr<ASTNodeGeneral_79_sumop>
Parser::ReduceTo_ASTNodeGeneral_79_sumop_ViaItem_79(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_79_sumop>(79);
}

inline std::shared_ptr<ASTNodeGeneral_80_mulExp>
Parser::ReduceTo_ASTNodeGeneral_80_mulExp_ViaItem_80(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mulExp>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mulop>& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_unaryExp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_80_mulExp>(80);
  new_node->mulExp_node = argument_0;
  new_node->mulop_node = argument_1;
  new_node->unaryExp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_81_mulExp>
Parser::ReduceTo_ASTNodeGeneral_81_mulExp_ViaItem_81(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_unaryExp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_81_mulExp>(81);
  new_node->unaryExp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_82_mulop>
Parser::ReduceTo_ASTNodeGeneral_82_mulop_ViaItem_82(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_82_mulop>(82);
}

inline std::shared_ptr<ASTNodeGeneral_83_mulop>
Parser::ReduceTo_ASTNodeGeneral_83_mulop_ViaItem_83(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_83_mulop>(83);
}

inline std::shared_ptr<ASTNodeGeneral_84_mulop>
Parser::ReduceTo_ASTNodeGeneral_84_mulop_ViaItem_84(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_84_mulop>(84);
}

inline std::shared_ptr<ASTNodeGeneral_85_unaryExp>
Parser::ReduceTo_ASTNodeGeneral_85_unaryExp_ViaItem_85(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_unaryop>& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_unaryExp>& argument_1) {
  auto new_node = std::make_shared<ASTNodeGeneral_85_unaryExp>(85);
  new_node->unaryop_node = argument_0;
  new_node->unaryExp_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_86_unaryExp>
Parser::ReduceTo_ASTNodeGeneral_86_unaryExp_ViaItem_86(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_factor>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_86_unaryExp>(86);
  new_node->factor_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_87_unaryop>
Parser::ReduceTo_ASTNodeGeneral_87_unaryop_ViaItem_87(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_87_unaryop>(87);
}

inline std::shared_ptr<ASTNodeGeneral_88_unaryop>
Parser::ReduceTo_ASTNodeGeneral_88_unaryop_ViaItem_88(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_88_unaryop>(88);
}

inline std::shared_ptr<ASTNodeGeneral_89_unaryop>
Parser::ReduceTo_ASTNodeGeneral_89_unaryop_ViaItem_89(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_89_unaryop>(89);
}

inline std::shared_ptr<ASTNodeGeneral_90_factor>
Parser::ReduceTo_ASTNodeGeneral_90_factor_ViaItem_90(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_immutable>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_90_factor>(90);
  new_node->immutable_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_91_factor>
Parser::ReduceTo_ASTNodeGeneral_91_factor_ViaItem_91(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_mutable>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_91_factor>(91);
  new_node->mutable__node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_92_mutable>
Parser::ReduceTo_ASTNodeGeneral_92_mutable_ViaItem_92(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_92_mutable>(92);
  new_node->ID = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_93_mutable>
Parser::ReduceTo_ASTNodeGeneral_93_mutable_ViaItem_93(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNodeGeneral_93_mutable>(93);
  new_node->ID = argument_0;
  new_node->exp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_94_immutable>
Parser::ReduceTo_ASTNodeGeneral_94_immutable_ViaItem_94(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_1,
    [[maybe_unused]] const std::string& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_94_immutable>(94);
  new_node->exp_node = argument_1;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_95_immutable>
Parser::ReduceTo_ASTNodeGeneral_95_immutable_ViaItem_95(
    [[maybe_unused]] const std::shared_ptr<ASTNodeGeneral_97_call>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_95_immutable>(95);
  new_node->call_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_96_immutable>
Parser::ReduceTo_ASTNodeGeneral_96_immutable_ViaItem_96(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_constant>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_96_immutable>(96);
  new_node->constant_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_97_call>
Parser::ReduceTo_ASTNodeGeneral_97_call_ViaItem_97(
    [[maybe_unused]] const std::string& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_args>& argument_2,
    [[maybe_unused]] const std::string& argument_3) {
  auto new_node = std::make_shared<ASTNodeGeneral_97_call>(97);
  new_node->ID = argument_0;
  new_node->args_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_98_args>
Parser::ReduceTo_ASTNodeGeneral_98_args_ViaItem_98(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_argList>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_98_args>(98);
  new_node->argList_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_99_args>
Parser::ReduceTo_ASTNodeGeneral_99_args_ViaItem_99() {
  return std::make_shared<ASTNodeGeneral_99_args>(99);
}

inline std::shared_ptr<ASTNodeGeneral_100_argList>
Parser::ReduceTo_ASTNodeGeneral_100_argList_ViaItem_100(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_argList>& argument_0,
    [[maybe_unused]] const std::string& argument_1,
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_2) {
  auto new_node = std::make_shared<ASTNodeGeneral_100_argList>(100);
  new_node->argList_node = argument_0;
  new_node->exp_node = argument_2;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_101_argList>
Parser::ReduceTo_ASTNodeGeneral_101_argList_ViaItem_101(
    [[maybe_unused]] const std::shared_ptr<ASTNodeBase_exp>& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_101_argList>(101);
  new_node->exp_node = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_102_constant>
Parser::ReduceTo_ASTNodeGeneral_102_constant_ViaItem_102(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_102_constant>(102);
  new_node->NUMCONST = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_103_constant>
Parser::ReduceTo_ASTNodeGeneral_103_constant_ViaItem_103(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_103_constant>(103);
  new_node->CHARCONST = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_104_constant>
Parser::ReduceTo_ASTNodeGeneral_104_constant_ViaItem_104(
    [[maybe_unused]] const std::string& argument_0) {
  auto new_node = std::make_shared<ASTNodeGeneral_104_constant>(104);
  new_node->STRINGCONST = argument_0;

  return new_node;
}

inline std::shared_ptr<ASTNodeGeneral_105_constant>
Parser::ReduceTo_ASTNodeGeneral_105_constant_ViaItem_105(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_105_constant>(105);
}

inline std::shared_ptr<ASTNodeGeneral_106_constant>
Parser::ReduceTo_ASTNodeGeneral_106_constant_ViaItem_106(
    [[maybe_unused]] const std::string& argument_0) {
  return std::make_shared<ASTNodeGeneral_106_constant>(106);
}

#define MANTA_PARSER_GENERATED
