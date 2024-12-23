#include "AST/AST.h"
#include <memory>

namespace plsm {
namespace ast {

static const std::unordered_map<UnOp, std::string> unOpToString = {
    {UnOp::POS, "+"},
    {UnOp::NEG, "-"},
    {UnOp::NOT, "!"},
};

static const std::unordered_map<std::string, UnOp> stringToUnOp = {
    {"+", UnOp::POS},
    {"-", UnOp::NEG},
    {"!", UnOp::NOT},
};

boost::json::value UnExpr::toJson() const {
  return {
      {"@type", "UnExpr"},
      {"op", unOpToString.at(op)},
      {"expr", expr->toJson()},
  };
}

UnExpr *UnExpr::fromJson(boost::json::value json) {
  auto opString = getJsonValue<UnExpr, std::string>(json, "op");
  auto op = stringToUnOp.at(opString);

  auto expr = fromJsonProperty<UnExpr, Expr>(json, "expr");
  return new UnExpr(SourceRange::json(), op, std::move(expr));
}

} // namespace ast
} // namespace plsm
