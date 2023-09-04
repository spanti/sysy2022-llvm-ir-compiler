#include "ast/ASTBuilder.h"
#include "support/Any.h"
#include <any>
#include <memory>

// implement visitxx methods
// full these function
std::any ASTBuilder::visitCompUnit(SysYParser::CompUnitContext *ctx) {
  std::vector<std::shared_ptr<DecAST>> decls;
  std::cout << "visit CompUnit!" << std::endl;
  for (auto declCtx : ctx->decl()) {
    auto decl = visit(declCtx);
  }
  for (auto funcCtx : ctx->funcDef()) {
    auto func = visit(funcCtx);
    // insert ast into array
    auto ast = std::any_cast<std::shared_ptr<DecAST>>(func);
    decls.push_back(ast);
  }
  auto compUnit = std::make_shared<ProgramAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      decls);
  return compUnit;
}

std::any ASTBuilder::visitDecl(SysYParser::DeclContext *ctx) {
  std::cout << "visit Decl!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
  std::cout << "visit ConstDecl!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitBType(SysYParser::BTypeContext *ctx) {
  std::cout << "visit BType!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitConstDef(SysYParser::ConstDefContext *ctx) {
  std::cout << "visit ConstDef!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitScalarConstInitVal(
    SysYParser::ScalarConstInitValContext *ctx) {
  std::cout << "visit ScalarConstInitVal!" << std::endl;
  return visitChildren(ctx);
}

std::any
ASTBuilder::visitListConstInitVal(SysYParser::ListConstInitValContext *ctx) {
  std::cout << "visit ListConstInitVal!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitVarDecl(SysYParser::VarDeclContext *ctx) {
  std::cout << "visit VarDecl!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) {
  std::string idname = ctx->getText();
  std::cout << "visit UninitVarDef!" << idname << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitInitVarDef(SysYParser::InitVarDefContext *ctx) {
  std::cout << "visit InitVarDef!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) {
  std::cout << "visit ScalarInitVal!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitListInitval(SysYParser::ListInitvalContext *ctx) {
  std::cout << "visit ListInitval!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitFuncDef(SysYParser::FuncDefContext *ctx) {
  std::cout << "visit FuncDef! "
            << "function-name:" << ctx->Identifier()->getText() << std::endl;
  // function typr - 临时
  auto *FuncTy = new spanti::FunctionType(spanti::Type::SP_EMPTYTYPE);
  // function name
  std::string name = ctx->Identifier()->getText();
  // funcparam
  std::vector<std::shared_ptr<VarDecAST>> Params;
  // body - block
  auto block = std::any_cast<std::shared_ptr<StmtAST>>(visit(ctx->block()));
  // build decl
  std::shared_ptr<DecAST> func_ast = std::make_shared<FuncDecAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      name, FuncTy, Params, block);
  return func_ast;
}

std::any ASTBuilder::visitFuncType(SysYParser::FuncTypeContext *ctx) {
  std::cout << "visit FuncType!" << std::endl;
  // not handle now
  return visitChildren(ctx);
}

std::any ASTBuilder::visitFuncFParams(SysYParser::FuncFParamsContext *ctx) {
  std::cout << "visit FuncFParams!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitFuncFParam(SysYParser::FuncFParamContext *ctx) {
  std::cout << "visit FuncFParam!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitBlock(SysYParser::BlockContext *ctx) {
  // store stmts
  std::vector<std::shared_ptr<ASTNode>> blockitems;
  std::cout << "visit Block!" << std::endl;
  for (auto *itemCtx : ctx->blockItem()) {
    auto blockitem = visit(itemCtx);
    auto ast = std::any_cast<std::shared_ptr<ASTNode>>(blockitem);
    blockitems.push_back(std::move(ast));
  }
  // build astnode
  std::shared_ptr<StmtAST> ast = std::make_shared<BlockStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      std::move(blockitems));
  return ast;
}

std::any ASTBuilder::visitBlockItem(SysYParser::BlockItemContext *ctx) {
  std::cout << "visit BlockItem!" << std::endl;
  // stmt
  auto stmt = visit(ctx->stmt());
  std::shared_ptr<ASTNode> ast = std::any_cast<std::shared_ptr<StmtAST>>(stmt);
  // decl
  return ast;
}

