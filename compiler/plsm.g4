grammar plsm;

@parser::header {
#include "AST/AST.h"
#include "Utils.h"
#include <memory>
#include <boost/json.hpp>
#include <boost/algorithm/string.hpp>

using namespace plsm::utils;
using namespace plsm::ast;
}

@parser::members {
  std::string filename, sourceText;

  void setFileName(const std::string &filename) { this->filename = filename; }
  void setSourceText(const std::string &sourceText) { this->sourceText = sourceText; }

  inline SourceRange getSourceRange(antlr4::ParserRuleContext *ctx) {
    auto start = ctx->start;
    auto stop = getCurrentToken();

    auto startPos = std::pair<size_t, size_t>(start->getLine(),
                                              start->getCharPositionInLine());
    auto stopPos = std::pair<size_t, size_t>(stop->getLine(),
                                             stop->getCharPositionInLine());

    auto text = sourceText.substr(
        start->getStartIndex(), stop->getStartIndex() - start->getStartIndex());
    text = boost::trim_copy(text);

//     std::cout << "text: `" << text << "`" << std::endl;

    return SourceRange(filename, text, startPos, stopPos);
  }
}

module
	returns[std::unique_ptr<Module> ast]: (stmts += topLevelStmt)* EOF {
          std::vector<std::unique_ptr<Stmt>> stmts;
          for (auto &stmt : $ctx->stmts) {
               stmts.push_back(std::move(stmt->ast));
          }

          $ast = std::make_unique<Module>(getSourceRange($ctx), "default", std::vector<std::unique_ptr<Import>>(), std::move(stmts));
     };

topLevelStmt
	returns[std::unique_ptr<Stmt> ast]:
	varDecl {
          $ast = ptrcast<Stmt>($ctx->varDecl()->ast);
     }
	| fnDecl {
          $ast = ptrcast<Stmt>($ctx->fnDecl()->ast);
     };

block
	returns[std::unique_ptr<Block> ast]:
	(stmts += stmt)* {
          std::vector<std::unique_ptr<Stmt>> stmts;
          for (auto &stmt : $ctx->stmts) {
               stmts.push_back(std::move(stmt->ast));
          }

          $ast = std::make_unique<Block>(getSourceRange($ctx), std::move(stmts));
     };

singleStmtBlock
	returns[std::unique_ptr<Block> ast]:
	stmt {
          std::vector<std::unique_ptr<Stmt>> stmts;
          stmts.push_back(std::move($ctx->stmt()->ast));
          $ast = std::make_unique<Block>(getSourceRange($ctx), std::move(stmts));
     };

stmt
	returns[std::unique_ptr<Stmt> ast]:
	exprStmt {
          $ast = ptrcast<Stmt>($ctx->exprStmt()->ast);
     }
	| varDecl {
          $ast = ptrcast<Stmt>($ctx->varDecl()->ast);
     }
	| retStmt {
          $ast = ptrcast<Stmt>($ctx->retStmt()->ast);
     }
	| assignStmt {
          $ast = ptrcast<Stmt>($ctx->assignStmt()->ast);
     }
	| implDeclAssignStmt {
          $ast = ptrcast<Stmt>($ctx->implDeclAssignStmt()->ast);
     }
	| ifStmt {
          $ast = ptrcast<Stmt>($ctx->ifStmt()->ast);
     }
	| whileStmt {
          $ast = ptrcast<Stmt>($ctx->whileStmt()->ast);
     }
	| inlineAsm {
          $ast = ptrcast<Stmt>($ctx->inlineAsm()->ast);
     };

inlineAsm
	returns[std::unique_ptr<InlineAsm> ast]:
	'inline' 'asm' '(' (inlineAsmCode += string)+ (
		':' (
			outputs += inlineAsmConstraint (
				',' outputs += inlineAsmConstraint
			)*
		)?
	)? (
		':' (
			inputs += inlineAsmConstraint (
				',' inputs += inlineAsmConstraint
			)*
		)?
	)? (':' (clobbers += string ( ',' clobbers += string)*)?)? ')' ';' {
          std::string code = "";
          for (auto &asmCode : $ctx->inlineAsmCode) {
               code += asmCode->value;
               code += ";";
          }
          code.pop_back();

          std::vector<std::unique_ptr<InlineAsmConstraint>> outputs;
          for (auto &output : $ctx->outputs) {
               outputs.push_back(std::move(output->ast));
          }

          std::vector<std::unique_ptr<InlineAsmConstraint>> inputs;
          for (auto &input : $ctx->inputs) {
               inputs.push_back(std::move(input->ast));
          }

          std::vector<std::string> clobbers;
          for (auto &clobber : $ctx->clobbers) {
               clobbers.push_back(clobber->value);
          }

          $ast = std::make_unique<InlineAsm>(
               getSourceRange($ctx), code, std::move(outputs), std::move(inputs), clobbers);
    };

