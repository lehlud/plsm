%{
#include "parser.gen.h"
#include "ParserDriver.h"

#define yyterminate() yy::parser::make_END(yy::location())

#define _token(token) yy::parser::make_##token(yytext, driver.location)
%}

%option noyywrap nounput noinput batch

digit       [0-9]
bindigit    [0-1]
octdigit    [0-7]
hexdigit    [0-9a-fA-F]

letter      [a-zA-Z]

opchar      ("+"|"-"|"*"|"/"|"%"|"!"|"&"|"$"|"§"|"|"|"="|"<"|">"|"?"|"~"|"#"|":"|"^"|"\\")

whitespace  [ \n\t\r\v]+

%%

"0b"{bindigit}+       { return yy::parser::make_INT(std::strtol(yytext, NULL, 2), driver.location); }
"0o"{octdigit}+       { return yy::parser::make_INT(std::strtol(yytext, NULL, 8), driver.location); }
"0x"{hexdigit}+       { return yy::parser::make_INT(std::strtol(yytext, NULL, 16), driver.location); }
{digit}+              { return yy::parser::make_INT(std::strtol(yytext, NULL, 10), driver.location); }

{digit}*"."{digit}+   { return yy::parser::make_FLOAT(std::strtod(yytext, NULL), driver.location); }


{opchar}+     { std::string text = yytext;
                if (text == "=") return _token(EQUALS);
                if (text == "->") return _token(RARR);
                return _token(OPERATOR); }

","           { return _token(COMMA); }
";"           { return _token(SEMI); }
"("           { return _token(LPAREN); }
")"           { return _token(RPAREN); }
"["           { return _token(LBRACKET); }
"]"           { return _token(RBRACKET); }

"fn"          { return _token(FN); }
"unop"        { return _token(UNOP); }
"binop"       { return _token(BINOP); }
"val"         { return _token(VAL); }
"import"      { return _token(IMPORT); }
"declare"     { return _token(DECLARE); }


{letter}({digit}|{letter})*   { return _token(IDENTIFIER); }

<<EOF>>   { return yy::parser::make_END(driver.location); }

{whitespace}                  { ; }

%%

void ParserDriver::startLexer() {
  yy_scan_string(this->file.data());
}

void ParserDriver::stopLexer() {}
