grammar plsm;

module: moduleImport* (let | fnDecl | fnDef | traitDef | typeDef)*;

moduleImport: IMPORT moduleName ';';
moduleName: identifier ('.' identifier)*;

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

type: type2;
type2: type1
     | '(' type ')'
     | '(' tupleTypeList ')'          // tuple
     | '[' typeList? ']' '->' type;   // closure
type1: type0
     | type0 '{' typeList '}';        // generic
type0: identifier;

tupleTypeList: type ',' type (',' type)*;
typeList: type (',' type)*;


expr: expr3;
expr3: expr2
     | '[' identifierList? ']' '->' expr     // closure
     | '{' (let | exprStmt | fnDef)* (expr ';') '}';
expr2: expr1
     | expr2 operator expr1;                 // binary expr
expr1: expr0
     | operator expr0;                       // unary expr
expr0: literal
     | identifier
     | expr0 '[' exprList? ']'               // fn call
     | '(' expr ')'
     | expr0 '.' identifier;                 // property accessor

exprList: expr (',' expr)*;

identifierList: identifier (',' identifier)*;

literal: 'null' | INT_LIT | FLOAT_LIT;

operator: '=' | '->'
        | OPERATOR;

identifier: keyword | IDENTIFIER;
keyword: BINOP | FN | IMPORT | LET | NATIVE | TRAIT | TYPE | UNOP;


INT_LIT: [0-9]+ | '0x' [0-9a-fA-F]+ | '0o' [0-7]+ | '0b' [01]+;
FLOAT_LIT: [0-9]+ '.' | [0-9]* '.' [0-9]+;

OPERATOR: ('+'|'-'|'*'|'/'|'%'|'!'|'&'|'$'|'§'|'|'|'='|'<'|'>'|'?'|'~'|'#'|':'|'^'|'\\'|'.')+;

BINOP: 'binop';
FN: 'fn';
IMPORT: 'import';
LET: 'let';
NATIVE: 'native';
TRAIT: 'trait';
TYPE: 'type';
UNOP: 'unop';

IDENTIFIER: [a-zA-Z_] [a-zA-Z0-9_]*;
