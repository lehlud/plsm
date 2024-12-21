grammar plsm;

@parser::header {
#include "AST/AST.h"

using namespace plsm::ast;
}

@parser::members {

std::string filename;

void setFileName(const std::string &filename) { this->filename = filename; }

inline SourceRange getSourceRange(antlr4::ParserRuleContext *ctx) {
     if (!ctx->getStart() || !ctx->getStop()) {
          return SourceRange::unknown();
     }

     return SourceRange(
          filename, ctx->getText(),
          std::pair<size_t, size_t>(ctx->getStart()->getLine(),
                                    ctx->getStart()->getCharPositionInLine()),
          std::pair<size_t, size_t>(ctx->getStop()->getLine(),
                                    ctx->getStop()->getCharPositionInLine()));
}

}

module
	returns[Module *ast]: (stmts += topLevelStmt)* {
          std::vector<Stmt *> stmts;
          for (auto &stmt : $ctx->stmts) {
               stmts.push_back(stmt->ast);
          }

          $ast = new Module(getSourceRange($ctx), "default", std::vector<Import *>(), stmts);
     };

topLevelStmt
	returns[Stmt *ast]:
	varDecl {
          $ast = $ctx->varDecl()->ast;
     }
	| fnDecl {
          $ast = $ctx->fnDecl()->ast;
     };

stmt
	returns[Stmt *ast]:
	exprStmt {
          $ast = $ctx->exprStmt()->ast;
     }
	| varDecl {
          $ast = $ctx->varDecl()->ast;
     }
	| retStmt {
          $ast = $ctx->retStmt()->ast;
     }
	| assignStmt {
          $ast = $ctx->assignStmt()->ast;
     }
	| implDeclAssignStmt {
          $ast = $ctx->implDeclAssignStmt()->ast;
     }
	| ifStmt {
          $ast = $ctx->ifStmt()->ast;
     }
	| whileStmt {
          $ast = $ctx->whileStmt()->ast;
     };

whileStmt
	returns[WhileStmt *ast]:
	'while' '(' condition = expr ')' (
		'{' (stmts += stmt)* '}'
		| singleStmt = stmt
	) {
          auto cond = $ctx->condition->ast;

          std::vector<Stmt *> body;
          for (auto &stmt : $ctx->stmts) {
               body.push_back(stmt->ast);
          }
          if ($ctx->singleStmt) body.push_back($ctx->singleStmt->ast);

          $ast = new WhileStmt(getSourceRange($ctx), cond, body);
     };

ifStmt
	returns[IfStmt *ast]:
	'if' '(' condition = expr ')' (
		'{' (ifStmts += stmt)* '}'
		| ifSingleStmt = stmt
	) (
		'else' (
			'{' (elseStmts += stmt)* '}'
			| elseSingleStmt = stmt
		)
	)? {
          auto cond = $ctx->condition->ast;

          std::vector<Stmt *> ifBody;
          for (auto &stmt : $ctx->ifStmts) {
               ifBody.push_back(stmt->ast);
          }
          if ($ctx->ifSingleStmt) ifBody.push_back($ctx->ifSingleStmt->ast);


          std::vector<Stmt *> elseBody;
          for (auto &stmt : $ctx->elseStmts) {
               elseBody.push_back(stmt->ast);
          }
          if ($ctx->elseSingleStmt) elseBody.push_back($ctx->elseSingleStmt->ast);

          $ast = new IfStmt(getSourceRange($ctx), cond, ifBody, elseBody);
     };

implDeclAssignStmt
	returns[Stmt *ast]:
	IDENTIFIER ':=' expr {
          // TODO
     };

assignStmt
	returns[AssignStmt *ast]:
	lval = expr '=' rval = expr {
          $ast = new AssignStmt(getSourceRange($ctx), $ctx->lval->ast, $ctx->rval->ast);
     };

retStmt
	returns[RetStmt *ast]:
	'ret' expr ';' {
          $ast = new RetStmt(getSourceRange($ctx), $ctx->expr()->ast);
     };

fnDecl
	returns[FnDecl *ast]:
	'fun' name = IDENTIFIER '(' (
		params += fnParam (',' params += fnParam)*
	)? ')' typeName '{' (stmts += stmt)* '}' {
          auto name = $ctx->name->getText();

          std::vector<FnParam *> params;
          for (auto &param : $ctx->params) {
               params.push_back(param->ast);
          }

          auto returnTypeName = $ctx->typeName()->ast;

          std::vector<Stmt *> body;
          for (auto &stmt : $ctx->stmts) {
               body.push_back(stmt->ast);
          }

          $ast = new FnDecl(getSourceRange($ctx), name, params, returnTypeName, body);
     };

fnParam
	returns[FnParam *ast]:
	IDENTIFIER typeName {
          $ast = new FnParam(getSourceRange($ctx), $ctx->IDENTIFIER()->getText(), $ctx->typeName()->ast);
     };

varDecl
	returns[VarDecl *ast]:
	'var' IDENTIFIER ':' typeName ';' {
          auto name = $ctx->IDENTIFIER()->getText();
          $ast = new VarDecl(getSourceRange($ctx), name, $ctx->typeName()->ast);
     };

exprStmt
	returns[ExprStmt *ast]:
	expr ';' {
          $ast = new ExprStmt(getSourceRange($ctx), $ctx->expr()->ast);
     };

