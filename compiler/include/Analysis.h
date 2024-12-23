#pragma once

#include "AST/AST.h"

namespace plsm {

void performNameAnalysis(std::unique_ptr<ast::Module> &module);
void performTypeAnalysis(std::unique_ptr<ast::Module> &module);

} // namespace plsm
