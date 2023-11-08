#include "SymbolTable.h"
#include "errors.h"
#include <memory>
FunctionTableEntry *FunctionTable::lookUp(const std::string &name) {
  auto it = mapping.find(name);
  if (it != mapping.end()) {
    return it->second.get();
  } else {
    return nullptr;
  }
}
void FunctionTable::insert(const std::string &name, spanti::FunctionType *type,
                           VariableTableScope *func_scope) {
  FunctionTableEntry *new_entry = new FunctionTableEntry(type);
  // set func scope
  new_entry->func_scope = func_scope;
  // if name already exists,throw error
  auto it = mapping.find(name);
  if (it != mapping.end()) {
    throw DuplicateFuncName(name);
  }
  mapping[name] = std::unique_ptr<FunctionTableEntry>{new_entry};
}
void VariableTableScope::insert(const std::string &name,
                                VariableTableEntry *entry) {
  // set local
  if (this->is_global())
    entry->is_global = true;
  else
    entry->is_global = false;
  // if cur = head,set it global

  // if name already exists,throw error
  auto it = mapping.find(name);
  if (it != mapping.end()) {
    throw DuplicateVarName(name);
  }
  mapping[name] = std::unique_ptr<VariableTableEntry>{entry};
}
// variable decl
void VariableTableScope::insert(const std::string &name,
                                spanti::Type *var_type) {
  VariableTableEntry *new_entry = new VariableTableEntry(var_type);
  // set local
  if (this->is_global())
    new_entry->is_global = true;
  else
    new_entry->is_global = false;
  // if cur = head,set it global

  // if name already exists,throw error
  auto it = mapping.find(name);
  if (it != mapping.end()) {
    throw DuplicateVarName(name);
  }
  mapping[name] = std::unique_ptr<VariableTableEntry>{new_entry};
}
// constant decl
void VariableTableScope::insert(const std::string &name, spanti::Type *var_type,
                                std::vector<int32_t> init) {
  VariableTableEntry *new_entry = new VariableTableEntry(var_type);
  // set local
  if (this->is_global())
    new_entry->is_global = true;
  else
    new_entry->is_global = false;
  // if name already exists,throw error
  auto it = mapping.find(name);
  if (it != mapping.end()) {
    throw DuplicateVarName(name);
  }
  new_entry->const_init = init;
  mapping[name] = std::unique_ptr<VariableTableEntry>{new_entry};
}
VariableTableEntry *VariableTableScope::lookUp(std::string name) {
  auto it = mapping.find(name);
  if (it == mapping.end()) {
    return nullptr;
  } else {
    // goal name exist in the current scope
    //  analysis name location is in entry's lifetime
    // if not,return nullptr
    return it->second.get();
  }
}
// 引入变量作用域后的查询
VariableTableEntry *VariableTableScope::lookUp(const std::string &name,
                                               int cur_line) {
  auto it = mapping.find(name);
  if (it == mapping.end()) {
    return nullptr;
  } else {
    // goal name exist in the current scope
    //  analysis name location is in entry's lifetime
    // if not,return nullptr
    auto entry = it->second.get();
    if (cur_line < entry->def_line) {
      // 变量作用域检查
      return nullptr;
    }
    return it->second.get();
  }
}
bool VariableTableScope::is_global() { return global; }
VariableTable::VariableTable() {
  head = new VariableTableScope();
  // set global scope
  head->global = true;
  cur = head;
}
void VariableTable::EntryScope() {
  // create new scope
  VariableTableScope *new_scope = new VariableTableScope();
  // set global info - false
  new_scope->global = false;
  //  add children
  cur->childrens.push_back(new_scope);
  new_scope->parent = cur;
  cur = new_scope;
}
void VariableTable::EntryScope(VariableTableScope *goal) {
  // 设置当前scope为链中已存在的Scope
  cur = goal;
}
void VariableTable::ExitScope() {
  if (cur->parent != nullptr)
    cur = cur->parent;
  // 保留符号表结构
}
void VariableTable::insert(const std::string &name, VariableTableEntry *entry) {
  cur->insert(name, entry);
}
void VariableTable::insert(std::string name, spanti::Type *var_type) {
  cur->insert(name, var_type);
}

void VariableTable::insert(std::string name, spanti::Type *var_type,
                           std::vector<int32_t> init) {
  cur->insert(name, var_type, init);
}

bool VariableTable::is_global() { return cur == head; }
VariableTableEntry *VariableTable::lookUp(const std::string &name) {
  VariableTableScope *c_scope = cur;
  while (c_scope != nullptr) {
    auto s = c_scope->lookUp(name);
    if (s)
      return s;
    // lookup next symbol_table
    c_scope = c_scope->parent;
  }
  return nullptr;
}
VariableTableEntry *VariableTable::lookUp(const std::string &name,
                                          VariableTableScope *cur_scope) {
  VariableTableScope *c_scope = cur_scope;
  while (c_scope != nullptr) {
    auto s = c_scope->lookUp(name);
    if (s)
      return s;
    // lookup next symbol_table
    c_scope = c_scope->parent;
  }
  return nullptr;
}
// 引入变量作用域后的查询
VariableTableEntry *VariableTable::lookUp(const std::string &name,
                                          VariableTableScope *cur_scope,
                                          int cur_line) {
  VariableTableScope *c_scope = cur_scope;
  while (c_scope != nullptr) {
    auto s = c_scope->lookUp(name, cur_line);
    if (s)
      return s;
    // lookup next symbol_table
    c_scope = c_scope->parent;
  }
  return nullptr;
}
VariableTableEntry *VariableTable::lookUp(const std::string &name,
                                          int cur_line) {
  VariableTableScope *c_scope = cur;
  while (c_scope != nullptr) {
    auto s = c_scope->lookUp(name, cur_line);
    if (s)
      return s;
    // lookup next symbol_table
    c_scope = c_scope->parent;
  }
  return nullptr;
}