expr
	returns[Expr *ast]:
	value = binaryExpr {
          $ast = $ctx->value->ast;
     };

binaryExpr
	returns[Expr *ast]:
	value = unaryExpr {
          $ast = $ctx->value->ast;
     }
	| lhs = binaryExpr op = ('*' | '/' | '%') rhs = binaryExpr {
          auto opText = $ctx->op->getText();

          BinOp op;
          if (opText == "*") op = BinOp::MUL;
          if (opText == "/") op = BinOp::DIV;
          if (opText == "%") op = BinOp::MOD;

          $ast = new BinExpr(getSourceRange($ctx), op, $ctx->lhs->ast, $ctx->rhs->ast);
     }
	| lhs = binaryExpr op = ('+' | '-') rhs = binaryExpr {
          auto opText = $ctx->op->getText();

          BinOp op;
          if (opText == "+") op = BinOp::ADD;
          if (opText == "-") op = BinOp::SUB;

          $ast = new BinExpr(getSourceRange($ctx), op, $ctx->lhs->ast, $ctx->rhs->ast);
     }
	| operand = binaryExpr 'as' typeName {
          $ast = new CastExpr(getSourceRange($ctx), $ctx->operand->ast, $ctx->typeName()->ast);
     }
	| lhs = binaryExpr op = (
		'=='
		| '!='
		| '>='
		| '<='
		| '>'
		| '<'
	) rhs = binaryExpr {
          auto opText = $ctx->op->getText();

          BinOp op;
          if (opText == "==") op = BinOp::EQ;
          if (opText == "!=") op = BinOp::NE;
          if (opText == ">=") op = BinOp::GE;
          if (opText == "<=") op = BinOp::LE;
          if (opText == ">") op = BinOp::GT;
          if (opText == "<") op = BinOp::LT;

          $ast = new BinExpr(getSourceRange($ctx), op, $ctx->lhs->ast, $ctx->rhs->ast);
     }
	| lhs = binaryExpr '&&' rhs = binaryExpr {
          $ast = new BinExpr(getSourceRange($ctx), BinOp::AND, $ctx->lhs->ast, $ctx->rhs->ast);
     }
	| lhs = binaryExpr '||' rhs = binaryExpr {
          $ast = new BinExpr(getSourceRange($ctx), BinOp::OR, $ctx->lhs->ast, $ctx->rhs->ast);
     };

unaryExpr
	returns[Expr *ast]:
	factorExpr {
          $ast = $ctx->factorExpr()->ast;
     }
	| functionCall {
          $ast = $ctx->functionCall()->ast;
     }
	| '!' unaryExpr {
          $ast = new UnExpr(getSourceRange($ctx), UnOp::NOT, $ctx->unaryExpr()->ast);
     }
	| '+' unaryExpr {
          $ast = new UnExpr(getSourceRange($ctx), UnOp::POS, $ctx->unaryExpr()->ast);
     }
	| '-' unaryExpr {
          $ast = new UnExpr(getSourceRange($ctx), UnOp::NEG, $ctx->unaryExpr()->ast);
     };

factorExpr
	returns[Expr *ast]:
	IDENTIFIER {
          $ast = new Identifier(getSourceRange($ctx), $ctx->IDENTIFIER()->getText());
     }
	| INT {
          auto text = $ctx->INT()->getText();

          int64_t value;

          if (!text.rfind("0x", 0))
               value = std::strtol(text.substr(2).data(), NULL, 16);

          else if (!text.rfind("0o", 0))
               value = std::strtol(text.substr(2).data(), NULL, 8);

          else if (!text.rfind("0b", 0))
               value = std::strtol(text.substr(2).data(), NULL, 2);

          else
               value = std::strtol(text.data(), NULL, 10);

          $ast = new IntValue(getSourceRange($ctx), value);
     }
	| FLOAT {
          auto text = $ctx->FLOAT()->getText();

          if (text.at(text.size() - 1) == '.')
               text += "0";
          if (text.at(0) == '0')
               text = "0" + text;

          double value = std::strtod(text.data(), NULL);
          $ast = new FloatValue(getSourceRange($ctx), value);
     }
	| BOOL {
          auto text = $ctx->BOOL()->getText();
          $ast = new IntValue(getSourceRange($ctx), text == "true" ? 1 : 0);
     }
	| '(' expr ')' {
          $ast = $ctx->expr()->ast;
     };

functionCall
	returns[Expr *ast]:
	callee = factorExpr '(' (args += expr (',' args += expr)*)? ')' {
          std::vector<Expr *> args;
          for (auto &arg : $ctx->args) {
               args.push_back(arg->ast);
          }

          $ast = new CallExpr(getSourceRange($ctx), $ctx->callee->ast, args);
     };

typeName
	returns[TypeName *ast]:
	IDENTIFIER {
          auto text = $ctx->IDENTIFIER()->getText();
          $ast = new PrimitiveTypeName(getSourceRange($ctx), text);
     };

INT: [0-9]+ | '0x' [0-9a-fA-F]+ | '0o' [0-7]+ | '0b' [01]+;
FLOAT: [0-9]+ '.' | [0-9]* '.' [0-9]+;
BOOL: 'true' | 'false';

IDENTIFIER: [a-zA-Z_] [a-zA-Z0-9_]*;

WHITESPACE: [ \r\n\t]+ -> skip;