# Rough Sketch of the Argon Language Grammar

program
    : function_definition_list
;

function_definition_list
    : function_definition
    | function_definition_list function_definition
;

function_definition
    : DEF variable_identifier LPAREN RPAREN function_return_type function_body
    | DEF variable_identifier LPAREN function_parameter_list RPAREN function_return_type function_body
    | DEF variable_identifier function_body
;

function_return_type
    : type_name
    | LPAREN function_multiple_return_types RPAREN
    | %empty
;

function_multiple_return_types
    : type_name COMMA type_name
    | function_multiple_return_types COMMA type_name
;

function_parameter_list
    : %empty
    | non_empty_function_parameter_list
;

function_non_empty_parameter_list
    : parameter
    | non_empty_function_parameter_list COMMA function_parameter
;

function_parameter
    : variable_identifier type_name
;

function_body
    : block_statement
;

statement_list
    : statement
    | statement_list statement
;

statement
    : declaration_statement
    | execution_statement
    | control_statement
;

declaration-statement
    : variable_declaration
;

execution_statement
    : expression
    | assignment_statement
    | block_statement
;

control_statement
    : return_statement
    | repeat_statement
;

block_statement 
    : LBRACE  statement_list RBRACE
    | LBRACE RBRACE
;

return_statement
    : RETURN expression
    | RETURN
;

repeat_statement
    : REPEAT block_statement
    | REPEAT expression block_statement
;

variable_declaration
    : VAR IDENT type_specifier optional_initialiser
;

optional_initialiser
    : %empty
    | ASSIGN expression
;

type_specifier
    : TYPE_IDENT
;

expression
    : logical_expression
    | assignment_statement
    | bitwise_expression
;

logical_expression
    : relational_expression
    | logical_expression LOGIC_AND relational_expression
    | logical_expression LOGIC_OR relational_expression
;

bitwise_expression
    : arithmetic_expression BITWISE_AND arithmetic_expression
    | arithmetic_expression BITWISE_OR arithmetic_expression
;

relational_expression
    : arithmetic_expression
    | arithmetic_expression EQ arithmetic_expression
    | arithmetic_expression NEQ arithmetic_expression
    | arithmetic_expression LT arithmetic_expression
    | arithmetic_expression GT arithmetic_expression
    | arithmetic_expression LEQ arithmetic_expression
    | arithmetic_expression GEQ arithmetic_expression
;

arithmetic_expression
    : term
    | arithmetic_expression PLUS term
    | arithmetic_expression MINUS term
;

term 
    : factor
    | term TIMES factor
    | term DIVIDE factor
    | term MODULO factor
;

factor
    : primary
    | unary_expression
;

primary
    : constant
    | variable
    | LPAREN expression RPAREN
;

unary_expression
    : NOT factor
    | MINUS factor %prec UNARY_MINUS
;

variable
    : IDENT
;

assignment_statement
    : IDENT ASSIGN expression   
;

constant
    : INTEGER
    | FLOAT
    | CHAR
    | boolean_constant
;

boolean_constant
    : TRUE
    | FALSE
;

variable_identifier
    : IDENT
;

type_name
    : IDENT
;

identifier
    : IDENT
;