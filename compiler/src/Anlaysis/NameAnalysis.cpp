#include "AST/BaseASTVisitor.h"
#include "Analysis.h"

#include <map>
#include <stack>

namespace plsm {

namespace {

class NameAnalysisVisitor1 : public ast::BaseASTVisitor {
  std::stack<std::map<std::string, std::shared_ptr<ast::Symbol>>> *scopes;

public:
  NameAnalysisVisitor1(
      std::stack<std::map<std::string, std::shared_ptr<ast::Symbol>>> *scopes)
      : scopes(scopes) {}

  virtual std::any visit(ast::FnDecl &fnDecl, std::any param) override {
    auto symbol = std::make_shared<ast::Symbol>(fnDecl.name);
    fnDecl.symbol = symbol;
    scopes->top()[fnDecl.name] = symbol;

    return std::any();
  }

  virtual std::any visit(ast::VarDecl &varDecl, std::any param) override {
    auto symbol = std::make_shared<ast::Symbol>(varDecl.name);
    varDecl.symbol = symbol;
    scopes->top()[varDecl.name] = symbol;

    return std::any();
  }
};

class NameAnalysisVisitor2 : public ast::BaseASTVisitor {
  std::stack<std::map<std::string, std::shared_ptr<ast::Symbol>>> *scopes;

  void push() {
    scopes->push(std::map<std::string, std::shared_ptr<ast::Symbol>>());
  }

  void pop() { scopes->pop(); }

public:
  NameAnalysisVisitor2(
      std::stack<std::map<std::string, std::shared_ptr<ast::Symbol>>> *scopes)
      : scopes(scopes) {}
};

} // namespace

void performNameAnalysis(std::shared_ptr<ast::Module> module) {
  std::stack<std::map<std::string, std::shared_ptr<ast::Symbol>>> scopes;
  scopes.push(std::map<std::string, std::shared_ptr<ast::Symbol>>());

  NameAnalysisVisitor1 visitor1(&scopes);
  NameAnalysisVisitor2 visitor2(&scopes);

  for (auto &stmt : module->stmts) {
    stmt->accept(&visitor1, nullptr);
  }

  for (auto &stmt : module->stmts) {
    stmt->accept(&visitor2, nullptr);
  }
}

} // namespace plsm