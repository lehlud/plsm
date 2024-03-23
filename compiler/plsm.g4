grammar plsm;

module: MODULE identifier ';' moduleImport* moduleStmt*;

moduleImport: IMPORT importName ';';
importName: identifier ('/' identifier)*;
moduleStmt: let | fnDecl | fnDef | traitDef | typeDef;

traitDef: TRAIT identifier '=' identifier ';'
        | TRAIT identifier '=' '{' (fnDecl)* '}' ';';

typeDef: TYPE identifier '=' type ';'
       | TYPE identifier '=' ('(' fnDefArgs ')')? '{' (fnDecl | fnDef)* '}' ';';

fnDef: FN identifier '[' fnDefArgs? ']' type '=' expr ';';
fnDefArgs: identifier type (',' identifier type)*;

fnDecl: FN identifier '[' fnDeclArgs? ']' type ';';
fnDeclArgs: identifier? type (',' identifier? type)*;

let: LET identifier type? '=' expr ';';

exprStmt: expr ';';

type: type1; // novisit
type1: type0
     | '(' type ')'
     | '(' tupleTypeList ')'          // tuple
     | '[' typeList? ']' '->' type;   // closure
type0: identifier
     | identifier '{' typeList '}';   // generic

tupleTypeList: type (',' type)+;
typeList: type (',' type)*;


expr: expr3; // novisit
expr3: expr2
     | '[' fnDefArgs? ']' '->' expr     // closure
     | '{' blockStmt* (expr ';') '}';
expr2: expr1
     | expr2 operator expr1;        // binary expr
expr1: expr0
     | operator expr0;              // unary expr
expr0: literal
     | identifier
     | expr0 '[' exprList? ']'      // fn call
     | '(' expr ')'
     | '(' tupleExprList ')'        // tuple
     | expr0 '.' identifier;        // property accessor

blockStmt: let | exprStmt | fnDef;
tupleExprList: expr (',' expr)+;
exprList: expr (',' expr)*;

identifierList: identifier (',' identifier)*;

literal: NULL='null' | INT_LIT | FLOAT_LIT;

operator: '=' | '->'
        | OPERATOR;

identifier: keyword | IDENTIFIER;
keyword: BINOP | FN | IMPORT | LET | MODULE | NATIVE | TRAIT | TYPE | UNOP;


INT_LIT: [0-9]+ | '0x' [0-9a-fA-F]+ | '0o' [0-7]+ | '0b' [01]+;
FLOAT_LIT: [0-9]+ '.' | [0-9]* '.' [0-9]+;

OPERATOR: ('+'|'-'|'*'|'/'|'%'|'!'|'&'|'$'|'§'|'|'|'='|'<'|'>'|'?'|'~'|'#'|':'|'^'|'\\'|'.')+;

BINOP: 'binop';
FN: 'fn';
IMPORT: 'import';
LET: 'let';
MODULE: 'module';
NATIVE: 'native';
TRAIT: 'trait';
TYPE: 'type';
UNOP: 'unop';

IDENTIFIER: [a-zA-Z_] [a-zA-Z0-9_]*;

WHITESPACE: [ \r\n\t]+ -> skip;
