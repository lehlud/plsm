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
#include "AST.h"
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
  COMMA ","
  SEMI ";"
  LPAREN "("
  RPAREN ")"
  LBRACKET "["
  RBRACKET "]"

  FN "fn"
  UNOP "unop"
  BINOP "binop"
  VAL "val"
  IMPORT "import"
  DECLARE "declare"
;

%token END 0 "end of file"

%type <std::vector<Stmt *>> stmts;
%type <Stmt *>
  stmt
  decl
;

%type <FnDecl *> fnDecl;
%type <ValDecl *> valDecl;

%type <Type *>
  type
  type0
;

%type <std::vector<Expr *>>
  exprs
  args
;

%type <Expr *>
  expr
  expr3
  expr2
  expr1
  expr0
  literal
;

%type <FnArg> fnArg;
%type <std::vector<FnArg>>
  fnArgs
  fnArgs0
;

%type <std::vector<std::string>>
  lambdaArgs
  identifiers
;

%type <std::string>
  operator
  identifier
  keyword
;

%start module

%%

module: stmts END { driver.module = new Module($1); };

stmts: stmt         { $$ = std::vector<Stmt *>(); $$.push_back($1); }
     | stmts stmt   { $1.push_back($2); $$ = std::move($1); }
     ;

stmt: decl;

decl: valDecl { $$ = $1; }
    | fnDecl  { $$ = $1; }
    ;

valDecl: VAL identifier "=" expr ";"        { $$ = new ValDecl($2, nullptr, $4); }
       | VAL identifier type "=" expr ";"   { $$ = new ValDecl($2, $3, $5); }
       ;

fnDecl: FN identifier "[" fnArgs "]" type "=" expr ";"  { $$ = new FnDecl($2, $4, $6, $8); }
      ;

fnArgs: %empty    { $$ = std::vector<FnArg>(); }
      | fnArgs0   { $$ = std::move($1); }
      ;

fnArgs0: fnArg              { $$ = std::vector<FnArg>(); $$.push_back($1); }
       | fnArgs0 "," fnArg  { $$.push_back($3); $$ = std::move($1); }
       ;

fnArg: identifier type  { $$ = FnArg($1, $2); }
     ;

type: type0
    ;

type0: identifier     { $$ = new NamedType($1); }
     | "(" type ")"   { $$ = $2; }   
     ;

exprs: expr             { $$ = std::vector<Expr *>(); $$.push_back($1); }
     | exprs "," expr   { $1.push_back($3); $$ = std::move($1); }
     ;

expr: expr3 { $$ = $1; }
    ;

expr3: expr2                          { $$ = $1; }
     | "[" lambdaArgs "]" "->" expr   { $$ = NULL; }
     ;

expr2: expr1                  { $$ = $1; }
     | expr2 operator expr1   { $$ = new BinExpr($1, $2, $3); }
     ;

expr1: expr0            { $$ = $1; }
     | operator expr1   { $$ = new UnaryExpr($1, $2); }
     ;

expr0: literal              { $$ = $1; }
     | identifier           { $$ = new Identifier($1); }
     | expr0 "[" args "]"   { $$ = new CallExpr($1, $3); }
     | "(" expr ")"         { $$ = new PrefExpr($2); }
     ;

lambdaArgs: %empty        { $$ = std::vector<std::string>(); }
          | identifiers   { $$ = std::move($1); }
          ;

args: %empty  { $$ = std::vector<Expr *>(); }
    | exprs   { $$ = std::move($1); }
    ;


literal: INT        { $$ = new IntValue($1); }
       | FLOAT      { $$ = new FloatValue($1); }
       ;

operator: "->"      { $$ = $1; }
        | "="       { $$ = $1; }
        | OPERATOR  { $$ = $1; }
        ;

identifiers: identifier                   { $$ = std::vector<std::string>(); $$.push_back($1); }
           | identifiers "," identifier   { $1.push_back($3); $$ = std::move($1); }
           ;

identifier: keyword     { $$ = $1; }
          | IDENTIFIER  { $$ = $1; }
          ;

keyword: FN       { $$ = $1; }
       | VAL      { $$ = $1; }
       | UNOP     { $$ = $1; }
       | BINOP    { $$ = $1; }
       | IMPORT   { $$ = $1; }
       | DECLARE  { $$ = $1; }
       ;

%%

void yy::parser::error(const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
