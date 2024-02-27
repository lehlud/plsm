%require "3.8"
%language "c++"

%header

%define api.token.raw
%define api.value.type variant
%define api.token.constructor
%define api.token.prefix {TOK_}

%define parse.assert
%define parse.trace
%define parse.error detailed
%define parse.lac full

%locations
%define api.location.file "../include/location.gen.h"
%define api.location.include {"location.gen.h"}

%code requires {
#include "AST/Def.h"
class ParserDriver;
}

%lex-param { ParserDriver &driver }
%parse-param { ParserDriver &driver }

%code {
#include "ParserDriver.h"
}

%token <long> INT "integer literal"
%token <double> FLOAT "float literal"

%token <std::string>
  OPERATOR "operator"
  IDENTIFIER "identifier"

  RARR "->"
  EQUALS "="
  DOT "."
  COMMA ","
  SEMI ";"
  LPAREN "("
  RPAREN ")"
  LBRACKET "["
  RBRACKET "]"
  LBRACE "{"
  RBRACE "}"

  BINOP "binop"
  DECLARE "declare"
  FN "fn"
  IMPORT "import"
  TRAIT "trait"
  TYPE "type"
  UNOP "unop"
  VAL "val"
;

%token END 0 "end of file"

%type <std::vector<ast::Stmt *>>
  topLevelStmts
  blockStmts
;

%type <ast::Stmt *>
  topLevelStmt
  blockStmt
;

%type <ast::FnDecl *> extFnDecl fnDecl;
%type <ast::ValDecl *> valDecl;

%type <std::vector<ast::Type *>>
  types
  tupleTypes
  fnTypeArgs
;

%type <ast::Type *>
  type
;

%type <std::vector<ast::Expr *>>
  exprs
  args
;

%type <ast::Expr *>
  expr
  expr3
  expr2
  expr1
  expr0
  literal
;

%type <ast::FnArg> fnArg;
%type <std::vector<ast::FnArg>>
  fnArgs
  fnArgs0
;

%type <std::vector<std::string>>
  lambdaArgs
  identifiers
;

%type <std::string>
  moduleName
  operator
  identifier
  keyword
;

%type <ast::Import *> import;
%type <std::vector<ast::Import *>>
  imports0
  imports
;

%start module

%%

module: imports topLevelStmts { driver.module = new ast::Module($2); }
      ;


imports: %empty     { ; }
       | imports0   { $$ = std::move($1); }
       ;

imports0: import            { $$.push_back($1); }
        | imports0 import   { $1.push_back($2); $$ = std::move($1); }
        ;

import: IMPORT moduleName ";" { $$ = new ast::Import($2); }
      ;

moduleName: identifier                  { $$ = $1; }
          | moduleName "." identifier   { $$ = $1 + $2 + $3; }
          ;


topLevelStmts: topLevelStmt                 { $$.push_back($1); }
             | topLevelStmts topLevelStmt   { $1.push_back($2); $$ = std::move($1); }
             ;

topLevelStmt: fnDecl      { $$ = $1; }
            | extFnDecl   { $$ = $1; }
            | valDecl     { $$ = $1; }
            ;

blockStmts: blockStmt              { $$.push_back($1); }
          | blockStmts blockStmt   { $1.push_back($2); $$ = std::move($1); }
          ;

blockStmt: valDecl    { $$ = $1; }
         | fnDecl     { $$ = $1; }
         | expr ";"   { $$ = new ast::ExprStmt($1); }
         ;

valDecl: VAL identifier "=" expr ";"        { $$ = new ast::ValDecl($2, nullptr, $4); }
       | VAL identifier type "=" expr ";"   { $$ = new ast::ValDecl($2, $3, $5); }
       ;

fnDecl: FN identifier "[" fnArgs "]" type "=" expr ";"  { $$ = new ast::FnDecl($2, $4, $6, $8); }
      ;

extFnDecl: DECLARE FN identifier "[" fnArgs "]" type ";"  { $$ = new ast::FnDecl($3, $5, $7, nullptr); }
         ;

fnArgs: %empty    { ; }
      | fnArgs0   { $$ = std::move($1); }
      ;

fnArgs0: fnArg              { $$.push_back($1); }
       | fnArgs0 "," fnArg  { $$.push_back($3); $$ = std::move($1); }
       ;

fnArg: identifier type  { $$ = ast::FnArg($1, $2); }
     ;

types: type             { $$.push_back($1); }
     | types "," type   { $1.push_back($3); $$ = std::move($1); }
     ;

type: identifier                     { $$ = new ast::NamedType($1); }
    | "(" type ")"                   { $$ = $2; }   
    | "(" tupleTypes ")"             { $$ = new ast::TupleType($2); }
    | "[" fnTypeArgs "]" "->" type   { $$ = new ast::FunctionType($2, $5); }
    ;

tupleTypes: type "," type             { $$.push_back($1); $$.push_back($3); }
          | tupleTypes "," type       { $1.push_back($3); $$ = std::move($1); }
          ;

fnTypeArgs: %empty { ; }
          | types  { $$ = std::move($1); }
          ;

exprs: expr             { $$.push_back($1); }
     | exprs "," expr   { $1.push_back($3); $$ = std::move($1); }
     ;

expr: expr3 { $$ = $1; }
    ;

expr3: expr2                          { $$ = $1; }
     | "[" lambdaArgs "]" "->" expr   { $$ = NULL; }
     | "{" blockStmts "}"                  { $$ = new ast::BlockExpr($2); }
     ;

expr2: expr1                  { $$ = $1; }
     | expr2 operator expr1   { $$ = new ast::BinExpr($1, $2, $3); }
     ;

expr1: expr0            { $$ = $1; }
     | operator expr1   { $$ = new ast::UnaryExpr($1, $2); }
     ;

expr0: literal              { $$ = $1; }
     | identifier           { $$ = new ast::Identifier($1); }
     | expr0 "[" args "]"   { $$ = new ast::CallExpr($1, $3); }
     | "(" expr ")"         { $$ = new ast::PrefExpr($2); }
     ;

lambdaArgs: %empty        { ; }
          | identifiers   { $$ = std::move($1); }
          ;

args: %empty  { ; }
    | exprs   { $$ = std::move($1); }
    ;


literal: INT        { $$ = new ast::IntValue($1); }
       | FLOAT      { $$ = new ast::FloatValue($1); }
       ;

operator: "->"
        | "="
        | OPERATOR
        ;

identifiers: identifier                   { $$.push_back($1); }
           | identifiers "," identifier   { $1.push_back($3); $$ = std::move($1); }
           ;

identifier: keyword
          | IDENTIFIER
          ;

keyword: FN
       | VAL
       | UNOP
       | BINOP
       | IMPORT
       | DECLARE
       ;

%%

void yy::parser::error(const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
