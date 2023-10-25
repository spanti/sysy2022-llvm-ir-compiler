#pragma once
#include <stdexcept>
#include <string>
// 异常通常在语法分析，语义分析过程中抛出
class SyntaxError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};
// 设计一系列异常类来处理异常
class UnrecognizedVarName : public SyntaxError {
public:
  UnrecognizedVarName(std::string name)
      : SyntaxError("unrecognized variable name: " + std::move(name)) {}
};
class UnrecognizedFuncName : public SyntaxError {
public:
  UnrecognizedFuncName(std::string name)
      : SyntaxError("unrecognized function name: " + std::move(name)) {}
};
// 与函数参数相关的异常抛出
class InvalidFuncCallArg : public SyntaxError {
public:
  InvalidFuncCallArg(std::string msg) : SyntaxError(std::move(msg)) {}
};
class MainFuncNotFound : public SyntaxError {
public:
  MainFuncNotFound() : SyntaxError("main function not found") {}
};
class InvalidMainFuncEntry : public SyntaxError {
public:
  InvalidMainFuncEntry(std::string msg) : SyntaxError(std::move(msg)) {}
};
class InvalidArrayEntry : public SyntaxError {
public:
  InvalidArrayEntry() : SyntaxError("invalid array sizes") {}
};
class DuplicateFuncName : public SyntaxError {
 public:
  DuplicateFuncName(std::string name)
      : SyntaxError("duplicate function name: " + std::move(name)) {}
};
class DuplicateVarName : public SyntaxError {
 public:
  DuplicateVarName(std::string name)
      : SyntaxError("duplicate variable name: " + std::move(name)) {}
};
class InvalidInitList : public SyntaxError {
 public:
  InvalidInitList() : SyntaxError("invalid initialization list") {}
};
class InitTypeError : public SyntaxError {
 public:
  InitTypeError() : SyntaxError("the type of init not match the type decl") {}
};
class InvalidIndexOperator : public SyntaxError {
 public:
  InvalidIndexOperator() : SyntaxError("operator[] on non-array") {}
};
class InvalidLiteral : public SyntaxError {
 public:
  InvalidLiteral(std::string msg) : SyntaxError(std::move(msg)) {}
};
class AssignmentTypeError : public SyntaxError {
 public:
  AssignmentTypeError(std::string msg) : SyntaxError(std::move(msg)) {}
};




