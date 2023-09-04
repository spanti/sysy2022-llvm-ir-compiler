#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "parser/SysYBaseVisitor.h"
#include "AST.h"

class ASTBuilder : public SysYBaseVisitor {
public:
  ASTBuilder() {}
  ASTBuilder(const ASTBuilder &) = default;
  ASTBuilder(ASTBuilder &&) = default;
  ASTBuilder &operator=(const ASTBuilder &) = default;
  ASTBuilder &operator=(ASTBuilder &&) = default;

  std::any visitCompUnit(SysYParser::CompUnitContext *ctx) override;

  std::any visitDecl(SysYParser::DeclContext *ctx) override;

  std::any visitConstDecl(SysYParser::ConstDeclContext *ctx) override;

  std::any visitBType(SysYParser::BTypeContext *ctx) override;

  std::any visitConstDef(SysYParser::ConstDefContext *ctx) override;

  std::any
  visitScalarConstInitVal(SysYParser::ScalarConstInitValContext *ctx) override;

  std::any
  visitListConstInitVal(SysYParser::ListConstInitValContext *ctx) override;

  std::any visitVarDecl(SysYParser::VarDeclContext *ctx) override;

  std::any visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) override;

  std::any visitInitVarDef(SysYParser::InitVarDefContext *ctx) override;

  std::any visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) override;

  std::any visitListInitval(SysYParser::ListInitvalContext *ctx) override;

  std::any visitFuncDef(SysYParser::FuncDefContext *ctx) override;

  std::any visitFuncType(SysYParser::FuncTypeContext *ctx) override;

  std::any visitFuncFParams(SysYParser::FuncFParamsContext *ctx) override;

  std::any visitFuncFParam(SysYParser::FuncFParamContext *ctx) override;

  std::any visitBlock(SysYParser::BlockContext *ctx) override;

  std::any visitBlockItem(SysYParser::BlockItemContext *ctx) override;

  std::any visitAssignment(SysYParser::AssignmentContext *ctx) override;

  std::any visitExpStmt(SysYParser::ExpStmtContext *ctx) override;

  std::any visitBlockStmt(SysYParser::BlockStmtContext *ctx) override;

  std::any visitIfStmt1(SysYParser::IfStmt1Context *ctx) override;

  std::any visitIfStmt2(SysYParser::IfStmt2Context *ctx) override;

  std::any visitWhileStmt(SysYParser::WhileStmtContext *ctx) override;

  std::any visitBreakStmt(SysYParser::BreakStmtContext *ctx) override;

  std::any visitContinueStmt(SysYParser::ContinueStmtContext *ctx) override;

  std::any visitReturnStmt(SysYParser::ReturnStmtContext *ctx) override;

  std::any visitExp(SysYParser::ExpContext *ctx) override;

  std::any visitCond(SysYParser::CondContext *ctx) override;

  std::any visitLVal(SysYParser::LValContext *ctx) override;

  std::any visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) override;

  std::any visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) override;

  std::any visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) override;

  std::any visitNumber(SysYParser::NumberContext *ctx) override;

  std::any visitUnary1(SysYParser::Unary1Context *ctx) override;

  std::any visitUnary2(SysYParser::Unary2Context *ctx) override;

  std::any visitUnary3(SysYParser::Unary3Context *ctx) override;

  std::any visitUnaryOp(SysYParser::UnaryOpContext *ctx) override;

  std::any visitFuncRParams(SysYParser::FuncRParamsContext *ctx) override;

  std::any visitExpAsRParam(SysYParser::ExpAsRParamContext *ctx) override;

  std::any visitStringAsRParam(SysYParser::StringAsRParamContext *ctx) override;

  std::any visitMul2(SysYParser::Mul2Context *ctx) override;

  std::any visitMul1(SysYParser::Mul1Context *ctx) override;

  std::any visitAdd2(SysYParser::Add2Context *ctx) override;

  std::any visitAdd1(SysYParser::Add1Context *ctx) override;

  std::any visitRel2(SysYParser::Rel2Context *ctx) override;

  std::any visitRel1(SysYParser::Rel1Context *ctx) override;

  std::any visitEq1(SysYParser::Eq1Context *ctx) override;

  std::any visitEq2(SysYParser::Eq2Context *ctx) override;

  std::any visitLAnd2(SysYParser::LAnd2Context *ctx) override;

  std::any visitLAnd1(SysYParser::LAnd1Context *ctx) override;

  std::any visitLOr1(SysYParser::LOr1Context *ctx) override;

  std::any visitLOr2(SysYParser::LOr2Context *ctx) override;

  std::any visitConstExp(SysYParser::ConstExpContext *ctx) override;
};