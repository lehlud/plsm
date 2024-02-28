#include "Parser.h"

#include "plsmBaseVisitor.h"

#include <sstream>

class Visitor : public plsmBaseVisitor
{
public:
  Visitor() {}

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

  virtual std::any visitModule(plsmParser::ModuleContext *ctx) override
  {
    std::vector<std::unique_ptr<ast::Import>> imports;
    std::vector<std::unique_ptr<ast::Stmt>> stmts;

    for (auto &child : ctx->children)
    {
      if (plsmParser::ModuleImportContext *c = dynamic_cast<plsmParser::ModuleImportContext *>(child))
        imports.push_back(visitU<ast::Import>(child));
      else
        stmts.push_back(visitU<ast::Stmt>(child));
    }

    return std::any(new ast::Module(imports, stmts));
  }

  virtual std::any visitModuleImport(plsmParser::ModuleImportContext *ctx) override
  {
    return std::any(new ast::Import(visitT<std::string>(ctx->moduleName())));
  }

  virtual std::any visitModuleName(plsmParser::ModuleNameContext *ctx) override
  {
    auto name = visitT<std::string>(ctx->identifier(0));

    for (size_t i = 1; ctx->identifier(i); i++)
    {
      name = name + "." + visitT<std::string>(ctx->identifier(i));
    }

    return std::any(name);
  }

  virtual std::any visitTraitDef(plsmParser::TraitDefContext *ctx) override
  {
    auto name = visitT<std::string>(ctx->identifier(0));

    if (ctx->identifier(1))
    {
      auto aliased = visitT<std::string>(ctx->identifier(1));
      return std::any(new ast::TraitAlias(name, aliased));
    }

    std::vector<std::unique_ptr<ast::FnDecl>> fns;

    for (size_t i = 0; ctx->fnDecl(i); i++)
      fns.push_back(visitU<ast::FnDecl>(ctx->fnDecl(i)));

    return std::any(new ast::TraitDef(name, fns));
  }

  virtual std::any visitTypeDef(plsmParser::TypeDefContext *ctx) override
  {
    auto name = visitT<std::string>(ctx->identifier());

    if (ctx->type())
    {
      auto gen = visitU<ast::Type>(ctx->type());
      return std::any(new ast::TypeAlias(name, gen));
    }

    std::vector<int> args;
  }
};

class ErrorListener : public antlr4::BaseErrorListener
{
  std::string *error;

public:
  ErrorListener(std::string *error) : error(error) {}

  virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line, size_t charPositionInLine,
                           const std::string &msg, std::exception_ptr e) override
  {
    std::stringstream ss;
    ss << "line " << line << ":" << charPositionInLine << ": " << msg;
    *error = ss.str();
  }
};

std::unique_ptr<ast::Module>
plsm::parse(const std::string &input)
{
  auto istream = antlr4::ANTLRInputStream(input);
  auto lexer = plsmLexer(&istream);
  auto tokens = antlr4::CommonTokenStream(&lexer);
  auto parser = plsmParser(&tokens);

  std::string error;
  ErrorListener listener(&error);
  parser.removeErrorListeners();
  parser.addErrorListener(&listener);

  auto tree = parser.module();

  if (error.size())
    throw std::runtime_error(error);

  auto module = std::any_cast<ast::Module *>(Visitor().visitModule(tree));
  return std::unique_ptr<ast::Module>(module);
}

std::unique_ptr<ast::Module>
adscript::parse(const std::string &input)
{
  throw std::logic_error("adscript::parse not implemented");
}
