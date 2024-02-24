%{
#include "parser.gen.h"
#include "ParserDriver.h"

#define yyterminate() yy::parser::make_END(yy::location())

#define _token(token) yy::parser::make_##token(driver.location)
%}

%option noyywrap nounput noinput batch debug

digit       [0-9]
bindigit    [0-1]
octdigit    [0-7]
hexdigit    [0-9a-fA-F]

letter      [a-zA-Z]

whitespace  [ \n\t\r\v]+

%%

"0b"{bindigit}+       { return yy::parser::make_INT(std::strtol(yytext, NULL, 2), driver.location); }
"0o"{octdigit}+       { return yy::parser::make_INT(std::strtol(yytext, NULL, 8), driver.location); }
"0x"{hexdigit}+       { return yy::parser::make_INT(std::strtol(yytext, NULL, 16), driver.location); }
{digit}+              { return yy::parser::make_INT(std::strtol(yytext, NULL, 10), driver.location); }

{digit}*"."{digit}+   { return yy::parser::make_FLOAT(std::strtod(yytext, NULL), driver.location); }

// TODO: operator '==' won't work this way
"->"    { return _token(R_ARR); }
"<-"    { return _token(L_ARR); }
";"     { return _token(SEMI); }
"="     { return _token(EQUALS); } 

("+"|"-"|"*"|"/"|"%"|"!"|"&"|"$"|"§"|"|"|"="|"<"|">"|"?"|"~"|"#"|":"|"^"|"\\")+  { return _token(OPERATOR); }


"fn"          { return _token(FN); }
"unop"        { return _token(UNOP); }
"binop"       { return _token(BINOP); }
"val"         { return _token(VAL); }
"import"      { return _token(IMPORT); }
"declare"     { return _token(DECLARE); }


{letter}({digit}|{letter})*   { return yy::parser::make_IDENTIFIER(yytext, driver.location); }

{whitespace}                  { ; }

%%

void ParserDriver::startLexer() {
  yy_scan_string(this->file.data());
}

void ParserDriver::stopLexer() {}
