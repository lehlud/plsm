#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>

#include "Analysis.h"
#include "Errors.h"
#include "Parser.h"

static std::string readFile(const std::string &path) {
  std::ifstream f(path);
  auto data = (std::stringstream() << f.rdbuf()).str();
  f.close();

  return data;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage:" << std::endl
              << "\t" << argv[0] << " <path/to/file>" << std::endl;
    exit(EXIT_FAILURE);
  }

  auto input = readFile(argv[1]);

  // std::cout << input << std::endl;

  int exitStatus = EXIT_SUCCESS;

  try {
    auto module = plsm::parse(argv[1], input);
    // std::cout << module->toJsonString() << std::endl;

    plsm::performNameAnalysis(module);
    plsm::performTypeAnalysis(module);

    // std::cout << "\n\n";

    // std::cout << plsm::ast::Module::fromJson(module->toJson())->toJson() <<
    // std::endl;
  } catch (std::runtime_error &err) {
    if (strlen(err.what()) > 0)
      std::cerr << err.what() << std::endl;
    exitStatus = EXIT_FAILURE;
  }

  auto errors = plsm::errors::get();
  for (size_t i = 0; i < errors.size(); i++) {
    exitStatus = EXIT_FAILURE;

    std::cerr << errors[i] << std::endl;
    if (i != errors.size() - 1)
      std::cerr << std::endl;
  }

  exit(exitStatus);

  // auto module = Parser().parse(argv[1], readFile(argv[1]));

  // auto fn = (ast::FnDef *)module->stmts.at(0);
  // std::cout << fn->name << std::endl;

  // delete module;
}
