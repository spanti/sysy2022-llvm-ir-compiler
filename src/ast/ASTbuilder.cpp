#include "ast/ASTBuilder.h"
#include "AST.h"
#include "SymbolTable.h"
#include "Type.h"
#include "commom.h"
#include "common/errors.h"
#include "parser/SysYParser.h"
#include "support/Any.h"
#include <any>
#include <cstdlib>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
// register lib function
void ASTBuilder::register_lib_function(
    std::string name, spanti::Type *re_type,
    std::vector<spanti::Type *> params_type) {
  spanti::FunctionType *func_type =
      new spanti::FunctionType(params_type, re_type);
  functions->insert(name, func_type, variables->cur);
}
void ASTBuilder::register_lib_functions() {
  // register lib function
  // IN IR builder,创建库函数声明
  register_lib_function("getint", new spanti::IntType(), {});
  register_lib_function("getch", new spanti::IntType(), {});
  register_lib_function(
      "getarray", new spanti::IntType(),
      {new spanti::ArrayType(new spanti::IntType(), true, {})}); // int[]
  register_lib_function("getfloat", new spanti::FloatType(), {});
  register_lib_function(
      "getfarray", new spanti::IntType(),
      {new spanti::ArrayType(new spanti::FloatType(), true, {})}); // float[]
  register_lib_function("putint", new spanti::VoidType(),
                        {new spanti::IntType()}); // int
  register_lib_function("putch", new spanti::VoidType(),
                        {new spanti::IntType()}); // int
  register_lib_function(
      "putarray", new spanti::VoidType(),
      {new spanti::IntType(),
       new spanti::ArrayType(new spanti::IntType(), true, {})}); // int,int[]
  register_lib_function("putfloat", new spanti::VoidType(),
                        {new spanti::FloatType()});
  register_lib_function("putfarray", new spanti::VoidType(),
                        {new spanti::IntType(),
                         new spanti::ArrayType(new spanti::FloatType(), true,
                                               {})}); // float,float[]
  // putf可能不完善
  register_lib_function("putf", new spanti::VoidType(),
                        {new spanti::StringType()});
  register_lib_function("starttime", new spanti::VoidType(), {});
  register_lib_function("stoptime", new spanti::VoidType(), {});
}
// implement visitxx methods
// full these function
std::any ASTBuilder::visitCompUnit(SysYParser::CompUnitContext *ctx) {
  std::vector<std::shared_ptr<DecAST>> decls;
  // initial stage
  cur_func_name = ".init";
  is_found_main = false;
  variables = new VariableTable();
  functions = new FunctionTable();
  register_lib_functions();
  // analysis stage
  ////std::cout << "visit CompUnit!" << std::endl;
  for (auto declCtx : ctx->decl()) {
    auto decl = visit(declCtx);
    auto d_ast = std::any_cast<std::vector<std::shared_ptr<DecAST>>>(decl);
    for (auto d : d_ast) {
      decls.push_back(d);
    }
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
  // 未查找到main
  if (!is_found_main)
    throw MainFuncNotFound();
  return compUnit;
}

std::any ASTBuilder::visitDecl(SysYParser::DeclContext *ctx) {
  // std::cout << "visit Decl!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitConstDecl(SysYParser::ConstDeclContext *ctx) {
  // 与符号表相关，存储对应的变量信息
  // 与VarDecl相类似,但需存储常量信息于类型中
  // 一系列constDef
  // std::cout << "visit ConstDecl!" << std::endl;
  spanti::Type *v_type;
  spanti::Type *c_type;
  std::shared_ptr<DecAST> var_ast;
  std::vector<std::shared_ptr<DecAST>> vdec_array;
  std::vector<int32_t> dims_array;
  int32_t init_const;
  std::vector<int32_t> init_array;
  // visit btype - transtrom string to *Ty
  std::string var_type = ctx->bType()->getText();
  if (var_type == "int") {
    // const int
    v_type = new spanti::IntType(true);
  } else if (var_type == "float") {
    // const float
    v_type = new spanti::FloatType(true);
  }
  std::string var_name;
  // set init type to check type of every init-value
  init_type = v_type;
  // visit all VarDef
  for (auto varDefCtx : ctx->constDef()) {
    auto initval = std::any_cast<std::shared_ptr<ExprAST>>(visit(varDefCtx));
    var_name = varDefCtx->Identifier()->getText();

    // initval 为 初始化值
    // 将去除initval，使用其他方式获取常量信息
    //  需使用var_type对应的类型
    // constexp不存在时
    if (varDefCtx->constExp().empty()) {
      // 非数组
      // 同步符号表信息
      // 常量表插入初始化值
      // 生成初始化值
      // 一个函数,接受数组维度和初始值的ctx，输出数组vector<int32_t>
      // problem!
      // 初始化值必须为常量
      init_array = parse_const_init(varDefCtx->constInitVal(), {1});
      v_type->set_const();
      VariableTableEntry *entry = new VariableTableEntry();
      entry->const_init = init_array;
      entry->var_type = v_type;
      //变量声明的行
      entry->def_line = ctx->getStart()->getLine();
      variables->insert(var_name, entry);
      var_ast = std::make_shared<VarDecAST>(
          sourceLocation{ctx->getStart()->getLine(),
                         ctx->getStart()->getCharPositionInLine()},
          var_name, v_type, initval);
    } else {
      // 对数组维度初始值的处理，常量表达式且非负
      // 当前，缺少对数组维度初始值的处理
      dims_array = get_array_dims(varDefCtx->constExp());
      c_type = new spanti::ArrayType(v_type, dims_array);
      // 测试代码
      init_array = parse_const_init(varDefCtx->constInitVal(), dims_array);
      // 同步符号表信息
      // 常量表插入初始化值
      // ConstInitVal中的值必须为
      c_type->set_const();
      VariableTableEntry *entry = new VariableTableEntry();
      entry->const_init = init_array;
      entry->var_type = c_type;
      //变量声明的行
      entry->def_line = ctx->getStart()->getLine();
      variables->insert(var_name, entry);
      var_ast = std::make_shared<VarDecAST>(
          sourceLocation{ctx->getStart()->getLine(),
                         ctx->getStart()->getCharPositionInLine()},
          var_name, c_type, initval);
    }
    vdec_array.push_back(var_ast);
  }
  // build VarDecAST Node
  return vdec_array;
}

std::any ASTBuilder::visitBType(SysYParser::BTypeContext *ctx) {
  // std::cout << "visit BType!";
  //  简单的类型判定 trasform btype to spanti::type -> need to build VarDecAST
  if (ctx->Int()) {
    std::cout << "btype:int" << std::endl;
  } else if (ctx->Float()) {
    std::cout << "btype:float" << std::endl;
  }
  return visitChildren(ctx);
}

std::any ASTBuilder::visitConstDef(SysYParser::ConstDefContext *ctx) {
  // 与vardef相类似
  std::tuple<std::string, std::shared_ptr<ExprAST>> tuble;
  // std::pair<std::vector<std::shared_ptr<ExprAST>>, std::string> pair;
  //  uint64_t n = 0;
  //   build array-type info
  std::string idname = ctx->Identifier()->getText();
  // std::cout << "visit ConstDef! IDNAME:" << idname << std::endl;
  auto init_ast =
      std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->constInitVal()));
  // send message
  // pass array-type info
  return init_ast;
}

std::any ASTBuilder::visitScalarConstInitVal(
    SysYParser::ScalarConstInitValContext *ctx) {
  // std::cout << "visit ScalarConstInitVal!" << std::endl;
  //  constExp
  return visitChildren(ctx);
}