std::any ASTBuilder::visitAssignment(SysYParser::AssignmentContext *ctx) {
  std::cout << "visit Assignment!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitExpStmt(SysYParser::ExpStmtContext *ctx) {
  std::cout << "visit ExpStmt!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitBlockStmt(SysYParser::BlockStmtContext *ctx) {
  std::cout << "visit BlockStmt!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitIfStmt1(SysYParser::IfStmt1Context *ctx) {
  std::cout << "visit IfStmt1!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitIfStmt2(SysYParser::IfStmt2Context *ctx) {
  std::cout << "visit IfStmt2!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitWhileStmt(SysYParser::WhileStmtContext *ctx) {
  std::cout << "visit WhileStmt!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitBreakStmt(SysYParser::BreakStmtContext *ctx) {
  std::cout << "visit BreakStmt!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitContinueStmt(SysYParser::ContinueStmtContext *ctx) {
  std::cout << "visit ContinueStmt!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitReturnStmt(SysYParser::ReturnStmtContext *ctx) {
  std::cout << "visit ReturnStmt!" << std::endl;
  // build return AST node
  auto re_exp = visit(ctx->exp());
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(re_exp);
  std::shared_ptr<StmtAST> retn_ast = std::make_shared<RetuStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      ast);
  return retn_ast;
}

std::any ASTBuilder::visitExp(SysYParser::ExpContext *ctx) {
  std::cout << "visit Exp!" << std::endl;
  auto addexp = visit(ctx->addExp());
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(addexp);
  return ast;
}

std::any ASTBuilder::visitCond(SysYParser::CondContext *ctx) {
  std::cout << "visit Cond!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitLVal(SysYParser::LValContext *ctx) {
  std::cout << "visit LVal!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) {
  std::cout << "visit PrimaryExp1!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) {
  std::cout << "visit PrimaryExp2!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) {
  std::cout << "visit PrimaryExp3!" << std::endl;
  auto num = visit(ctx->number());
  auto n_ast = std::any_cast<std::shared_ptr<ExprAST>>(num);
  return n_ast;
}

std::any ASTBuilder::visitNumber(SysYParser::NumberContext *ctx) {
  std::shared_ptr<ExprAST> num_ast;
  std::string num = ctx->getText();
  // 1.IntLiteral:100
  // transform string to num (base 10)
  if(ctx->IntLiteral()){
    int64_t Val = std::strtoll(num.c_str(), nullptr, 10);
    num_ast = std::make_shared<IntergarExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      new spanti::IntType(), false, Val);
  }
  else{
    int64_t Val = std::strtoll(num.c_str(), nullptr, 16);
    //float value - store it in Val;
  }
  std::cout << "visit Number! "
            << "number:" << num << std::endl;
  return num_ast;
  // return visitChildren(ctx);
}

std::any ASTBuilder::visitUnary1(SysYParser::Unary1Context *ctx) {
  std::cout << "visit Unary1!" << std::endl;
  auto primary = visit(ctx->primaryExp());
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(primary);
  return ast;
}

std::any ASTBuilder::visitUnary2(SysYParser::Unary2Context *ctx) {
  std::cout << "visit Unary2!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitUnary3(SysYParser::Unary3Context *ctx) {
  std::cout << "visit Unary3!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitUnaryOp(SysYParser::UnaryOpContext *ctx) {
  std::cout << "visit UnaryOp!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitFuncRParams(SysYParser::FuncRParamsContext *ctx) {
  std::cout << "visit FuncRParams!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitExpAsRParam(SysYParser::ExpAsRParamContext *ctx) {
  std::cout << "visit ExpAsRParam!" << std::endl;
  return visitChildren(ctx);
}

