// some include file

// namespace - avoid duplicate names
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
  };

public:
  Type(TypeKind kind) : TyKind(kind) {}
  TypeKind getTypeKind() const{return TyKind;}
private:
  const TypeKind TyKind;
};
// represent type variable
class EmptyType : public Type {};
class VarType : public Type {};
class IntType : public Type {
  public:
  IntType():Type(SP_INTTYPE){}
};

class FloatType : public Type {};
class ArrayType : public Type {};
class FunctionType : public Type {
  public:
  FunctionType(TypeKind kind) : Type(kind) {}
};
} // namespace spanti