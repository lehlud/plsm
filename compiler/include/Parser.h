#pragma once

#include <stdexcept>
#include <string>

#include "AST/AST.h"

namespace plsm {
std::unique_ptr<ast::Module> parse(const std::string &file,
                                   const std::string &input);
}