std::any
ASTBuilder::visitListConstInitVal(SysYParser::ListConstInitValContext *ctx) {
  // 设计一个嵌套构造过程构造合法节点
  // constInitVal
  // need to know shape to decorate the AST
  std::shared_ptr<ExprAST> init_AST;
  std::shared_ptr<ExprAST> elems_AST;
  std::vector<std::shared_ptr<ExprAST>> inits;
  std::vector<int32_t> elems;
  int size = 0; // 统计维度数目
  // what is the type of {2,3}
  // need to create new type
  spanti::Type *c_type;
  spanti::Type *v_type;
  // array_type
  // std::cout << "visit ListConstInitVal!" << std::endl;
  for (auto c : ctx->constInitVal()) {
    auto init_exp = std::any_cast<std::shared_ptr<ExprAST>>(visit(c));
    inits.push_back(init_exp);
    size++;
  }
  elems.push_back(size);
  // get v_type from the children
  // 但v_type不定
  c_type = new spanti::ArrayType(v_type, elems);
  init_AST = std::make_shared<InitValExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      c_type, inits);
  return init_AST;
}

std::any ASTBuilder::visitVarDecl(SysYParser::VarDeclContext *ctx) {

  // std::cout << "visit VarDecl!" << std::endl;
  spanti::Type *v_type;
  spanti::Type *c_type;
  std::shared_ptr<DecAST> var_ast;
  std::vector<std::shared_ptr<DecAST>> vdec_array;
  std::vector<int32_t> dims_array;
  int32_t init_const;
  // visit btype - transtrom string to *Ty
  std::string var_type = ctx->bType()->getText();
  if (var_type == "int") {
    v_type = new spanti::IntType();
  } else if (var_type == "float") {
    v_type = new spanti::FloatType();
  }
  std::string var_name;
  // set init type to check type of every init-value
  init_type = v_type;
  // visit all VarDef
  for (auto varDefCtx : ctx->varDef()) {
    if (auto uninit =
            dynamic_cast<SysYParser::UninitVarDefContext *>(varDefCtx)) {
      var_name = uninit->Identifier()->getText();
      // 不存在初始值 initval = nullptr
      if (uninit->constExp().empty()) {
        // 无数组 int a;
        VariableTableEntry *entry = new VariableTableEntry();
        entry->var_type = v_type;
        //变量声明的行
      entry->def_line = ctx->getStart()->getLine();
        variables->insert(var_name, entry);
        var_ast = std::make_shared<VarDecAST>(
            sourceLocation{ctx->getStart()->getLine(),
                           ctx->getStart()->getCharPositionInLine()},
            var_name, v_type, nullptr);
      } else {
        dims_array = get_array_dims(uninit->constExp());
        // warning:数组初始化列表未存储
        // 初始化列表
        // 设计一个转化函数
        c_type = new spanti::ArrayType(v_type, dims_array);
        // 同步符号表信息
        VariableTableEntry *entry = new VariableTableEntry();
        entry->var_type = c_type;
        //变量声明的行
      entry->def_line = ctx->getStart()->getLine();
        variables->insert(var_name, entry);
        var_ast = std::make_shared<VarDecAST>(
            sourceLocation{ctx->getStart()->getLine(),
                           ctx->getStart()->getCharPositionInLine()},
            var_name, c_type, nullptr);
      }
    } else {
      auto init = dynamic_cast<SysYParser::InitVarDefContext *>(varDefCtx);
      assert(init);
      var_name = init->Identifier()->getText();
      // 必定存在初始值
      if (init->constExp().empty()) {
        // 无数组 int a=3;
        VariableTableEntry *entry = new VariableTableEntry();
        entry->var_type = v_type;
        //变量声明的行
      entry->def_line = ctx->getStart()->getLine();
        variables->insert(var_name, entry);
        // 列表
        // 全局及局部
        // 目前 :检测全局变量声明中的初值表达式 未完成
        auto init_vector = parse_var_init(init->initVal(), {1});
        var_ast = std::make_shared<VarDecAST>(
            sourceLocation{ctx->getStart()->getLine(),
                           ctx->getStart()->getCharPositionInLine()},
            var_name, v_type, init_vector[0]);
      } else {
        // int a[2][3] = {d,e,d,s,f,s}
        dims_array = get_array_dims(init->constExp());
        // 可引用变量，变量必须在符号表中
        c_type = new spanti::ArrayType(v_type, dims_array);
        // 同步符号表信息
        VariableTableEntry *entry = new VariableTableEntry();
        entry->var_type = c_type;
        //变量声明的行
      entry->def_line = ctx->getStart()->getLine();
        variables->insert(var_name, entry);
        // 列表
        auto init_vector = parse_var_init(init->initVal(), dims_array);
        // 构造InitValExpr 后 用其构造VarDecAST
        // 设计一个函数例程，接受维度和ctx，返回对应的ctx节点
        // ArrayType
        auto init_expr = std::make_shared<InitValExprAST>(
            sourceLocation{ctx->getStart()->getLine(),
                           ctx->getStart()->getCharPositionInLine()},
            v_type, init_vector);
        var_ast = std::make_shared<VarDecAST>(
            sourceLocation{ctx->getStart()->getLine(),
                           ctx->getStart()->getCharPositionInLine()},
            var_name, c_type, init_expr);
      }
    }
    vdec_array.push_back(var_ast);
  }
  // build VarDecAST Node
  return vdec_array;
}

std::any ASTBuilder::visitUninitVarDef(SysYParser::UninitVarDefContext *ctx) {
  std::vector<std::shared_ptr<ExprAST>> elements;
  std::tuple<std::vector<std::shared_ptr<ExprAST>>, std::string,
             std::shared_ptr<ExprAST>>
      tuble;
  // uint64_t n = 0;
  //  build array-type info
  std::string idname = ctx->Identifier()->getText();
  // std::cout << "visit UninitVarDef!" << idname << std::endl;
  //  send message
  //  pass array-type info
  return nullptr;
}

std::any ASTBuilder::visitInitVarDef(SysYParser::InitVarDefContext *ctx) {
  std::vector<std::shared_ptr<ExprAST>> elements;
  std::tuple<std::vector<std::shared_ptr<ExprAST>>, std::string,
             std::shared_ptr<ExprAST>>
      tuble;
  // std::pair<std::vector<std::shared_ptr<ExprAST>>, std::string> pair;
  //  uint64_t n = 0;
  //   build array-type info
  std::string idname = ctx->Identifier()->getText();
  // std::cout << "visit InitVarDef!" << idname << std::endl;
  //  get initval
  auto init_ast =
      std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->initVal()));
  // send message
  // pass array-type info
  // pair = std::pair<std::vector<std::shared_ptr<ExprAST>>,
  // std::string>(elements,idname);
  return init_ast;
}

std::any ASTBuilder::visitScalarInitVal(SysYParser::ScalarInitValContext *ctx) {
  // return std::shared_ptr<ExprAST>
  // pass node
  // std::cout << "visit ScalarInitVal!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitListInitval(SysYParser::ListInitvalContext *ctx) {
  std::shared_ptr<ExprAST> init_AST;
  std::shared_ptr<ExprAST> elems_AST;
  std::vector<std::shared_ptr<ExprAST>> inits;
  std::vector<int32_t> elems;
  int size = 0; // 统计维度数目
  // what is the type of {2,3}
  // need to create new type
  spanti::Type *c_type;
  spanti::Type *v_type;
  // array_type
  // std::cout << "visit ListInitVal!" << std::endl;
  for (auto c : ctx->initVal()) {
    auto init_exp = std::any_cast<std::shared_ptr<ExprAST>>(visit(c));
    inits.push_back(init_exp);
    size++;
  }
  elems.push_back(size);
  // get v_type from the children
  // 但v_type不定
  // 目前只保留第一层的元素数目信息
  c_type = new spanti::ArrayType(v_type, elems);
  init_AST = std::make_shared<InitValExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      c_type, inits);
  return init_AST;
}

