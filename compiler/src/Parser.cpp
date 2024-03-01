#include "Parser.h"

#include "plsmBaseVisitor.h"

#include <sstream>

class ErrorListener : public antlr4::BaseErrorListener
{
  std::string file;
  std::string *error;

public:
  ErrorListener(const std::string &file, std::string *error)
      : file(file), error(error) {}

  virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine,
                           const std::string &msg, std::exception_ptr e) override
  {
    std::stringstream ss;
    ss << file << ": line " << line << ":" << charPositionInLine << ": " << msg;
    *error = ss.str();
  }
};

namespace plsm
{
  class Visitor : public plsmBaseVisitor
  {
  private:
    const std::string file;

  public:
    Visitor(const std::string &file) : file(file) {}

    inline ast::Location loc(antlr4::ParserRuleContext *ctx)
    {
      return ast::Location(
          file,
          std::pair<size_t, size_t>(
              ctx->getStart()->getLine(),
              ctx->getStart()->getCharPositionInLine()),
          std::pair<size_t, size_t>(
              ctx->getStop()->getLine(),
              ctx->getStop()->getCharPositionInLine()));
    }

    template <typename T>
    inline T visitT(antlr4::tree::ParseTree *ctx)
    {
      return std::any_cast<T>(visit(ctx));
    }

    template <typename T>
    inline std::unique_ptr<T> visitU(antlr4::tree::ParseTree *ctx)
    {
      return std::unique_ptr<T>(visitT<T *>(ctx));
    }

    template <typename T>
    inline std::vector<std::unique_ptr<T>> visitVU(antlr4::tree::ParseTree *ctx)
    {
      std::vector<std::unique_ptr<T>> res;
      for (auto &el : visitT<std::vector<T *>>(ctx))
        res.push_back(std::unique_ptr<T>(el));
      return res;
    }

    virtual std::any visitModule(plsmParser::ModuleContext *ctx) override
    {
      auto name = ctx->identifier()->getText();

      std::vector<std::unique_ptr<ast::Import>> imports;
      for (auto &import : ctx->moduleImport())
        imports.push_back(visitU<ast::Import>(import));

      std::vector<std::unique_ptr<ast::Stmt>> stmts;
      for (auto &stmt : ctx->moduleStmt())
        stmts.push_back(visitU<ast::Stmt>(stmt));

      return std::any(new ast::Module(loc(ctx), name, imports, stmts));
    }

    virtual std::any visitModuleImport(plsmParser::ModuleImportContext *ctx) override
    {
      return std::any(new ast::Import(loc(ctx), visitT<std::string>(ctx->importName())));
    }

    virtual std::any visitImportName(plsmParser::ImportNameContext *ctx) override
    {
      auto name = ctx->identifier(0)->getText();

      for (size_t i = 1; i < ctx->identifier().size(); i++)
        name = name + "/" + ctx->identifier(i)->getText();

      return std::any(name);
    }

    virtual std::any visitModuleStmt(plsmParser::ModuleStmtContext *ctx) override
    {
      ast::Stmt *stmt;

      if (ctx->let())
        stmt = (ast::Stmt *)visitT<ast::ValDecl *>(ctx->let());
      if (ctx->fnDecl())
        stmt = (ast::Stmt *)visitT<ast::FnDef *>(ctx->fnDecl());
      if (ctx->fnDef())
        stmt = (ast::Stmt *)visitT<ast::FnDef *>(ctx->fnDef());
      if (ctx->traitDef())
        stmt = (ast::Stmt *)visitT<ast::Stmt *>(ctx->traitDef());
      if (ctx->typeDef())
        stmt = (ast::Stmt *)visitT<ast::Stmt *>(ctx->typeDef());

      return std::any(stmt);
    }

    virtual std::any visitTraitDef(plsmParser::TraitDefContext *ctx) override
    {
      auto name = ctx->identifier(0)->getText();

      if (ctx->identifier(1))
      {
        auto aliased = ctx->identifier(1)->getText();
        return std::any((ast::Stmt *)new ast::TraitAlias(loc(ctx), name, aliased));
      }

      std::vector<std::unique_ptr<ast::FnDef>> fns;
      for (auto &decl : ctx->fnDecl())
        fns.push_back(visitU<ast::FnDef>(decl));

      return std::any((ast::Stmt *)new ast::TraitDef(loc(ctx), name, fns));
    }

