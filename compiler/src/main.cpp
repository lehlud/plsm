#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

#include "Parser.h"

static std::string readFile(const std::string &path)
{
  std::ifstream f(path);
  auto data = (std::stringstream() << f.rdbuf()).str();
  f.close();

  return data;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage:" << std::endl
              << "\t" << argv[0] << " <path/to/file>" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto input = readFile(argv[1]);

  std::cout << input << std::endl;

  try
  {
    auto module = plsm::parse(argv[1], input);
    std::cout << module->toJsonString() << std::endl;

    std::cout << "\n\n";

    std::cout << plsm::ast::Module::fromJson(module->toJson())->toJson() << std::endl;
  }
  catch (std::runtime_error &err)
  {
    std::cerr << err.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  // auto module = Parser().parse(argv[1], readFile(argv[1]));

  // auto fn = (ast::FnDef *)module->stmts.at(0);
  // std::cout << fn->name << std::endl;

  // delete module;
}
