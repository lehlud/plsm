#include "Errors.h"

static std::vector<std::string> _errors;

void plsm::errors::put(const std::string &error) { _errors.push_back(error); }

std::vector<std::string> plsm::errors::get() { return std::vector(_errors); }
