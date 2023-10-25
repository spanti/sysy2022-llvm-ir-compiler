#pragma once
#include "SymbolTable.h"
#include "Type.h"
#include <cstdint>
#include <map>
#include <memory>
#include <utility>
#include <vector>

struct sourceLocation {
  size_t Line;
  size_t Col;
};
// extend these ASTNode
class ASTNode {
public:
  enum Kind {
    // program
    Program,
    // decl
    VarDecl,
    FunctionDecl,
    // expr
    NumberExpr,
    BinaryExpr,
    UnaryExpr,
    CallExpr,
    VarDefExpr,
    // stmt
    IfStmt,
    AssignStmt,
    BlockStmt,
    ContinueStmt,
    RetuStmt,
    WhileStmt,
  };
  ASTNode(sourceLocation Loc) : loc(Loc) {}
  virtual ~ASTNode(){};
  // 纯虚函数，派生类必须实例化
  virtual void accept(class ASTNodeVisitor *visitor) = 0;
  int getLine() { return loc.Line; }
  int getCol() { return loc.Col; }
  // accept method prepared for tree visitor
  // example : virtual void accept(class ASTNodeVisitor*) = 0;
public:
  sourceLocation loc;

  // kind yield : store some type information
};
// some class inherit from the base ast node
class ExprAST : public ASTNode {
public:
  ExprAST(sourceLocation loc, spanti::Type *Ty, bool LValue)
      : ASTNode(loc), Ty(Ty), LValue(LValue) {}
  bool isLValue() const { return LValue; }
  spanti::Type *getType() const { return Ty; }
  void setType(spanti::Type *type) { Ty = type; }
  virtual bool isconst(VariableTable *variables) { return false; }
  int32_t get_constant() { return ret; }

public:
  spanti::Type *Ty;
  // left value
  bool LValue;
  // constant ret
  int32_t ret;
};
class StmtAST : public ASTNode {
public:
  StmtAST(sourceLocation loc) : ASTNode(loc) {}
};
class DecAST : public ASTNode {
public:
  DecAST(sourceLocation loc, std::string name, spanti::Type *Ty)
      : ASTNode(loc), name(name), Ty(Ty) {}
  std::string getName() const { return name; }
  spanti::Type *getType() const { return Ty; }

public:
  std::string name;
  spanti::Type *Ty;
};
// inherit from ExprAST
//  1 + 2
class BinaryExprAST : public ExprAST {
  // transform tokens -> OpKind
public:
  enum OpKind {
    Mul,
    Div,
    Mod,
    Add,
    Sub,
    Lt, //'<'
    Gt, //'>'
    Le,
    Ge,
    Eq, //'='
    Ne,
    And, //'&&'
    Or,  //'||'
  };
  BinaryExprAST(sourceLocation loc, spanti::Type *Ty, bool LValue, OpKind Op,
                std::shared_ptr<ExprAST> LHS, std::shared_ptr<ExprAST> RHS)
      : ExprAST(loc, Ty, LValue), Op(Op), LHS(std::move(LHS)),
        RHS(std::move(RHS)) {}
  void accept(class ASTNodeVisitor *visitor) override;
  // design for ASTprinter
  std::string getOpStr() const {
    std::string OpStr;
    switch (Op) {
    case OpKind::Mul:
      OpStr = "*";
      break;
    case OpKind::Div:
      OpStr = "/";
      break;
    case OpKind::Add:
      OpStr = "+";
      break;
    case OpKind::Sub:
      OpStr = "-";
      break;
    case OpKind::Mod:
      OpStr = "%";
      break;
    case OpKind::Gt:
      OpStr = ">";
      break;
    case OpKind::Lt:
      OpStr = "<";
      break;
    case OpKind::Ge:
      OpStr = ">=";
      break;
    case OpKind::Le:
      OpStr = "<=";
      break;
    case OpKind::Eq:
      OpStr = "==";
      break;
    case OpKind::Ne:
      OpStr = "!=";
      break;
    case OpKind::And:
      OpStr = "&&";
      break;
    case OpKind::Or:
      OpStr = "||";
      break;
    default:
      break;
    }
    return OpStr;
  }
  bool isconst(VariableTable *variables) override;
  int32_t get_ret() { return ret; }

public:
  OpKind Op;
  std::shared_ptr<ExprAST> LHS, RHS;
};
//-5
class UnaryExprAST : public ExprAST {
public:
  enum UpKind {
    Pos, //+
    Neg, //-
    Not, //!
  };
  UnaryExprAST(sourceLocation loc, spanti::Type *Ty, bool LValue, UpKind Op,
               std::shared_ptr<ExprAST> UHS)
      : ExprAST(loc, Ty, LValue), Op(Op), UHS(std::move(UHS)) {}
  void accept(class ASTNodeVisitor *visitor) override;
  bool isconst(VariableTable *variables) override;
  std::string getOpStr() const {
    std::string OpStr;
    switch (Op) {
    case UpKind::Pos:
      OpStr = "+";
      break;
    case UpKind::Neg:
      OpStr = "-";
      break;
    case UpKind::Not:
      OpStr = "!";
      break;
    default:
      break;
    }
    return OpStr;
  }

public:
  UpKind Op;
  std::shared_ptr<ExprAST> UHS;
};
// 5
class IntergarExprAST : public ExprAST {
public:
  IntergarExprAST(sourceLocation loc, bool LValue, int64_t value)
      : ExprAST(loc, new spanti::IntType(true), LValue), value(value) {}
  void accept(class ASTNodeVisitor *visitor) override;
  bool isconst(VariableTable *variables) override;

public:
  int32_t value;
};
// 12.5
class FloatExprAST : public ExprAST {
public:
  FloatExprAST(sourceLocation loc, bool LValue, std::string value, int32_t num)
      : ExprAST(loc, new spanti::FloatType(true), LValue), text(value),
        value(num) {}
  // the accept() method of element
  void accept(class ASTNodeVisitor *visitor) override;
  std::string getText() const { return text; }
  bool isconst(VariableTable *variables) override;

public:
  std::string text; // 浮点数
  // 使用C自带的float存储值
  float value;
  // int64_t value; // need to change
};
// LVal: a,a[5]
class IdentifierAST : public ExprAST {
public:
  IdentifierAST(sourceLocation loc, spanti::Type *Ty, bool LValue,
                std::string Name)
      : ExprAST(loc, Ty, LValue), Name(Name) {}
  void accept(class ASTNodeVisitor *visitor) override;
  std::string getName() const { return Name; }
  bool isconst(VariableTable *variables) override;

public:
  std::string Name;
};
class ArrayExprAST : public ExprAST {
public:
  ArrayExprAST(sourceLocation loc, spanti::Type *Ty, bool LValue,
               std::string Name, std::vector<std::shared_ptr<ExprAST>> &elems)
      : ExprAST(loc, Ty, LValue), Name(Name), elems(std::move(elems)) {}
  void accept(class ASTNodeVisitor *visitor) override;
  bool isconst(VariableTable *variables) override;

public:
  std::string Name;
  // index
  std::vector<std::shared_ptr<ExprAST>> elems;
};
// gcd(12,3)/gcd(a,3)
class CallExprAST : public ExprAST {
public:
  CallExprAST(sourceLocation loc, spanti::Type *Ty, std::string Callee,
              std::vector<std::shared_ptr<ExprAST>> Args)
      : ExprAST(loc, Ty, false), Callee(std::move(Callee)),
        Args(std::move(Args)) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::string Callee;
  // std::shared_ptr<ExprAST> Callee;
  std::vector<std::shared_ptr<ExprAST>> Args;
};
// 处理数组中的隐式值
//  具备类型，不具备定位，非左值
class ImplicitValExprAST : public ExprAST {
public:
  ImplicitValExprAST(sourceLocation loc, spanti::Type *Ty)
      : ExprAST(loc, Ty, false) {}
  void accept(class ASTNodeVisitor *visitor) override;

