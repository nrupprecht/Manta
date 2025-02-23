#pragma once

//#include <llvm/ADT/APFloat.h>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"


#include "manta/parser/ParseNode.h"

namespace manta::codegen {

class ModuleBuilder {
 public:
  ModuleBuilder()
      : llvm_context_(std::make_unique<llvm::LLVMContext>())
      , ir_builder_(std::make_unique<llvm::IRBuilder<>>(*llvm_context_))
      , module_(std::make_unique<llvm::Module>("module", *llvm_context_))
  {}

  void GenerateCode(ParseNode& node) {
    auto& node_designator = node.designator;
    if (node_designator == "statements") {
      for (auto& child : node.children) {
        GenerateCode(*child);
      }
    }
    else if (node_designator == "module_import") {
      // TODO
    }
    else if (node_designator == "class_definition") {

    }
    else if (node_designator == "function_declaration") {
      functionDeclaration(node);
    }
    else if (node_designator == "declare_assignment") {
      auto& variable_name = node.children[0]->designator;
      auto value = evaluate(*node.children[1]);

      if (value) {
        value->print(llvm::errs());
      }
      else {
        std::cout << "Value was null, could not evaluate." << std::endl;
      }
    }
    else if (node_designator == "assignment") {
      node.children[0];
    }
    else if (node_designator == "function_call") {
      functionCall(node);
    }
    else if (node_designator == "null_statement"); // Nothing to do.
    else {
      MANTA_FAIL("cannot handle this type of node (" << node.designator << ") right now");
    }

    // module_->print(llvm::errs(), nullptr);
  }

  llvm::Function* functionDeclaration(const ParseNode& node) {
    auto& function_name = node.children[0]->designator;
    auto& arg_list = *node.children[1];
    auto& ret_type = *node.children[2];
    auto& body = *node.children[3];

    if (auto fnc = module_->getFunction(function_name); fnc) {
      MANTA_FAIL("redefinition of function '" << fnc << "'");
    }

    // ==============================================================
    // Handle function prototype
    // ==============================================================

    std::vector<llvm::Type*> argument_types(arg_list.children.size(), llvm::Type::getDoubleTy(*llvm_context_));
    llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*llvm_context_), argument_types, false);
    llvm::Function *function = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, function_name, module_.get());

    // Set names for all arguments.
    auto i = 0u;
    for (auto &arg : function->args()) {
      arg.setName(arg_list.children[i++]->children[0]->designator);
    }

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *basic_block = llvm::BasicBlock::Create(*llvm_context_, "entry", function);
    ir_builder_->SetInsertPoint(basic_block);

    // Record the function arguments in the NamedValues map.
    // TODO: Do this right.
    named_values_.clear();
    for (auto &Arg : function->args()) {
      named_values_[std::string(Arg.getName())] = &Arg;
    }

    // Create the body of the function.

    auto ret_value = functionBody(body);

    if (ret_value) {
      // Finish off the function.
      ir_builder_->CreateRet(ret_value);
      // Validate the generated code, checking for consistency.
      verifyFunction(*function);
      return function;
    }
    // If there is an error, erase the function.
    function->eraseFromParent();
    return nullptr;
  }

  llvm::Value* functionBody(const ParseNode& node) {
    // TODO
    return llvm::ConstantFP::get(*llvm_context_, llvm::APFloat(0.));
  }

  //! \brief Evaluate an evaluable node.
  llvm::Value* evaluate(const ParseNode& node) {
    if (node.designator == "number") {
      return llvm::ConstantFP::get(*llvm_context_, llvm::APFloat(std::stod(node.children[0]->designator)));
    }
    else if (node.designator == "potentially_qualified_name" || node.designator == "independent_name") {
      return nullptr; // TODO: Variables.
      // return evaluate(*node.children[0]);
    }
    else if (node.designator == "evaluable") {
      return evaluate(*node.children[0]);
    }
    else {
      auto lhs = evaluate(*node.children[0]);
      auto rhs = evaluate(*node.children[1]);

      if (!lhs || !rhs) {
        return nullptr;
      }

      if (node.designator == "SUM") {
        return ir_builder_->CreateFAdd(lhs, rhs, "addtmp");
      }
      else if (node.designator == "SUM") {
        return ir_builder_->CreateFSub(lhs, rhs, "subtmp");
      }
      else if (node.designator == "MULT") {
        return ir_builder_->CreateFMul(lhs, rhs, "multmp");
      }
      else if (node.designator == "DIV") {
        return ir_builder_->CreateFDiv(lhs, rhs, "divtmp");
      }
      else if (node.designator == "EXP") {
        MANTA_FAIL("cannot handle exponentiation right now");
      }
      MANTA_FAIL("unrecognized evaluation type '" << node.designator << "'");
    }
  }

  llvm::Value* functionCall(const ParseNode& node) {
    auto& function_name = node.children[0]->designator;
    llvm::Function *callee = module_->getFunction(function_name);

    // Get all arguments.
    std::vector<llvm::Value*> arguments;

    // Make sure the number of arguments (and types?) match.
    for (auto i = 1; i < node.children.size(); ++i) {
      arguments.push_back(evaluate(*node.children[i]));
      if (!arguments.back()) {
        MANTA_FAIL("could not generate code for argument");
      }
    }

    return ir_builder_->CreateCall(callee, arguments, "calltmp");
  }

 private:
  // Context is an opaque object that owns a lot of core LLVM data structures, such as the type and constant value tables
  std::unique_ptr<llvm::LLVMContext> llvm_context_;

  // The Builder object is a helper object that makes it easy to generate LLVM instructions.
  // Instances of the IRBuilder class template keep track of the current place to insert instructions
  // and has methods to create new instructions.
  std::unique_ptr<llvm::IRBuilder<>> ir_builder_;

  // TheModule is an LLVM construct that contains functions and global variables. In many ways, it is the
  // top-level structure that the LLVM IR uses to contain code. It will own the memory for all of the IR that we generate.
  std::unique_ptr<llvm::Module> module_;

  std::map<std::string, llvm::Value*> named_values_;
};

} // namespace manta::codegen
