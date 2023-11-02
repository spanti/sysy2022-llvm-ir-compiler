#include "AST.h"
#include "ASTvisitor.h"
#include "Type.h"
#include <iostream>
void ASTPrinter::visit(class BinaryExprAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-BinaryExprAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " " << *(ast->getType()) << " " << ast->getOpStr() << "\n";
  // before type checking,not ensure the type of ExprAST

  ind++;
  ast->LHS->accept(this);
  ast->RHS->accept(this);
  ind--;
}
void ASTPrinter::visit(class UnaryExprAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-UnaryExprAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " " << *(ast->getType()) << " " << ast->getOpStr()  << "\n";

  // before type checking,not ensure the type of ExprAST
  ind++;
  ast->UHS->accept(this);
  ind--;
}
void ASTPrinter::visit(class IntergarExprAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-IntergarExprAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " " << *(ast->getType()) << " " << ast->value << "\n";
}
void ASTPrinter::visit(class FloatExprAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-FloatExprAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " " << *(ast->getType()) << " " << ast->getText() << "\n";
}
void ASTPrinter::visit(class IdentifierAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-IdentifierAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " " << *(ast->getType()) << " " << ast->getName() << "\n";
}
void ASTPrinter::visit(class ArrayExprAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-ArrayExprAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " " << *(ast->getType()) << " " << ast->Name<< "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  for (auto elem : ast->elems) {
    elem->accept(this);
  }
  ind--;
}
void ASTPrinter::visit(class CallExprAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-CallExprAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " ";
  std::cout << *(ast->getType()) << " " << ast->Callee<< "\n";
  // lack of function-type info
  // lack of type
  // col line
  // traverse the children
  ind++;
  //ast->Callee->accept(this);
  for (auto arg : ast->Args) {
    arg->accept(this);
  }
  ind--;
}
void ASTPrinter::visit(class ImplicitValExprAST *ast) {
    for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-ImplicitValExprAST";
  std::cout << " " << *(ast->getType()) << "\n";
}
void ASTPrinter::visit(class InitValExprAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-InitValExprAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  for (auto init : ast->Inits) {
    if (init) {
      init->accept(this);
    }
    else {
      for (int i = 0; i < ind; i++) {
        std::cout << " ";
      }
      std::cout << "-Nullptr" << std::endl;
    }
  }
  ind--;
}
void ASTPrinter::visit(class AssignStmtAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-AssignStmtAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  ast->LHS->accept(this);
  ast->RHS->accept(this);
  ind--;
}
void ASTPrinter::visit(class IfStmtAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-IfStmtAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  ast->Cond->accept(this);
  ast->Then->accept(this);
  if (ast->Else) {
    ast->Else->accept(this);
  }
  ind--;
}
void ASTPrinter::visit(class WhileStmtAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-WhileStmtAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  ast->Cond->accept(this);
  ast->Body->accept(this);
  ind--;
}
void ASTPrinter::visit(class BreakStmtAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-BreakStmtAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
}
void ASTPrinter::visit(class ContinueStmtAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-ContinueStmtAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
}
void ASTPrinter::visit(class BlockStmtAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-BlockStmtAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  for (auto stmt : ast->stmts) {
    stmt->accept(this);
  }
  ind--;
}
void ASTPrinter::visit(class ExpStmtAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-ExpStmtAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  if(ast->Expr)
    ast->Expr->accept(this);
  ind--;
}
void ASTPrinter::visit(class RetuStmtAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-RetuStmtAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  if (ast->Expr)
    ast->Expr->accept(this);
  ind--;
}
void ASTPrinter::visit(class ConstDecAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-ConstDecAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " ";
  std::cout << ast->getName() << " " << *(ast->getType()) << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  if (ast->initval)
    ast->initval->accept(this);
  ind--;
}
void ASTPrinter::visit(class VarDecAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-VarDecAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">";
  std::cout << " ";
  std::cout << ast->getName() << " " << *(ast->getType()) << "\n";
  // lack of type
  // col line
  // traverse the children
  ind++;
  if (ast->initval) {
    ast->initval->accept(this);
  }
  ind--;
}
void ASTPrinter::visit(class FuncDecAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-FuncDecAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";
  // lack of type
  // col line
  // traverse the children
  // param
  ind++;
  for (auto param : ast->Params) {
    param->accept(this);
  }
  ast->Body->accept(this);
  ind--;
}
void ASTPrinter::visit(class ProgramAST *ast) {
  for (int i = 0; i < ind; i++) {
    std::cout << " ";
  }
  std::cout << "-ProgramAST";
  std::cout << " <line:" << ast->getLine() << ", "
            << "col:" << ast->getCol() << ">"
            << "\n";

  // col line
  // traverse the children
  ind++;
  for (auto decl : ast->decls) {
    decl->accept(this);
  }
  ind--;
}