#pragma once

#include <string>
#include <stdexcept>

#include "AST/Def.h"
#include "plsmLexer.h"
#include "plsmParser.h"

namespace plsm
{
  std::unique_ptr<ast::Module> parse(const std::string &input);
}

namespace adscript
{
  std::unique_ptr<ast::Module> parse(const std::string &input);
}