std::any ASTBuilder::visitFuncDef(SysYParser::FuncDefContext *ctx) {
  // std::cout << "visit FuncDef! " << "function-name:" <<
  // ctx->Identifier()->getText() << std::endl;
  std::vector<spanti::Type *> params_type;
  spanti::Type *re_type;
  std::string re_str = ctx->funcType()->getText();
  // get re_type
  if (re_str == "int") {
    re_type = new spanti::IntType();
  } else if (re_str == "void") {
    re_type = new spanti::VoidType();
  } else if (re_str == "float") {
    re_type = new spanti::FloatType();
  }

  // function name
  std::string name = ctx->Identifier()->getText();
  // funcparam
  // entryscope
  variables->EntryScope();
  // 重定义
  // 若cur_func_name != ".init",变量为本地声明
  cur_func_name = name;
  std::vector<std::shared_ptr<VarDecAST>> Params;
  if (ctx->funcFParams()) {
    // handle funcparam
    for (auto p_ctx : ctx->funcFParams()->funcFParam()) {
      auto p_ast = std::any_cast<std::shared_ptr<VarDecAST>>(visit(p_ctx));
      std::string id_name = p_ast->getName();
      auto id_type = p_ast->getType();
      // type
      params_type.push_back(id_type);
      // 变量表更新
      VariableTableEntry* entry = new VariableTableEntry();
      entry->var_type = id_type;
      //变量声明的行
      entry->def_line = ctx->getStart()->getLine();
      variables->insert(id_name, entry);
      // DecAST
      Params.push_back(p_ast);
    }
  }
  // function typr - 返回值类型，函数类型
  auto *FuncTy = new spanti::FunctionType(params_type, re_type);
  // scope problem
  //  register function
  // set cur func scope
  functions->insert(name, FuncTy, variables->cur);
  // 查找到main，设置标记
  if (name == "main") {
    is_found_main = true;
    // 无参数，返回类型为INT
    if (FuncTy->get_re_type()->getTypeKind() != spanti::Type::SP_INTTYPE) {
      throw InvalidMainFuncEntry("main function not return int");
    }
    if (!FuncTy->get_params_type().empty()) {
      throw InvalidMainFuncEntry("main function have params");
    }
  }
  // record cur scope
  cur_func_scope = variables->cur;
  // exitscope
  variables->ExitScope();
  cur_func_name = ".init";
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
  // std::cout << "visit FuncType!" << std::endl;
  //  not handle now
  return visitChildren(ctx);
}

std::any ASTBuilder::visitFuncFParams(SysYParser::FuncFParamsContext *ctx) {
  // std::cout << "visit FuncFParams!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitFuncFParam(SysYParser::FuncFParamContext *ctx) {
  // std::cout << "visit FuncFParam!" << std::endl;
  //  return DecAST
  std::shared_ptr<VarDecAST> param_ast;
  spanti::Type *b_type;
  auto b_text = ctx->bType()->getText();
  std::vector<std::string> params;
  std::vector<int32_t> elems;
  // 未处理int a[] 一类的不定数组参数
  if (b_text == "int") {
    b_type = new spanti::IntType();
  } else if (b_text == "float") {
    b_type = new spanti::FloatType();
  }
  auto id_name = ctx->Identifier()->getText();
  // FuncFParam → BType Ident ['[' ']' { '[' Exp ']' }]
  if (!ctx->Lbrkt().empty() && !ctx->Rbrkt().empty()) {
    // ARRAYTYPE
    for (auto csctx : ctx->constExp()) {
      // transfrom ctx -> AST
      auto csast = std::any_cast<std::shared_ptr<ExprAST>>(visit(csctx));
      if (!csast->isconst(variables)) {
        throw SyntaxError("the expr isn't const expr");
      }
      elems.push_back(csast->get_constant());
    }
    // create type
    // 不定类型
    b_type = new spanti::ArrayType(b_type, true, elems);
    param_ast = std::make_shared<VarDecAST>(
        sourceLocation{ctx->getStart()->getLine(),
                       ctx->getStart()->getCharPositionInLine()},
        id_name, b_type);
  } else {
    // int a;
    param_ast = std::make_shared<VarDecAST>(
        sourceLocation{ctx->getStart()->getLine(),
                       ctx->getStart()->getCharPositionInLine()},
        id_name, b_type);
  }
  // 处理int a[][5]时，创建对应类型
  // 要求维度大小必须为constexp

  return param_ast;
}

std::any ASTBuilder::visitBlock(SysYParser::BlockContext *ctx) {
  // store stmts
  std::vector<std::shared_ptr<ASTNode>> blockitems_arr;
  std::vector<std::shared_ptr<ASTNode>> blockitems;
  // std::cout << "visit Block!" << std::endl;
  //  entryscope
  if (cur_func_scope != nullptr) {
    variables->EntryScope(cur_func_scope);
    cur_func_scope = nullptr;
  } else {
    variables->EntryScope();
  }

  for (auto *itemCtx : ctx->blockItem()) {
    auto blockitem = visit(itemCtx);
    blockitems =
        std::any_cast<std::vector<std::shared_ptr<ASTNode>>>(blockitem);
    for (auto item : blockitems) {
      blockitems_arr.push_back(std::move(item));
    }
  }
  // build astnode - pass scope
  std::shared_ptr<StmtAST> ast = std::make_shared<BlockStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      std::move(blockitems_arr), variables->cur);
  variables->ExitScope();
  return ast;
}

std::any ASTBuilder::visitBlockItem(SysYParser::BlockItemContext *ctx) {
  // std::cout << "visit BlockItem!" << std::endl;
  std::vector<std::shared_ptr<ASTNode>> blockitems;
  std::vector<std::shared_ptr<DecAST>> decls;
  std::shared_ptr<StmtAST> stmt_ast;
  // decide to choose stmt/decl
  // stmt
  if (ctx->stmt()) {
    // stmt
    auto stmt = visit(ctx->stmt());
    stmt_ast = std::any_cast<std::shared_ptr<StmtAST>>(stmt);
    blockitems.push_back(stmt_ast);
  } else if (ctx->decl()) {
    // decl
    auto decl = visit(ctx->decl());
    decls = std::any_cast<std::vector<std::shared_ptr<DecAST>>>(decl);
    for (auto dec : decls) {
      blockitems.push_back(dec);
    }
    // ast = std::any_cast<std::shared_ptr<DecAST>>(decl);
  } else {
    return visitChildren(ctx);
  }
  return blockitems;
}

