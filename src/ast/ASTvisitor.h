#pragma once
#include "AST.h"
#include "SymbolTable.h"
#include "Type.h"
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
#include <cstddef>
#include <fstream>
#include <llvm-16/llvm/IR/DerivedTypes.h>
#include <llvm-16/llvm/IR/Type.h>
#include <llvm-16/llvm/IR/Value.h>
#include <llvm-16/llvm/Support/raw_ostream.h>
#include <memory>
#include <stack>
using namespace llvm;
// 抽象访问器类，继承实现
class ASTNodeVisitor {
  // based visitor on ast tree
public:
  virtual void visit(class BinaryExprAST *ast) = 0;
  virtual void visit(class UnaryExprAST *ast) = 0;
  virtual void visit(class IntergarExprAST *ast) = 0;
  virtual void visit(class FloatExprAST *ast) = 0;
  virtual void visit(class IdentifierAST *ast) = 0;
  virtual void visit(class ArrayExprAST *ast) = 0;
  virtual void visit(class CallExprAST *ast) = 0;
  virtual void visit(class InitValExprAST *ast) = 0;
  virtual void visit(class ImplicitValExprAST *ast) = 0;
  virtual void visit(class AssignStmtAST *ast) = 0;
  virtual void visit(class IfStmtAST *ast) = 0;
  virtual void visit(class WhileStmtAST *ast) = 0;
  virtual void visit(class BreakStmtAST *ast) = 0;
  virtual void visit(class ContinueStmtAST *ast) = 0;
  virtual void visit(class BlockStmtAST *ast) = 0;
  virtual void visit(class ExpStmtAST *ast) = 0;
  virtual void visit(class RetuStmtAST *ast) = 0;
  virtual void visit(class ConstDecAST *ast) = 0;
  virtual void visit(class VarDecAST *ast) = 0;
  virtual void visit(class FuncDecAST *ast) = 0;
  virtual void visit(class ProgramAST *ast) = 0;
};
class ASTPrinter : public ASTNodeVisitor {
private:
  unsigned int ind;

public:
  ASTPrinter() : ASTNodeVisitor(), ind(0) {}
  void visit(class BinaryExprAST *ast) override;
  void visit(class UnaryExprAST *ast) override;
  void visit(class IntergarExprAST *ast) override;
  void visit(class FloatExprAST *ast) override;
  void visit(class IdentifierAST *ast) override;
  void visit(class ArrayExprAST *ast) override;
  void visit(class CallExprAST *ast) override;
  void visit(class InitValExprAST *ast) override;
  void visit(class ImplicitValExprAST *ast) override;
  void visit(class AssignStmtAST *ast) override;
  void visit(class IfStmtAST *ast) override;
  void visit(class WhileStmtAST *ast) override;
  void visit(class BreakStmtAST *ast) override;
  void visit(class ContinueStmtAST *ast) override;
  void visit(class BlockStmtAST *ast) override;
  void visit(class ExpStmtAST *ast) override;
  void visit(class RetuStmtAST *ast) override;
  void visit(class ConstDecAST *ast) override;
  void visit(class VarDecAST *ast) override;
  void visit(class FuncDecAST *ast) override;
  void visit(class ProgramAST *ast) override;
};
// need to design a visitor to semantic analysis
class ASTIRGenerator : public ASTNodeVisitor {
private:
  // function-table;
  // a file stream to record the text
  std::unique_ptr<LLVMContext> TheContext;
  std::unique_ptr<IRBuilder<>> IRbuilder;
  std::unique_ptr<Module> TheModule;
  std::string filename = "test.ll";
  std::error_code EC;
  llvm::raw_fd_ostream irs;
  // 保存visit方法生成的re_vlaue
  Value *re_value;
  FunctionTable *functions;
  // variable
  VariableTable *variables;
  // use for block scope
  VariableTableScope *cur_func_scope;
  // save C BB block and B bb Block
  std::stack<BasicBlock *> Cbb;
  std::stack<BasicBlock *> Bbb;

public:
  ASTIRGenerator() : irs(filename, EC){};
  void Initialize();
  void setSymbolTables(FunctionTable *functions, VariableTable *variables);
  void register_libs_to_module();
  void register_lib_to_module(std::string name, spanti::Type *re_type,
                              std::vector<spanti::Type *> params_type);
  // 返回保存的visit
  Value *get_value() { return re_value; }
  llvm::Type *mapToLLVMType(spanti::Type *sym_type);
  llvm::FunctionType *mapToLLVMFType(spanti::FunctionType *sym_type);
  llvm::Value* getArrayBasePtr(VariableTableEntry* entry);
  llvm::Value* getArrayElemPtr(VariableTableEntry* entry,class ArrayExprAST *ast);
  // int a[2][2] -> value{0.0}
  std::vector<llvm::Value *> getZRefArrayFromType(int child_size);
  llvm::Constant * getConstantArray(std::vector<int32_t> &const_init,std::vector<uint64_t>& dims);
  llvm::Constant * getConstantArray(std::vector<std::shared_ptr<ExprAST>> &const_init,std::vector<uint64_t>& dims);
  void getDimsFromType(llvm::ArrayType *a_type, std::vector<uint64_t> &dims);
  void visit(class BinaryExprAST *ast) override;
  void visit(class UnaryExprAST *ast) override;
  void visit(class IntergarExprAST *ast) override;
  void visit(class FloatExprAST *ast) override;
  void visit(class IdentifierAST *ast) override;
  void visit(class ArrayExprAST *ast) override;
  void visit(class CallExprAST *ast) override;
  void visit(class InitValExprAST *ast) override;
  void visit(class ImplicitValExprAST *ast) override;
  void visit(class AssignStmtAST *ast) override;
  void visit(class IfStmtAST *ast) override;
  void visit(class WhileStmtAST *ast) override;
  void visit(class BreakStmtAST *ast) override;
  void visit(class ContinueStmtAST *ast) override;
  void visit(class BlockStmtAST *ast) override;
  void visit(class ExpStmtAST *ast) override;
  void visit(class RetuStmtAST *ast) override;
  void visit(class ConstDecAST *ast) override;
  void visit(class VarDecAST *ast) override;
  void visit(class FuncDecAST *ast) override;
  void visit(class ProgramAST *ast) override;
};