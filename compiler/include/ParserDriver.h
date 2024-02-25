#pragma once

#include <string>

#include "AST.h"
#include "parser.gen.h"

class ParserDriver
{
public:
  ParserDriver() {}

  int parse(const std::string &input);

  void startLexer();
  void stopLexer();

  std::string file;
  yy::location location;
  Module *module;
};

#define YY_DECL yy::parser::symbol_type yylex(ParserDriver &driver)
YY_DECL;
