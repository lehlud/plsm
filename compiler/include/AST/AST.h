#pragma once

#include "Base.h"

#include "Expr/BinExpr.h"
#include "Expr/CallExpr.h"
#include "Expr/CastExpr.h"
#include "Expr/Identifier.h"
#include "Expr/LambdaExpr.h"
#include "Expr/UnExpr.h"
#include "Expr/Value.h"

#include "Module/Import.h"
#include "Module/Module.h"

#include "Stmt/AssignStmt.h"
#include "Stmt/Block.h"
#include "Stmt/ExprStmt.h"
#include "Stmt/FnDecl.h"
#include "Stmt/IfStmt.h"
#include "Stmt/InlineAsm.h"
#include "Stmt/RetStmt.h"
#include "Stmt/VarDecl.h"
#include "Stmt/WhileStmt.h"

#include "Type/FunctionType.h"
#include "Type/PrimitiveType.h"

#include "TypeName/NamedTypeName.h"