inlineAsmConstraint
	returns[std::unique_ptr<InlineAsmConstraint> ast]:
	string '(' expr ')' {
        auto constraint = $ctx->string()->value;
        $ast = std::make_unique<InlineAsmConstraint>(getSourceRange($ctx), constraint, std::move($ctx->expr()->ast));
    };

inlineAsmLvalueConstraint
	returns[std::unique_ptr<InlineAsmConstraint> ast]:
	string '(' lvalue ')' {
        auto constraint = $ctx->string()->value;
        $ast = std::make_unique<InlineAsmConstraint>(getSourceRange($ctx), constraint, std::move($ctx->lvalue()->ast));
    };

whileStmt
	returns[std::unique_ptr<WhileStmt> ast]:
	'while' '(' condition = expr ')' (
		'{' block '}'
		| singleStmtBlock
	) {
          std::unique_ptr<Block> body;
          if ($ctx->block()) body = std::move($ctx->block()->ast);
          else body = std::move($ctx->singleStmtBlock()->ast);

          $ast = std::make_unique<WhileStmt>(getSourceRange($ctx), std::move($ctx->condition->ast), std::move(body));
     };

ifStmt
	returns[std::unique_ptr<IfStmt> ast]:
	'if' '(' condition = expr ')' (
		'{' ifBlock = block '}'
		| ifSingleBlock = singleStmtBlock
	) (
		'else' (
			'{' elseBlock = block '}'
			| elseSingleBlock = singleStmtBlock
		)
	)? {
          std::unique_ptr<Block> ifBody;
          if ($ctx->ifBlock) ifBody = std::move($ctx->ifBlock->ast);
          else ifBody = std::move($ctx->ifSingleBlock->ast);

          std::unique_ptr<Block> elseBody;
          if ($ctx->elseBlock) elseBody = std::move($ctx->elseBlock->ast);
          else if ($ctx->elseSingleBlock) elseBody = std::move($ctx->elseSingleBlock->ast);
          else elseBody = std::make_unique<Block>(SourceRange::unknown(), std::vector<std::unique_ptr<Stmt>>());

          $ast = std::make_unique<IfStmt>(getSourceRange($ctx), std::move($ctx->condition->ast), std::move(ifBody), std::move(elseBody));
     };

implDeclAssignStmt
	returns[std::unique_ptr<Stmt> ast]:
	IDENTIFIER ':=' expr {
          // TODO
     };

assignStmt
	returns[std::unique_ptr<AssignStmt> ast]:
	lval = lvalue '=' rval = expr ';' {
          $ast = std::make_unique<AssignStmt>(getSourceRange($ctx), std::move($ctx->lval->ast), std::move($ctx->rval->ast));
     };

lvalue
	returns[std::unique_ptr<Expr> ast]:
	identifier {
          $ast = ptrcast<Expr>($ctx->identifier()->ast);
     };

retStmt
	returns[std::unique_ptr<RetStmt> ast]:
	'ret' expr ';' {
          $ast = std::make_unique<RetStmt>(getSourceRange($ctx), std::move($ctx->expr()->ast));
     };

fnDecl
	returns[std::unique_ptr<FnDecl> ast]:
	'fun' name = IDENTIFIER '(' (
		params += fnParam (',' params += fnParam)*
	)? ')' typeName '{' block '}' {
          auto name = $ctx->name->getText();

          std::vector<std::unique_ptr<FnParam>> params;
          for (auto &param : $ctx->params) {
               params.push_back(std::move(param->ast));
          }

          $ast = std::make_unique<FnDecl>(getSourceRange($ctx), name, std::move(params), std::move($ctx->typeName()->ast), std::move($ctx->block()->ast));
     };

