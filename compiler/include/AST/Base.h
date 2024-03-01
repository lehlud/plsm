#pragma once

#include <utility>
#include <string>
#include <functional>

#define LOC_ARG const Location &location

namespace ast
{
  class Location
  {
  public:
    Location(const std::string &file, std::pair<size_t, size_t> start, std::pair<size_t, size_t> end)
        : file(file), start(start), end(end) {}

    const std::string file;
    const std::pair<size_t, size_t> start, end;
  };

  class ASTNode
  {
  public:
    ASTNode(LOC_ARG) : location(location) {}
    virtual ~ASTNode() = default;

    const Location location;

    virtual std::string str() = 0;

    virtual bool isExpr() { return false; }
    virtual bool isStmt() { return false; }
    virtual bool isType() { return false; }
  };

  class Expr : public ASTNode
  {
  public:
    Expr(LOC_ARG) : ASTNode(location) {}
    virtual bool isExpr() override { return true; }
  };

  class Stmt : public ASTNode
  {
  public:
    Stmt(LOC_ARG) : ASTNode(location) {}
    virtual bool isStmt() override { return true; }
  };

  class Type : public ASTNode
  {
  public:
    Type(LOC_ARG) : ASTNode(location) {}
    virtual bool isType() override { return true; }
  };
}