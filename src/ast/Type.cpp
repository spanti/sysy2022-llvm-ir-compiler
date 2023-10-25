#include "Type.h"
std::ostream & spanti::operator<<(std::ostream &o, const spanti::Type &obj) {
  if(obj.is_const){
    o << "CONST ";
  }
  switch (obj.getTypeKind()) {
  case spanti::Type::SP_VARTYPE:
    o << "Variable Type";
    break;
  case spanti::Type::SP_INTTYPE:
    o << "INT_TYPE";
    break;
  case spanti::Type::SP_FLOATTYPE:
    o << "FLOAT_TYPE";
    break;
  case spanti::Type::SP_ARRAYTYPE:
    o << "ARRAY_TYPE";
    break;
  case spanti::Type::SP_FUNCTIONTYPE:
    o << "FUNCTION_TYPE";
    break;
  case spanti::Type::SP_EMPTYTYPE:
    o << "EMPTY_TYPE";
    break;
  case spanti::Type::SP_VOIDTYPE:
    o << "VOID_TYPE";
    break;
  case spanti::Type::SP_STRINGTYPE:
    o << "STRING_TYPE";
    break;
  default:
    break;
  }
  return o;
}
//结构等价性
bool spanti::Type::is_match(spanti::Type* second){
  if((this->TyKind == second->TyKind)){
    return true;
  }
  else{
    return false;
  }
}
int spanti::ArrayType::get_count_dim(){
  if(unsize){
    return elements.size() + 1;
  }
  else{
    return elements.size();
  }
}