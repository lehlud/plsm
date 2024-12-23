#include "Parser.h"

#include "AST/AST.h"
#include "Errors.h"
#include "plsmBaseVisitor.h"
#include "plsmLexer.h"
#include "plsmParser.h"

namespace plsm {

namespace {
class MyAntlr4ErrorListener : public antlr4::BaseErrorListener {
  std::string file;

public:
  MyAntlr4ErrorListener(const std::string &file) : file(file) {}

  virtual void syntaxError(antlr4::Recognizer *recognizer,
                           antlr4::Token *offendingSymbol, size_t line,
                           size_t charPositionInLine, const std::string &msg,
                           std::exception_ptr e) override {

    std::stringstream ss;
    ss << "In file " << file << ":" << line << ":" << charPositionInLine << "\n"
       << terminal::red << msg << terminal::reset;

    errors::put(ss.str());
  }
};
} // namespace

std::unique_ptr<ast::Module> parse(const std::string &file,
                                   const std::string &input) {
  auto istream = antlr4::ANTLRInputStream(input);
  auto lexer = plsmLexer(&istream);
  auto tokens = antlr4::CommonTokenStream(&lexer);
  auto parser = plsmParser(&tokens);
  parser.setFileName(file);
  parser.setSourceText(input);

  MyAntlr4ErrorListener listener(file);
  parser.removeErrorListeners();
  parser.addErrorListener(&listener);

  auto tree = parser.module();
  return std::move(tree->ast);
}

} // namespace plsm