fnParam
	returns[std::unique_ptr<FnParam> ast]:
	IDENTIFIER ':' typeName {
          $ast = std::make_unique<FnParam>(getSourceRange($ctx), $ctx->IDENTIFIER()->getText(), std::move($ctx->typeName()->ast));
     };

varDecl
	returns[std::unique_ptr<VarDecl> ast]:
	'var' IDENTIFIER ':' typeName ';' {
          auto name = $ctx->IDENTIFIER()->getText();
          $ast = std::make_unique<VarDecl>(getSourceRange($ctx), name, std::move($ctx->typeName()->ast));
     };

exprStmt
	returns[std::unique_ptr<ExprStmt> ast]:
	expr ';' {
          $ast = std::make_unique<ExprStmt>(getSourceRange($ctx), std::move($ctx->expr()->ast));
     };

expr
	returns[std::unique_ptr<Expr> ast]:
	value = binaryExpr {
          $ast = std::move($ctx->value->ast);
     };

binaryExpr
	returns[std::unique_ptr<Expr> ast]:
	value = unaryExpr {
          $ast = std::move($ctx->value->ast);
     }
	| lhs = binaryExpr op = ('*' | '/' | '%') rhs = binaryExpr {
          auto opText = $ctx->op->getText();

          BinOp op;
          if (opText == "*") op = BinOp::MUL;
          if (opText == "/") op = BinOp::DIV;
          if (opText == "%") op = BinOp::MOD;

          auto binExpr = std::make_unique<BinExpr>(getSourceRange($ctx), op, std::move($ctx->lhs->ast), std::move($ctx->rhs->ast));
          $ast = ptrcast<Expr>(binExpr);
     }
	| lhs = binaryExpr op = ('+' | '-') rhs = binaryExpr {
          auto opText = $ctx->op->getText();

          BinOp op;
          if (opText == "+") op = BinOp::ADD;
          if (opText == "-") op = BinOp::SUB;

          auto binExpr = std::make_unique<BinExpr>(getSourceRange($ctx), op, std::move($ctx->lhs->ast), std::move($ctx->rhs->ast));
          $ast = ptrcast<Expr>(binExpr);
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

          auto binExpr = std::make_unique<BinExpr>(getSourceRange($ctx), op, std::move($ctx->lhs->ast), std::move($ctx->rhs->ast));
          $ast = ptrcast<Expr>(binExpr);
     }
	| lhs = binaryExpr '&&' rhs = binaryExpr {
          auto binExpr = std::make_unique<BinExpr>(getSourceRange($ctx), BinOp::AND, std::move($ctx->lhs->ast), std::move($ctx->rhs->ast));
          $ast = ptrcast<Expr>(binExpr);
     }
	| lhs = binaryExpr '||' rhs = binaryExpr {
          auto binExpr = std::make_unique<BinExpr>(getSourceRange($ctx), BinOp::OR, std::move($ctx->lhs->ast), std::move($ctx->rhs->ast));
          $ast = ptrcast<Expr>(binExpr);
     };

unaryExpr
	returns[std::unique_ptr<Expr> ast]:
	factorExpr {
          $ast = ptrcast<Expr>($ctx->factorExpr()->ast);
     }
	| functionCall {
          $ast = ptrcast<Expr>($ctx->functionCall()->ast);
     }
	| unaryExpr 'as' typeName {
          auto castExpr = std::make_unique<CastExpr>(getSourceRange($ctx), std::move($ctx->unaryExpr()->ast), std::move($ctx->typeName()->ast));
          $ast = ptrcast<Expr>(castExpr);
     }
	| '!' unaryExpr {
          auto unExpr = std::make_unique<UnExpr>(getSourceRange($ctx), UnOp::NOT, std::move($ctx->unaryExpr()->ast));
          $ast = ptrcast<Expr>(unExpr);
     }
	| '+' unaryExpr {
          auto unExpr = std::make_unique<UnExpr>(getSourceRange($ctx), UnOp::POS, std::move($ctx->unaryExpr()->ast));
          $ast = ptrcast<Expr>(unExpr);
     }
	| '-' unaryExpr {
          auto unExpr = std::make_unique<UnExpr>(getSourceRange($ctx), UnOp::NEG, std::move($ctx->unaryExpr()->ast));
          $ast = ptrcast<Expr>(unExpr);
     };

