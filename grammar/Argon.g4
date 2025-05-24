grammar Argon;

moduleDeclaration
	:	'module' IDENTIFIER NEWLINE statement* EOF
;

statement
    :   expressionStatement NEWLINE
;

expressionStatement
    : expression
;

expression
    :   expression op=('*' | '/') expression    # MulDivExpr
    |   expression op=('+' | '-') expression    # AddSubExpr
    |   atom                                    # AtomExpr
;

atom
    :   INTEGER                                 # IntAtom
    | '(' expression ')'                        # ParenExpr
;


IDENTIFIER  : [a-zA-Z]+             ;

INTEGER     :   [0-9]+              ;

NEWLINE     :   [\r\n]+             ;

WHITESPACE  :   [ \t]+  ->  skip    ;
