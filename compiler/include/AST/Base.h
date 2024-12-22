#pragma once

#include <any>
#include <boost/json/serialize.hpp>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/json.hpp>

#define LOC_ARG const SourceRange &sourceRange

namespace plsm {
namespace ast {

class BinExpr;
class CallExpr;
class CastExpr;
class Identifier;
class LambdaExpr;
class UnExpr;
class IntValue;
class FloatValue;

class Import;
class Module;

class AssignStmt;
class ExprStmt;
class FnParam;
class FnDecl;
class IfStmt;
class RetStmt;
class VarDecl;
class WhileStmt;

class PrimitiveTypeName;

class ASTVisitor {
public:
  virtual ~ASTVisitor() = default;

  virtual std::any visit(BinExpr &binExpr, std::any param) = 0;
  virtual std::any visit(CallExpr &callExpr, std::any param) = 0;
  virtual std::any visit(CastExpr &castExpr, std::any param) = 0;
  virtual std::any visit(Identifier &identifier, std::any param) = 0;
  virtual std::any visit(LambdaExpr &lambdaExpr, std::any param) = 0;
  virtual std::any visit(UnExpr &unExpr, std::any param) = 0;
  virtual std::any visit(IntValue &intValue, std::any param) = 0;
  virtual std::any visit(FloatValue &floatValue, std::any param) = 0;

  virtual std::any visit(Import &import, std::any param) = 0;
  virtual std::any visit(Module &module, std::any param) = 0;

  virtual std::any visit(AssignStmt &assignStmt, std::any param) = 0;
  virtual std::any visit(ExprStmt &exprStmt, std::any param) = 0;
  virtual std::any visit(FnParam &fnParam, std::any param) = 0;
  virtual std::any visit(FnDecl &fnDecl, std::any param) = 0;
  virtual std::any visit(IfStmt &ifStmt, std::any param) = 0;
  virtual std::any visit(RetStmt &retStmt, std::any param) = 0;
  virtual std::any visit(VarDecl &varDecl, std::any param) = 0;
  virtual std::any visit(WhileStmt &whileStmt, std::any param) = 0;

  virtual std::any visit(PrimitiveTypeName &primitiveTypeName,
                         std::any param) = 0;
};

} // namespace ast
} // namespace plsm

namespace plsm {
namespace ast {

class Jsonable {
public:
  virtual ~Jsonable() = default;

  virtual boost::json::value toJson() = 0;

protected:
  template <class CurrNode>
  static inline boost::json::value getJsonProperty(boost::json::value json,
                                                   std::string property) {
    boost::json::value prop;
    try {
      if (!json.as_object().contains(property))
        throw std::exception();
      prop = json.as_object().at(property);
    } catch (...) {
      std::cout << boost::json::serialize(json) << std::endl;
      throw std::runtime_error("missing property '" + property + "' in " +
                               typeid(CurrNode).name() + "::fromJson");
    }

    return prop;
  }

  template <class CurrNode, typename T>
  static inline T getJsonValue(boost::json::value json, std::string property) {
    auto prop = getJsonProperty<CurrNode>(json, property);
    try {
      return boost::json::value_to<T>(prop);
    } catch (...) {
      throw std::runtime_error("invalid value for property '" + property +
                               "' in " + typeid(CurrNode).name() +
                               "::fromJson");
    }
  }

  template <class CurrNode, class SubNode>
  static inline auto fromJsonProperty(boost::json::value json,
                                      std::string property) {
    return SubNode::fromJson(getJsonProperty<CurrNode>(json, property));
  }

  template <class CurrNode, class SubNode>
  static inline auto fromJsonVector(boost::json::value json,
                                    std::string property) {
    auto arr = getJsonProperty<CurrNode>(json, property).as_array();

    std::vector<SubNode *> result;
    for (auto &el : arr) {
      result.push_back(SubNode::fromJson(el));
    }

    return result;
  }
};

class SourceRange {
public:
  SourceRange(const std::string &file, const std::string &text,
              std::pair<size_t, size_t> start, std::pair<size_t, size_t> end)
      : file(file), text(text), start(start), end(end) {}

  const std::string file, text;
  const std::pair<size_t, size_t> start, end;

  static SourceRange unknown() {
    return SourceRange("<?>", "<?>", {-1, -1}, {-1, -1});
  };

  static SourceRange json() {
    return SourceRange("<json>", "<?>", {-1, -1}, {-1, -1});
  };
};

class Type : public Jsonable {
public:
  Type() : Jsonable() {}
  virtual ~Type() = default;

  static Type *fromJson(boost::json::value json);
};

class Symbol {
  const std::string name;
  std::shared_ptr<Type> type;

public:
  Symbol(const std::string &name) : name(name) {}
  Symbol(const std::string &name, Type *type) : name(name), type(type) {}
};

class ASTNode : public Jsonable {
public:
  ASTNode(LOC_ARG) : Jsonable(), sourceRange(sourceRange) {}
  virtual ~ASTNode() = default;

  const SourceRange sourceRange;

  virtual std::string toJsonString() {
    return boost::json::serialize(toJson(), {});
  }

  static ASTNode *fromJson(boost::json::value json);

  virtual bool isExpr() { return false; }
  virtual bool isStmt() { return false; }
  virtual bool isTypeName() { return false; }

  virtual std::any accept(ASTVisitor *visitor, std::any param) = 0;
};

class Expr : public ASTNode {
  std::shared_ptr<Type> type;

public:
  Expr(LOC_ARG) : ASTNode(sourceRange) {}
  virtual ~Expr() = default;

  static Expr *fromJson(boost::json::value json);

  virtual bool isExpr() override { return true; }
};

class Stmt : public ASTNode {
public:
  Stmt(LOC_ARG) : ASTNode(sourceRange) {}
  virtual ~Stmt() = default;

  static Stmt *fromJson(boost::json::value json);

  virtual bool isStmt() override { return true; }
};

class TypeName : public ASTNode {
public:
  TypeName(LOC_ARG) : ASTNode(sourceRange) {}
  virtual ~TypeName() = default;

  static TypeName *fromJson(boost::json::value json);

  virtual bool isTypeName() override { return true; }
};

} // namespace ast
} // namespace plsm
