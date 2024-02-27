#pragma once

#include <string>

#include "AST/Def.h"
#include "parser.gen.h"

class Parser
{
public:
  Parser() {}

  ast::Module *parse(const std::string &file, const std::stringstream &input);

  std::string file;
  yy::location location;
  ast::Module *module;
};
