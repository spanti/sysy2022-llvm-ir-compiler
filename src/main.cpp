#include <iostream>

#include "antlr4-runtime.h"
#include "ast/ASTBuilder.h"
#include "parser/SysYBaseListener.h"
#include "parser/SysYLexer.h"
#include "parser/SysYParser.h"
#include "tree/pattern/ParseTreePattern.h"

using namespace antlr4;

int main(int argc, const char *argv[]) {
  std::ifstream stream;
  stream.open(argv[1]);
  ANTLRInputStream input(stream);
  SysYLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SysYParser parser(&tokens);

  SysYParser::CompUnitContext *root = parser.compUnit();
  tree::ParseTree *tree = root;
  // tree::ParseTree *tree = parser.varDecl();

  // can't use parseTreePattern - occur error: std::bad_alloc
  /*
  tree::pattern::ParseTreePattern p = parser.compileParseTreePattern("<bType>
  <varDef> ;",parser.RuleVarDecl);

  antlr4::tree::pattern::ParseTreeMatch m = p.match(tree);

  if(m.succeeded()){
      std::cout<<"tree match success!"<<std::endl;
  }
  else{
      std::cout<<"tree match failed!"<<std::endl;
  }*/

  ASTBuilder builder;
  //visitXXX - to generate the abstract syntax tree
  builder.visitCompUnit(root);
  // test output the tree
  std::cout << tree->toStringTree(&parser) << std::endl;
  return 0;
}