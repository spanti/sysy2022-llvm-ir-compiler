#include "AST.h"
#include "ast/ASTvisitor.h"

void BinaryExprAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void UnaryExprAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void IntergarExprAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void FloatExprAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void IdentifierAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void ArrayExprAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void CallExprAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void ImplicitValExprAST::accept(class ASTNodeVisitor *visitor){
  visitor->visit(this);
}
void InitValExprAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void AssignStmtAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void IfStmtAST::accept(class ASTNodeVisitor *visitor) { visitor->visit(this); }
void WhileStmtAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void BreakStmtAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void ContinueStmtAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void BlockStmtAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void ConstDecAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
void VarDecAST::accept(class ASTNodeVisitor *visitor) { visitor->visit(this); }
void FuncDecAST::accept(class ASTNodeVisitor *visitor) { visitor->visit(this); }
void ProgramAST::accept(class ASTNodeVisitor *visitor) { visitor->visit(this); }
void ExpStmtAST::accept(class ASTNodeVisitor *visitor) { visitor->visit(this); }
void RetuStmtAST::accept(class ASTNodeVisitor *visitor) {
  visitor->visit(this);
}
// isconst负责推断表达式节点是否为常量并更新常量值
// 递归算法
bool BinaryExprAST::isconst(VariableTable *variables) {
  if (RHS->isconst(variables) && LHS->isconst(variables)) {
    switch (Op) {
    case Mul:
      ret = LHS->ret * RHS->ret;
      break;
    case Div:
    ret = LHS->ret / RHS->ret;
      break;
      // 除法和除数，常量不为0
    case Mod:
    ret = LHS->ret % RHS->ret;
      break;
    case Add:
    ret = LHS->ret + RHS->ret;
      break;
    case Sub:
    ret = LHS->ret - RHS->ret;
      break;
    case Lt:
    ret = LHS->ret < RHS->ret;
      break;
    case Gt:
    ret = LHS->ret > RHS->ret;
      break;
    case Le:
    ret = LHS->ret <= RHS->ret;
      break;
    case Ge:
    ret = LHS->ret >= RHS->ret;
      break;
    case Eq:
    ret = LHS->ret == RHS->ret;
      break;
    case Ne:
    ret = LHS->ret != RHS->ret;
      break;
    case And:
    ret = LHS->ret && RHS->ret;
      break;
    case Or:
    ret = LHS->ret || RHS->ret;
      break;
    default:
      return false;
    }
    return true;
  }
  return false;
}
bool UnaryExprAST::isconst(VariableTable *variables) {
  if (UHS->isconst(variables)) {
    // change ret
    if (Op == UpKind::Neg) {
      ret = -(UHS->ret);
      return true;
    } else if (Op == UpKind::Pos) {
      ret = UHS->ret;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
bool IntergarExprAST::isconst(VariableTable *variables) {
  // 常量
  ret = this->value;
  return true;
}
bool FloatExprAST::isconst(VariableTable *variables) {
  // 常量
  //  浮点数转化未实现,等待实现
  ret = -1;
  return true;
}
bool IdentifierAST::isconst(VariableTable *variables) {
  // find const in symboltable
  auto entry = variables->lookUp(this->getName());
  if (entry && entry->var_type->get_const()) {
    // set ret
    // handle N = 3
    // 3 的值未被记录
    ret = entry->const_init[0];
    return true;
  }
  return false;
}
bool ArrayExprAST::isconst(VariableTable *variables) {
  // a[3].a[3][4]
  // 不定，暂时为true
  ret = -1;
  return false;
}
bool InitValExprAST::isconst(VariableTable *variables) {
  // 默认为true
  // {1,2,3}
  ret = -1;
  return true;
}

bool ImplicitValExprAST::isconst(VariableTable *variables){
  ret = 0;
  return true;
}
