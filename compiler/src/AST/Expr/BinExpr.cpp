#include "AST/AST.h"
#include <memory>

namespace plsm {
namespace ast {

static const std::unordered_map<BinOp, std::string> binOpToString = {
    {BinOp::ADD, "+"}, {BinOp::SUB, "-"}, {BinOp::MUL, "*"}, {BinOp::DIV, "/"},
    {BinOp::MOD, "%"}, {BinOp::EQ, "=="}, {BinOp::NE, "!="}, {BinOp::LT, "<"},
    {BinOp::LE, "<="}, {BinOp::GT, ">"},  {BinOp::GE, ">="}, {BinOp::AND, "&&"},
    {BinOp::OR, "||"},
};

static const std::unordered_map<std::string, BinOp> stringToBinOp = {
    {"+", BinOp::ADD}, {"-", BinOp::SUB}, {"*", BinOp::MUL}, {"/", BinOp::DIV},
    {"%", BinOp::MOD}, {"==", BinOp::EQ}, {"!=", BinOp::NE}, {"<", BinOp::LT},
    {"<=", BinOp::LE}, {">", BinOp::GT},  {">=", BinOp::GE}, {"&&", BinOp::AND},
    {"||", BinOp::OR},
};

boost::json::value BinExpr::toJson() const {
  return {
      {"@type", "BinExpr"},
      {"op", binOpToString.at(op)},
      {"lhs", lhs->toJson()},
      {"rhs", rhs->toJson()},
  };
}

BinExpr *BinExpr::fromJson(boost::json::value json) {
  auto opString = getJsonValue<BinExpr, std::string>(json, "op");
  auto op = stringToBinOp.at(opString);

  auto lhs = fromJsonProperty<BinExpr, Expr>(json, "lhs");
  auto rhs = fromJsonProperty<BinExpr, Expr>(json, "rhs");

  return new BinExpr(SourceRange::json(), op, std::move(lhs), std::move(rhs));
}

} // namespace ast
} // namespace plsm
