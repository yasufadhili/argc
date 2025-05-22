grammar Argon;

moduleDeclaration
	:	'module' statement* EOF
;

statement
    :   'return' expression NEWLINE
;

expression
    :   INTEGER
;


INTEGER     :   [0-9]+  ;

NEWLINE     :   [\r\n]+   ;
WHITESPACE  :   [ \t]+  ->  skip ;