std::any ASTBuilder::visitAssignment(SysYParser::AssignmentContext *ctx) {
  // std::cout << "visit Assignment!" << std::endl;
  VariableTableEntry *l_entry;
  // 构造对应节点
  auto lval = visit(ctx->lVal());
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(lval);
  auto exp = visit(ctx->exp());
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(exp);
  // 类型推断部分
  spanti::Type::TypeKind l_kind = LHS->getType()->getTypeKind();
  spanti::Type::TypeKind r_kind = RHS->getType()->getTypeKind();
  // 左侧类型与右侧类型必须满足某一定则
  // 左侧为常量时，抛出错误
  if (auto l_ast = dynamic_cast<IdentifierAST *>(LHS.get())) {
    l_entry = variables->lookUp(l_ast->getName(),l_ast->loc.Line);
    // a = 5; generate store instruction
    if (!l_entry)
      throw UnrecognizedVarName(l_ast->getName());
    if (l_entry->var_type->get_const())
      // ask the type isn't const
      throw AssignmentTypeError("Assign a value to constant!");
  } else {
    auto a_ast = dynamic_cast<ArrayExprAST *>(LHS.get());
    if (a_ast) {
      l_entry = variables->lookUp(a_ast->Name,a_ast->loc.Line);
      auto array_type = dynamic_cast<spanti::ArrayType *>(l_entry->var_type);
      // a = 5; generate store instruction
      // handle int a[2][2] a[2] = 5 error
      if (a_ast->elems.size() != array_type->get_count_dim())
        throw SyntaxError("the number of [] not match");
      if (!l_entry)
        throw UnrecognizedVarName(a_ast->Name);
      // ask the type isn't const
      if (l_entry->var_type->get_const())
        throw AssignmentTypeError("Assign a value to constant!");
    }
  }
  // 检查左右两侧的类型等价性
  std::shared_ptr<StmtAST> ass_ast = std::make_shared<AssignStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      LHS, RHS);
  return ass_ast;
}
// control flow
std::any ASTBuilder::visitExpStmt(SysYParser::ExpStmtContext *ctx) {
  // std::cout << "visit ExpStmt!" << std::endl;
  std::shared_ptr<ExprAST> ast = nullptr;
  if (ctx->exp())
    ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->exp()));
  std::shared_ptr<StmtAST> stmt = std::make_shared<ExpStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      ast);
  // passing upward
  return stmt;
}

std::any ASTBuilder::visitBlockStmt(SysYParser::BlockStmtContext *ctx) {
  // std::cout << "visit BlockStmt!" << std::endl;
  //  passing upward
  return visitChildren(ctx);
}

std::any ASTBuilder::visitIfStmt1(SysYParser::IfStmt1Context *ctx) {
  // std::cout << "visit IfStmt1!" << std::endl;
  auto Cond = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->cond()));
  auto Then = std::any_cast<std::shared_ptr<StmtAST>>(visit(ctx->stmt()));

  // Cond,Then,Elses
  // build the same IfAST
  std::shared_ptr<StmtAST> ifAst = std::make_shared<IfStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      Cond, Then, nullptr);
  return ifAst;
}

std::any ASTBuilder::visitIfStmt2(SysYParser::IfStmt2Context *ctx) {
  // std::cout << "visit IfStmt2!" << std::endl;
  auto Cond = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->cond()));
  auto Then = std::any_cast<std::shared_ptr<StmtAST>>(visit((ctx->stmt())[0]));
  auto Else = std::any_cast<std::shared_ptr<StmtAST>>(visit((ctx->stmt())[1]));
  // Cond,Then,Elses
  // build the same IfAST
  std::shared_ptr<StmtAST> ifAst = std::make_shared<IfStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      Cond, Then, Else);
  return ifAst;
}

std::any ASTBuilder::visitWhileStmt(SysYParser::WhileStmtContext *ctx) {
  // std::cout << "visit WhileStmt!" << std::endl;
  //  Cond,Else
  auto Cond = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->cond()));
  auto Body = std::any_cast<std::shared_ptr<StmtAST>>(visit((ctx->stmt())));
  // Cond,Then,Elses
  // build the same IfAST
  std::shared_ptr<StmtAST> whileAst = std::make_shared<WhileStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      Cond, Body);
  return whileAst;
}

std::any ASTBuilder::visitBreakStmt(SysYParser::BreakStmtContext *ctx) {
  // std::cout << "visit BreakStmt!" << std::endl;
  std::shared_ptr<StmtAST> ast = std::make_shared<BreakStmtAST>(sourceLocation{
      ctx->getStart()->getLine(), ctx->getStart()->getCharPositionInLine()});
  return ast;
}

std::any ASTBuilder::visitContinueStmt(SysYParser::ContinueStmtContext *ctx) {
  // std::cout << "visit ContinueStmt!" << std::endl;
  std::shared_ptr<StmtAST> ast = std::make_shared<ContinueStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()});
  return ast;
}

std::any ASTBuilder::visitReturnStmt(SysYParser::ReturnStmtContext *ctx) {
  // std::cout << "visit ReturnStmt!" << std::endl;
  std::shared_ptr<ExprAST> ast = nullptr;
  if (ctx->exp()) {
    // return;
    auto re_exp = visit(ctx->exp());
    ast = std::any_cast<std::shared_ptr<ExprAST>>(re_exp);
  }
  // return 7+5;
  // build return AST node
  std::shared_ptr<StmtAST> retn_ast = std::make_shared<RetuStmtAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      ast);
  return retn_ast;
}

std::any ASTBuilder::visitExp(SysYParser::ExpContext *ctx) {
  // std::cout << "visit Exp!" << std::endl;
  auto addexp = visit(ctx->addExp());
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(addexp);
  return ast;
}

