#pragma once

#include "AST/AST.h"

namespace plsm {

void performNameAnalysis(std::shared_ptr<ast::Module> module);

}