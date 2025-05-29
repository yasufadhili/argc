grammar Argon;

moduleDeclaration
	:	'module' IDENTIFIER NEWLINE statement* EOF
;

statement
    :   expression NEWLINE                      # ExpressionStmt
    |   'ret' expression NEWLINE                # ReturnStmt
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