std::any ASTBuilder::visitCond(SysYParser::CondContext *ctx) {
  // std::cout << "visit Cond!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitLVal(SysYParser::LValContext *ctx) {
  // std::cout << "visit LVal!" << std::endl;
  std::shared_ptr<ExprAST> l_ast;
  std::shared_ptr<ExprAST> elem_ast;
  std::vector<std::shared_ptr<ExprAST>> elems;
  std::string id_name = ctx->Identifier()->getText();
  spanti::Type *ae_type;
  // 1.a
  auto v_entry = variables->lookUp(id_name,ctx->getStart()->getLine());
  spanti::Type *var_type;
  if (v_entry) {
    var_type = v_entry->var_type;
  } else {
    // not found var
    throw UnrecognizedVarName(id_name);
  }
  // 当 LVal 表示数组时，方括号个数必须和数组变量的维数相同

  if (!ctx->exp().empty()) {
    // a[2][3]
    // a为数组类型，方括号个数必须和数组变量的维数相同
    if (auto a_type = dynamic_cast<spanti::ArrayType *>(var_type)) {

      // a[3] a is array
      // unsize = false
      int cnt = 0;
      for (auto s : ctx->exp()) {
        // 获取每个索引的ExprAST
        auto elem_ctx = visit(s);
        elem_ast = std::any_cast<std::shared_ptr<ExprAST>>(elem_ctx);
        elems.push_back(elem_ast);
        cnt++;
      }
      // 当 LVal 表示数组时，方括号个数必须和数组变量的维数相同
      // 新规则，部分索引时，数组元素AST的类型为数组类型
      // 这一判定与规则（多维数组的其中一部分可作为函数参数矛盾）
      if (cnt != a_type->get_count_dim()) {
        // 数组的一部分
        // 每个elems索引必须满足数组声明的各维度
        // EXP无法进行越界检查
        if (cnt > a_type->get_count_dim())
          throw SyntaxError("Invalid Array Element");
        // create new array type
        auto elems = a_type->get_elements();
        // 去除元素
        for (int i = 0; i < cnt; i++)
          elems.erase(elems.begin());
        // 创建新的数组类型
        ae_type = new spanti::ArrayType(a_type->getElementTy(), elems);
      } else {
        // 定位到元素
        ae_type = a_type->getElementTy();
      }
      // 构造数组节点 - 类型确定为INT/FLOAT
      // 设定左值
      l_ast = std::make_shared<ArrayExprAST>(
          sourceLocation{ctx->getStart()->getLine(),
                         ctx->getStart()->getCharPositionInLine()},
          ae_type, true, id_name, elems);
    } else {
      throw SyntaxError("LVal(a[]) a is not a array!");
    }
  } else {
    // a pass IdentifierAST
    // 为数组类型 ，构造数组索引节点
    // 否则，构造IdentifierASt 节点
    if (auto a_type = dynamic_cast<spanti::ArrayType *>(var_type)) {
      // a 索引 数组 / 不定数组
      // elems为空
      l_ast = std::make_shared<ArrayExprAST>(
          sourceLocation{ctx->getStart()->getLine(),
                         ctx->getStart()->getCharPositionInLine()},
          a_type, true, id_name, elems);
    } else {
      // 查表，传递类型
      l_ast = std::make_shared<IdentifierAST>(
          sourceLocation{ctx->getStart()->getLine(),
                         ctx->getStart()->getCharPositionInLine()},
          var_type, true, id_name);
    }
  }
  /*
  if (auto a_type = dynamic_cast<spanti::ArrayType *>(var_type)) {
    // a[3] a is array
    // unsize = false
    int cnt = 0;
    for (auto s : ctx->exp()) {
      // 获取每个索引的ExprAST
      auto elem_ctx = visit(s);
      elem_ast = std::any_cast<std::shared_ptr<ExprAST>>(elem_ctx);
      elems.push_back(elem_ast);
      cnt++;
    }
    // 当 LVal 表示数组时，方括号个数必须和数组变量的维数相同
    // 新规则，部分索引时，数组元素AST的类型为数组类型
    // 这一判定与规则（多维数组的其中一部分可作为函数参数矛盾）
    if (cnt != a_type->get_count_dim()) {
      // 数组的一部分
      // 每个elems索引必须满足数组声明的各维度
      // EXP无法进行越界检查
      if (cnt > a_type->get_count_dim())
        throw SyntaxError("Invalid Array Element");
      // create new array type
      auto elems = a_type->get_elements();
      // 去除元素
      for (int i = 0; i < cnt; i++)
        elems.erase(elems.begin());
      // 创建新的数组类型
      ae_type = new spanti::ArrayType(a_type->getElementTy(), elems);
    } else {
      // 定位到元素
      ae_type = a_type->getElementTy();
    }
    // 构造数组节点 - 类型确定为INT/FLOAT
    // 设定左值
    l_ast = std::make_shared<ArrayExprAST>(
        sourceLocation{ctx->getStart()->getLine(),
                       ctx->getStart()->getCharPositionInLine()},
        ae_type, true, id_name, elems);
  } else {
    // 当 LVal 表示单个变量时，不能出现后面的方括号
    // 包括
    //  a is variable
    if (ctx->exp().empty()) {
      // look up the type of identifier

      // build Identifier - 类型不定
      // 设定左值
      l_ast = std::make_shared<IdentifierAST>(
          sourceLocation{ctx->getStart()->getLine(),
                         ctx->getStart()->getCharPositionInLine()},
          var_type, true, id_name);
    } else {
      throw InvalidIndexOperator();
    }
  }*/
  return l_ast;
}

std::any ASTBuilder::visitPrimaryExp1(SysYParser::PrimaryExp1Context *ctx) {
  // std::cout << "visit PrimaryExp1!" << std::endl;
  //  pass node:忽略
  //  return visitChildren(ctx);
  auto addexp = visit(ctx->exp());
  std::shared_ptr<ExprAST> ast =
      std::any_cast<std::shared_ptr<ExprAST>>(addexp);
  return ast;
}

std::any ASTBuilder::visitPrimaryExp2(SysYParser::PrimaryExp2Context *ctx) {
  // std::cout << "visit PrimaryExp2!" << std::endl;
  //  pass node:忽略
  return visitChildren(ctx);
}

std::any ASTBuilder::visitPrimaryExp3(SysYParser::PrimaryExp3Context *ctx) {
  // std::cout << "visit PrimaryExp3!" << std::endl;
  auto num = visit(ctx->number());
  auto n_ast = std::any_cast<std::shared_ptr<ExprAST>>(num);
  return n_ast;
}

std::any ASTBuilder::visitNumber(SysYParser::NumberContext *ctx) {
  std::shared_ptr<ExprAST> num_ast;
  std::string num = ctx->getText();
  // 1.IntLiteral:100
  // transform string to num (base 10)
  if (ctx->IntLiteral()) {
    int32_t Val = parse_int32_literal(num);
    num_ast = std::make_shared<IntergarExprAST>(
        sourceLocation{ctx->getStart()->getLine(),
                       ctx->getStart()->getCharPositionInLine()},
        false, Val);
  } else {
    // 设计用于读取浮点数的函数
    float Val = std::stof(num);
    num_ast = std::make_shared<FloatExprAST>(
        sourceLocation{ctx->getStart()->getLine(),
                       ctx->getStart()->getCharPositionInLine()},
        false, num, Val);
    // float value - store it in Val;
    // not decide how to handle it
  }
  // std::cout << "visit Number! " << " number:" << num << std::endl;
  return num_ast;
  // return visitChildren(ctx);
}

std::any ASTBuilder::visitUnary1(SysYParser::Unary1Context *ctx) {
  // std::cout << "visit Unary1!" << std::endl;
  auto primary = visit(ctx->primaryExp());
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(primary);
  return ast;
}

std::any ASTBuilder::visitUnary2(SysYParser::Unary2Context *ctx) {
  // build function-call node - a(5,6)
  // std::cout << "visit Unary2!" << std::endl;
  std::string func_name = ctx->Identifier()->getText();
  // build call node - 调用函数已声明或为库函数
  auto func_entry = functions->lookUp(func_name);
  spanti::Type *func_type = new spanti::Type(spanti::Type::SP_EMPTYTYPE);
  spanti::Type *re_type = new spanti::Type(spanti::Type::SP_EMPTYTYPE);
  if (func_entry) {
    func_type = func_entry->func_type;
  } else {
    throw UnrecognizedFuncName(func_name);
  }
  // 类型未处理
  // build list of params
  std::vector<std::shared_ptr<ExprAST>> args;
  // 判断funcRParams是否存在
  auto func_params = ctx->funcRParams();
  auto func_Ty = dynamic_cast<spanti::FunctionType *>(func_type);
  if (func_params) {
    // 若参数数目不匹配，抛出错误
    // 存疑，可能存在函数，参数数目可变，使实参数目小于等于形参
    if (func_params->funcRParam().size() != func_Ty->get_size()) {
      throw InvalidFuncCallArg("wrong number of function arguments");
    }
    // 参数必须预先已声明
    for (auto Arg : func_params->funcRParam()) {
      // 依次验证类型是否匹配
      auto arg_item = visit(Arg);
      // handle exp/STRING
      // 目前未处理字符常量
      auto ast = std::any_cast<std::shared_ptr<ExprAST>>(arg_item);
      // 参数类型匹配确保
      args.push_back(ast);
    }
    // 形参和实参类型匹配处理
    // 使用严格的兼容规则，形参和实参类型必须完全相等
    for (int i = 0; i < func_params->funcRParam().size(); i++) {
      // 类型等价部分，必要时设计为独立模块
      //  形参
      auto params_kind_1 = func_Ty->get_params_type()[i]->getTypeKind();
      // 实参
      auto params_kind_2 = args[i]->getType()->getTypeKind();
      // 测试
      if (params_kind_1 != params_kind_2) {
        throw InvalidFuncCallArg("type error on function arguments");
      }
      // 若两者均为数组时，要求维度数目和大小均对应
      if (params_kind_1 == spanti::Type::SP_ARRAYTYPE) {
        // 若维度数不对应，报错
        auto a_1 =
            dynamic_cast<spanti::ArrayType *>(func_Ty->get_params_type()[i]);
        // args[i] 对应的类型
        auto a_2 = dynamic_cast<spanti::ArrayType *>(args[i]->getType());
        if (a_1->get_count_dim() != a_2->get_count_dim()) {
          // 数组维度数目不匹配
          throw InvalidFuncCallArg("type error on function arguments");
        } else {
          int a_1_u = 0;
          int a_2_u = 0;
          // problem : 参数匹配报错
          if (a_1->get_unsize())
            a_1_u = 1;
          if (a_2->get_unsize())
            a_2_u = 1;
          // 基于维度数目匹配
          for (int i = 0; i < a_1->get_count_dim(); i++) {
            // i=0 若有一方为unsize，不参与比较
            if (i == 0) {
              if (a_1->get_unsize() || a_2->get_unsize()) {
                continue;
              }
            }
            if (a_1->get_elements()[i - a_1_u] !=
                a_2->get_elements()[i - a_2_u]) {
              // 各维度大小不匹配
              throw InvalidFuncCallArg("type error on function arguments");
            }
          }
        }
      }
    }
  }
  // putint() 实参数目为0，形参数目大于0
  else {
    if (func_Ty->get_size() > 0) {
      throw InvalidFuncCallArg("wrong number of function arguments");
    }
  }

  // 进行类型推断的时机
  // 形参和实参类型相匹配
  // 设置函数调用的返回值类型
  re_type = func_Ty->get_re_type();
  std::shared_ptr<ExprAST> call_ast = std::make_shared<CallExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      re_type, func_name, args);
  // a std::vector
  return call_ast;
}

