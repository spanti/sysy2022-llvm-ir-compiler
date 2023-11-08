#include "AST.h"
#include "ASTvisitor.h"
#include "SysY2022JIT.h"
#include "Type.h"
#include "errors.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/MemoryDependenceAnalysis.h"
#include "llvm/Analysis/MemorySSA.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/Analysis/ProfileSummaryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include <cassert>
#include <llvm-16/llvm/ADT/APInt.h>
#include <llvm-16/llvm/IR/BasicBlock.h>
#include <llvm-16/llvm/IR/Constant.h>
#include <llvm-16/llvm/IR/Constants.h>
#include <llvm-16/llvm/IR/DebugInfo.h>
#include <llvm-16/llvm/IR/DerivedTypes.h>
#include <llvm-16/llvm/IR/Function.h>
#include <llvm-16/llvm/IR/GlobalVariable.h>
#include <llvm-16/llvm/IR/Instructions.h>
#include <llvm-16/llvm/IR/Intrinsics.h>
#include <llvm-16/llvm/IR/Type.h>
#include <llvm-16/llvm/IR/Verifier.h>
#include <llvm-16/llvm/Support/Casting.h>
#include <llvm-16/llvm/Support/Error.h>
#include <memory>
#include <unistd.h>
#include <vector>
void ASTIRGenerator::setBLoopBB(BasicBlock *bb) {
  assert(cycle == 0);
  BLoopBB = bb;
}
void ASTIRGenerator::clearBLoopBB() {
  assert(cycle == 0);
  BLoopBB = nullptr;
}
void ASTIRGenerator::storeValueArray(std::vector<Value *> &VAS, Value *gepR,
                                     Type *e_type) {
  // gepR 一维数组指针
  // gepFR 索引指针
  Value *gepFR = gepR;
  for (int i = 0; i < VAS.size(); i++) {
    // generate paor <GEP,STORE>
    //  pair GEP - Store
    //  若Value的值为nullptr，停止生成
    Value *SV = VAS[i];
    if (SV) {
      if (i != 0) {
        // update gepFR - Value:OFFSET
        gepFR = IRbuilder->CreateGEP(
            e_type, gepR,
            ConstantInt::get(Type::getInt32Ty(*TheContext), i, "geptmp"));
      }
      IRbuilder->CreateStore(SV, gepFR);
    }
  }
}
void ASTIRGenerator::localArrayInitialization(VariableTableEntry *entry,
                                              class VarDecAST *ast) {
  // generate code for local array initial
  // 简化目标
  Type *e_type;
  std::vector<Value *> vp_array;
  std::vector<uint64_t> dims;
  // handle InitvalExpr
  // int a[2][2] = {{1},2,3}
  // 多维数组指针 -> 一维数组指针
  Value *gepR = getArrayBasePtr(entry);
  spanti::ArrayType *sp_type =
      dynamic_cast<spanti::ArrayType *>(entry->var_type);
  e_type = mapToLLVMType(sp_type->getElementTy());
  // 使用基地址+偏移方式 处理初始值问题
  // 为各个初始化值生成getelementptr - store对
  // 基于常量及非常累声明采用不同策略
  // 自定义初始化器 局部常量数组
  if (entry->var_type->get_const()) {
    // 常量，使用数组常量化器
    auto CAS = entry->const_init;
    std::vector<Value *> VAS;
    // transfrom it to Constant
    // 存储初始值
    for (int i = 0; i < CAS.size(); i++) {
      Value *SV =
          ConstantInt::get(Type::getInt32Ty(*TheContext), entry->const_init[i]);
      VAS.push_back(SV);
    }
    // transform int32_t to Value*
    // input: std::vector<Value *> VAS;PTR gepR
    // 两个for循环设置为函数
    storeValueArray(VAS, gepR, e_type);
  } else {
    // 局部变量数组
    // 使用数组initial - ExprAST
    // void localArrayInitialization(entry)
    std::vector<Value *> VAS;
    // GET vector<Value*> from InitialExprAST
    auto i_ast = dynamic_cast<InitValExprAST *>(ast->initval.get());
    for (auto e_ast : i_ast->Inits) {
      e_ast->accept(this);
      VAS.push_back(get_value());
    }
    // 存储初始值
    // input: std::vector<Value *> VAS;PTR gepR
    storeValueArray(VAS, gepR, e_type);
  }
}
llvm::Value *ASTIRGenerator::getArrayElemPtr(VariableTableEntry *entry,
                                             class ArrayExprAST *ast) {
  Type *e_type;
  Value *gepR;
  std::vector<uint64_t> dims;
  auto llvm_type = mapToLLVMType(entry->var_type);
  if (entry->is_global) {
    gepR = entry->global_var;
  } else {
    gepR = entry->inst_memory;
  }
  if (!gepR)
    throw UnrecognizedVarName(ast->Name);
  if (llvm_type->isArrayTy()) {
    // ArrayType - ArrayType;
    ArrayType *a_type = dyn_cast<ArrayType>(llvm_type);
    // 获取数组信息
    getDimsFromType(a_type, dims);
    int array_size = dims.size();
    int index_size = ast->elems.size();
    // 元素类型
    // 获取数组指定偏移的指针
    // 若数组的声明为int a[3][4]
    // 只能处理int a[1][1]的情况
    // 如何处理 a[0]的情形
    for (int i = 0; i < index_size; i++) {
      std::vector<Value *> refArray;
      // 填充0
      refArray.push_back(ConstantInt::get(Type::getInt32Ty(*TheContext), 0));
      // get Value from ast->elems[i]
      ast->elems[i]->accept(this);
      Value *e_value = get_value();
      if (!e_value)
        throw SyntaxError("Unknown Array Elems!");
      refArray.push_back(e_value);
      // refArray:{0，Indexi}
      gepR = IRbuilder->CreateGEP(a_type, gepR, refArray, "geptmp");
      // gepR->print(outs()); // test
      e_type = a_type->getElementType();
      a_type = dyn_cast<ArrayType>(e_type);
    }
  } else if (llvm_type->isPtrOrPtrVectorTy()) {
    // 处理不定数组的情形
    // load 加载数组指针
    // handle int[] -> int[8]
    // POINTER-TYPE - ARRAYTYPE
    // 默认维度数为1
    // Load加载数组指针

    PointerType *p_type = dyn_cast<PointerType>(llvm_type);
    gepR = IRbuilder->CreateLoad(llvm_type, gepR, ast->Name);
    ArrayType *a_type;
    Type *elem_type = p_type->getNonOpaquePointerElementType();
    // 索引为空 - 直接返回gepR
    if (ast->elems.empty()) {
      return gepR; // problem! a a unsize - array
    }
    if (elem_type->isIntegerTy()) {
      // 元素为整数 - 一维不定数组
      // p[1] = ?
      // 直接索引1
      ast->elems[0]->accept(this);
      Value *e_value = get_value();
      gepR = IRbuilder->CreateGEP(elem_type, gepR, {e_value}, "geptmp");
      return gepR;
    } else if (elem_type->isArrayTy()) {
      // 生成数组的指针

      // 直接索引
      // 数组类型
      a_type = dyn_cast<ArrayType>(elem_type);
      // 获取elem—_type类型
    }
    // 为参数生成地址解析函数
    // 不透明指针 无法获取 指针指向元素的信息
    // 初始的数组元素 类型信息
    e_type = a_type;
    // 若获取的地址为数组
    //  获取数组信息
    getDimsFromType(a_type, dims);
    int array_size = dims.size() + 1;
    int index_size = ast->elems.size();
    Type *c_type;
    // 复制后的index
    // 对于不定数组，一次性解析所有的elems
    std::vector<Value *> refArray;
    for (int i = 0; i < index_size; i++) {
      ast->elems[i]->accept(this);
      Value *e_value = get_value();
      if (!e_value)
        throw SyntaxError("Unknown Array Elems!");
      refArray.push_back(e_value);
    }
    // gepR->print(outs());
    // e_type->print(outs());
    gepR = IRbuilder->CreateGEP(e_type, gepR, refArray, "geptmp");
    // gepR->print(outs()); // test
  } else {
    throw SyntaxError("can't get PTR from no array");
  }

  return gepR;
}
llvm::Value *
ASTIRGenerator::getArrayBasePtr(VariableTableEntry *entry) { // 默认为局部
  Type *e_type;
  Value *gepR;
  std::vector<uint64_t> dims;
  auto llvm_type = mapToLLVMType(entry->var_type);
  if (llvm_type->isArrayTy()) {
    gepR = entry->inst_memory;
    ArrayType *a_type = dyn_cast<ArrayType>(llvm_type);
    // 数组各维度信息
    getDimsFromType(a_type, dims);
    int array_size = dims.size();
    auto refArray = getZRefArrayFromType(2);
    for (int i = 0; i < array_size; i++) {
      gepR = IRbuilder->CreateGEP(a_type, gepR, refArray, "geptmp");
      // gepR->print(outs()); // test
      e_type = a_type->getElementType();
      a_type = dyn_cast<ArrayType>(e_type);
      // update a_type
    }
  } else {
    throw SyntaxError("can't get PTR from no array");
  }

  return gepR;
}
// 可能无用
std::vector<llvm::Value *>
ASTIRGenerator::getZRefArrayFromType(int child_size) {
  // 生成一个数组对应的{0，0}Value
  std::vector<llvm::Value *> result;
  for (int i = 0; i < child_size; i++) {
    result.push_back(ConstantInt::get(IntegerType::get(*TheContext, 32), 0));
  }
  /*
  for(int i=0;i<total_size;i++){
    for()
  }*/
  return result;
}
// useful
void ASTIRGenerator::getDimsFromType(llvm::ArrayType *a_type,
                                     std::vector<uint64_t> &dims) {
  Type *elementType = a_type->getElementType();
  uint64_t numElements = a_type->getNumElements();

  if (ArrayType *innerArrayType = dyn_cast<ArrayType>(elementType)) {
    // 继续递归，处理内部维度
    dims.push_back(numElements);
    getDimsFromType(innerArrayType, dims);
  } else {
    // 终止递归
    dims.push_back(numElements);
  }
}
void ASTIRGenerator::register_lib_to_module(
    std::string name, spanti::Type *re_type,
    std::vector<spanti::Type *> params_type) {
  // llvm function type
  auto FT = mapToLLVMFType(new spanti::FunctionType(params_type, re_type));
  // function declaration
  llvm::Function *function =
      Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());
  // 添加模块？
}
void ASTIRGenerator::register_libs_to_module() {
  register_lib_to_module("getint", new spanti::IntType(), {});
  register_lib_to_module("getch", new spanti::IntType(), {});
  register_lib_to_module(
      "getarray", new spanti::IntType(),
      {new spanti::ArrayType(new spanti::IntType(), true, {})}); // int[]

  register_lib_to_module("putint", new spanti::VoidType(),
                         {new spanti::IntType()}); // int
  register_lib_to_module("putch", new spanti::VoidType(),
                         {new spanti::IntType()}); // int
  register_lib_to_module(
      "putarray", new spanti::VoidType(),
      {new spanti::IntType(),
       new spanti::ArrayType(new spanti::IntType(), true, {})}); // int,int[]
}
// transform function type
llvm::FunctionType *
ASTIRGenerator::mapToLLVMFType(spanti::FunctionType *sym_type) {
  if (sym_type) {
    // consider return type
    auto re_type = mapToLLVMType(sym_type->get_re_type());
    // consider params type
    std::vector<Type *> lp_types;
    for (auto pTs : sym_type->get_params_type()) {
      lp_types.push_back(mapToLLVMType(pTs));
    }
    // generate function type
    return FunctionType::get(re_type, lp_types, false);
  }
  // transfrom error
  //  暂时不使用
  return nullptr;
}
llvm::Type *ASTIRGenerator::mapToLLVMType(spanti::Type *sym_type) {
  // 将自定义类型转换为LLVM Type
  //  int,float,array,function
  auto typekind = sym_type->getTypeKind();
  spanti::FunctionType *func_type;
  spanti::ArrayType *array_type;
  switch (typekind) {
  case spanti::Type::SP_VOIDTYPE:
    return llvm::Type::getVoidTy(*TheContext);
  case spanti::Type::SP_INTTYPE:
    // 32-bit int value
    return llvm::Type::getInt32Ty(*TheContext);
  case spanti::Type::SP_FLOATTYPE:
    // 32-bit float value
    return llvm::Type::getFloatTy(*TheContext);
  case spanti::Type::SP_ARRAYTYPE:
    // 暂时不使用
    // 构造类型
    // 元素类型，维度信息，是否定型
    array_type = dynamic_cast<spanti::ArrayType *>(sym_type);
    if (array_type) {
      // 元素类型 int/float
      auto e_type = mapToLLVMType(array_type->getElementTy());
      // 暂时处理一维情形
      // 多维数组的情形 int[][2],int[3][2]
      // 使用nested type 创建
      if (array_type->get_unsize()) {
        // no need size
        // 暂时忽略不定数组的处理
        // create array-type
        // 指向int的指针/指向数组的指针
        // need size
        // nested type
        auto dims_array = array_type->get_elements();
        if (dims_array.size() != 0) {
          // 多维度不定数组
          llvm::ArrayType *a_type;
          int dims_size = dims_array.size();
          for (int i = dims_size - 1; i >= 0; i--) {
            if (i < dims_size - 1) {
              // 嵌套构造
              a_type = llvm::ArrayType::get(a_type, dims_array[i]);
            } else {
              // 初始情形
              a_type = llvm::ArrayType::get(e_type, dims_array[i]);
            }
          }
          e_type = a_type;
        }
        return llvm::PointerType::get(e_type, 0);
      } else {
        // need size
        // nested type
        auto dims_array = array_type->get_elements();
        llvm::ArrayType *a_type;
        int dims_size = dims_array.size();
        for (int i = dims_size - 1; i >= 0; i--) {
          if (i < dims_size - 1) {
            // 嵌套构造
            a_type = llvm::ArrayType::get(a_type, dims_array[i]);
          } else {
            // 初始情形
            a_type = llvm::ArrayType::get(e_type, dims_array[i]);
          }
        }
        // return llvm::Array
        return a_type;
      }
    }
    return nullptr;
  case spanti::Type::SP_FUNCTIONTYPE:
    func_type = dynamic_cast<spanti::FunctionType *>(sym_type);
    return mapToLLVMFType(func_type);
  default:
    return nullptr;
  }
}
// 包含一系列LLVM IR文件
void ASTIRGenerator::Initialize() {

  // initialize the JIT!
  // TheJIT = ExitOnErr(orc::SysY2022JIT::Create());
  // 构造函数
  // Context* 上下文
  TheContext = std::make_unique<LLVMContext>();
  // 设置禁用不透明指针形式
  TheContext->setOpaquePointers(false);
  // Module 文件模块
  TheModule = std::make_unique<Module>("test jit", *TheContext);
  // IRBuilder = a file pointer when reading/writing a file
  IRbuilder = std::make_unique<IRBuilder<>>(*TheContext);
  // Create new pass and analysis manager
  TheFPM = std::make_unique<FunctionPassManager>();
  TheFAM = std::make_unique<FunctionAnalysisManager>();
  TheMAM = std::make_unique<ModuleAnalysisManager>();
  ThePIC = std::make_unique<PassInstrumentationCallbacks>();
  TheSI = std::make_unique<StandardInstrumentations>(*TheContext,
                                                     /*DebugLogging*/ true);
  TheSI->registerCallbacks(*ThePIC, TheFAM.get());
  // Add transform passes.
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  TheFPM->addPass(InstCombinePass());
  // Reassociate expressions.
  TheFPM->addPass(ReassociatePass());
  // Eliminate Common SubExpressions.
  TheFPM->addPass(GVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->addPass(SimplifyCFGPass());
  // Register analysis passes used in these transform passes.

  TheFAM->registerPass([&] { return AAManager(); });
  TheFAM->registerPass([&] { return AssumptionAnalysis(); });
  TheFAM->registerPass([&] { return DominatorTreeAnalysis(); });
  TheFAM->registerPass([&] { return LoopAnalysis(); });
  TheFAM->registerPass([&] { return MemoryDependenceAnalysis(); });
  TheFAM->registerPass([&] { return MemorySSAAnalysis(); });
  TheFAM->registerPass([&] { return OptimizationRemarkEmitterAnalysis(); });
  TheFAM->registerPass([&] {
    return OuterAnalysisManagerProxy<ModuleAnalysisManager, Function>(*TheMAM);
  });
  TheFAM->registerPass(
      [&] { return PassInstrumentationAnalysis(ThePIC.get()); });
  TheFAM->registerPass([&] { return TargetIRAnalysis(); });
  TheFAM->registerPass([&] { return TargetLibraryAnalysis(); });

  TheMAM->registerPass([&] { return ProfileSummaryAnalysis(); });

  // 初始化库函数声明
  register_libs_to_module();
}

void ASTIRGenerator::setSymbolTables(FunctionTable *functions,
                                     VariableTable *variables) {
  this->functions = functions;
  this->variables = variables;
}
void ASTIRGenerator::visit(class BinaryExprAST *ast) {
  // 默认 re_value = nullptr
  re_value = nullptr;
  // 短路求值部分
  if (ast->Op == BinaryExprAST::OpKind::And) {
    // And &&
    // icmp ne %L,0
    //  create blocks for then,else and marge cases
    //  create edges between basic blocks
    // handle a && b
    // need pass MergeBB?

    BasicBlock *EntryBB = IRbuilder->GetInsertBlock(); // a
    Function *TheFunction = EntryBB->getParent();
    // Blocks for if-else
    BasicBlock *ThenBB =
        BasicBlock::Create(*TheContext, "then", TheFunction); // b
    // 传递继承属性 节点属性 -> 子节点属性
    // 为全局变量可能会引入复杂性和潜在的错误
    // LHS 属性传递
    ast->LHS.get()->TBB = ThenBB;
    ast->LHS.get()->FBB = ast->FBB;
    // IfTBB1 = ThenBB;
    //  set insert block -> entry
    ast->LHS->accept(this);
    // set L jump command
    Value *L = get_value(); // a
    // icmp ne %L,0
    re_value = IRbuilder->CreateICmpNE(
        L, ConstantInt::get(Type::getInt32Ty(*TheContext), 0), "icmptmp");

    // Create Cond Br - newlabel(),IfFBB
    // B1.true = newlabel() <B2.true,B2.false> = <B.true,B.false>
    re_value = IRbuilder->CreateCondBr(re_value, ThenBB, ast->FBB);
    // re_value = IRbuilder->CreateCondBr(re_value, ThenBB, IfFBB1);
    //  set insert block -> then
    IRbuilder->SetInsertPoint(ThenBB);
    // RHS属性传递
    ast->RHS.get()->TBB = ast->TBB;
    ast->RHS.get()->FBB = ast->FBB;
    ast->RHS->accept(this);
    // set R jump command and generate code
    Value *R = get_value(); // b
    re_value = IRbuilder->CreateICmpNE(
        R, ConstantInt::get(Type::getInt32Ty(*TheContext), 0), "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    // Create Cond Br -
    // IRbuilder->CreateCondBr(re_value,IfTBB,IfFBB);
    // Add new merge block to end of function  blockset
    return;
  } else if (ast->Op == BinaryExprAST::OpKind::Or) {
    // Or ||
    // 与And 指令 相同 icmp eq %L,0

    BasicBlock *EntryBB = IRbuilder->GetInsertBlock(); // a
    Function *TheFunction = EntryBB->getParent();
    // Blocks for if-else
    BasicBlock *ThenBB =
        BasicBlock::Create(*TheContext, "then", TheFunction); // b
    // 传递一次继承属性
    ast->LHS.get()->TBB = ast->TBB;
    ast->LHS.get()->FBB = ThenBB;
    // IfFBB1 = ThenBB;
    //  set insert block -> entry
    ast->LHS->accept(this);
    // set L jump command

    Value *L = get_value(); // a
    // L->print(outs());
    //  icmp ne %L,0
    re_value = IRbuilder->CreateICmpNE(
        L, ConstantInt::get(Type::getInt32Ty(*TheContext), 0), "icmptmp");
    // Create Cond Br - newlabel(),IfFBB
    re_value = IRbuilder->CreateCondBr(re_value, ast->TBB, ThenBB);
    // set insert block -> then
    IRbuilder->SetInsertPoint(ThenBB);
    // 传递继承属性
    // RHS属性传递
    ast->RHS.get()->TBB = ast->TBB;
    ast->RHS.get()->FBB = ast->FBB;
    ast->RHS->accept(this);
    // set R jump command and generate code
    Value *R = get_value(); // b
    re_value = IRbuilder->CreateICmpNE(
        R, ConstantInt::get(Type::getInt32Ty(*TheContext), 0), "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    // Create Cond Br -
    // IRbuilder->CreateCondBr(re_value,IfTBB,IfFBB);
    // Add new merge block to end of function  blockset
    return;
  }
  // 确保下列代码仅执行一次
  ast->LHS->accept(this);
  Value *L = get_value();
  ast->RHS->accept(this);
  Value *R = get_value();
  // 提前化对操作符&&及||的实现
  if (!L || !R) {
    return;
  }
  // 基于操作符
  // 调用一系列Create__ method
  // 左右操作数的节点类型必须相同
  // 情形 float/int Op float/int
  switch (ast->Op) {
  case BinaryExprAST::OpKind::Mul:
    re_value = IRbuilder->CreateMul(L, R, "multmp");
    break;
  case BinaryExprAST::OpKind::Div:
    re_value = IRbuilder->CreateSDiv(L, R, "divtmp");
    break;
  case BinaryExprAST::OpKind::Add:
    re_value = IRbuilder->CreateAdd(L, R, "addtmp");
    break;
  case BinaryExprAST::OpKind::Sub:
    re_value = IRbuilder->CreateSub(L, R, "subtmp");
    break;
  case BinaryExprAST::OpKind::Mod:
    re_value = IRbuilder->CreateSRem(L, R, "sremtmp");
    break;
  // 比较符层次
  case BinaryExprAST::OpKind::Gt:
    // 第三个参数为指令指定名称
    re_value = IRbuilder->CreateICmpSGT(L, R, "icmptmp");
    // 数值转换为32位
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    break;
  case BinaryExprAST::OpKind::Lt:
    re_value = IRbuilder->CreateICmpSLT(L, R, "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    break;
  case BinaryExprAST::OpKind::Ge:
    re_value = IRbuilder->CreateICmpSGE(L, R, "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    break;
  case BinaryExprAST::OpKind::Le:
    re_value = IRbuilder->CreateICmpSLE(L, R, "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    break;
  case BinaryExprAST::OpKind::Eq:
    re_value = IRbuilder->CreateICmpEQ(L, R, "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    break;
  case BinaryExprAST::OpKind::Ne:
    re_value = IRbuilder->CreateICmpNE(L, R, "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    break;
  // 未正确实现短路求值
  //  使用控制流法，将逻辑表达式转化为控制流结构
  // 注释掉的原有处理逻辑
  /*
  case BinaryExprAST::OpKind::And:
    re_value = IRbuilder->CreateAnd(L, R, "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    break;
  case BinaryExprAST::OpKind::Or:
    re_value = IRbuilder->CreateOr(L, R, "icmptmp");
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    break;*/
  default:
    throw SyntaxError("unexpect binary op");
    break;
  }
}
void ASTIRGenerator::visit(class UnaryExprAST *ast) {
  // 默认 re_value = nullptr
  re_value = nullptr;
  ast->UHS->accept(this);
  Value *U = get_value();
  if (!U) {
    return;
  }
  switch (ast->Op) {
  case UnaryExprAST::UpKind::Pos:
    // 直接传递
    re_value = U;
    break;
  case UnaryExprAST::UpKind::Neg:
    // 取负
    re_value = IRbuilder->CreateNeg(U, "iutmp");
    break;
  case UnaryExprAST::UpKind::Not:
    // 仅用于条件表达式，！0 = 1，！1 = 0
    // CreateICmpEQ(value, llvm::ConstantInt::get(int32Type, 0), "result");
    // re_value = IRbuilder->CreateNot(U,"iutmp");
    re_value = IRbuilder->CreateICmpEQ(
        U, llvm::ConstantInt::get(IntegerType::get(*TheContext, 32), 0),
        "iutmp");
    // transfrom i1 to i32
    re_value = IRbuilder->CreateZExt(re_value, Type::getInt32Ty(*TheContext),
                                     "zextmp");
    // OpStr = "!";
    break;
  default:
    throw SyntaxError("unexpect unary op");
    break;
  }
}
void ASTIRGenerator::visit(class IntergarExprAST *ast) {
  re_value = ConstantInt::get(*TheContext, APInt(32, ast->value));
}
void ASTIRGenerator::visit(class FloatExprAST *ast) {
  re_value = ConstantFP::get(*TheContext, APFloat(ast->value));
}
void ASTIRGenerator::visit(class IdentifierAST *ast) {
  // 在访问前处理好exprast问题
  auto entry = variables->lookUp(ast->getName(),ast->loc.Line);
  // int a;
  // 查询符号表
  if (!entry)
    throw UnrecognizedVarName(ast->getName());
  // 基于全局和局部变量完成区分
  if (entry->is_global) {
    auto G = entry->global_var;
    if (!G)
      throw UnrecognizedVarName(ast->getName());
    // 对全局声明的处理
    // 当前作用域为全局 不使用load
    if (variables->is_global()) {
      // problem：应当传递ConstantInt::get
      // 转换常数
      // problem:无法解析a+1为常数
      Constant *GlobalVarConstant = G->getInitializer();
      // GlobalVarConstant->print(outs());
      re_value = GlobalVarConstant;
    } else {
      // 当前作用域为局部，load
      re_value = IRbuilder->CreateLoad(G->getValueType(), G, ast->getName());
      //  Builder.CreateLoad(GlobalVar, "loadVar");
    }

  } else {
    // find alloca_inst
    auto A = entry->inst_memory;
    // if A = NULL,that mean A is uninitialize
    if (!A) {
      // find a global variable which has the same name
      auto A_G = variables->lookUp(ast->getName(), variables->head,ast->loc.Line);
      if (!A_G)
        throw UnrecognizedVarName(ast->getName());
      // Global handle
      auto G = A_G->global_var;
      Constant *GlobalVarConstant = G->getInitializer();
      // GlobalVarConstant->print(outs());
      re_value = GlobalVarConstant;
    } else {
      // 变量引用，创建对应的load指令
      // Create___ method
      re_value =
          IRbuilder->CreateLoad(A->getAllocatedType(), A, ast->getName());
    }
  }
}
void ASTIRGenerator::visit(class ArrayExprAST *ast) {

  // loop up memory location
  // GEP series - get ptr to Value
  // 目前处理局部情形
  auto entry = variables->lookUp(ast->Name,ast->loc.Line);
  auto llvm_type = mapToLLVMType(entry->var_type);
  if (!entry)
    throw UnrecognizedVarName(ast->Name);
  std::vector<uint64_t> dims;
  Type *e_type;
  // get dim info from symboltable
  Value *gepR = getArrayElemPtr(entry, ast);
  spanti::ArrayType *sp_type =
      dynamic_cast<spanti::ArrayType *>(entry->var_type);
  e_type = mapToLLVMType(sp_type->getElementTy());
  int array_size = sp_type->get_count_dim();
  int index_size = ast->elems.size();
  // 获取数组指定偏移的指针
  if (array_size == index_size) {
    // gepFR为一个一维数组指针
    // 读取该数组元素
    // gepR->print(outs());
    re_value = IRbuilder->CreateLoad(e_type, gepR, ast->Name);
  } else {
    // 直接返回
    re_value = gepR;
  }
}
void ASTIRGenerator::visit(class CallExprAST *ast) {
  re_value = nullptr;
  // 检测到的库函数 生成对应declare语句
  // get function type from functions
  // 获取模块中保存的getFunction/在函数声明时将function指针保存至符号表中
  Function *CalleeF = TheModule->getFunction(ast->Callee);
  if (!CalleeF)
    throw SyntaxError("Unknown Register Function!");
  // get params
  std::vector<Value *> ArgsV;
  // 获取每个参数的Value，pushback
  for (int i = 0; i < ast->Args.size(); i++) {
    ast->Args[i]->accept(this);
    auto AV = get_value();
    ArgsV.push_back(AV);
  }
  // callee + vector<Value*>

  // void 不出现 %calltmp2
  llvm::Type *returnType = CalleeF->getReturnType();
  if (returnType == llvm::Type::getVoidTy(*TheContext)) {
    // 不要为一个void返回值的指令设计名称
    re_value = IRbuilder->CreateCall(CalleeF, ArgsV);
  } else {
    re_value = IRbuilder->CreateCall(CalleeF, ArgsV, "calltmp");
  }
}
void ASTIRGenerator::visit(class InitValExprAST *ast) { re_value = nullptr; }
void ASTIRGenerator::visit(class ImplicitValExprAST *ast) {
  re_value = nullptr;
}
void ASTIRGenerator::visit(class AssignStmtAST *ast) {
  re_value = nullptr;
  // 针对数组初始化值的设计
  // 左侧必须为左值
  // 简单情形 a
  if (auto l_ast = dynamic_cast<IdentifierAST *>(ast->LHS.get())) {
    // get RHS
    ast->RHS->accept(this);
    auto r_value = get_value();
    if (!r_value)
      return;
    auto entry = variables->lookUp(l_ast->getName(),l_ast->loc.Line);
    // a = 5; generate store instruction
    if (!entry)
      throw UnrecognizedVarName(l_ast->getName());
    // find alloca_inst
    Value *gepR;
    if (entry->is_global) {
      gepR = entry->global_var;
    } else {
      gepR = entry->inst_memory;
    }
    if (!gepR)
      throw UnrecognizedVarName(l_ast->getName());
    // generate instructions
    IRbuilder->CreateStore(r_value, gepR);
    // 传递返回值
    re_value = r_value;
  } else {
    // 检索数组地址
    // a[4]
    auto a_ast = dynamic_cast<ArrayExprAST *>(ast->LHS.get());
    // 手动处理a_ast
    auto entry = variables->lookUp(a_ast->Name,a_ast->loc.Line);
    // a = 5; generate store instruction
    if (!entry)
      throw UnrecognizedVarName(a_ast->Name);
    // find alloca_inst
    std::vector<uint64_t> dims;
    Type *e_type;
    // 需要在内存中加载指针
    Value *gepR = getArrayElemPtr(entry, a_ast);
    spanti::ArrayType *sp_type =
        dynamic_cast<spanti::ArrayType *>(entry->var_type);
    e_type = mapToLLVMType(sp_type->getElementTy());
    // 初始值处理部分
    // create instructions
    ast->RHS->accept(this);
    auto r_value = get_value();
    if (!r_value)
      return;
    IRbuilder->CreateStore(r_value, gepR);
    // 传递返回值
    re_value = r_value;
    // GEP instruction to generate PTR

    // STORE initval to PTE
  }
}
void ASTIRGenerator::visit(class IfStmtAST *ast) {
  // else 可能为 bullptr
  // if-then-else 与 if-then
  re_value = nullptr;
  // pass code
  //  if()
  //   create blocks for then,else and marge cases
  //   create edges between basic blocks
  Function *TheFunction = IRbuilder->GetInsertBlock()->getParent();
  // insert BB to function
  BasicBlock *ThenBB = BasicBlock::Create(*TheContext, "then", TheFunction);
  // not inseet BB to funciton
  BasicBlock *ElseBB = BasicBlock::Create(*TheContext, "else");
  BasicBlock *MergeBB = BasicBlock::Create(*TheContext, "ifcont");

  // 继承属性传递 IF_F
  // 规定B.True B.false
  if (ast->Else) {
    ast->Cond.get()->TBB = ThenBB;
    ast->Cond.get()->FBB = ElseBB;
    // IfFBB1 = ElseBB;
    // IfTBB1 = ThenBB;
  } else {
    ast->Cond.get()->TBB = ThenBB;
    ast->Cond.get()->FBB = MergeBB;
    // IfFBB1 = MergeBB;
    // IfTBB1 = ThenBB;
  }

  // 如何处理表达式的短路求值问题
  // 调用Cond节点时，将传回ThenBB 和 ElseBB的引用
  //传递Cond属性
  ast->Cond->accept(this);
  Value *CondV = get_value();
  if (!CondV)
    return;
  // ThenBB可能改变 - problem
  // ThenBB = IfTBB;
  // CondV 不为cmp类型指令时，创建一层cmp指令
  // transfrom i32 - i1
  // 这个代码会使示例3报错，因为类型不匹配
  // CreateICmpEQ:i1 , +123:i32
  if (CondV->getType() == IRbuilder->getInt32Ty()) {
    CondV = IRbuilder->CreateICmpSGT(CondV, IRbuilder->getInt32(0), "ifcond");
  }

  if (ast->Else) {
    // CondV的类型必须为i1
    // 生成对应的比较指令
    // 若表达式为&& 类型，则不需要生成多余的跳转指令
    IRbuilder->CreateCondBr(CondV, ThenBB, ElseBB);
    // Emit then value
    IRbuilder->SetInsertPoint(ThenBB);
    ast->Then->accept(this);
    // create br(uncond)
    // ThenBB在调用Then—>accept时刻可能改变，update
    ThenBB = IRbuilder->GetInsertBlock();
    // 在CreateBr前确定插入点是否存在分支指令(ret void)
    if (!ThenBB->getTerminator()) {
      IRbuilder->CreateBr(MergeBB);
    }
    // Add Else block to Function
    TheFunction->insert(TheFunction->end(), ElseBB);
    // emit else bb
    IRbuilder->SetInsertPoint(ElseBB);
    ast->Else->accept(this);
    Value *ElseV = get_value();
    if (!ElseV)
      return;
    // ElseV在调用Else—>accept时刻可能改变，update
    ElseBB = IRbuilder->GetInsertBlock();
    // create br(uncond)
    if (!ElseBB->getTerminator()) {
      IRbuilder->CreateBr(MergeBB);
    }
    // CONNECT BLOCKS
    // Add merge blockset
    TheFunction->insert(TheFunction->end(), MergeBB);
    // emit merge block
    IRbuilder->SetInsertPoint(MergeBB);

    // phi - 一定添加PHI?
    // 使用内存存储变量，PHI不必要，内存可以不为SSA形式
    // 目前不考虑PHI
    // PHINode *PN =
    // IRbuilder->CreatePHI(Type::getInt32Ty(*TheContext),2,"iftmp"); income
    // values PN->addIncoming(ThenV,ThenBB); PN->addIncoming(ElseV,ElseBB);
    //  return a value - not se't
    // re_value = PN;
  } else {
    // if-then
    IRbuilder->CreateCondBr(CondV, ThenBB, MergeBB);
    // Emit then value
    IRbuilder->SetInsertPoint(ThenBB);
    ast->Then->accept(this);
    Value *ThenV = get_value();
    // create br(uncond)
    // ThenBB在调用Then—>accept时刻可能改变，update
    ThenBB = IRbuilder->GetInsertBlock();
    // 已存在终结指令，不再创建跳转
    if (!ThenBB->getTerminator()) {
      IRbuilder->CreateBr(MergeBB);
    }
    // CONNECT BLOCKS
    // Add merge blockset
    TheFunction->insert(TheFunction->end(), MergeBB);
    // emit merge block
    IRbuilder->SetInsertPoint(MergeBB);
    // phi - 一定添加PHI?
    // 使用内存存储变量，PHI不必要，内存可以不为SSA形式
    // 目前不考虑PHI
    // PHINode *PN =
    // IRbuilder->CreatePHI(Type::getInt32Ty(*TheContext),2,"iftmp"); income
    // values PN->addIncoming(ThenV,ThenBB); PN->addIncoming(ElseV,ElseBB);
    //  return a value - not se't
    // re_value = PN;
  }
  // crate br(cond)
}
/*void ASTIRGenerator::visit(class IfStmtAST *ast) {
  // else 可能为 bullptr
  // if-then-else 与 if-then
  re_value = nullptr;
  //pass code

  // 如何处理表达式的短路求值问题
  //调用Cond节点时，将传回ThenBB 和 ElseBB的引用
  ast->Cond->accept(this);
  Value *CondV = get_value();
  if (!CondV)
    return;
  // CondV 不为cmp类型指令时，创建一层cmp指令
  // transfrom i32 - i1
  // 这个代码会使示例3报错，因为类型不匹配
  // CreateICmpEQ:i1 , +123:i32
  if (CondV->getType() == IRbuilder->getInt32Ty()) {
    CondV = IRbuilder->CreateICmpSGT(CondV, IRbuilder->getInt32(0), "ifcond");
  }
  // if()
  //  create blocks for then,else and marge cases
  //  create edges between basic blocks
  Function *TheFunction = IRbuilder->GetInsertBlock()->getParent();
  // insert BB to function
  BasicBlock *ThenBB = BasicBlock::Create(*TheContext, "then", TheFunction);
  // not inseet BB to funciton
  BasicBlock *ElseBB = BasicBlock::Create(*TheContext, "else");
  BasicBlock *MergeBB = BasicBlock::Create(*TheContext, "ifcont");
  if (ast->Else) {
    // CondV的类型必须为i1
    // 生成对应的比较指令
    IRbuilder->CreateCondBr(CondV, ThenBB, ElseBB);
    // Emit then value
    IRbuilder->SetInsertPoint(ThenBB);
    ast->Then->accept(this);
    Value *ThenV = get_value();
    if (!ThenV)
      return;
    // create br(uncond)
    IRbuilder->CreateBr(MergeBB);
    // ThenBB在调用Then—>accept时刻可能改变，update
    ThenBB = IRbuilder->GetInsertBlock();
    // Add Else block to Function
    TheFunction->insert(TheFunction->end(), ElseBB);
    // emit else bb
    IRbuilder->SetInsertPoint(ElseBB);
    ast->Else->accept(this);
    Value *ElseV = get_value();
    if (!ElseV)
      return;
    // create br(uncond)
    IRbuilder->CreateBr(MergeBB);
    // ElseV在调用Else—>accept时刻可能改变，update
    ElseBB = IRbuilder->GetInsertBlock();
    // CONNECT BLOCKS
    // Add merge blockset
    TheFunction->insert(TheFunction->end(), MergeBB);
    // emit merge block
    IRbuilder->SetInsertPoint(MergeBB);

    // phi - 一定添加PHI?
    // 使用内存存储变量，PHI不必要，内存可以不为SSA形式
    // 目前不考虑PHI
    // PHINode *PN =
    // IRbuilder->CreatePHI(Type::getInt32Ty(*TheContext),2,"iftmp"); income
    // values PN->addIncoming(ThenV,ThenBB); PN->addIncoming(ElseV,ElseBB);
    //  return a value - not se't
    // re_value = PN;
  } else {
    // if-then
    IRbuilder->CreateCondBr(CondV, ThenBB, MergeBB);
    // Emit then value
    IRbuilder->SetInsertPoint(ThenBB);
    ast->Then->accept(this);
    Value *ThenV = get_value();
    if (!ThenV)
      return;
    // create br(uncond)
    IRbuilder->CreateBr(MergeBB);
    // ThenBB在调用Then—>accept时刻可能改变，update
    ThenBB = IRbuilder->GetInsertBlock();

    // CONNECT BLOCKS
    // Add merge blockset
    TheFunction->insert(TheFunction->end(), MergeBB);
    // emit merge block
    IRbuilder->SetInsertPoint(MergeBB);
    // phi - 一定添加PHI?
    // 使用内存存储变量，PHI不必要，内存可以不为SSA形式
    // 目前不考虑PHI
    // PHINode *PN =
    // IRbuilder->CreatePHI(Type::getInt32Ty(*TheContext),2,"iftmp"); income
    // values PN->addIncoming(ThenV,ThenBB); PN->addIncoming(ElseV,ElseBB);
    //  return a value - not se't
    // re_value = PN;
  }
  // crate br(cond)
}*/
// while-cycle的设计错误
void ASTIRGenerator::visit(class WhileStmtAST *ast) {
  // some br instruction

  // 基于比较值，生成条件跳转
  //  while(){}
  //   create blocks for then,else and marge cases
  //   create edges between basic
  Function *TheFunction = IRbuilder->GetInsertBlock()->getParent();
  BasicBlock *PreLoopBB = IRbuilder->GetInsertBlock();
  if (cycle == 0)
    setBLoopBB(PreLoopBB);
  addcycle();
  // begin
  BasicBlock *PreheaderBB =
      BasicBlock::Create(*TheContext, "looppre", TheFunction);
  // b.true
  BasicBlock *BodyBB = BasicBlock::Create(*TheContext, "loop");
  // jump goal? // 针对break/continue指令，跳转指令可能位置，跳转位置在之后填充
  // b.false <- S.next;
  BasicBlock *AfterBB = BasicBlock::Create(*TheContext, "loopend");
  // 设置 布尔表达式的继承属性
  ast->Cond.get()->TBB = BodyBB;
  ast->Cond.get()->FBB = AfterBB;
  //IfFBB1 = AfterBB;
  //IfTBB1 = BodyBB;
  // save
  Cbb.push(PreheaderBB);
  Bbb.push(AfterBB);
  // Body
  // create alloca before it
  // create br
  // IRbuilder->CreateBr(PreheaderBB);
  //  Emit then value
  // Block:
  IRbuilder->SetInsertPoint(PreheaderBB);
  // condition - expression
  ast->Cond->accept(this);
  auto CondV = get_value();
  if (!CondV)
    return;
  // CondV 不为cmp类型指令时，创建一层cmp指令
  // transfrom i32 - i1
  // 这个代码会使示例3报错，因为类型不匹配
  // CreateICmpEQ:i1 , +123:i32
  if (CondV->getType() == IRbuilder->getInt32Ty()) {
    CondV = IRbuilder->CreateICmpSGT(CondV, IRbuilder->getInt32(0), "ifcond");
  }
  // CreateBr B.true = BodyBB B.false = S.next
  IRbuilder->CreateCondBr(CondV, BodyBB, AfterBB);

  TheFunction->insert(TheFunction->end(), BodyBB);
  // insert code to BodyBB
  IRbuilder->SetInsertPoint(BodyBB);
  // emit body code
  ast->Body->accept(this);
  auto BodyV = get_value();
  if (!BodyV)
    return;
  // create br
  IRbuilder->CreateBr(PreheaderBB);

  // AfterBb 不需要二维创建
  //  insert code to AfterBB
  //  icmp.../br...
  //  AfterBB不再为不完整类型
  TheFunction->insert(TheFunction->end(), AfterBB);
  IRbuilder->SetInsertPoint(AfterBB);

  // 在创建完声明后 创建分支代码
  IRbuilder->SetInsertPoint(PreLoopBB);
  IRbuilder->CreateBr(PreheaderBB);
  IRbuilder->SetInsertPoint(AfterBB);
  subcycle();
  if (cycle == 0)
    clearBLoopBB();
  // 对应之前的push
  Cbb.pop();
  Bbb.pop();
}
void ASTIRGenerator::visit(class BreakStmtAST *ast) {
  // 基本块指针
  // 建议将指令保存在栈中，之后再作填充
  // jump to Loop.start
  // br>
  // CreateBr，bB不可为不完整类型
  // 无名称BB以处理跳转目标未知
  Function *TheFunction = IRbuilder->GetInsertBlock()->getParent();
  BasicBlock *bB = BasicBlock::Create(
      *TheContext, "bb",
      TheFunction); // 指令创建于当前块，problem:跳转目标不可不确定
  BasicBlock *bnB = BasicBlock::Create(
      *TheContext, "bnb",
      TheFunction); // 指令创建于当前块，problem:跳转目标不可不确定
  // bB保存break 对应 跳转指令
  re_value = IRbuilder->CreateBr(bB);
  IRbuilder->SetInsertPoint(bB);
  IRbuilder->CreateBr(Bbb.top());
  // 创建新Basicblock块并设置插入点于新Basicblock开头
  IRbuilder->SetInsertPoint(bnB);
}
void ASTIRGenerator::visit(class ContinueStmtAST *ast) {
  // 基本块指针
  // br>
  // jump to Loop.next
  Function *TheFunction = IRbuilder->GetInsertBlock()->getParent();
  BasicBlock *cB = BasicBlock::Create(*TheContext, "cb", TheFunction);
  BasicBlock *cnB = BasicBlock::Create(
      *TheContext, "cnb",
      TheFunction); // 指令创建于当前块，problem:跳转目标不可不确定
  re_value = IRbuilder->CreateBr(cB);
  IRbuilder->SetInsertPoint(cB);
  IRbuilder->CreateBr(Cbb.top());
  // 创建新Basicblock块并设置插入点于新Basicblock开头
  IRbuilder->SetInsertPoint(cnB);
}
void ASTIRGenerator::visit(class BlockStmtAST *ast) {
  re_value = nullptr;
  // block为顺序序列
  // block:body/{}
  // block将返回最后一个计算值
  // 若为函数，重新进入函数作用域
  // entryscope
  variables->EntryScope(ast->cur_ast_scope);

  for (auto stmt : ast->stmts) {
    stmt->accept(this);
    re_value = get_value();
  }

  // 退出block作用域
  variables->ExitScope();
}
void ASTIRGenerator::visit(class ExpStmtAST *ast) {
  // Expr
  if (ast->Expr)
    ast->Expr->accept(this);
  re_value = get_value();
}
void ASTIRGenerator::visit(class RetuStmtAST *ast) {
  re_value = nullptr;
  if (ast->Expr) {
    ast->Expr->accept(this); // return Exp;
    re_value = IRbuilder->CreateRet(get_value());
  } else {
    // return;
    re_value = IRbuilder->CreateRetVoid();
  }
}
void ASTIRGenerator::visit(class ConstDecAST *ast) { re_value = nullptr; }
void ASTIRGenerator::visit(class VarDecAST *ast) {
  auto entry = variables->lookUp(ast->getName(),ast->loc.Line);
  // int a;
  // 查询符号表
  if (!entry)
    throw UnrecognizedVarName(ast->getName());
  // 获取llvm类型信息
  auto llvm_type = mapToLLVMType(entry->var_type);
  GlobalVariable *global_var;
  // 若为全局变量
  if (entry->is_global) {
    // 全局变量的声明有错误
    //  创建全局变量
    global_var = new GlobalVariable(
        *TheModule, llvm_type, entry->var_type->get_const(),
        GlobalValue::LinkageTypes::ExternalLinkage, 0, ast->getName());
    global_var->setDSOLocal(true);
    // 存储全局变量？
    entry->global_var = global_var;
    // 全局 int 类型变量/常量的初值必须是编译时可求值的常量表达式。
    // 初始化值
    if (llvm_type->isArrayTy()) {
      // 设置数组初始化器 - 全部置为0
      global_var->setInitializer(ConstantAggregateZero::get(llvm_type));
    } else {
      global_var->setInitializer(llvm::ConstantInt::get(llvm_type, 0));
    }
    if (ast->initval) {

      if (llvm_type->isArrayTy()) {
        // 获取数组元素的类型
        ArrayType *a_type = dyn_cast<ArrayType>(llvm_type);
        // 获取数组的各维度大小
        std::vector<uint64_t> dims;
        Constant *result;
        getDimsFromType(a_type, dims);
        // 基于常数和非常数声明分类设计，生成对应的ConstantArray
        // 基于维度，嵌套构造ConstantArray
        if (entry->var_type->get_const()) {
          result = getConstantArray(entry->const_init, dims);
        } else {
          auto r_ast = dynamic_cast<InitValExprAST *>(ast->initval.get());
          result = getConstantArray(r_ast->Inits, dims);
        }
        global_var->setInitializer(result);
        // 处理数组的初始化值
      } else {
        ast->initval->accept(this);
        // 右侧值必须为常量值
        Value *r_value = get_value();
        // test
        // create_store
        if (!r_value)
          return;
        llvm::Constant *r_Constant = llvm::dyn_cast<llvm::Constant>(r_value);
        // 全局变/常量声明中指定的初值表达式必须是常量表达式
        if (r_Constant) {
          // 使用 myConstant，它现在是一个不可变的常量
          global_var->setInitializer(r_Constant);
        } else {
          // 处理转换失败的情况
          throw SyntaxError(
              "global declaration with non-constant initial value");
        }
        // 初始值的类型与全局变量类型匹配
      }
    }
  } else {
    // 若为非全局变量
    // 使用allova为数组分配内存位置
    // 若声明位于循环内部，需将alloca 提取到循环外部 以避免多次的alloca
    // 一个循环计数器，两个设置标志位的方法，一个指针记录循环外的节点
    // set insert point outside of the loop
    BasicBlock *sB = IRbuilder->GetInsertBlock();
    AllocaInst *alloca_inst;
    if (cycle > 0) {
      IRbuilder->SetInsertPoint(BLoopBB);
      alloca_inst = IRbuilder->CreateAlloca(llvm_type, nullptr, ast->getName());
      // set back the insert point
      IRbuilder->SetInsertPoint(sB);
    } else {
      alloca_inst = IRbuilder->CreateAlloca(llvm_type, nullptr, ast->getName());
    }

    // alloca_inst->print(outs());
    //  若为数组，另设情况讨论
    //  存储内存位置
    entry->inst_memory = alloca_inst;
    // local array : memset
    // get params
    std::vector<Value *> ArgsV;
    if (llvm_type->isArrayTy()) {
      spanti::ArrayType *a_type =
          dynamic_cast<spanti::ArrayType *>(entry->var_type);
      int array_size = 1;
      for (int i = 0; i < a_type->get_count_dim(); i++) {
        array_size *= a_type->get_elements()[i];
      }

      // get Type
      std::vector<llvm::Type *> params;
      // Tys 复写 可重载函数（同名函数，不同的参数类型）
      //  重载 (PTR,I32,I8,I1) -> (I32*,I8,I64,I1)
      params.push_back(llvm::Type::getInt32PtrTy(*TheContext));
      params.push_back(llvm::Type::getInt64Ty(*TheContext));
      // set ArgsV
      //  base ptr of array
      ArgsV.push_back(alloca_inst);
      // 0
      ArgsV.push_back(ConstantInt::get(Type::getInt8Ty(*TheContext), 0));
      // size and sizeof 4*
      ArgsV.push_back(
          ConstantInt::get(Type::getInt64Ty(*TheContext), array_size * 4)),
          // false
          ArgsV.push_back(ConstantInt::getFalse(*TheContext));
      // 当参数为重载类型，Tys不为空 error?
      // memset is overload function
      Function *InstF = Intrinsic::getDeclaration(
          TheModule.get(), llvm::Intrinsic::memset, params);
      InstF->print(outs());
      IRbuilder->CreateCall(InstF, ArgsV);
    }
    // 返回合格
    // 若存在初始值，处理对应情况(generate load/store instruction)
    // 基于llvm type判断
    if (ast->initval) {
      if (llvm_type->isArrayTy()) {
        localArrayInitialization(entry, ast);
      } else {

        // Initial value -> -IntergarExprAST
        ast->initval->accept(this);
        auto r_value = get_value();
        // test
        // create_store
        if (!r_value)
          return;
        IRbuilder->CreateStore(r_value, alloca_inst);
      }
    }
  }

  // 使用符号表信息处理
  // 最终返回?
  re_value = nullptr;
}
llvm::Constant *
ASTIRGenerator::getConstantArray(std::vector<int32_t> &const_init,
                                 std::vector<uint64_t> &dims) {
  // 将一个一维列表转化为多维嵌套结构
  // 只处理整数情况
  // 将一个初始化列表转化为
  std::vector<llvm::Constant *> result;
  // 数组基础类型
  ArrayType *innerArrayType;

  for (int i = 0; i < const_init.size(); i++) {
    Constant *c =
        ConstantInt::get(Type::getInt32Ty(*TheContext), const_init[i]);
    result.push_back(c);
  }
  innerArrayType =
      ArrayType::get(Type::getInt32Ty(*TheContext), const_init.size());
  if (dims.size() == 1) {
    // 直接构造
    Constant *child_con = ConstantArray::get(innerArrayType, result);
    return child_con;
  }
  // 若数组为二维或更高的数组
  int b_num = 1;
  for (int i = 0; i < dims.size(); i++) {
    b_num *= dims[i];
  }
  // 构造次数
  for (int i = dims.size() - 1; i >= 0; i--) {
    // 更新构造次数
    b_num = b_num / dims[i];
    std::vector<llvm::Constant *> garray;
    // 构造次数 b_num
    int child_size = dims[i];
    for (int k = 0; k < b_num; k++) {
      // 构造一个Vector -> 转化为Constant
      std::vector<llvm::Constant *> carray;

      // 桥套构造innerArrayType
      if (i == dims.size() - 1) {
        innerArrayType =
            ArrayType::get(Type::getInt32Ty(*TheContext), child_size);
      } else {
        innerArrayType = ArrayType::get(innerArrayType, child_size);
      }
      for (int j = 0; j < child_size; j++) {
        carray.push_back(result[k * child_size + j]);
      }

      // Vector -> 转化为Constant
      Constant *child_con = ConstantArray::get(innerArrayType, carray);
      // 推入一个新的数组，最终替换原有vector
      garray.push_back(child_con);
    }

    result = garray;
  }
  return result[0];
}

llvm::Constant *ASTIRGenerator::getConstantArray(
    std::vector<std::shared_ptr<ExprAST>> &const_init,
    std::vector<uint64_t> &dims) {
  std::vector<int32_t> result;
  // 额外的步骤 -> 将std::vector<std::shared_ptr<ExprAST>> 转化为
  // std::vector<int32_t>
  for (auto ast : const_init) {
    if (ast->isconst(variables)) {
      int32_t ret = ast->ret;
      result.push_back(ret);
    } else {
      throw SyntaxError("Global Declaration No Const Expr");
    }
  }

  return getConstantArray(result, dims);
}
void ASTIRGenerator::visit(class FuncDecAST *ast) {
  // return Function (which extends value)
  // write ir code to stream
  // get type info from functions
  auto entry = functions->lookUp(ast->getName());
  auto func_type = entry->func_type;
  // get return type
  spanti::Type *r_type = func_type->get_re_type();
  // set cur scope
  variables->EntryScope(entry->func_scope);

  Type *lr_type;
  // 设计函数，使spanti::Type 可转换为对应的Type
  // transfrom function type
  // 获取返回值的Type
  lr_type = mapToLLVMType(r_type);
  // 获取参数的Type
  // handle params
  std::vector<Type *> lp_types;
  FunctionType *FT;
  // handle param types
  for (auto param : ast->Params) {
    // 处理函数参数为int[] / int[][2]的情形
    auto p = mapToLLVMType(param->Ty);
    lp_types.push_back(p);
  }

  // create IR Function Type
  // if function has params,generate FunctionType *FT =
  //    FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);
  if (lp_types.empty()) {
    FT = FunctionType::get(lr_type, false);
  } else {
    FT = FunctionType::get(lr_type, lp_types, false);
  }

  // create IR Function and register it to Module
  Function *F = Function::Create(FT, Function::ExternalLinkage, ast->getName(),
                                 TheModule.get());
  // Set names for all arguments.
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(ast->Params[Idx++]->name);

  // 提示符测试
  // Create a new basic block to start insertion into.
  BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", F);
  IRbuilder->SetInsertPoint(BB);
  // entry symbol table - need to design new function

  // 如有必要，bkock对应节点设置一对entryscope - exitscope
  //  record cur_func scope
  cur_func_scope = variables->cur;
  // handle param declaration
  // alloca + store

  auto Arg_begin = F->args().begin();
  for (int i = 0; i < ast->Params.size(); i++) {
    Argument *arg = &*(Arg_begin + i);
    // 使用allocate ，基于函数类型分配存储空间
    ast->Params[i]->accept(this);
    std::string p_name = arg->getName().str();
    auto entry = variables->lookUp(p_name);
    AllocaInst *alloca = entry->inst_memory;

    // 形参来源于Function-Args
    // 使用store指令将参数存储到分配空间中
    IRbuilder->CreateStore(arg, alloca);
  }
  // exit scope
  variables->ExitScope();
  Type *fr_type = IRbuilder->getCurrentFunctionReturnType();
  // handle body right
  ast->Body->accept(this);
  // the value of body
  auto value = get_value();
  verifyFunction(*F);
  // Optimizie the function
  // TheFPM->run(*F, *TheFAM);
  // F->print(outs());
  re_value = F;
  // 创建一条ret void指令
  // 确保所有分支的re指令满足条件
  // 插入点不能为分支指令
  auto CurBB = IRbuilder->GetInsertBlock();
  // create br(uncond)
  if (!CurBB->getTerminator()) {
    if (fr_type->isVoidTy()) {
      IRbuilder->CreateRetVoid();
    } else if (fr_type->isIntegerTy()) {
      // 未创建返回语句
      IRbuilder->CreateRet(ConstantInt::get(Type::getInt32Ty(*TheContext), 0));
    }
  }
}
void ASTIRGenerator::visit(class ProgramAST *ast) {

  for (auto decl : ast->decls) {
    decl->accept(this);
  }
  // examine error
  /*
  if (verifyModule(*TheModule, &errs())) {
    errs() << "Error in module" << "\n";
    // 验证失败，处理错误信息
  } else {
    errs() << "Module is valid.\n";
    // 验证成功
  }*/

  // M.print write .ll file
  TheModule->print(irs, nullptr);
  /*
  auto RT = TheJIT->getMainJITDylib().createResourceTracker();

  auto TSM =  llvm::orc::ThreadSafeModule(std::move(TheModule),
  std::move(TheContext)); ExitOnErr(TheJIT->addModule(std::move(TSM), RT)); auto
  ExprSymbol = ExitOnErr(TheJIT->lookup("main"));*/

  // Get the symbol's address and cast it to the right type (takes no
  // arguments, returns a double) so we can call it as a native function.
  // int (*FP)() = ExprSymbol.getAddress().toPtr<int (*)()>();
  // fprintf(stderr, "Return %d\n", FP());

  // 向文件中写入必要内容

  irs.flush();
  // Initialize the target registry etc.
  InitializeAllTargetInfos();
  InitializeNativeTarget();
  InitializeAllTargetMCs();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();
  // specify and set the architecture
  auto TargetTriple = LLVMGetDefaultTargetTriple();
  TheModule->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    throw SyntaxError(std::move(Error));
  }

  // TargetMachine
  auto CPU = "generic";
  auto Features = "";

  TargetOptions opt;
  auto RM = std::optional<Reloc::Model>();
  TheTargetMachine =
      Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
  TheModule->setDataLayout(TheTargetMachine->createDataLayout());
  // emit object code
  auto Filename = "output.o";
  std::error_code EC;
  raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

  if (EC) {
    throw SyntaxError("Could not open file: " + EC.message());
  }
  // define a pass that emits object code, then we run that pass
  legacy::PassManager pass;
  auto FileType = CodeGenFileType::CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    throw SyntaxError("TheTargetMachine can't emit a file of this type");
  }

  pass.run(*TheModule);
  dest.flush();

  outs() << "Wrote " << Filename << "\n";
  re_value = nullptr;
}