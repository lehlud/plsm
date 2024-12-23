#include "AST/Base.h"
#include "AST/Type/PrimitiveType.h"
#include "Analysis.h"

#include "AST/BaseASTVisitor.h"
#include "Errors.h"
#include "Utils.h"

#include <map>
#include <memory>
#include <stdexcept>

namespace plsm {

using namespace ast;

namespace {

static std::map<std::string, std::shared_ptr<PrimitiveType>> primitiveTypes = {
    {"i8", std::make_shared<PrimitiveType>("i8")},
    {"i16", std::make_shared<PrimitiveType>("i16")},
    {"i32", std::make_shared<PrimitiveType>("i32")},
    {"i64", std::make_shared<PrimitiveType>("i64")},
    {"u8", std::make_shared<PrimitiveType>("u8")},
    {"u16", std::make_shared<PrimitiveType>("u16")},
    {"u32", std::make_shared<PrimitiveType>("u32")},
    {"u64", std::make_shared<PrimitiveType>("u64")},
    {"float", std::make_shared<PrimitiveType>("float")},
    {"double", std::make_shared<PrimitiveType>("double")},
};

static std::shared_ptr<Type> resolveTypeName(const TypeName *typeName) {
  if (utils::is<NamedTypeName>(typeName)) {
    auto named = (NamedTypeName *)typeName;
    if (primitiveTypes.count(named->name))
      return primitiveTypes[named->name];

    errors::put(
        typeName->error("unable to resolve named type '" + named->name + "'"));
    return std::shared_ptr<Type>(nullptr);
  }

  // TODO: function type

  errors::put(typeName->error("unable to resolve type"));
  return std::shared_ptr<Type>(nullptr);
}

static void castTo(std::unique_ptr<Expr> &expr,
                   const std::shared_ptr<Type> &type) {
  auto cast = new CastExpr(expr->sourceRange, std::move(expr),
                           std::unique_ptr<TypeName>(type->toTypeName()));
  cast->type = type;
  cast->typeName->type = type;

  auto newExpr = std::unique_ptr<Expr>(cast);
  expr.swap(newExpr);
}

static bool tryAssignTo(std::unique_ptr<Expr> &from,
                        const std::shared_ptr<Type> &toType) {
  if (*from->type == *toType)
    return true;

  if (utils::is<PrimitiveType>(from->type.get()) &&
      utils::is<PrimitiveType>(toType.get())) {
    PrimitiveType *fromT = (PrimitiveType *)from->type.get();
    const PrimitiveType *toT = (PrimitiveType *)toType.get();

    std::map<std::string, std::vector<std::string>> castMatrix = {
        {"i8", {"i16", "i32", "i64", "u8", "u16", "u32", "u64"}},
        {"i16", {"i32", "i64", "u16", "u32", "u64"}},
        {"i32", {"i64", "u32", "u64"}},
        {"i64", {"u64"}},
        {"u8", {"i16", "i32", "i64", "u16", "u32", "u64"}},
        {"u16", {"i32", "i64", "u32", "u64"}},
        {"u32", {"i64", "u64"}},
        {"u64", {}},
        {"float", {"double"}},
        {"double", {}},
    };

    if (!castMatrix.count(fromT->name))
      return false;

    auto castableTo = castMatrix[fromT->name];

    for (auto &toName : castableTo) {
      if (toT->name == toName) {
        castTo(from, toType);
        return true;
      }
    }

    return false;
  }

  return false;
}

static bool canBeCastedTo(std::unique_ptr<Expr> &from,
                          const std::shared_ptr<Type> &toType) {
  if (*from->type == *toType)
    return true;

  if (utils::is<PrimitiveType>(from->type.get()) &&
      utils::is<PrimitiveType>(toType.get())) {
    PrimitiveType *fromT = (PrimitiveType *)from->type.get();
    const PrimitiveType *toT = (PrimitiveType *)toType.get();

    std::vector<std::string> allNumberTypes = {"i8",    "i16",   "i32", "i64",
                                               "u8",    "u16",   "u32", "u64",
                                               "float", "double"};
    std::map<std::string, std::vector<std::string>> castMatrix = {
        {"i8", allNumberTypes},    {"i16", allNumberTypes},
        {"i32", allNumberTypes},   {"i64", allNumberTypes},
        {"u8", allNumberTypes},    {"u16", allNumberTypes},
        {"u32", allNumberTypes},   {"u64", allNumberTypes},
        {"float", allNumberTypes}, {"double", allNumberTypes},
    };

    if (!castMatrix.count(fromT->name))
      return false;

    auto castableTo = castMatrix[fromT->name];

    for (auto &toName : castableTo) {
      if (toT->name == toName) {
        castTo(from, toType);
        return true;
      }
    }

    return false;
  }

  return false;
}

class TypeAnalysisVisitor1 : public BaseASTVisitor {
public:
  virtual std::any visit(NamedTypeName &namedTypeName,
                         std::any param) override {
    namedTypeName.type = resolveTypeName(&namedTypeName);
    return std::any();
  }

