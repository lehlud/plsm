#include "Parser.h"

#include <iostream>
#include <fstream>
#include <sstream>

static std::stringstream readFile(const std::string &path)
{
  std::ifstream t(path);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage:" << std::endl
              << "\t" << argv[0] << " <path/to/file>" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto module = Parser().parse(argv[1], readFile(argv[1]));

  auto fn = (ast::FnDecl *)module->stmts.at(0);
  std::cout << fn->name << std::endl;

  delete module;
}