    virtual std::any visitTypeDef(plsmParser::TypeDefContext *ctx) override
    {
      auto name = ctx->identifier()->getText();

      if (ctx->type())
      {
        auto gen = visitU<ast::Type>(ctx->type());
        return std::any((ast::Stmt *)new ast::TypeAlias(loc(ctx), name, gen));
      }

      std::vector<std::unique_ptr<ast::FnArg>> attrs;
      if (ctx->fnDefArgs())
        attrs = visitVU<ast::FnArg>(ctx->fnDefArgs());

      std::vector<std::unique_ptr<ast::FnDef>> members;
      for (auto &def : ctx->fnDef())
        members.push_back(visitU<ast::FnDef>(def));
      for (auto &decl : ctx->fnDecl())
        members.push_back(visitU<ast::FnDef>(decl));

      return std::any((ast::Stmt *)new ast::TypeDef(loc(ctx), name, attrs, members));
    }

    virtual std::any visitFnDef(plsmParser::FnDefContext *ctx) override
    {
      auto name = ctx->identifier()->getText();

      std::vector<std::unique_ptr<ast::FnArg>> args;
      if (ctx->fnDefArgs())
        args = std::move(visitVU<ast::FnArg>(ctx->fnDefArgs()));

      auto returnType = visitU<ast::Type>(ctx->type());
      auto body = visitU<ast::Expr>(ctx->expr());

      return std::any(new ast::FnDef(loc(ctx), name, args, returnType, body));
    }

    virtual std::any visitFnDefArgs(plsmParser::FnDefArgsContext *ctx) override
    {
      std::vector<ast::FnArg *> args;

      for (size_t i = 0; i < ctx->children.size(); i += 3)
      {
        auto name = ctx->children[i]->getText();
        auto type = visitU<ast::Type>(ctx->children[i + 1]);
        args.push_back(new ast::FnArg(loc(ctx), name, type));
      }

      return std::any(args);
    }

    virtual std::any visitFnDecl(plsmParser::FnDeclContext *ctx) override
    {
      auto name = ctx->identifier()->getText();

      std::vector<std::unique_ptr<ast::FnArg>> args;
      if (ctx->fnDeclArgs())
        args = visitVU<ast::FnArg>(ctx->fnDeclArgs());

      auto returnType = visitU<ast::Type>(ctx->type());
      auto body = std::unique_ptr<ast::Expr>(nullptr);

      return std::any(new ast::FnDef(loc(ctx), name, args, returnType, body));
    }

    virtual std::any visitFnDeclArgs(plsmParser::FnDeclArgsContext *ctx) override
    {
      std::vector<ast::FnArg *> args;

      for (size_t i = 0; i < ctx->children.size(); i++)
      {
        std::string name;
        if (dynamic_cast<plsmParser::IdentifierContext *>(ctx->children[i]))
          name = ctx->children[i++]->getText();

        auto type = visitU<ast::Type>(ctx->children[i++]);

        args.push_back(new ast::FnArg(loc(ctx), name, type));
      }

      return std::any(args);
    }

    virtual std::any visitLet(plsmParser::LetContext *ctx) override
    {
      auto name = ctx->identifier()->getText();

      std::unique_ptr<ast::Type> type(nullptr);
      if (ctx->type())
        type = visitU<ast::Type>(ctx->type());

      auto expr = visitU<ast::Expr>(ctx->expr());

      return std::any(new ast::ValDecl(loc(ctx), name, type, expr));
    }

    virtual std::any visitExprStmt(plsmParser::ExprStmtContext *ctx) override
    {
      auto expr = visitU<ast::Expr>(ctx->expr());
      return std::any(new ast::ExprStmt(loc(ctx), expr));
    }

