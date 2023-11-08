#pragma once
#include "Type.h"
#include <llvm-16/llvm/IR/GlobalVariable.h>
#include <llvm-16/llvm/IR/Instructions.h>
#include <map>
#include <memory>
// 建立多个编译器符号表 - design FunctionTable and VariableTable
// variable info record 
class VariableTableEntry {
public:
  //manage life-cycle
  int def_line;//line where declare , variable scope
  spanti::Type *var_type;
  std::vector<int32_t> const_init;  // empty when !type.is_const
  llvm::AllocaInst* inst_memory; //store the memory location of variable - local-var
  llvm::GlobalVariable* global_var; //store llvm global variable - global-var
  bool is_global; //set global entry
  VariableTableEntry(spanti::Type *var_type) : var_type(var_type) {}
  VariableTableEntry() {}
};
class VariableTableScope {
  // a hashmap: <name,entry>
public:
  std::map<std::string, std::unique_ptr<VariableTableEntry>> mapping; //
  VariableTableScope *parent; // point to parent scope
  std::vector<VariableTableScope *> childrens;
  // if store in memory , add vector<Scope*> children
  // register
  // scope is global?
  bool global;
  //重构函数 insertt(name,record)
  void insert(const std::string &name,VariableTableEntry* entry);
  void insert(const std::string &name, spanti::Type *var_type); // decl
  void insert(const std::string &name, spanti::Type *var_type,std::vector<int32_t> init);
  VariableTableEntry *lookUp(std::string name);                 // search
    //引入变量作用域后的查询
  VariableTableEntry *lookUp(const std::string &name,int cur_line);
  bool is_global();
};
class VariableTable {
public:
  VariableTableScope *head;
  VariableTableScope *cur; // current scope
  //若head==cur，cur指向全局变量,当前作用域为全局
  bool is_global();
  void EntryScope();
  void EntryScope(VariableTableScope *goal);
  void ExitScope();
  VariableTable();
  void insert(const std::string &name,VariableTableEntry* entry);
  //register var
  void insert(std::string name, spanti::Type *var_type);
  //register const
  void insert(std::string name, spanti::Type *var_type,std::vector<int32_t> init);
  VariableTableEntry *lookUp(const std::string &name);
  VariableTableEntry *lookUp(const std::string &name,
                             VariableTableScope *cur_scope);
  //引入变量作用域后的查询
  VariableTableEntry *lookUp(const std::string &name,
                             VariableTableScope *cur_scope,int cur_line);
  VariableTableEntry *lookUp(const std::string &name,int cur_line);
};
class FunctionTableEntry {
public:
  spanti::FunctionType *func_type;
  VariableTableScope* func_scope;
  FunctionTableEntry(spanti::FunctionType *func_type) : func_type(func_type) {}
  FunctionTableEntry() {}
};
class FunctionTable {
public:
  std::map<std::string, std::unique_ptr<FunctionTableEntry>> mapping;
  // 查询符号表条目
  FunctionTableEntry *lookUp(const std::string &name);
  // 插入函数符号表条目
  void insert(const std::string &name, spanti::FunctionType *type,VariableTableScope* func_scope);
  // 设计函数，打印符号表条目
};