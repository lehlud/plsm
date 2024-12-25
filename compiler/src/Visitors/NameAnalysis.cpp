#include "AST/BaseASTVisitor.h"
#include "Analysis.h"
#include "Errors.h"
#include "Utils.h"

#include <map>
#include <vector>

namespace plsm {
namespace {

class NameAnalysisVisitor1 : public ast::BaseASTVisitor {
  std::vector<std::map<std::string, std::shared_ptr<ast::Symbol>>> *scopes;

public:
  NameAnalysisVisitor1(std::vector<std::map<std::string, std::shared_ptr<ast::Symbol>>> *scopes)
      : scopes(scopes) {}

  virtual std::any visit(ast::FnDecl &fnDecl, std::any param) override {
    if (!fnDecl.name.size())
      return std::any();

    if (scopes->back().count(fnDecl.name)) {
      errors::put(fnDecl.error("redeclaration of global symbol '" + fnDecl.name + "'"));
      return std::any();
    }

    auto symbol = std::make_shared<ast::Symbol>(fnDecl.name);
    fnDecl.symbol = symbol;
    scopes->back()[fnDecl.name] = symbol;

    return std::any();
  }

  virtual std::any visit(ast::VarDecl &varDecl, std::any param) override {
    if (!varDecl.name.size())
      return std::any();

    if (scopes->back().count(varDecl.name)) {
      errors::put(varDecl.error("redeclaration of global symbol '" + varDecl.name + "'"));
      return std::any();
    }

    auto symbol = std::make_shared<ast::Symbol>(varDecl.name);
    varDecl.symbol = symbol;
    scopes->back()[varDecl.name] = symbol;

    return std::any();
  }
};

class NameAnalysisVisitor2 : public ast::BaseASTVisitor {
  std::vector<std::map<std::string, std::shared_ptr<ast::Symbol>>> *scopes;

  void push() { scopes->push_back(std::map<std::string, std::shared_ptr<ast::Symbol>>()); }

  void pop() { scopes->pop_back(); }

  std::shared_ptr<ast::Symbol> findSymbol(std::string name) {
    for (long i = scopes->size() - 1; i >= 0; i--) {
      auto scope = scopes->at(i);
      if (scope.count(name))
        return scope[name];
    }

    return std::shared_ptr<ast::Symbol>();
  }

public:
  NameAnalysisVisitor2(std::vector<std::map<std::string, std::shared_ptr<ast::Symbol>>> *scopes)
      : scopes(scopes) {}

  virtual std::any visit(ast::FnParam &fnParam, std::any param) override {
    if (!fnParam.name.size())
      return std::any();

    auto symbol = std::make_shared<ast::Symbol>(fnParam.name);
    fnParam.symbol = symbol;
    scopes->back()[fnParam.name] = symbol;

    return std::any();
  }

  virtual std::any visit(ast::FnDecl &fnDecl, std::any param) override {
    push();
    BaseASTVisitor::visit(fnDecl, param);
    pop();

    return std::any();
  }

  virtual std::any visit(ast::Block &block, std::any param) override {
    push();
    BaseASTVisitor::visit(block, param);
    pop();

    return std::any();
  }

  virtual std::any visit(ast::LambdaExpr &lambdaExpr, std::any param) override {
    push();
    BaseASTVisitor::visit(lambdaExpr, param);
    pop();

    return std::any();
  }

  virtual std::any visit(ast::Identifier &identifier, std::any param) override {
    if (!identifier.name.size())
      return std::any();

    auto symbol = findSymbol(identifier.name);

    if (!symbol.get()) {
      errors::put(identifier.error("unable to resolve identifier '" + identifier.name + "'"));
      return std::any();
    }

    identifier.symbol = symbol;

    return std::any();
  }

  virtual std::any visit(ast::VarDecl &varDecl, std::any param) override {
    if (!varDecl.name.size())
      return std::any();

    auto symbol = std::make_shared<ast::Symbol>(varDecl.name);
    varDecl.symbol = symbol;
    scopes->back()[varDecl.name] = symbol;

    return std::any();
  }
};

} // namespace

void performNameAnalysis(std::unique_ptr<ast::Module> &module) {
  std::vector<std::map<std::string, std::shared_ptr<ast::Symbol>>> scopes;
  scopes.push_back(std::map<std::string, std::shared_ptr<ast::Symbol>>());

  NameAnalysisVisitor1 visitor1(&scopes);
  NameAnalysisVisitor2 visitor2(&scopes);

  module->accept(&visitor1, nullptr);

  for (auto &stmt : module->stmts) {
    if (utils::is<ast::FnDecl>(stmt.get()))
      stmt->accept(&visitor2, nullptr);
  }
}

} // namespace plsm