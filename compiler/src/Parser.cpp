#include "Parser.h"

#include "AST/AST.h"
#include "plsmBaseVisitor.h"
#include "plsmLexer.h"
#include "plsmParser.h"

namespace {
class MyAntlr4ErrorListener : public antlr4::BaseErrorListener {
  std::string file;
  std::string *error;

public:
  MyAntlr4ErrorListener(const std::string &file, std::string *error)
      : file(file), error(error) {}

  virtual void syntaxError(antlr4::Recognizer *recognizer,
                           antlr4::Token *offendingSymbol, size_t line,
                           size_t charPositionInLine, const std::string &msg,
                           std::exception_ptr e) override {
    std::stringstream ss;
    ss << file << ": line " << line << ":" << charPositionInLine << ": " << msg;
    *error = ss.str();
  }
};
} // namespace

namespace plsm {

std::shared_ptr<ast::Module> parse(const std::string &file,
                                   const std::string &input) {
  auto istream = antlr4::ANTLRInputStream(input);
  auto lexer = plsmLexer(&istream);
  auto tokens = antlr4::CommonTokenStream(&lexer);
  auto parser = plsmParser(&tokens);

  std::string error;
  MyAntlr4ErrorListener listener(file, &error);
  parser.removeErrorListeners();
  parser.addErrorListener(&listener);

  auto tree = parser.module();
  return std::shared_ptr<Module>(tree->ast);
}

} // namespace plsm
