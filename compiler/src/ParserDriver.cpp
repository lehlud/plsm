#include "ParserDriver.h"

int ParserDriver::parse(const std::string &input)
{
  file = input;
  location.initialize(&file);

  startLexer();
  yy::parser parse(*this);
  int res = parse();
  stopLexer();

  return res;
}