std::any ASTBuilder::visitUnary3(SysYParser::Unary3Context *ctx) {
  // 节点嵌套 - 构造一元表达式节点
  // std::cout << "visit Unary3!" << std::endl;
  UnaryExprAST::UpKind upkind;
  // get upkind
  if (ctx->unaryOp()->Addition()) {
    upkind = UnaryExprAST::Pos;
  } else if (ctx->unaryOp()->Minus()) {
    upkind = UnaryExprAST::Neg;
  } else if (ctx->unaryOp()->Exclamation()) {
    upkind = UnaryExprAST::Not;
  }
  auto UHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->unaryExp()));
  std::shared_ptr<ExprAST> unary_ast = std::make_shared<UnaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      new spanti::IntType(), false, upkind, UHS);
  return unary_ast;
}

std::any ASTBuilder::visitUnaryOp(SysYParser::UnaryOpContext *ctx) {
  // std::cout << "visit UnaryOp!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitFuncRParams(SysYParser::FuncRParamsContext *ctx) {
  // std::cout << "visit FuncRParams!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitExpAsRParam(SysYParser::ExpAsRParamContext *ctx) {
  // 处理为exp
  // std::cout << "visit ExpAsRParam!" << std::endl;
  return visitChildren(ctx);
}

std::any
ASTBuilder::visitStringAsRParam(SysYParser::StringAsRParamContext *ctx) {
  // 处理为STRING
  // std::cout << "visit StringAsRParam!" << std::endl;
  return visitChildren(ctx);
}

std::any ASTBuilder::visitMul2(SysYParser::Mul2Context *ctx) {
  // std::cout << "visit Mul2!" << std::endl;
  spanti::Type *b_type;
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
  spanti::Type::TypeKind l_kind = LHS->getType()->getTypeKind();
  spanti::Type::TypeKind r_kind = RHS->getType()->getTypeKind();
  if (l_kind == spanti::Type::SP_FLOATTYPE ||
      r_kind == spanti::Type::SP_FLOATTYPE) {
    // RETURN FLOAT_TYPE
    b_type = new spanti::FloatType();
  } else if (l_kind == spanti::Type::SP_INTTYPE &&
             r_kind == spanti::Type::SP_INTTYPE) {
    // RETURN INT_TYPE
    b_type = new spanti::IntType();
  }
  // need to handle const-expr
  // if expr is constant,calculate its value and replace the BinaryAST with
  // IntAST
  //  the type of Expr 1+2;
  // 暂时处理两侧为const int的情形
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      b_type, false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitMul1(SysYParser::Mul1Context *ctx) {
  // std::cout << "visit Mul1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->unaryExp()));
  return ast;
}

std::any ASTBuilder::visitAdd2(SysYParser::Add2Context *ctx) {
  // std::cout << "visit Add2!" << std::endl;
  spanti::Type *b_type;
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
  // 当前类型不定
  // 类型转换问题 - need to consider
  spanti::Type::TypeKind l_kind = LHS->getType()->getTypeKind();
  spanti::Type::TypeKind r_kind = RHS->getType()->getTypeKind();
  if (l_kind == spanti::Type::SP_FLOATTYPE ||
      r_kind == spanti::Type::SP_FLOATTYPE) {
    // RETURN FLOAT_TYPE
    b_type = new spanti::FloatType();
  } else if (l_kind == spanti::Type::SP_INTTYPE &&
             r_kind == spanti::Type::SP_INTTYPE) {
    // RETURN INT_TYPE
    b_type = new spanti::IntType();
  }
  // when encounter 1 + 5
  // rewriting its nodes
  // requiring careful implementation of constant folding in `every lowering
  // phase`
  //  若5+2
  //  the type of Expr a+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      b_type, false, opkind, LHS, RHS);
  // 用于测试
  // bin_ast->isconst(variables);
  return bin_ast;
}

std::any ASTBuilder::visitAdd1(SysYParser::Add1Context *ctx) {
  // std::cout << "visit Add1!" << std::endl;
  auto mulexp = visit(ctx->mulExp());
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(mulexp);
  return ast;
}

std::any ASTBuilder::visitRel2(SysYParser::Rel2Context *ctx) {
  // std::cout << "visit Rel2!" << std::endl;
  spanti::Type *b_type;
  BinaryExprAST::OpKind opkind;
  if (ctx->GE()) {
    //'+'
    opkind = BinaryExprAST::Ge;
  } else if (ctx->GT()) {
    opkind = BinaryExprAST::Gt;
  } else if (ctx->LE()) {
    opkind = BinaryExprAST::Le;
  } else if (ctx->LT()) {
    opkind = BinaryExprAST::Lt;
  }
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->relExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->addExp()));
  // 类型转换问题 - need to consider
  spanti::Type::TypeKind l_kind = LHS->getType()->getTypeKind();
  spanti::Type::TypeKind r_kind = RHS->getType()->getTypeKind();
  if (l_kind == spanti::Type::SP_FLOATTYPE ||
      r_kind == spanti::Type::SP_FLOATTYPE) {
    // RETURN FLOAT_TYPE
    b_type = new spanti::FloatType();
  } else if (l_kind == spanti::Type::SP_INTTYPE &&
             r_kind == spanti::Type::SP_INTTYPE) {
    // RETURN INT_TYPE
    b_type = new spanti::IntType();
  }
  // the type of Expr 1+2;
  // 暂时使用SP_EMPTYTYPE以代替操作
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      b_type, false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitRel1(SysYParser::Rel1Context *ctx) {
  // std::cout << "visit Rel1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->addExp()));
  return ast;
}

