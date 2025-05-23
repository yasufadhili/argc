grammar Argon;

moduleDeclaration
	:	'module' IDENTIFIER NEWLINE statement* EOF
;

statement
    :   'return' expression NEWLINE
;

expression
    :   INTEGER
;


IDENTIFIER  : [a-zA-Z]+             ;

INTEGER     :   [0-9]+              ;

NEWLINE     :   [\r\n]+             ;

WHITESPACE  :   [ \t]+  ->  skip    ;
