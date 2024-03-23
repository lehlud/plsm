#pragma once

#include <string>
#include <stdexcept>

#include "AST/Def.h"

namespace plsm
{
  std::unique_ptr<ast::Module> parse(const std::string &file, const std::string &input);
}