    virtual std::any visitType1(plsmParser::Type1Context *ctx) override
    {
      if (ctx->type0())
        return visit(ctx->type0());

      if (ctx->children[0]->getText() == "(")
      {
        if (ctx->type())
          return visit(ctx->type());

        if (ctx->tupleTypeList())
        {
          auto types = visitVU<ast::Type>(ctx->tupleTypeList());
          return std::any((ast::Type *)new ast::TupleType(loc(ctx), types));
        }
      }

      if (ctx->children[0]->getText() == "[")
      {
        std::vector<std::unique_ptr<ast::Type>> args;
        if (ctx->typeList())
          args = visitVU<ast::Type>(ctx->typeList());

        auto returnType = visitU<ast::Type>(ctx->type());

        return std::any((ast::Type *)new ast::ClosureType(loc(ctx), args, returnType));
      }

      throw std::logic_error("type2 not implemented");
    }

    virtual std::any visitType0(plsmParser::Type0Context *ctx) override
    {
      auto name = ctx->identifier()->getText();

      if (ctx->typeList())
      {
        auto types = visitVU<ast::Type>(ctx->typeList());
        return std::any((ast::Type *)new ast::GenericType(loc(ctx), name, types));
      }

      return std::any((ast::Type *)new ast::NamedType(loc(ctx), name));
    }

    virtual std::any visitTupleTypeList(plsmParser::TupleTypeListContext *ctx) override
    {
      std::vector<ast::Type *> types;
      for (auto &type : ctx->type())
        types.push_back(visitT<ast::Type *>(type));

      return std::any(types);
    }

    virtual std::any visitTypeList(plsmParser::TypeListContext *ctx) override
    {
      std::vector<ast::Type *> types;
      for (auto &type : ctx->type())
        types.push_back(visitT<ast::Type *>(type));

      return std::any(types);
    }

    virtual std::any visitExpr3(plsmParser::Expr3Context *ctx) override
    {
      if (ctx->expr2())
        return visit(ctx->expr2());

      // closure
      if (ctx->children[0]->getText() == "[")
      {
        std::vector<std::string> args;
        if (ctx->identifierList())
          args = visitT<std::vector<std::string>>(ctx->identifierList());

        auto body = visitU<ast::Expr>(ctx->expr());

        return std::any((ast::Expr *)new ast::Closure(loc(ctx), args, body));
      }

      // block
      if (ctx->children[0]->getText() == "{")
      {
        std::vector<std::unique_ptr<ast::Stmt>> stmts;
        for (auto &stmt : ctx->blockStmt())
          stmts.push_back(visitU<ast::Stmt>(stmt));

        auto result = visitU<ast::Expr>(ctx->expr());

        return std::any((ast::Expr *)new ast::BlockExpr(loc(ctx), stmts, result));
      }

      throw std::logic_error("expr3 not implemented");
    }

    virtual std::any visitExpr2(plsmParser::Expr2Context *ctx) override
    {
      if (ctx->expr2())
      {
        auto left = visitU<ast::Expr>(ctx->expr2());
        auto op = ctx->operator_()->getText();
        auto right = visitU<ast::Expr>(ctx->expr1());
        return std::any((ast::Expr *)new ast::BinExpr(loc(ctx), left, op, right));
      }

      return visit(ctx->expr1());
    }

    virtual std::any visitExpr1(plsmParser::Expr1Context *ctx) override
    {
      if (ctx->operator_())
      {
        auto op = ctx->operator_()->getText();
        auto expr = visitU<ast::Expr>(ctx->expr0());
        return std::any((ast::Expr *)new ast::UnaryExpr(loc(ctx), op, expr));
      }

      return visit(ctx->expr0());
    }

