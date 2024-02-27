#include "Parser.h"

#include <sstream>
#include <fstream>

void yy_scan_string(const char *);

ast::Module *Parser::parse(const std::string &file, const std::stringstream &input)
{
  module = nullptr;

  this->file = file;
  location.initialize(&file);

  yy_scan_string(input.str().data());

  int res = yy::parser(*this)();

  return module;
}
