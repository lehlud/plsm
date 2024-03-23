#pragma once

#include <boost/json/serialize.hpp>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <boost/json.hpp>

#define LOC_ARG const Location &location

namespace plsm {
namespace ast {

class Location {
public:
  Location(const std::string &file, std::pair<size_t, size_t> start,
           std::pair<size_t, size_t> end)
      : file(file), start(start), end(end) {}

  const std::string file;
  const std::pair<size_t, size_t> start, end;

  static Location json() { return Location("<json>", {-1, -1}, {-1, -1}); };
};

class ASTNode {
public:
  ASTNode(LOC_ARG) : location(location) {}
  virtual ~ASTNode() = default;

  const Location location;

  virtual boost::json::value toJson() = 0;

  virtual std::string toJsonString() {
    return boost::json::serialize(toJson());
  }

  static std::unique_ptr<ASTNode> fromJson(boost::json::value json);

  virtual bool isExpr() { return false; }
  virtual bool isStmt() { return false; }
  virtual bool isType() { return false; }

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

    std::vector<std::unique_ptr<SubNode>> result;
    for (auto &el : arr) {
      result.push_back(SubNode::fromJson(el));
    }

    return result;
  }
};

class Expr : public ASTNode {
public:
  Expr(LOC_ARG) : ASTNode(location) {}

  static std::unique_ptr<Expr> fromJson(boost::json::value json);

  virtual bool isExpr() override { return true; }
};

class Stmt : public ASTNode {
public:
  Stmt(LOC_ARG) : ASTNode(location) {}

  static std::unique_ptr<Stmt> fromJson(boost::json::value json);

  virtual bool isStmt() override { return true; }
};

class Type : public ASTNode {
public:
  Type(LOC_ARG) : ASTNode(location) {}

  static std::unique_ptr<Type> fromJson(boost::json::value json);

  virtual bool isType() override { return true; }
};

} // namespace ast
} // namespace plsm