    virtual std::any visitExpr0(plsmParser::Expr0Context *ctx) override
    {
      if (ctx->literal())
        return visit(ctx->literal());

      if (ctx->expr())
      {
        auto expr = visitU<ast::Expr>(ctx->expr());
        return std::any((ast::Expr *)new ast::PrefExpr(loc(ctx), expr));
      }

      if (ctx->expr0())
      {
        auto expr = visitU<ast::Expr>(ctx->expr0());

        // property accessor
        if (ctx->identifier())
        {
          auto name = ctx->identifier()->getText();
          return std::any((ast::Expr *)new ast::PropExpr(loc(ctx), expr, name));
        }

        // function/closure call
        std::vector<std::unique_ptr<ast::Expr>> args;
        if (ctx->exprList())
          args = visitVU<ast::Expr>(ctx->exprList());

        return std::any((ast::Expr *)new ast::CallExpr(loc(ctx), expr, args));
      }

      // tuple
      if (ctx->tupleExprList())
      {
        auto values = visitVU<ast::Expr>(ctx->tupleExprList());
        return std::any((ast::Expr *)new ast::Tuple(loc(ctx), values));
      }

      // identifier
      if (ctx->identifier())
      {
        auto name = ctx->identifier()->getText();
        return std::any((ast::Expr *)new ast::Identifier(loc(ctx), name));
      }

      throw std::logic_error("expr0 not implemented");
    }

    virtual std::any visitBlockStmt(plsmParser::BlockStmtContext *ctx) override
    {
      ast::Stmt *stmt;

      if (ctx->let())
        stmt = (ast::Stmt *)visitT<ast::ValDecl *>(ctx->let());
      if (ctx->exprStmt())
        stmt = (ast::Stmt *)visitT<ast::ExprStmt *>(ctx->exprStmt());
      if (ctx->fnDef())
        stmt = (ast::Stmt *)visitT<ast::FnDef *>(ctx->fnDef());

      return std::any(stmt);
    }

    virtual std::any visitTupleExprList(plsmParser::TupleExprListContext *ctx) override
    {
      std::vector<ast::Expr *> exprs;
      for (auto &expr : ctx->expr())
        exprs.push_back(visitT<ast::Expr *>(expr));

      return std::any(exprs);
    }

    virtual std::any visitExprList(plsmParser::ExprListContext *ctx) override
    {
      std::vector<ast::Expr *> exprs;
      for (auto &expr : ctx->expr())
        exprs.push_back(visitT<ast::Expr *>(expr));

      return std::any(exprs);
    }

    virtual std::any visitIdentifierList(plsmParser::IdentifierListContext *ctx) override
    {
      std::vector<std::string> identifiers;
      for (auto &identifier : ctx->identifier())
        identifiers.push_back(visitT<std::string>(identifier));

      return std::any(identifiers);
    }

    virtual std::any visitLiteral(plsmParser::LiteralContext *ctx) override
    {
      if (ctx->NULL_)
        return std::any((ast::Expr *)new ast::NullValue(loc(ctx)));

      if (ctx->INT_LIT())
      {
        auto text = ctx->INT_LIT()->getText();

        int64_t value;

        if (!text.rfind("0x", 0))
          value = std::strtol(text.substr(2).data(), NULL, 16);

        if (!text.rfind("0o", 0))
          value = std::strtol(text.substr(2).data(), NULL, 8);

        if (!text.rfind("0b", 0))
          value = std::strtol(text.substr(2).data(), NULL, 2);

        return std::any((ast::Expr *)new ast::IntValue(loc(ctx), value));
      }

      if (ctx->FLOAT_LIT())
      {
        auto text = ctx->FLOAT_LIT()->getText();
        if (text.at(text.size() - 1) == '.')
          text += "0";

        double value = std::strtod(text.data(), NULL);
        return std::any((ast::Expr *)new ast::FloatValue(loc(ctx), value));
      }

      throw std::logic_error("literal not implemented");
    }
  };

  std::unique_ptr<ast::Module> parse(const std::string &file, const std::string &input)
  {
    auto istream = antlr4::ANTLRInputStream(input);
    auto lexer = plsmLexer(&istream);
    auto tokens = antlr4::CommonTokenStream(&lexer);
    auto parser = plsmParser(&tokens);

    std::string error;
    ErrorListener listener(file, &error);
    parser.removeErrorListeners();
    parser.addErrorListener(&listener);

    auto tree = parser.module();

    if (error.size())
      throw std::runtime_error(error);

    auto module = std::any_cast<ast::Module *>(Visitor(file).visitModule(tree));
    return std::unique_ptr<ast::Module>(module);
  }
}

std::unique_ptr<ast::Module>
adscript::parse(const std::string &file, const std::string &input)
{
  throw std::logic_error("adscript::parse not implemented");
}
