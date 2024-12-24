#pragma once

#include "AST/AST.h"

namespace plsm {

void compileModule(std::unique_ptr<ast::Module> &module,
                   const std::string &filename);

} // namespace plsm
