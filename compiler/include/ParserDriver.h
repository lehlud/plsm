#pragma once

#include <string>

#include "AST/Def.h"
#include "parser.gen.h"

class ParserDriver
{
public:
  ParserDriver() {}
  ~ParserDriver() { delete module; }

  int parse(const std::string &input);

  void startLexer();
  void stopLexer();

  std::string file;
  yy::location location;
  ast::Module *module;
};

#define YY_DECL yy::parser::symbol_type yylex(ParserDriver &driver)
YY_DECL;
