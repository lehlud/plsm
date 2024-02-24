#pragma once

#include <stdexcept>

#include "AST.h"

template <typename T>
class ASTVisitor
{
  virtual T visit(const ASTNode *node)
  {
    if (IntValue *cnode = dynamic_cast<IntValue *>(node))
      return visitIntValue(cnode);
    else if (FloatValue *cnode = dynamic_cast<FloatValue *>(node))
      return visitFloatValue(cnode);

    throw std::logic_error("not implemented");
  }

  virtual T visitIntValue(const IntValue *node);
  virtual T visitFloatValue(const IntValue *node);
};