factorExpr
	returns[std::unique_ptr<Expr> ast]:
	IDENTIFIER {
          auto id = std::make_unique<Identifier>(getSourceRange($ctx), $ctx->IDENTIFIER()->getText());
          $ast = ptrcast<Expr>(id);
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

          auto val = std::make_unique<IntValue>(getSourceRange($ctx), value);
          $ast = ptrcast<Expr>(val);
     }
	| FLOAT {
          auto text = $ctx->FLOAT()->getText();

          if (text.at(text.size() - 1) == '.')
               text += "0";
          if (text.at(0) == '0')
               text = "0" + text;

          double value = std::strtod(text.data(), NULL);
          auto val = std::make_unique<FloatValue>(getSourceRange($ctx), value);
          $ast = ptrcast<Expr>(val);
     }
	| BOOL {
          auto text = $ctx->BOOL()->getText();
          auto val = std::make_unique<IntValue>(getSourceRange($ctx), text == "true" ? 1 : 0);
          $ast = ptrcast<Expr>(val);
     }
	| lambdaExpr {
          $ast = ptrcast<Expr>($ctx->lambdaExpr()->ast);
     }
	| string {
          $ast = std::unique_ptr<Expr>((Expr *) new StringValue(getSourceRange($ctx), $ctx->string()->value));
     }
	| '(' expr ')' {
          $ast = std::move($ctx->expr()->ast);
     };

functionCall
	returns[std::unique_ptr<CallExpr> ast]:
	callee = factorExpr '(' (args += expr (',' args += expr)*)? ')' {
          std::vector<std::unique_ptr<Expr>> args;
          for (auto &arg : $ctx->args) {
               args.push_back(std::move(arg->ast));
          }

          $ast = std::make_unique<CallExpr>(getSourceRange($ctx), std::move($ctx->callee->ast), std::move(args));
     };

lambdaExpr
	returns[std::unique_ptr<LambdaExpr> ast]:
	'@' '(' (params += fnParam (',' params += fnParam)*)? ')' typeName '{' block '}' {
          std::vector<std::unique_ptr<FnParam>> params;
          for (auto &param : $ctx->params) {
               params.push_back(std::move(param->ast));
          }

          $ast = std::make_unique<LambdaExpr>(getSourceRange($ctx), std::move(params), std::move($ctx->typeName()->ast), std::move($ctx->block()->ast));
     };

typeName
	returns[std::unique_ptr<TypeName> ast]:
	IDENTIFIER {
          auto text = $ctx->IDENTIFIER()->getText();
          auto named = std::make_unique<NamedTypeName>(getSourceRange($ctx), text);
          $ast = ptrcast<TypeName>(named);
     }
	| '&' typeName {
          auto typeName = std::move($ctx->typeName()->ast);
          auto ptr = std::make_unique<PointerTypeName>(getSourceRange($ctx), std::move(typeName));
          $ast = ptrcast<TypeName>(ptr);
     };

string
	returns[std::string value]:
	STRING {
          auto encoded = $ctx->STRING()->getText();
          auto decoded = boost::json::parse(encoded);
          $value = decoded.as_string();
     };

identifier
	returns[std::unique_ptr<Identifier> ast]:
	IDENTIFIER {
          $ast = std::make_unique<Identifier>(getSourceRange($ctx), $ctx->IDENTIFIER()->getText());
     };

STRING: '"' (ESC | ~["\\\r\n])* '"';
fragment ESC: '\\' (["\\/bfnrt] | 'u' HEX HEX HEX HEX);
fragment HEX: [0-9a-fA-F];

INT: [0-9]+ | '0x' [0-9a-fA-F]+ | '0o' [0-7]+ | '0b' [01]+;
FLOAT: [0-9]+ '.' | [0-9]* '.' [0-9]+;
BOOL: 'true' | 'false';

IDENTIFIER: [a-zA-Z_] [a-zA-Z0-9_]*;

WHITESPACE: [ \r\n\t]+ -> skip;
COMMENT: (('//' ~( '\r' | '\n')*) | ('/*' .*? '*/')) -> skip;