  bool isconst(VariableTable *variables) override;

public:
  // 只允许嵌套InitValExprAST
  std::vector<std::shared_ptr<ExprAST>> Inits;
};
// 初始化值AST
//{1,2,3,4}
// 其类型为ArrayType
class InitValExprAST : public ExprAST {
public:
  InitValExprAST(sourceLocation loc, spanti::Type *Ty,
                 std::vector<std::shared_ptr<ExprAST>> Inits)
      : ExprAST(loc, Ty, false), Inits(Inits) {}
  void accept(class ASTNodeVisitor *visitor) override;

  bool isconst(VariableTable *variables) override;

public:
  // 只允许嵌套InitValExprAST
  std::vector<std::shared_ptr<ExprAST>> Inits;
};
// StmtAST
// need to add
// a = 5
class AssignStmtAST : public StmtAST {
public:
  AssignStmtAST(sourceLocation loc, std::shared_ptr<ExprAST> LHS,
                std::shared_ptr<ExprAST> RHS)
      : StmtAST(loc), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::shared_ptr<ExprAST> LHS, RHS;
};
class IfStmtAST : public StmtAST {
public:
  IfStmtAST(sourceLocation loc, std::shared_ptr<ExprAST> Cond,
            std::shared_ptr<StmtAST> Then, std::shared_ptr<StmtAST> Else)
      : StmtAST(loc), Cond(std::move(Cond)), Then(std::move(Then)),
        Else(std::move(Else)) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::shared_ptr<ExprAST> Cond;
  std::shared_ptr<StmtAST> Then, Else;
};
class WhileStmtAST : public StmtAST {
public:
  WhileStmtAST(sourceLocation loc, std::shared_ptr<ExprAST> Cond,
               std::shared_ptr<StmtAST> Body)
      : StmtAST(loc), Cond(std::move(Cond)), Body(std::move(Body)) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::shared_ptr<ExprAST> Cond;
  std::shared_ptr<StmtAST> Body;
};
// break;
class BreakStmtAST : public StmtAST {
public:
  BreakStmtAST(sourceLocation loc) : StmtAST(loc) {}
  void accept(class ASTNodeVisitor *visitor) override;
};
// continue;
class ContinueStmtAST : public StmtAST {
public:
  ContinueStmtAST(sourceLocation loc) : StmtAST(loc){};
  void accept(class ASTNodeVisitor *visitor) override;
};
class RetuStmtAST : public StmtAST {
public:
  RetuStmtAST(sourceLocation loc, std::shared_ptr<ExprAST> Expr)
      : StmtAST(loc), Expr(std::move(Expr)) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::shared_ptr<ExprAST> Expr;
};
//[stmt1,stmt2,stmt3,stmt4]
class BlockStmtAST : public StmtAST {
public:
  BlockStmtAST(sourceLocation loc, std::vector<std::shared_ptr<ASTNode>> stmts,
               VariableTableScope *cur_scope)
      : StmtAST(loc), stmts(std::move(stmts)), cur_ast_scope(cur_scope) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  VariableTableScope *cur_ast_scope;           // ast对应的作用域节点
  std::vector<std::shared_ptr<ASTNode>> stmts; // store stmt or decl
};
// 1+2;
class ExpStmtAST : public StmtAST {
public:
  ExpStmtAST(sourceLocation loc, std::shared_ptr<ExprAST> exp)
      : StmtAST(loc), Expr(std::move(exp)) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::shared_ptr<ExprAST> Expr;
};
// DecAST - need type
// need to add
// const int a;
class ConstDecAST : public DecAST {
public:
  ConstDecAST(sourceLocation loc, std::string name, spanti::Type *Ty,
              std::shared_ptr<ExprAST> initval)
      : DecAST(loc, name, Ty), initval(std::move(initval)) {}
  ConstDecAST(sourceLocation loc, std::string name, spanti::Type *Ty)
      : DecAST(loc, name, Ty) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::shared_ptr<ExprAST> initval;
};
// int a;
// int b=5;
class VarDecAST : public DecAST {
public:
  VarDecAST(sourceLocation loc, std::string name, spanti::Type *Ty,
            std::shared_ptr<ExprAST> initval)
      : DecAST(loc, name, Ty), initval(std::move(initval)) {}
  VarDecAST(sourceLocation loc, std::string name, spanti::Type *Ty)
      : DecAST(loc, name, Ty), initval(nullptr) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::shared_ptr<ExprAST> initval;
};
class FuncDecAST : public DecAST {
public:
  FuncDecAST(sourceLocation loc, std::string name, spanti::FunctionType *FuncTy,
             std::vector<std::shared_ptr<VarDecAST>> Params,
             std::shared_ptr<StmtAST> Body)
      : DecAST(loc, name, FuncTy), Params(std::move(Params)),
        Body(std::move(Body)) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  // Param
  std::vector<std::shared_ptr<VarDecAST>> Params;
  // Body
  std::shared_ptr<StmtAST> Body;
};
// ProgramAST
class ProgramAST : public ASTNode {
public:
  ProgramAST(sourceLocation loc, std::vector<std::shared_ptr<DecAST>> decls)
      : ASTNode(loc), decls(std::move(decls)) {}
  void accept(class ASTNodeVisitor *visitor) override;

public:
  std::vector<std::shared_ptr<DecAST>> decls;
};
