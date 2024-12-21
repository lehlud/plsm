#pragma once

#include <stdexcept>
#include <string>

#include "AST/AST.h"

namespace plsm {
std::shared_ptr<ast::Module> parse(const std::string &file,
                                   const std::string &input);
}