std::any ASTBuilder::visitEq1(SysYParser::Eq1Context *ctx) {
  // std::cout << "visit Eq1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->relExp()));
  return ast;
}

std::any ASTBuilder::visitEq2(SysYParser::Eq2Context *ctx) {
  // std::cout << "visit Eq2!" << std::endl;
  spanti::Type *b_type;
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
  spanti::Type::TypeKind l_kind = LHS->getType()->getTypeKind();
  spanti::Type::TypeKind r_kind = RHS->getType()->getTypeKind();
  if (l_kind == spanti::Type::SP_FLOATTYPE ||
      r_kind == spanti::Type::SP_FLOATTYPE) {
    // RETURN FLOAT_TYPE
    b_type = new spanti::FloatType();
  } else if (l_kind == spanti::Type::SP_INTTYPE &&
             r_kind == spanti::Type::SP_INTTYPE) {
    // RETURN INT_TYPE
    b_type = new spanti::IntType();
  }
  // the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      b_type, false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitLAnd2(SysYParser::LAnd2Context *ctx) {
  // std::cout << "visit LAnd2!" << std::endl;
  spanti::Type *b_type;
  BinaryExprAST::OpKind opkind;
  opkind = BinaryExprAST::OpKind::And;
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->lAndExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->eqExp()));
  // 类型转换问题 - need to consider
  spanti::Type::TypeKind l_kind = LHS->getType()->getTypeKind();
  spanti::Type::TypeKind r_kind = RHS->getType()->getTypeKind();
  if (l_kind == spanti::Type::SP_FLOATTYPE ||
      r_kind == spanti::Type::SP_FLOATTYPE) {
    // RETURN FLOAT_TYPE
    b_type = new spanti::FloatType();
  } else if (l_kind == spanti::Type::SP_INTTYPE &&
             r_kind == spanti::Type::SP_INTTYPE) {
    // RETURN INT_TYPE
    b_type = new spanti::IntType();
  }
  // the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      b_type, false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitLAnd1(SysYParser::LAnd1Context *ctx) {
  // std::cout << "visit LAnd1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->eqExp()));
  return ast;
}

std::any ASTBuilder::visitLOr1(SysYParser::LOr1Context *ctx) {
  // std::cout << "visit LOr1!" << std::endl;
  auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->lAndExp()));
  return ast;
}

std::any ASTBuilder::visitLOr2(SysYParser::LOr2Context *ctx) {
  // std::cout << "visit LOr2!" << std::endl;
  spanti::Type *b_type;
  BinaryExprAST::OpKind opkind;
  opkind = BinaryExprAST::OpKind::Or;
  auto LHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->lOrExp()));
  auto RHS = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx->lAndExp()));
  // 类型转换问题 - need to consider
  spanti::Type::TypeKind l_kind = LHS->getType()->getTypeKind();
  spanti::Type::TypeKind r_kind = RHS->getType()->getTypeKind();
  if (l_kind == spanti::Type::SP_FLOATTYPE ||
      r_kind == spanti::Type::SP_FLOATTYPE) {
    // RETURN FLOAT_TYPE
    b_type = new spanti::FloatType();
  } else if (l_kind == spanti::Type::SP_INTTYPE &&
             r_kind == spanti::Type::SP_INTTYPE) {
    // RETURN INT_TYPE
    b_type = new spanti::IntType();
  }
  // need to ensure the type of Expr 1+2;
  std::shared_ptr<ExprAST> bin_ast = std::make_shared<BinaryExprAST>(
      sourceLocation{ctx->getStart()->getLine(),
                     ctx->getStart()->getCharPositionInLine()},
      b_type, false, opkind, LHS, RHS);
  return bin_ast;
}

std::any ASTBuilder::visitConstExp(SysYParser::ConstExpContext *ctx) {
  // similar to exp,but const
  // 向上传递addExp
  // std::cout << "visit ConstExp!" << std::endl;
  return visitChildren(ctx);
}

std::vector<int32_t>
ASTBuilder::parse_const_init(SysYParser::ConstInitValContext *root,
                             const std::vector<int32_t> dims) {
  // read array init into memory
  std::vector<int32_t> rinit;
  // something..
  // 1.constExp scalarConstInitVal
  if (auto scalar_root =
          dynamic_cast<SysYParser::ScalarConstInitValContext *>(root)) {
    // 3
    // ctx -> constExp
    // shape.size >= 0

    auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(scalar_root));
    if (ast->isconst(variables)) {
      rinit.push_back(ast->ret);
    } else {
      throw SyntaxError("no constant expr");
    }
    // rinit.push_back(value)
    return rinit;
  }
  auto list_root = dynamic_cast<SysYParser::ListConstInitValContext *>(root);
  assert(list_root);
  dfs_const_init(list_root, dims, rinit);
  // 2.'{' [ ConstInitVal { ',' ConstInitVal } ] '}'
  return rinit;
}

