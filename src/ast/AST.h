#include "Type.h"
#include <map>
#include <memory>
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
  virtual ~ASTNode() = default;
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

public:
  spanti::Type *Ty;
  // left value
  bool LValue;
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
      : ExprAST(loc, Ty, LValue), Op(Op),LHS(std::move(LHS)), RHS(std::move(RHS)) {}

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
      : ExprAST(loc, Ty, LValue), UHS(std::move(UHS)) {}

public:
  UpKind Op;
  std::shared_ptr<ExprAST> UHS;
};
// 5
class IntergarExprAST : public ExprAST {
public:
  IntergarExprAST(sourceLocation loc, spanti::Type *Ty, bool LValue,
                  int64_t value)
      : ExprAST(loc, Ty, LValue), value(value) {}

public:
  int64_t value;
};
//12.5
class FloatExprAST : public ExprAST {
public:
  FloatExprAST(sourceLocation loc, spanti::Type *Ty, bool LValue, int64_t value)
      : ExprAST(loc, Ty, LValue), value(value) {}

public:
  int64_t value; // need to change
};
// LVal: a,a[5]
class IdentifierAST : public ExprAST {
  public:
  IdentifierAST(sourceLocation loc, spanti::Type *Ty,bool LValue,std::string Name):
  ExprAST(loc,Ty,LValue),Name(Name){}
  public:
  std::string Name;
};
//gcd(12,3)
class CallExprAST : public ExprAST {
  public:
  CallExprAST(sourceLocation loc, spanti::Type *Ty,std::shared_ptr<ExprAST> Callee,
  std::vector<std::shared_ptr<ExprAST>> Args):ExprAST(loc,Ty,false),
  Callee(std::move(Callee)),Args(std::move(Args)){}
  public:
  std::shared_ptr<ExprAST> Callee;
  std::vector<std::shared_ptr<ExprAST>> Args;
};
// StmtAST
// need to add
// a = 5
class AssignStmtAST : public StmtAST {};
class IfStmtAST : public StmtAST {
public:
  IfStmtAST(sourceLocation loc, std::shared_ptr<ExprAST> Cond,
            std::shared_ptr<StmtAST> Then, std::shared_ptr<StmtAST> Else)
      : StmtAST(loc), Cond(std::move(Cond)), Then(std::move(Then)),
        Else(std::move(Else)) {}

public:
  std::shared_ptr<ExprAST> Cond;
  std::shared_ptr<StmtAST> Then, Else;
};
class WhileStmtAST : public StmtAST {
public:
  WhileStmtAST(sourceLocation loc, std::shared_ptr<ExprAST> Cond,
               std::shared_ptr<StmtAST> Body)
      : StmtAST(loc), Cond(std::move(Cond)), Body(std::move(Body)) {}

public:
  std::shared_ptr<ExprAST> Cond;
  std::shared_ptr<StmtAST> Body;
};
class BreakStmtAST : public StmtAST {};
class ContinueStmtAST : public StmtAST {};
class RetuStmtAST : public StmtAST {
public:
  RetuStmtAST(sourceLocation loc, std::shared_ptr<ExprAST> Expr)
      : StmtAST(loc), Expr(std::move(Expr)) {}

public:
  std::shared_ptr<ExprAST> Expr;
};
//[stmt1,stmt2,stmt3,stmt4]
class BlockStmtAST : public StmtAST {
public:
  BlockStmtAST(sourceLocation loc, std::vector<std::shared_ptr<ASTNode>> stmts)
      : StmtAST(loc), stmts(std::move(stmts)) {}

public:
  std::vector<std::shared_ptr<ASTNode>> stmts;//store stmt or decl
};
// DecAST - need type
// need to add
// const int a;
class ConstDecAST : public DecAST {
public:
  ConstDecAST(sourceLocation loc, std::string name, spanti::Type *Ty)
      : DecAST(loc, name, Ty) {}
};
// int a;
class VarDecAST : public DecAST {
public:
  VarDecAST(sourceLocation loc, std::string name, spanti::Type *Ty)
      : DecAST(loc, name, Ty) {}
};
class FuncDecAST : public DecAST {
public:
  FuncDecAST(sourceLocation loc, std::string name, spanti::FunctionType *FuncTy,
             std::vector<std::shared_ptr<VarDecAST>> Params,
             std::shared_ptr<StmtAST> Body)
      : DecAST(loc, name, FuncTy), Params(std::move(Params)),
        Body(std::move(Body)) {}

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

public:
  std::vector<std::shared_ptr<DecAST>> decls;
};
