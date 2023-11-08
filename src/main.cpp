#include <iostream>
#include <memory>

#include "antlr4-runtime.h"
#include "ast/ASTBuilder.h"
#include "ast/ASTvisitor.h"
#include "errors.h"
#include "parser/SysYBaseListener.h"
#include "parser/SysYLexer.h"
#include "parser/SysYParser.h"
#include "tree/pattern/ParseTreePattern.h"

using namespace antlr4;

int main(int argc, const char *argv[]) {
  // read argv
  std::string input_file, output_file;
  if (argv[1]) {
    input_file = argv[1];
  } else {
    throw SyntaxError("no input file");
  }
  if (argv[2]) {
    output_file = argv[2];
  } else {
    output_file = "test.ll";
  }
  ASTNodeVisitor *printer = new ASTPrinter();
  ASTIRGenerator *generator = new ASTIRGenerator(output_file);
  std::ifstream stream;
  stream.open(input_file);
  // 使用ANTLR自动生成词法分析器和语法分析器
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
  // visitXXX - to generate the abstract syntax tree
  auto root_ctx = builder.visitCompUnit(root);
  // parse an input stream and generate an AST(frontend)
  auto result = std::any_cast<std::shared_ptr<ProgramAST>>(root_ctx);
  // test output the tree
  // std::cout << "===== CST TEXT =====" << std::endl;
  // std::cout << tree->toStringTree(&parser) << std::endl;
  std::cout << "===== AST TEXT =====" << std::endl;
  //printer->visit(result.get());
  std::cout << "===== GENERATE LLVM IR =====" << std::endl;
  //use for JIT initialization
  // type checker prepare
  generator->Initialize();
  generator->setSymbolTables(builder.functions, builder.variables);
  generator->visit(result.get());
  printf("Running main() from %s\n", __FILE__);
  return 0;
}