std::vector<int32_t>
ASTBuilder::get_array_dims(std::vector<SysYParser::ConstExpContext *> ctxs) {
  std::vector<int32_t> dims_array;
  // transform ctx to AST
  // use AST.isconst() to get its constant
  for (auto ctx : ctxs) {
    auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(ctx));
    auto dim_ast = ast.get();
    // 常量表达式且非负
    if (dim_ast->isconst(variables)) {
      int32_t dim = dim_ast->ret;
      if (dim <= 0) {
        throw InvalidArrayEntry();
      } else {
        dims_array.push_back(dim);
      }
    } else {
      throw InvalidArrayEntry();
    }
  }
  return dims_array;
}
// depth tree search
// handle {1,2,{2},{3},3,4} ,shape{4,2}-> {1,2,2,0,3,0,3,4}
// 递归算法
void ASTBuilder::dfs_const_init(SysYParser::ListConstInitValContext *node,
                                const std::vector<int32_t> &shape,
                                std::vector<int32_t> &result) {
  // 在转换过程中，判定声明的shape与数组初始化器是否匹配
  // 各个数组元素必须为常数
  // AST.isconstant
  if (shape.size() == 0)
    throw InvalidInitList();
  // calculate total size and child_size ,child_size is the size of child array
  int total_size = 1, child_size = 1;
  for (int i = 0; i < shape.size(); i++) {
    total_size *= shape[i];
    if (i > 0) {
      child_size *= shape[i];
    }
  }
  // 预先alloacte memory for vector
  result.reserve(result.size() + total_size);
  std::vector<int32_t> child_shape = shape;
  child_shape.erase(child_shape.begin());
  // 计数，防止越界
  int cnt = 0;
  for (auto child : node->constInitVal()) {
    if (auto scalar_child =
            dynamic_cast<SysYParser::ScalarConstInitValContext *>(child)) {
      if (cnt + 1 > total_size)
        throw InvalidInitList();
      // transform ctx->ast
      auto ast = std::any_cast<std::shared_ptr<ExprAST>>(
          visit(scalar_child->constExp()));
      // 待验证
      // 验证初始值类型与声明类型是否匹配
      // init_type为INT，ast_type不能为FLOAT
      auto f_t = init_type->getTypeKind();
      auto s_t = ast->getType()->getTypeKind();
      if ((f_t == spanti::Type::SP_INTTYPE) &&
          (s_t == spanti::Type::SP_FLOATTYPE)) {
        throw InitTypeError();
      }
      if (ast->isconst(variables)) {
        // 求解每个constExp的常数值
        result.push_back(ast->ret);
      } else {
        throw SyntaxError("no constant expr");
      }
      ++cnt;
    } else {
      auto list_child =
          dynamic_cast<SysYParser::ListConstInitValContext *>(child);
      assert(list_child);
      // 填充完不可越界检查，填充子数组时，必须已填充子数组长度的整数倍
      if (cnt % child_size != 0 || cnt + child_size > total_size)
        throw InvalidInitList();
      // 递归调用函数
      dfs_const_init(list_child, child_shape, result);
      cnt += child_size;
    }
  }
  // 其他部分隐式初始化
  while (cnt < total_size) {
    result.push_back(0);
    ++cnt;
  }
}
std::vector<std::shared_ptr<ExprAST>>
ASTBuilder::parse_var_init(SysYParser::InitValContext *root,
                           const std::vector<int32_t> dims) {
  // read array init into memory
  std::vector<std::shared_ptr<ExprAST>> rinit;
  // something..
  // 1.constExp scalarConstInitVal
  if (auto scalar_root =
          dynamic_cast<SysYParser::ScalarInitValContext *>(root)) {
    // 3
    // ctx -> constExp
    // shape.size >= 0

    auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(scalar_root));
    rinit.push_back(ast);
    // rinit.push_back(value)
    return rinit;
    //{exprast}
  }
  auto list_root = dynamic_cast<SysYParser::ListInitvalContext *>(root);
  assert(list_root);
  // get std::vector<ExprAST>
  dfs_var_init(list_root, dims, rinit);
  // 2.'{' [ ConstInitVal { ',' ConstInitVal } ] '}'
  return rinit;
}
void ASTBuilder::dfs_var_init(SysYParser::ListInitvalContext *node,
                              const std::vector<int32_t> &shape,
                              std::vector<std::shared_ptr<ExprAST>> &result) {
  // 在转换过程中，判定声明的shape与数组初始化器是否匹配
  // 各个数组元素必须为常数
  // init的各个类型与声明类型必须匹配
  // AST.isconstant
  if (shape.size() == 0)
    throw InvalidInitList();
  // calculate total size and child_size ,child_size is the size of child array
  int total_size = 1, child_size = 1;
  for (int i = 0; i < shape.size(); i++) {
    total_size *= shape[i];
    if (i > 0) {
      child_size *= shape[i];
    }
  }
  // 预先alloacte memory for vector
  result.reserve(result.size() + total_size);
  std::vector<int32_t> child_shape = shape;
  child_shape.erase(child_shape.begin());
  // 计数，防止越界
  int cnt = 0;
  for (auto child : node->initVal()) {
    if (auto scalar_child =
            dynamic_cast<SysYParser::ScalarInitValContext *>(child)) {
      if (cnt + 1 > total_size)
        throw InvalidInitList();
      // transform ctx->ast
      auto ast =
          std::any_cast<std::shared_ptr<ExprAST>>(visit(scalar_child->exp()));
      // 待验证
      // 验证初始值类型与声明类型是否匹配
      // init_type为INT，ast_type不能为FLOAT
      auto f_t = init_type->getTypeKind();
      auto s_t = ast->getType()->getTypeKind();
      if ((f_t == spanti::Type::SP_INTTYPE) &&
          (s_t == spanti::Type::SP_FLOATTYPE)) {
        throw InitTypeError();
      }
      result.push_back(ast);
      ++cnt;
    } else {
      auto list_child = dynamic_cast<SysYParser::ListInitvalContext *>(child);
      assert(list_child);
      // 填充完不可越界检查，填充子数组时，必须已填充子数组长度的整数倍
      if (cnt % child_size != 0 || cnt + child_size > total_size)
        throw InvalidInitList();
      // 递归调用函数
      // pass result并继续填充数组
      dfs_var_init(list_child, child_shape, result);
      cnt += child_size;
    }
  }
  // 其他部分隐式初始化
  while (cnt < total_size) {
    // 必要时填充节点
    // 填充默认节点
    result.push_back(
        std::make_shared<ImplicitValExprAST>(sourceLocation(), init_type));
    // result.emplace_back();
    ++cnt;
  }
}
// 备用函数，用以生成修正后的列表AST
// 未设计正确
std::shared_ptr<ExprAST>
ASTBuilder::parse_const_ast(SysYParser::ConstInitValContext *root,
                            const std::vector<int32_t> &shape) {
  // 不需要通常检查
  // read array init into memory
  std::vector<int32_t> rinit;
  // something..
  // 1.constExp scalarConstInitVal
  if (auto scalar_root =
          dynamic_cast<SysYParser::ScalarConstInitValContext *>(root)) {
    // const int a = 3; 3 int_ast
    auto ast = std::any_cast<std::shared_ptr<ExprAST>>(visit(scalar_root));
    // rinit.push_back(value)
    return ast;
  }
  auto list_root = dynamic_cast<SysYParser::ListConstInitValContext *>(root);
  assert(list_root);
  return dfs_const_ast(list_root, shape);
  // 2.'{' [ ConstInitVal { ',' ConstInitVal } ] '}'
}
std::shared_ptr<InitValExprAST>
ASTBuilder::dfs_const_ast(SysYParser::ListConstInitValContext *node,
                          const std::vector<int32_t> &shape) {
  // 在转换过程中，判定声明的shape与数组初始化器是否匹配
  // 不需要检查各个数组元素必须为常数
  // AST.isconstant
  std::vector<int32_t> result;
  std::vector<std::shared_ptr<ExprAST>> Inits;
  if (shape.size() == 0)
    throw InvalidInitList();
  // calculate total size and child_size ,child_size is the size of child array
  int u_dim = shape[0];
  int l_dim = shape[-1];
  int total_size = 1, child_size = 1;
  for (int i = 0; i < shape.size(); i++) {
    total_size *= shape[i];
    if (i > 0) {
      child_size *= shape[i];
    }
  }
  // 预先alloacte memory for vector
  // create a std::vector<std::shared_ptr<ExprAST>> ,size = u_dim
  std::vector<int32_t> child_shape = shape;
  child_shape.erase(child_shape.begin());
  // 计数，防止越界
  int cnt = 0;
  for (auto child : node->constInitVal()) {
    // use u_dim
    if (auto scalar_child =
            dynamic_cast<SysYParser::ScalarConstInitValContext *>(child)) {
      // use l_dim
      if (cnt + 1 > total_size)
        throw InvalidInitList();
      // transform ctx->ast
      auto ast = std::any_cast<std::shared_ptr<ExprAST>>(
          visit(scalar_child->constExp()));
      // 待验证
      // 验证初始值类型与声明类型是否匹配
      // init_type为INT，ast_type不能为FLOAT
      auto f_t = init_type->getTypeKind();
      auto s_t = ast->getType()->getTypeKind();
      if ((f_t == spanti::Type::SP_INTTYPE) &&
          (s_t == spanti::Type::SP_FLOATTYPE)) {
        throw InitTypeError();
      }
      if (ast->isconst(variables)) {
        // 求解每个constExp的常数值
        result.push_back(ast->ret);
      } else {
        throw SyntaxError("no constant expr");
      }
      ++cnt;
    } else {
      auto list_child =
          dynamic_cast<SysYParser::ListConstInitValContext *>(child);
      assert(list_child);
      // 填充完不可越界检查，填充子数组时，必须已填充子数组长度的整数倍
      if (cnt % child_size != 0 || cnt + child_size > total_size)
        throw InvalidInitList();
      // 递归调用函数
      dfs_const_ast(list_child, child_shape);
      cnt += child_size;
    }
  }
  // 其他部分隐式初始化
  while (cnt < total_size) {
    result.push_back(0);
    ++cnt;
  }
  return nullptr;
}