  virtual std::any visit(VarDecl &varDecl, std::any param) override {
    if (!varDecl.typeName.get() || !varDecl.symbol.get())
      return std::any();

    varDecl.typeName->accept(this, param);
    if (varDecl.typeName->type.get())
      varDecl.symbol->type = varDecl.typeName->type;

    return std::any();
  }

  virtual std::any visit(FnParam &fnParam, std::any param) override {
    if (!fnParam.typeName.get() || !fnParam.symbol.get())
      return std::any();

    fnParam.typeName->accept(this, param);
    if (fnParam.typeName->type.get())
      fnParam.symbol->type = fnParam.typeName->type;

    return std::any();
  }

  virtual std::any visit(FnDecl &fnDecl, std::any param) override {
    if (!fnDecl.symbol.get())
      return std::any();

    std::shared_ptr<Type> returnType(nullptr);
    if (fnDecl.returnTypeName.get()) {
      fnDecl.returnTypeName->accept(this, param);
      returnType = fnDecl.returnTypeName->type;
    }

    std::vector<std::shared_ptr<Type>> paramTypes;
    for (auto &p : fnDecl.params) {
      if (p.get())
        p->accept(this, param);

      if (p->symbol.get()) {
        paramTypes.push_back(p->symbol->type);
      } else {
        paramTypes.push_back(std::shared_ptr<Type>(nullptr));
      }
    }

    fnDecl.symbol->type =
        std::make_shared<FunctionType>(paramTypes, returnType);

    return std::any();
  }
};

class TypeAnalysisVisitor2 : public BaseASTVisitor {
  std::shared_ptr<Type> currentReturnType = nullptr;

public:
  virtual std::any visit(NamedTypeName &namedTypeName,
                         std::any param) override {
    namedTypeName.type = resolveTypeName(&namedTypeName);
    return std::any();
  }

  virtual std::any visit(VarDecl &varDecl, std::any param) override {
    if (!varDecl.typeName.get() || !varDecl.symbol.get())
      return std::any();

    varDecl.typeName->accept(this, param);
    if (varDecl.typeName->type.get())
      varDecl.symbol->type = varDecl.typeName->type;

    return std::any();
  }

  virtual std::any visit(FnParam &fnParam, std::any param) override {
    if (!fnParam.typeName.get() || !fnParam.symbol.get())
      return std::any();

    fnParam.typeName->accept(this, param);
    fnParam.symbol->type = resolveTypeName(fnParam.typeName.get());
    return std::any();
  }

  virtual std::any visit(FnDecl &fnDecl, std::any param) override {
    if (fnDecl.symbol.get() && fnDecl.symbol->type.get()) {
      auto functionType = (FunctionType *)fnDecl.symbol->type.get();
      currentReturnType = functionType->returnType;
    } else {
      currentReturnType = std::shared_ptr<Type>(nullptr);
    }

    if (fnDecl.body.get())
      fnDecl.body->accept(this, param);

    return std::any();
  }

  virtual std::any visit(Identifier &identifier, std::any param) override {
    if (!identifier.symbol.get())
      return std::any();

    identifier.type = identifier.symbol->type;
    return std::any();
  }

  virtual std::any visit(IntValue &intValue, std::any param) override {
    if ((int8_t)intValue.value == intValue.value) {
      intValue.type = std::make_shared<PrimitiveType>("i8");
    } else if ((uint8_t)intValue.value == intValue.value) {
      intValue.type = std::make_shared<PrimitiveType>("u8");
    } else if ((int16_t)intValue.value == intValue.value) {
      intValue.type = std::make_shared<PrimitiveType>("i16");
    } else if ((uint16_t)intValue.value == intValue.value) {
      intValue.type = std::make_shared<PrimitiveType>("u16");
    } else if ((int32_t)intValue.value == intValue.value) {
      intValue.type = std::make_shared<PrimitiveType>("i32");
    } else if ((uint32_t)intValue.value == intValue.value) {
      intValue.type = std::make_shared<PrimitiveType>("u32");
    } else {
      intValue.type = std::make_shared<PrimitiveType>("i64");
    }

    return std::any();
  }