std::any
ASTBuilder::visitStringAsRParam(SysYParser::StringAsRParamContext *ctx) {
  std::cout << "visit StringAsRParam!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitMul2(SysYParser::Mul2Context *ctx) {
  std::cout << "visit Mul2!" << std::endl;
  BinaryExprAST::OpKind opkind;
  if (ctx->Division()) {
    opkind = BinaryExprAST::Div;
  } else if (ctx->Multiplication()) {
    opkind = BinaryExprAST::Mul;
  } else {
    opkind = BinaryExprAST::Mod;
  }
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->mulExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->unaryExp()));
  // 类型转换问题 - need to consider
  // the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      new spanti::Type(spanti::Type::SP_EMPTYTYPE), false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitMul1(SysYParser::Mul1Context *ctx) {
  std::cout << "visit Mul1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->unaryExp()));
  return ast;
}

std::any ASTBuilder::visitAdd2(SysYParser::Add2Context *ctx) {
  std::cout << "visit Add2!" << std::endl;
  BinaryExprAST::OpKind opkind;
  if (ctx->Addition()) {
    //'+'
    opkind = BinaryExprAST::Add;
  } else {
    //'-'
    opkind = BinaryExprAST::Sub;
  }
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->addExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->mulExp()));
  // 类型转换问题 - need to consider
  // the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      new spanti::Type(spanti::Type::SP_EMPTYTYPE), false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitAdd1(SysYParser::Add1Context *ctx) {
  std::cout << "visit Add1!" << std::endl;
  auto mulexp = visit(ctx->mulExp());
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(mulexp);
  return ast;
}

std::any ASTBuilder::visitRel2(SysYParser::Rel2Context *ctx) {
  std::cout << "visit Rel2!" << std::endl;
  BinaryExprAST::OpKind opkind;
  if (ctx->GE()) {
    //'+'
    opkind = BinaryExprAST::Ge;
  } else if (ctx->GT()) {
    opkind = BinaryExprAST::Gt;
  } else if (ctx->LE()) {
    opkind = BinaryExprAST::Le;
  } else if (ctx->GE()) {
    opkind = BinaryExprAST::Ge;
  }
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->relExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->addExp()));
  // 类型转换问题 - need to consider
  // the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      new spanti::Type(spanti::Type::SP_EMPTYTYPE), false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitRel1(SysYParser::Rel1Context *ctx) {
  std::cout << "visit Rel1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->addExp()));
  return ast;
}

std::any ASTBuilder::visitEq1(SysYParser::Eq1Context *ctx) {
  std::cout << "visit Eq1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->relExp()));
  return ast;
}

std::any ASTBuilder::visitEq2(SysYParser::Eq2Context *ctx) {
  std::cout << "visit Eq2!" << std::endl;
  BinaryExprAST::OpKind opkind;
  if (ctx->EQ()) {
    //'+'
    opkind = BinaryExprAST::Eq;
  } else if (ctx->NEQ()) {
    opkind = BinaryExprAST::Ne;
  }
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->eqExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->relExp()));
  // 类型转换问题 - need to consider
  // the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      new spanti::Type(spanti::Type::SP_EMPTYTYPE), false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitLAnd2(SysYParser::LAnd2Context *ctx) {
  std::cout << "visit LAnd2!" << std::endl;
BinaryExprAST::OpKind opkind;
  opkind = BinaryExprAST::OpKind::And;
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->lAndExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->eqExp()));
  // 类型转换问题 - need to consider
  // the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      new spanti::Type(spanti::Type::SP_EMPTYTYPE), false, opkind , LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitLAnd1(SysYParser::LAnd1Context *ctx) {
  std::cout << "visit LAnd1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->eqExp()));
  return ast;
}

std::any ASTBuilder::visitLOr1(SysYParser::LOr1Context *ctx) {
  std::cout << "visit LOr1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->lAndExp()));
  return ast;
}

std::any ASTBuilder::visitLOr2(SysYParser::LOr2Context *ctx) {
  std::cout << "visit LOr2!" << std::endl;
  BinaryExprAST::OpKind opkind;
  opkind = BinaryExprAST::OpKind::Or;
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->lOrExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->lAndExp()));
  // 类型转换问题 - need to consider
  // the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      new spanti::Type(spanti::Type::SP_EMPTYTYPE), false, opkind , LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitConstExp(SysYParser::ConstExpContext *ctx) {
  std::cout << "visit ConstExp!" << std::endl;
  return visitChildren(ctx);
}
