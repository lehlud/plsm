#include "AST/AST.h"
#include "Utils.h"

namespace plsm {
namespace ast {

boost::json::value Block::toJson() const {
  return {
      {"@type", "Block"},
      {"stmts", utils::mapToJson(stmts)},
  };
}

Block *Block::fromJson(boost::json::value json) {
  auto stmts = fromJsonVector<Block, Stmt>(json, "stmts");
  return new Block(SourceRange::json(), std::move(stmts));
}

} // namespace ast
} // namespace plsm
