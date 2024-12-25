#include "AST/Base.h"

namespace plsm {
namespace ast {

class InlineAsmConstraint : public ASTNode {
public:
  std::string constraint;
  std::unique_ptr<Expr> value;

  InlineAsmConstraint(LOC_ARG, std::string constraint, std::unique_ptr<Expr> value)
      : ASTNode(sourceRange), constraint(constraint), value(std::move(value)) {}

  virtual boost::json::value toJson() const override;
  static InlineAsmConstraint *fromJson(boost::json::value json);

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

class InlineAsm : public Stmt {
public:
  std::string code;
  std::vector<std::unique_ptr<InlineAsmConstraint>> outputs;
  std::vector<std::unique_ptr<InlineAsmConstraint>> inputs;
  std::vector<std::string> clobbers;

  InlineAsm(LOC_ARG, std::string code, std::vector<std::unique_ptr<InlineAsmConstraint>> outputs,
            std::vector<std::unique_ptr<InlineAsmConstraint>> inputs, std::vector<std::string> clobbers)
      : Stmt(sourceRange), code(code), outputs(std::move(outputs)), inputs(std::move(inputs)),
        clobbers(std::move(clobbers)) {}

  virtual boost::json::value toJson() const override;
  static InlineAsm *fromJson(boost::json::value json);

  virtual bool alywasReturns() const override { return false; }

  virtual std::any accept(ASTVisitor *visitor, std::any param) override {
    return visitor->visit(*this, param);
  }
};

} // namespace ast
} // namespace plsm