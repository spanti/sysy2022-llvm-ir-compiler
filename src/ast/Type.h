// some include file
#pragma once
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
// namespace - avoid duplicate names
class ExprAST;
namespace spanti {
// Type - base class of Type
class Type {
public:
  enum TypeKind {
    SP_VARTYPE,
    SP_INTTYPE,
    SP_FLOATTYPE,
    SP_ARRAYTYPE,
    SP_FUNCTIONTYPE,
    SP_EMPTYTYPE,
    SP_VOIDTYPE,
    SP_STRINGTYPE
  };

public:
  Type(TypeKind kind) : TyKind(kind),is_const(false) {}
  Type(TypeKind kind,bool is_const) : TyKind(kind),is_const(is_const) {}
  //设置多态
  virtual ~Type() {}
  TypeKind getTypeKind() const { return TyKind; }
  bool get_const(){return is_const;}
  void set_const(){is_const = true;}
  friend std::ostream& operator<<(std::ostream& o, const Type& obj);
  //is_match method
  //虚方法,结构等价性
  virtual bool is_match(Type* second);
private:
  const TypeKind TyKind;
  bool is_const;
};
// represent type variable
class VoidType : public Type {
public:
  VoidType() : Type(SP_VOIDTYPE) {}
};
class StringType : public Type {
  public:
  StringType() : Type(SP_STRINGTYPE) {}
};
class IntType : public Type {
public:
  IntType() : Type(SP_INTTYPE){}
  IntType(bool isconst) : Type(SP_INTTYPE,isconst){}
public:
};

class FloatType : public Type {
public:
  FloatType() : Type(SP_FLOATTYPE){}
  FloatType(bool isconst) : Type(SP_FLOATTYPE, isconst) {}
};
// 复合类型由基本类型组成
//  设计不合理
class ArrayType : public Type {
  // dangerous
public:
  ArrayType(Type *Ty, std::vector<int32_t> elements)
      : Type(SP_ARRAYTYPE), ElementTy(Ty), elements(std::move(elements)),unsize(false){}
  ArrayType(Type *Ty, bool unsize, std::vector<int32_t> elements)
      : Type(SP_ARRAYTYPE), ElementTy(Ty), elements(std::move(elements)),unsize(unsize){}
  int get_count_dim();
  std::vector<int32_t> get_elements(){return elements;}
  Type* getElementTy(){return ElementTy;}
  bool get_unsize(){return unsize;}
private:
  // type
  Type *ElementTy;
  // int a[N+1][3] 处理声明中存在表达式的情况，。代替std::vector<int>
  std::vector<int32_t> elements;
  //std::shared_ptr<ExprAST> index;
  //不定数组 第一维度是否存在。第一类a[][3]/a[5][4]
  bool unsize;
};
class FunctionType : public Type {
public:
  FunctionType(std::vector<Type *> ParamsTy, Type *RetTy)
      : Type(SP_FUNCTIONTYPE), params_type(ParamsTy), re_type(RetTy) {}
  int get_size(){return params_type.size();}
  Type* get_re_type(){return re_type;}
  std::vector<Type *>& get_params_type(){return params_type;} 
private:
  // return-type
  Type *re_type;
  std::vector<Type *> params_type;
};
bool TypeMatch(Type *s_t, Type *g_t);
} // namespace spanti