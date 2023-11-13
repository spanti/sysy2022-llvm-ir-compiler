lexer grammar CommonLex;
// antlr4 -Dlanguage=Cpp -visitor SysY.g4 keyword
Int: 'int';
Float: 'float';
Void: 'void';
Const: 'const';
Return: 'return';
If: 'if';
Else: 'else';
For: 'for';
While: 'while';
Do: 'do';
Break: 'break';
Continue: 'continue';

// operator
Lparen: '(';
Rparen: ')';
Lbrkt: '[';
Rbrkt: ']';
Lbrace: '{';
Rbrace: '}';
Comma: ',';
Semicolon: ';';
Question: '?';
Colon: ':';

Minus: '-';
Exclamation: '!';
Tilde: '~';
Addition: '+';
Multiplication: '*';
Division: '/';
Modulo: '%';
LAND: '&&';
LOR: '||';
EQ: '==';
NEQ: '!=';
LT: '<';
LE: '<=';
GT: '>';
GE: '>=';

Identifier: [a-zA-Z_][a-zA-Z_0-9]*;
// integer - 识别造成干扰
IntLiteral: [0-9]+ | '0x' [0-9a-fA-F]+ | '0X' [0-9a-fA-F]+;

// float
HexadecimalDigits: '0x' [0-9a-fA-F]+ | '0X' [0-9a-fA-F]+;

ExponentPart: [eE] [+-]? [0-9]+;

BinaryExponentPart: [pP] [+-]? [0-9]+;

//problem Hexad part
fragment HexadecimalFloatingConstant
    : '0' [xX] HexDigits? '.' HexDigits BinaryExponentPart
    | '0' [xX] HexDigits BinaryExponentPart
    ;

fragment HexDigits
    : HexDigit+
    ;

fragment HexDigit
    : [0-9a-fA-F]
    ;

fragment DecimalFloatingConstant:
	[0-9]+ '.' [0-9]* (ExponentPart)?
	| '.' [0-9]+ (ExponentPart)?
	| [0-9]+ ExponentPart;

FloatLiteral:
	DecimalFloatingConstant
	| HexadecimalFloatingConstant;

//identifier
//Identifier: [a-zA-Z_][a-zA-Z_0-9]*;

STRING: '"' (ESC | .)*? '"';

fragment ESC: '\\"' | '\\\\';

WS: [ \t\r\n] -> skip;

LINE_COMMENT: '//' .*? '\r'? '\n' -> skip;
COMMENT: '/*' .*? '*/' -> skip;