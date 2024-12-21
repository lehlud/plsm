#include "AST/AST.h"

#include <map>
#include <memory>
#include <stdexcept>

namespace plsm {
namespace ast {

ASTNode *ASTNode::fromJson(boost::json::value json) {
  // if (json.is_null())
  //   return std::make_unique<NullValue>(Location::json());

  if (json.is_int64())
    return new IntValue(SourceRange::json(), json.as_int64());

  if (json.is_double())
    return new FloatValue(SourceRange::json(), json.as_double());

  std::string type;
  try {
    type = json.as_object().at("@type").as_string().c_str();
  } catch (...) {
    throw std::runtime_error(
        "called ASTNode::fromJson without @type constraint");
  }

  if (type == "BinExpr")
    return BinExpr::fromJson(json);
  if (type == "CallExpr")
    return CallExpr::fromJson(json);
  if (type == "CastExpr")
    return CastExpr::fromJson(json);
  if (type == "Identifier")
    return Identifier::fromJson(json);
  if (type == "LambdaExpr")
    return LambdaExpr::fromJson(json);
  if (type == "LambdaExpr")
    return LambdaExpr::fromJson(json);
  if (type == "UnExpr")
    return UnExpr::fromJson(json);

  if (type == "AssignStmt")
    return AssignStmt::fromJson(json);
  if (type == "ExprStmt")
    return ExprStmt::fromJson(json);
  if (type == "FnParam")
    return FnParam::fromJson(json);
  if (type == "FnDecl")
    return FnDecl::fromJson(json);
  if (type == "IfStmt")
    return IfStmt::fromJson(json);
  if (type == "RetStmt")
    return RetStmt::fromJson(json);
  if (type == "VarDecl")
    return VarDecl::fromJson(json);
  if (type == "WhileStmt")
    return WhileStmt::fromJson(json);

  if (type == "PrimitiveTypeName")
    return PrimitiveTypeName::fromJson(json);

  throw std::runtime_error("json conversion for '" + type +
                           "' not implemented");
}

Expr *Expr::fromJson(boost::json::value json) {
  auto node = ASTNode::fromJson(json);

  if (dynamic_cast<Expr *>(node)) {
    return (Expr *)node;
  }

  throw std::runtime_error(json.as_object().at("@type").as_string().c_str() +
                           std::string(" is not of type 'Expr'"));
}

TypeName *TypeName::fromJson(boost::json::value json) {
  auto node = ASTNode::fromJson(json);

  if (dynamic_cast<TypeName *>(node)) {
    return (TypeName *)node;
  }

  throw std::runtime_error(json.as_object().at("@type").as_string().c_str() +
                           std::string(" is not of type 'TypeName'"));
}

Stmt *Stmt::fromJson(boost::json::value json) {
  auto node = ASTNode::fromJson(json);

  if (dynamic_cast<Stmt *>(node)) {
    return (Stmt *)node;
  }

  throw std::runtime_error(json.as_object().at("@type").as_string().c_str() +
                           std::string(" is not of type 'Stmt'"));
}

Type *Type::fromJson(boost::json::value json) {
  std::string type;
  try {
    type = json.as_object().at("@type").as_string().c_str();
  } catch (...) {
    throw std::runtime_error("called Type::fromJson without @type constraint");
  }

  if (type == "FunctionType")
    return FunctionType::fromJson(json);
  if (type == "PrimitiveType")
    return PrimitiveType::fromJson(json);

  throw std::runtime_error("json conversion for '" + type +
                           "' not implemented");
}

} // namespace ast
} // namespace plsm