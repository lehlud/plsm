#include "AST/Def.h"
#include "AST/Expr/BinExpr.h"
#include "AST/Expr/Value.h"

#include <map>
#include <memory>
#include <stdexcept>

namespace plsm {

std::unique_ptr<ast::ASTNode> ast::ASTNode::fromJson(boost::json::value json) {
  if (json.is_null())
    return std::make_unique<NullValue>(ast::Location::json());

  if (json.is_int64())
    return std::make_unique<IntValue>(ast::Location::json(), json.as_int64());

  if (json.is_double())
    return std::make_unique<FloatValue>(ast::Location::json(),
                                        json.as_double());

  std::string type;
  try {
    type = json.as_object().at("@type").as_string().c_str();
  } catch (...) {
    throw std::runtime_error(
        "called ASTNode::fromJson without @type constraint");
  }

  if (type == "BinExpr")
    return ast::BinExpr::fromJson(json);
  if (type == "BlockExpr")
    return ast::BlockExpr::fromJson(json);
  if (type == "CallExpr")
    return ast::CallExpr::fromJson(json);
  if (type == "Closure")
    return ast::Closure::fromJson(json);
  if (type == "Identifier")
    return ast::Identifier::fromJson(json);
  if (type == "PropExpr")
    return ast::PropExpr::fromJson(json);
  if (type == "Tuple")
    return ast::Tuple::fromJson(json);
  if (type == "UnaryExpr")
    return ast::UnaryExpr::fromJson(json);

  if (type == "Import")
    return ast::Import::fromJson(json);
  if (type == "Module")
    return ast::Module::fromJson(json);

  if (type == "ExprStmt")
    return ast::ExprStmt::fromJson(json);
  if (type == "FnDef")
    return ast::FnDef::fromJson(json);
  if (type == "TraitAlias")
    return ast::TraitAlias::fromJson(json);
  if (type == "TraitDef")
    return ast::TraitDef::fromJson(json);
  if (type == "TypeAlias")
    return ast::TypeAlias::fromJson(json);
  if (type == "TypeDef")
    return ast::TypeDef::fromJson(json);
  if (type == "ValDecl")
    return ast::ValDecl::fromJson(json);

  if (type == "ClosureType")
    return ast::ClosureType::fromJson(json);
  if (type == "GenericType")
    return ast::GenericType::fromJson(json);
  if (type == "NamedType")
    return ast::NamedType::fromJson(json);
  if (type == "TupleType")
    return ast::TupleType::fromJson(json);

  throw std::runtime_error("json conversion for '" + type +
                           "' not implemented");
}

std::unique_ptr<ast::Expr> ast::Expr::fromJson(boost::json::value json) {
  auto node = ast::ASTNode::fromJson(json);

  if (dynamic_cast<ast::Expr *>(node.get())) {
    return std::unique_ptr<ast::Expr>((ast::Expr *)node.release());
  }

  throw std::runtime_error(json.as_object().at("@type").as_string().c_str() +
                           std::string(" is not of type 'Expr'"));
}

std::unique_ptr<ast::Type> ast::Type::fromJson(boost::json::value json) {
  auto node = ast::ASTNode::fromJson(json);

  if (dynamic_cast<ast::Type *>(node.get())) {
    return std::unique_ptr<ast::Type>((ast::Type *)node.release());
  }

  throw std::runtime_error(json.as_object().at("@type").as_string().c_str() +
                           std::string(" is not of type 'Type'"));
}

std::unique_ptr<ast::Stmt> ast::Stmt::fromJson(boost::json::value json) {
  auto node = ast::ASTNode::fromJson(json);

  if (dynamic_cast<ast::Stmt *>(node.get())) {
    return std::unique_ptr<ast::Stmt>((ast::Stmt *)node.release());
  }

  throw std::runtime_error(json.as_object().at("@type").as_string().c_str() +
                           std::string(" is not of type 'Stmt'"));
}

} // namespace plsm