  virtual std::any visit(FloatValue &floatValue, std::any param) override {
    if ((float_t)floatValue.value == floatValue.value) {
      floatValue.type = std::make_shared<PrimitiveType>("float");
    } else {
      floatValue.type = std::make_shared<PrimitiveType>("double");
    }

    return std::any();
  }

  virtual std::any visit(CastExpr &castExpr, std::any param) override {
    BaseASTVisitor::visit(castExpr, param);

    // do not multiplicate errors
    if (!castExpr.value.get() || !castExpr.value->type.get())
      return std::any();
    if (!castExpr.typeName.get() || !castExpr.typeName->type.get())
      return std::any();

    if (!canBeCastedTo(castExpr.value, castExpr.typeName->type)) {
      errors::put(castExpr.error("cast type mismatch"));
      return std::any();
    }

    castExpr.type = castExpr.typeName->type;

    return std::any();
  }

  virtual std::any visit(AssignStmt &assignStmt, std::any param) override {
    BaseASTVisitor::visit(assignStmt, param);

    // do not multiplicate errors
    if (!assignStmt.lval.get() || !assignStmt.lval->type.get())
      return std::any();
    if (!assignStmt.lval.get() || !assignStmt.rval->type.get())
      return std::any();

    if (!tryAssignTo(assignStmt.rval, assignStmt.lval->type)) {
      errors::put(assignStmt.error("assignment type mismatch"));
      return std::any();
    }

    return std::any();
  }

  virtual std::any visit(RetStmt &retStmt, std::any param) override {
    BaseASTVisitor::visit(retStmt, param);

    // do not multiplicate errors
    if (!retStmt.value.get() || !retStmt.value->type.get())
      return std::any();
    if (!currentReturnType.get())
      return std::any();

    if (!tryAssignTo(retStmt.value, currentReturnType)) {
      errors::put(retStmt.error("return type mismatch"));
      return std::any();
    }

    return std::any();
  }

  virtual std::any visit(IfStmt &ifStmt, std::any param) override {
    BaseASTVisitor::visit(ifStmt, param);

    // do not multiplicate errors
    if (ifStmt.condition.get() && ifStmt.condition->type.get()) {
      if (!utils::is<PrimitiveType>(ifStmt.condition->type.get())) {
        errors::put(ifStmt.error("condition must be of primitive type"));
      } else {
        // cast condition to int (to make sure jnz succeeds)
        castTo(ifStmt.condition, std::make_shared<PrimitiveType>("i32"));
      }
    }

    return std::any();
  }

  virtual std::any visit(WhileStmt &whileStmt, std::any param) override {
    BaseASTVisitor::visit(whileStmt, param);

    // do not multiplicate errors
    if (whileStmt.condition.get() && whileStmt.condition->type.get()) {
      if (!utils::is<PrimitiveType>(whileStmt.condition->type.get())) {
        errors::put(whileStmt.error("condition must be of primitive type"));
        return std::any();
      } else {
        // cast condition to int (to make sure jnz succeeds)
        castTo(whileStmt.condition, std::make_shared<PrimitiveType>("i32"));
      }
    }

    return std::any();
  }

  virtual std::any visit(CallExpr &callExpr, std::any param) override {
    BaseASTVisitor::visit(callExpr, param);

    // do not multiplicate errors
    if (!callExpr.callee.get() || !callExpr.callee->type.get())
      return std::any();

    if (!utils::is<FunctionType>(callExpr.callee->type.get())) {
      errors::put(callExpr.error("callee must be a function"));
      return std::any();
    }

    callExpr.type = callExpr.callee->type;

    auto functionType = (FunctionType *)(callExpr.callee->type.get());
    if (functionType->paramTypes.size() != callExpr.args.size()) {
      errors::put(callExpr.error("wrong number of arguments"));
      return std::any();
    }

    return std::any();
  }
};

} // namespace

void performTypeAnalysis(std::unique_ptr<Module> &module) {
  TypeAnalysisVisitor1 visitor1;
  TypeAnalysisVisitor2 visitor2;

  for (auto &stmt : module->stmts) {
    stmt->accept(&visitor1, std::any());
  }

  for (auto &stmt : module->stmts) {
    if (utils::is<FnDecl>(stmt.get()))
      stmt->accept(&visitor2, std::any());
  }
}

} // namespace plsm