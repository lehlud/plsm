#pragma once

#include <string>
#include <vector>

namespace plsm {
namespace errors {

void put(const std::string &msg);
std::vector<std::string> get();

} // namespace errors
} // namespace plsm
