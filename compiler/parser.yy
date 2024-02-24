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

%token <std::string> OPERATOR "operator";
%token <std::string> IDENTIFIER "identifier"

%token L_ARR "<-"
%token R_ARR "->"
%token EQUALS "="
%token SEMI ";"

%token
  FN "fn"
  UNOP "unop"
  BINOP "binop"
  VAL "val"
  IMPORT "import"
  DECLARE "declare"
;

%token END 0 "end of file"

%type <std::vector<Stmt *>> stmts;
%type <Stmt *> stmt;
%type <Decl *> decl;
%type <Expr *> expr;
%type <Value *> literal;

%type <std::string> operator;
%type <std::string> identifier;
%type <std::string> keyword;

%%

module: stmts;

stmts: stmt         { $$ = std::vector<Stmt *>(); $$.push_back($1); }
     | stmts stmt   { $1.push_back($2); $$ = std::move($1); }
     ;

stmt: decl  { $$ = $1; }
    ;

decl: VAL identifier "=" expr ";"   { $$ = new Decl($2, $4); }
    ;

expr: literal       { $$ = $1; }
    ;

literal: INT        { $$ = new IntValue($1); }
       | FLOAT      { $$ = new FloatValue($1); }
       ;

operator: L_ARR     { $$ = $1; }
        | R_ARR     { $$ = $1; }
        | EQUALS    { $$ = $1; }
        | OPERATOR  { $$ = $1; }
        ;

identifier: keyword     { $$ = $1; }
          | IDENTIFIER  { $$ = $1; }
          ;

keyword: FN       { $$ = $1; }
       | UNOP     { $$ = $1; }
       | BINOP    { $$ = $1; }
       | IMPORT   { $$ = $1; }
       | DECLARE  { $$ = $1; }
       ;

%%

void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}
