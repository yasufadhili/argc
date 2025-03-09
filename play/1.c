/*
 * A Simple C Compiler Implementation
 * 
 * This compiler implements a subset of C, demonstrating the key components
 * of a compiler: lexical analysis, parsing, semantic analysis, and code generation.
 * The target is a simple virtual machine instruction set.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/*
 * LEXICAL ANALYSIS
 *
 * The lexical analyzer (or lexer) reads the source code character by character
 * and groups them into tokens like identifiers, keywords, operators, etc.
 */

// Token types
typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_STRING
} TokenType;

// Keywords
typedef enum {
    KW_IF = 0, KW_ELSE, KW_WHILE, KW_FOR, KW_RETURN,
    KW_INT, KW_CHAR, KW_VOID, KW_STRUCT
} Keyword;

// Structure to represent a token
typedef struct {
    TokenType type;
    union {
        char *identifier;
        int integer;
        Keyword keyword;
        char operator;
        char punctuation;
        char *string;
    } value;
    int line;
    int column;
} Token;

// Lexer state
typedef struct {
    const char *source;
    size_t source_length;
    size_t position;
    int line;
    int column;
    char current_char;
} Lexer;

// Initialize the lexer with source code
void lexer_init(Lexer *lexer, const char *source) {
    lexer->source = source;
    lexer->source_length = strlen(source);
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->current_char = (lexer->source_length > 0) ? lexer->source[0] : '\0';
}

// Advance the lexer to the next character
void lexer_advance(Lexer *lexer) {
    if (lexer->position < lexer->source_length) {
        lexer->position++;
        if (lexer->current_char == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        
        lexer->current_char = (lexer->position < lexer->source_length) 
                           ? lexer->source[lexer->position] 
                           : '\0';
    }
}

// Skip whitespace characters
void lexer_skip_whitespace(Lexer *lexer) {
    while (lexer->current_char != '\0' && isspace(lexer->current_char)) {
        lexer_advance(lexer);
    }
}

// Skip comments
void lexer_skip_comments(Lexer *lexer) {
    if (lexer->current_char == '/' && lexer->position + 1 < lexer->source_length) {
        // Single-line comment
        if (lexer->source[lexer->position + 1] == '/') {
            while (lexer->current_char != '\0' && lexer->current_char != '\n') {
                lexer_advance(lexer);
            }
        }
        // Multi-line comment
        else if (lexer->source[lexer->position + 1] == '*') {
            lexer_advance(lexer); // Skip '/'
            lexer_advance(lexer); // Skip '*'
            
            while (lexer->current_char != '\0') {
                if (lexer->current_char == '*' && 
                    lexer->position + 1 < lexer->source_length &&
                    lexer->source[lexer->position + 1] == '/') {
                    lexer_advance(lexer); // Skip '*'
                    lexer_advance(lexer); // Skip '/'
                    break;
                }
                lexer_advance(lexer);
            }
        }
    }
}

// Check if a string is a keyword
bool is_keyword(const char *str, Keyword *kw) {
    struct KeywordPair {
        const char *name;
        Keyword keyword;
    };
    
    static const struct KeywordPair keywords[] = {
        {"if", KW_IF},
        {"else", KW_ELSE},
        {"while", KW_WHILE},
        {"for", KW_FOR},
        {"return", KW_RETURN},
        {"int", KW_INT},
        {"char", KW_CHAR},
        {"void", KW_VOID},
        {"struct", KW_STRUCT},
        {NULL, 0}
    };
    
    for (int i = 0; keywords[i].name != NULL; i++) {
        if (strcmp(str, keywords[i].name) == 0) {
            *kw = keywords[i].keyword;
            return true;
        }
    }
    
    return false;
}

// Process an identifier or keyword
Token lexer_identifier(Lexer *lexer) {
    Token token;
    char buffer[256] = {0};
    int i = 0;
    
    while (lexer->current_char != '\0' && 
          (isalnum(lexer->current_char) || lexer->current_char == '_')) {
        buffer[i++] = lexer->current_char;
        lexer_advance(lexer);
    }
    
    buffer[i] = '\0';
    
    Keyword kw;
    if (is_keyword(buffer, &kw)) {
        token.type = TOKEN_KEYWORD;
        token.value.keyword = kw;
    } else {
        token.type = TOKEN_IDENTIFIER;
        token.value.identifier = strdup(buffer);
    }
    
    token.line = lexer->line;
    token.column = lexer->column - strlen(buffer);
    return token;
}

// Process a numeric literal
Token lexer_number(Lexer *lexer) {
    Token token;
    char buffer[256] = {0};
    int i = 0;
    
    while (lexer->current_char != '\0' && isdigit(lexer->current_char)) {
        buffer[i++] = lexer->current_char;
        lexer_advance(lexer);
    }
    
    buffer[i] = '\0';
    
    token.type = TOKEN_INTEGER;
    token.value.integer = atoi(buffer);
    token.line = lexer->line;
    token.column = lexer->column - strlen(buffer);
    return token;
}

// Process a string literal
Token lexer_string(Lexer *lexer) {
    Token token;
    char buffer[1024] = {0};
    int i = 0;
    
    // Skip opening quote
    lexer_advance(lexer);
    
    while (lexer->current_char != '\0' && lexer->current_char != '"') {
        // Handle escape sequences
        if (lexer->current_char == '\\') {
            lexer_advance(lexer);
            switch (lexer->current_char) {
                case 'n': buffer[i++] = '\n'; break;
                case 't': buffer[i++] = '\t'; break;
                case '\\': buffer[i++] = '\\'; break;
                case '"': buffer[i++] = '"'; break;
                default: buffer[i++] = lexer->current_char;
            }
        } else {
            buffer[i++] = lexer->current_char;
        }
        lexer_advance(lexer);
    }
    
    // Skip closing quote
    if (lexer->current_char == '"') {
        lexer_advance(lexer);
    }
    
    buffer[i] = '\0';
    
    token.type = TOKEN_STRING;
    token.value.string = strdup(buffer);
    token.line = lexer->line;
    token.column = lexer->column - strlen(buffer) - 2; // -2 for quotes
    return token;
}

// Get the next token from the source code
Token lexer_get_next_token(Lexer *lexer) {
    while (lexer->current_char != '\0') {
        // Skip whitespace
        if (isspace(lexer->current_char)) {
            lexer_skip_whitespace(lexer);
            continue;
        }
        
        // Skip comments
        if (lexer->current_char == '/' && 
            lexer->position + 1 < lexer->source_length &&
            (lexer->source[lexer->position + 1] == '/' || 
             lexer->source[lexer->position + 1] == '*')) {
            lexer_skip_comments(lexer);
            continue;
        }
        
        // Identifiers and keywords
        if (isalpha(lexer->current_char) || lexer->current_char == '_') {
            return lexer_identifier(lexer);
        }
        
        // Numbers
        if (isdigit(lexer->current_char)) {
            return lexer_number(lexer);
        }
        
        // Strings
        if (lexer->current_char == '"') {
            return lexer_string(lexer);
        }
        
        // Operators
        if (strchr("+-*/=<>!&|^%", lexer->current_char) != NULL) {
            Token token;
            token.type = TOKEN_OPERATOR;
            token.value.operator = lexer->current_char;
            token.line = lexer->line;
            token.column = lexer->column;
            lexer_advance(lexer);
            return token;
        }
        
        // Punctuation
        if (strchr("(){}[];,.", lexer->current_char) != NULL) {
            Token token;
            token.type = TOKEN_PUNCTUATION;
            token.value.punctuation = lexer->current_char;
            token.line = lexer->line;
            token.column = lexer->column;
            lexer_advance(lexer);
            return token;
        }
        
        // Unrecognized character
        fprintf(stderr, "Unrecognized character '%c' at line %d, column %d\n", 
                lexer->current_char, lexer->line, lexer->column);
        lexer_advance(lexer);
    }
    
    // End of file
    Token token;
    token.type = TOKEN_EOF;
    token.line = lexer->line;
    token.column = lexer->column;
    return token;
}

/*
 * PARSING
 *
 * The parser takes tokens from the lexer and constructs an abstract syntax tree (AST),
 * which represents the program structure according to the grammar of the language.
 */

// AST node types
typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_BLOCK,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_RETURN,
    NODE_BINARY_OP,
    NODE_UNARY_OP,
    NODE_CALL,
    NODE_IDENTIFIER,
    NODE_INTEGER,
    NODE_STRING,
    NODE_EXPRESSION
} NodeType;

// Structure to represent an AST node
typedef struct ASTNode {
    NodeType type;
    union {
        // Program
        struct {
            struct ASTNode **functions;
            int function_count;
        } program;
        
        // Function
        struct {
            char *name;
            struct ASTNode **parameters;
            int parameter_count;
            struct ASTNode *body;
            char *return_type;
        } function;
        
        // Block
        struct {
            struct ASTNode **statements;
            int statement_count;
        } block;
        
        // Declaration
        struct {
            char *type;
            char *name;
            struct ASTNode *initializer;
        } declaration;
        
        // Assignment
        struct {
            struct ASTNode *target;
            struct ASTNode *value;
        } assignment;
        
        // If statement
        struct {
            struct ASTNode *condition;
            struct ASTNode *then_branch;
            struct ASTNode *else_branch;
        } if_stmt;
        
        // While loop
        struct {
            struct ASTNode *condition;
            struct ASTNode *body;
        } while_loop;
        
        // For loop
        struct {
            struct ASTNode *initializer;
            struct ASTNode *condition;
            struct ASTNode *increment;
            struct ASTNode *body;
        } for_loop;
        
        // Return statement
        struct {
            struct ASTNode *value;
        } return_stmt;
        
        // Binary operation
        struct {
            char operator;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;
        
        // Unary operation
        struct {
            char operator;
            struct ASTNode *operand;
        } unary_op;
        
        // Function call
        struct {
            char *name;
            struct ASTNode **arguments;
            int argument_count;
        } call;
        
        // Identifier
        struct {
            char *name;
        } identifier;
        
        // Integer literal
        struct {
            int value;
        } integer;
        
        // String literal
        struct {
            char *value;
        } string;
    } value;
} ASTNode;

// Parser state
typedef struct {
    Lexer lexer;
    Token current_token;
} Parser;

// Initialize the parser with source code
void parser_init(Parser *parser, const char *source) {
    lexer_init(&parser->lexer, source);
    parser->current_token = lexer_get_next_token(&parser->lexer);
}

// Advance the parser to the next token
void parser_eat(Parser *parser, TokenType expected_type) {
    if (parser->current_token.type == expected_type) {
        parser->current_token = lexer_get_next_token(&parser->lexer);
    } else {
        fprintf(stderr, "Unexpected token type at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
}

// Forward declarations for recursive parsing functions
ASTNode *parser_parse_expression(Parser *parser);
ASTNode *parser_parse_statement(Parser *parser);
ASTNode *parser_parse_block(Parser *parser);

// Parse a primary expression (identifier, literal, or parenthesized expression)
ASTNode *parser_parse_primary(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    
    switch (parser->current_token.type) {
        case TOKEN_IDENTIFIER: {
            // Check if it's a function call
            char *name = strdup(parser->current_token.value.identifier);
            parser_eat(parser, TOKEN_IDENTIFIER);
            
            if (parser->current_token.type == TOKEN_PUNCTUATION && 
                parser->current_token.value.punctuation == '(') {
                node->type = NODE_CALL;
                node->value.call.name = name;
                node->value.call.arguments = NULL;
                node->value.call.argument_count = 0;
                
                parser_eat(parser, TOKEN_PUNCTUATION); // Eat '('
                
                // Parse arguments
                if (!(parser->current_token.type == TOKEN_PUNCTUATION && 
                      parser->current_token.value.punctuation == ')')) {
                    
                    node->value.call.arguments = malloc(sizeof(ASTNode*) * 10); // Up to 10 arguments
                    while (true) {
                        node->value.call.arguments[node->value.call.argument_count++] = 
                            parser_parse_expression(parser);
                        
                        if (parser->current_token.type == TOKEN_PUNCTUATION && 
                            parser->current_token.value.punctuation == ',') {
                            parser_eat(parser, TOKEN_PUNCTUATION); // Eat ','
                        } else {
                            break;
                        }
                    }
                }
                
                parser_eat(parser, TOKEN_PUNCTUATION); // Eat ')'
            } else {
                // Just an identifier
                node->type = NODE_IDENTIFIER;
                node->value.identifier.name = name;
            }
            break;
        }
        
        case TOKEN_INTEGER:
            node->type = NODE_INTEGER;
            node->value.integer.value = parser->current_token.value.integer;
            parser_eat(parser, TOKEN_INTEGER);
            break;
            
        case TOKEN_STRING:
            node->type = NODE_STRING;
            node->value.string.value = strdup(parser->current_token.value.string);
            parser_eat(parser, TOKEN_STRING);
            break;
            
        case TOKEN_PUNCTUATION:
            if (parser->current_token.value.punctuation == '(') {
                parser_eat(parser, TOKEN_PUNCTUATION); // Eat '('
                ASTNode *expr = parser_parse_expression(parser);
                parser_eat(parser, TOKEN_PUNCTUATION); // Eat ')'
                free(node); // Free unused node
                return expr;
            }
            // Fall through to error
            
        default:
            fprintf(stderr, "Unexpected token in primary expression at line %d, column %d\n",
                    parser->current_token.line, parser->current_token.column);
            exit(1);
    }
    
    return node;
}

// Parse a unary expression (e.g., -x, !x)
ASTNode *parser_parse_unary(Parser *parser) {
    if (parser->current_token.type == TOKEN_OPERATOR) {
        char op = parser->current_token.value.operator;
        if (op == '-' || op == '!' || op == '~' || op == '&' || op == '*') {
            parser_eat(parser, TOKEN_OPERATOR);
            
            ASTNode *node = malloc(sizeof(ASTNode));
            node->type = NODE_UNARY_OP;
            node->value.unary_op.operator = op;
            node->value.unary_op.operand = parser_parse_unary(parser);
            
            return node;
        }
    }
    
    return parser_parse_primary(parser);
}

// Parse a multiplicative expression (*, /, %)
ASTNode *parser_parse_multiplicative(Parser *parser) {
    ASTNode *node = parser_parse_unary(parser);
    
    while (parser->current_token.type == TOKEN_OPERATOR) {
        char op = parser->current_token.value.operator;
        if (op == '*' || op == '/' || op == '%') {
            parser_eat(parser, TOKEN_OPERATOR);
            
            ASTNode *binary_node = malloc(sizeof(ASTNode));
            binary_node->type = NODE_BINARY_OP;
            binary_node->value.binary_op.operator = op;
            binary_node->value.binary_op.left = node;
            binary_node->value.binary_op.right = parser_parse_unary(parser);
            
            node = binary_node;
        } else {
            break;
        }
    }
    
    return node;
}

// Parse an additive expression (+, -)
ASTNode *parser_parse_additive(Parser *parser) {
    ASTNode *node = parser_parse_multiplicative(parser);
    
    while (parser->current_token.type == TOKEN_OPERATOR) {
        char op = parser->current_token.value.operator;
        if (op == '+' || op == '-') {
            parser_eat(parser, TOKEN_OPERATOR);
            
            ASTNode *binary_node = malloc(sizeof(ASTNode));
            binary_node->type = NODE_BINARY_OP;
            binary_node->value.binary_op.operator = op;
            binary_node->value.binary_op.left = node;
            binary_node->value.binary_op.right = parser_parse_multiplicative(parser);
            
            node = binary_node;
        } else {
            break;
        }
    }
    
    return node;
}

// Parse a relational expression (<, >, <=, >=)
ASTNode *parser_parse_relational(Parser *parser) {
    ASTNode *node = parser_parse_additive(parser);
    
    while (parser->current_token.type == TOKEN_OPERATOR) {
        char op = parser->current_token.value.operator;
        if (op == '<' || op == '>' || 
            // Check for <= and >=
            (op == '<' && parser->lexer.current_char == '=') ||
            (op == '>' && parser->lexer.current_char == '=')) {
            
            parser_eat(parser, TOKEN_OPERATOR);
            
            // Handle two-character operators
            bool is_compound = false;
            if (parser->current_token.type == TOKEN_OPERATOR && 
                parser->current_token.value.operator == '=') {
                is_compound = true;
                parser_eat(parser, TOKEN_OPERATOR);
            }
            
            ASTNode *binary_node = malloc(sizeof(ASTNode));
            binary_node->type = NODE_BINARY_OP;
            binary_node->value.binary_op.operator = is_compound ? 
                (op == '<' ? 'L' : 'G') : op; // 'L' for <=, 'G' for >=
            binary_node->value.binary_op.left = node;
            binary_node->value.binary_op.right = parser_parse_additive(parser);
            
            node = binary_node;
        } else {
            break;
        }
    }
    
    return node;
}

// Parse an equality expression (==, !=)
ASTNode *parser_parse_equality(Parser *parser) {
    ASTNode *node = parser_parse_relational(parser);
    
    while (parser->current_token.type == TOKEN_OPERATOR) {
        char op = parser->current_token.value.operator;
        if ((op == '=' || op == '!') && parser->lexer.current_char == '=') {
            parser_eat(parser, TOKEN_OPERATOR); // Eat first character
            parser_eat(parser, TOKEN_OPERATOR); // Eat '='
            
            ASTNode *binary_node = malloc(sizeof(ASTNode));
            binary_node->type = NODE_BINARY_OP;
            binary_node->value.binary_op.operator = (op == '=') ? 'E' : 'N'; // 'E' for ==, 'N' for !=
            binary_node->value.binary_op.left = node;
            binary_node->value.binary_op.right = parser_parse_relational(parser);
            
            node = binary_node;
        } else {
            break;
        }
    }
    
    return node;
}

// Parse a logical AND expression (&&)
ASTNode *parser_parse_logical_and(Parser *parser) {
    ASTNode *node = parser_parse_equality(parser);
    
    while (parser->current_token.type == TOKEN_OPERATOR && 
           parser->current_token.value.operator == '&' && 
           parser->lexer.current_char == '&') {
        
        parser_eat(parser, TOKEN_OPERATOR); // Eat first '&'
        parser_eat(parser, TOKEN_OPERATOR); // Eat second '&'
        
        ASTNode *binary_node = malloc(sizeof(ASTNode));
        binary_node->type = NODE_BINARY_OP;
        binary_node->value.binary_op.operator = 'A'; // 'A' for &&
        binary_node->value.binary_op.left = node;
        binary_node->value.binary_op.right = parser_parse_equality(parser);
        
        node = binary_node;
    }
    
    return node;
}

// Parse a logical OR expression (||)
ASTNode *parser_parse_logical_or(Parser *parser) {
    ASTNode *node = parser_parse_logical_and(parser);
    
    while (parser->current_token.type == TOKEN_OPERATOR && 
           parser->current_token.value.operator == '|' && 
           parser->lexer.current_char == '|') {
        
        parser_eat(parser, TOKEN_OPERATOR); // Eat first '|'
        parser_eat(parser, TOKEN_OPERATOR); // Eat second '|'
        
        ASTNode *binary_node = malloc(sizeof(ASTNode));
        binary_node->type = NODE_BINARY_OP;
        binary_node->value.binary_op.operator = 'O'; // 'O' for ||
        binary_node->value.binary_op.left = node;
        binary_node->value.binary_op.right = parser_parse_logical_and(parser);
        
        node = binary_node;
    }
    
    return node;
}

// Parse an assignment expression (=)
ASTNode *parser_parse_assignment(Parser *parser) {
    ASTNode *node = parser_parse_logical_or(parser);
    
    if (parser->current_token.type == TOKEN_OPERATOR && 
        parser->current_token.value.operator == '=') {
        
        // Ensure left side is a valid lvalue (identifier)
        if (node->type != NODE_IDENTIFIER) {
            fprintf(stderr, "Invalid assignment target\n");
            exit(1);
        }
        
        parser_eat(parser, TOKEN_OPERATOR); // Eat '='
        
        ASTNode *assign_node = malloc(sizeof(ASTNode));
        assign_node->type = NODE_ASSIGNMENT;
        assign_node->value.assignment.target = node;
        assign_node->value.assignment.value = parser_parse_assignment(parser);
        
        return assign_node;
    }
    
    return node;
}

// Parse an expression
ASTNode *parser_parse_expression(Parser *parser) {
    return parser_parse_assignment(parser);
}

// Parse a variable declaration
ASTNode *parser_parse_declaration(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DECLARATION;
    
    // Get type
    char *type_keywords[] = {"int", "char", "void"};
    bool found = false;
    
    if (parser->current_token.type == TOKEN_KEYWORD) {
        for (int i = 0; i < 3; i++) {
            if (parser->current_token.value.keyword == KW_INT + i) {
                node->value.declaration.type = strdup(type_keywords[i]);
                found = true;
                break;
            }
        }
    }
    
    if (!found) {
        fprintf(stderr, "Expected type specifier at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_KEYWORD);
    
    // Get variable name
    if (parser->current_token.type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected identifier at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    node->value.declaration.name = strdup(parser->current_token.value.identifier);
    parser_eat(parser, TOKEN_IDENTIFIER);
    
    // Check for initializer
    if (parser->current_token.type == TOKEN_OPERATOR && 
        parser->current_token.value.operator == '=') {
        parser_eat(parser, TOKEN_OPERATOR); // Eat '='
        node->value.declaration.initializer = parser_parse_expression(parser);
    } else {
        node->value.declaration.initializer = NULL;
    }
    
    // Expect semicolon
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ';') {
        fprintf(stderr, "Expected ';' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat ';'
    
    return node;
}

// Parse an if statement
ASTNode *parser_parse_if(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IF;
    
    parser_eat(parser, TOKEN_KEYWORD); // Eat 'if'
    
    // Parse condition
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != '(') {
        fprintf(stderr, "Expected '(' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat '('
    node->value.if_stmt.condition = parser_parse_expression(parser);
    
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ')') {
        fprintf(stderr, "Expected ')' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat ')'
    
    // Parse then branch
    node->value.if_stmt.then_branch = parser_parse_statement(parser);
    
    // Parse optional else branch
    if (parser->current_token.type == TOKEN_KEYWORD && 
        parser->current_token.value.keyword == KW_ELSE) {
        parser_eat(parser, TOKEN_KEYWORD); // Eat 'else'
        node->value.if_stmt.else_branch = parser_parse_statement(parser);
    } else {
        node->value.if_stmt.else_branch = NULL;
    }
    
    return node;
}

// Parse a while loop
ASTNode *parser_parse_while(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    
    parser_eat(parser, TOKEN_KEYWORD); // Eat 'while'
    
    // Parse condition
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != '(') {
        fprintf(stderr, "Expected '(' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat '('
    node->value.while_loop.condition = parser_parse_expression(parser);
    
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ')') {
        fprintf(stderr, "Expected ')' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat ')'
    
    // Parse body
    node->value.while_loop.body = parser_parse_statement(parser);
    
    return node;
}

// Parse a for loop
ASTNode *parser_parse_for(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_FOR;
    
    parser_eat(parser, TOKEN_KEYWORD); // Eat 'for'
    
    // Parse initializer, condition, and increment
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != '(') {
        fprintf(stderr, "Expected '(' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat '('
    
    // Initializer
    if (parser->current_token.type == TOKEN_KEYWORD &&
        (parser->current_token.value.keyword == KW_INT ||
         parser->current_token.value.keyword == KW_CHAR ||
         parser->current_token.value.keyword == KW_VOID)) {
        node->value.for_loop.initializer = parser_parse_declaration(parser);
    } else if (parser->current_token.type != TOKEN_PUNCTUATION || 
               parser->current_token.value.punctuation != ';') {
        node->value.for_loop.initializer = parser_parse_expression(parser);
        
        if (parser->current_token.type != TOKEN_PUNCTUATION || 
            parser->current_token.value.punctuation != ';') {
            fprintf(stderr, "Expected ';' at line %d, column %d\n",
                    parser->current_token.line, parser->current_token.column);
            exit(1);
        }
        
        parser_eat(parser, TOKEN_PUNCTUATION); // Eat ';'
    } else {
        node->value.for_loop.initializer = NULL;
        parser_eat(parser, TOKEN_PUNCTUATION); // Eat ';'
    }
    
    // Condition
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ';') {
        node->value.for_loop.condition = parser_parse_expression(parser);
    } else {
        node->value.for_loop.condition = NULL;
    }
    
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ';') {
        fprintf(stderr, "Expected ';' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat ';'
    
    // Increment
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ')') {
        node->value.for_loop.increment = parser_parse_expression(parser);
    } else {
        node->value.for_loop.increment = NULL;
    }
    
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ')') {
        fprintf(stderr, "Expected ')' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat ')'
    
    // Parse body
    node->value.for_loop.body = parser_parse_statement(parser);
    
    return node;
}

// Parse a return statement
ASTNode *parser_parse_return(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_RETURN;
    
    parser_eat(parser, TOKEN_KEYWORD); // Eat 'return'
    
    // Parse optional return value
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ';') {
        node->value.return_stmt.value = parser_parse_expression(parser);
    } else {
        node->value.return_stmt.value = NULL;
    }
    
    // Expect semicolon
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ';') {
        fprintf(stderr, "Expected ';' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat ';'
    
    return node;
}

// Parse a block of statements
ASTNode *parser_parse_block(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BLOCK;
    node->value.block.statements = NULL;
    node->value.block.statement_count = 0;
    
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != '{') {
        fprintf(stderr, "Expected '{' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat '{'
    
    // Parse statements
    node->value.block.statements = malloc(sizeof(ASTNode*) * 100); // Up to 100 statements
    
    while (parser->current_token.type != TOKEN_PUNCTUATION || 
           parser->current_token.value.punctuation != '}') {
        node->value.block.statements[node->value.block.statement_count++] = 
            parser_parse_statement(parser);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat '}'
    
    return node;
}

// Parse an expression statement
ASTNode *parser_parse_expression_statement(Parser *parser) {
    ASTNode *expr = parser_parse_expression(parser);
    
    // Expect semicolon
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ';') {
        fprintf(stderr, "Expected ';' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat ';'
    
    return expr;
}

// Parse a statement
ASTNode *parser_parse_statement(Parser *parser) {
    if (parser->current_token.type == TOKEN_KEYWORD) {
        switch (parser->current_token.value.keyword) {
            case KW_IF:
                return parser_parse_if(parser);
                
            case KW_WHILE:
                return parser_parse_while(parser);
                
            case KW_FOR:
                return parser_parse_for(parser);
                
            case KW_RETURN:
                return parser_parse_return(parser);
                
            case KW_INT:
            case KW_CHAR:
            case KW_VOID:
                return parser_parse_declaration(parser);
                
            default:
                fprintf(stderr, "Unexpected keyword at line %d, column %d\n",
                        parser->current_token.line, parser->current_token.column);
                exit(1);
        }
    } else if (parser->current_token.type == TOKEN_PUNCTUATION && 
               parser->current_token.value.punctuation == '{') {
        return parser_parse_block(parser);
    } else {
        return parser_parse_expression_statement(parser);
    }
}

// Parse a function parameter
ASTNode *parser_parse_parameter(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DECLARATION;
    
    // Get type
    char *type_keywords[] = {"int", "char", "void"};
    bool found = false;
    
    if (parser->current_token.type == TOKEN_KEYWORD) {
        for (int i = 0; i < 3; i++) {
            if (parser->current_token.value.keyword == KW_INT + i) {
                node->value.declaration.type = strdup(type_keywords[i]);
                found = true;
                break;
            }
        }
    }
    
    if (!found) {
        fprintf(stderr, "Expected type specifier at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_KEYWORD);
    
    // Get parameter name
    if (parser->current_token.type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected identifier at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    node->value.declaration.name = strdup(parser->current_token.value.identifier);
    parser_eat(parser, TOKEN_IDENTIFIER);
    
    // No initializer for parameters
    node->value.declaration.initializer = NULL;
    
    return node;
}

// Parse a function definition
ASTNode *parser_parse_function(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_FUNCTION;
    
    // Get return type
    char *type_keywords[] = {"int", "char", "void"};
    bool found = false;
    
    if (parser->current_token.type == TOKEN_KEYWORD) {
        for (int i = 0; i < 3; i++) {
            if (parser->current_token.value.keyword == KW_INT + i) {
                node->value.function.return_type = strdup(type_keywords[i]);
                found = true;
                break;
            }
        }
    }
    
    if (!found) {
        fprintf(stderr, "Expected return type at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_KEYWORD);
    
    // Get function name
    if (parser->current_token.type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected function name at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    node->value.function.name = strdup(parser->current_token.value.identifier);
    parser_eat(parser, TOKEN_IDENTIFIER);
    
    // Parse parameter list
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != '(') {
        fprintf(stderr, "Expected '(' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat '('
    
    node->value.function.parameters = malloc(sizeof(ASTNode*) * 10); // Up to 10 parameters
    node->value.function.parameter_count = 0;
    
    // Parse parameters
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ')') {
        
        while (true) {
            node->value.function.parameters[node->value.function.parameter_count++] = 
                parser_parse_parameter(parser);
            
            if (parser->current_token.type == TOKEN_PUNCTUATION && 
                parser->current_token.value.punctuation == ',') {
                parser_eat(parser, TOKEN_PUNCTUATION); // Eat ','
            } else {
                break;
            }
        }
    }
    
    if (parser->current_token.type != TOKEN_PUNCTUATION || 
        parser->current_token.value.punctuation != ')') {
        fprintf(stderr, "Expected ')' at line %d, column %d\n",
                parser->current_token.line, parser->current_token.column);
        exit(1);
    }
    
    parser_eat(parser, TOKEN_PUNCTUATION); // Eat ')'
    
    // Parse function body
    node->value.function.body = parser_parse_block(parser);
    
    return node;
}

// Parse a program (a collection of functions)
ASTNode *parser_parse_program(Parser *parser) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->value.program.functions = malloc(sizeof(ASTNode*) * 100); // Up to 100 functions
    node->value.program.function_count = 0;
    
    while (parser->current_token.type != TOKEN_EOF) {
        node->value.program.functions[node->value.program.function_count++] = 
            parser_parse_function(parser);
    }
    
    return node;
}

// Parse the entire source code
ASTNode *parser_parse(Parser *parser) {
    return parser_parse_program(parser);
}

/*
 * SEMANTIC ANALYSIS
 *
 * The semantic analyzer checks the program for semantic errors, such as
 * type mismatches, undeclared variables, etc. It also builds symbol tables.
 */

// Symbol kind
typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION
} SymbolKind;

// Symbol table entry
typedef struct Symbol {
    char *name;
    char *type;
    SymbolKind kind;
    int offset;  // Add offset field for variable location in memory
    struct Symbol *next;
} Symbol;

// Symbol table
typedef struct SymbolTable {
    Symbol *symbols;
    struct SymbolTable *parent;
} SymbolTable;

// Create a new symbol table
SymbolTable *symbol_table_create(SymbolTable *parent) {
    SymbolTable *table = malloc(sizeof(SymbolTable));
    table->symbols = NULL;
    table->parent = parent;
    return table;
}

// Add a symbol to a symbol table
void symbol_table_add(SymbolTable *table, const char *name, const char *type, SymbolKind kind) {
    Symbol *symbol = malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->type = strdup(type);
    symbol->kind = kind;
    symbol->offset = 0;  // Initialize offset to 0, will be set by the code generator
    symbol->next = table->symbols;
    table->symbols = symbol;
}

// Find a symbol in a symbol table (including parent tables)
Symbol *symbol_table_lookup(SymbolTable *table, const char *name) {
    for (SymbolTable *current = table; current != NULL; current = current->parent) {
        for (Symbol *symbol = current->symbols; symbol != NULL; symbol = symbol->next) {
            if (strcmp(symbol->name, name) == 0) {
                return symbol;
            }
        }
    }
    
    return NULL;
}

// Check an expression node for semantic errors
void semantic_check_expression(ASTNode *node, SymbolTable *table) {
    if (node == NULL) return;
    
    switch (node->type) {
        case NODE_BINARY_OP:
            semantic_check_expression(node->value.binary_op.left, table);
            semantic_check_expression(node->value.binary_op.right, table);
            break;
            
        case NODE_UNARY_OP:
            semantic_check_expression(node->value.unary_op.operand, table);
            break;
            
        case NODE_IDENTIFIER: {
            Symbol *symbol = symbol_table_lookup(table, node->value.identifier.name);
            if (symbol == NULL) {
                fprintf(stderr, "Undeclared identifier: %s\n", node->value.identifier.name);
                exit(1);
            }
            break;
        }
        
        case NODE_CALL: {
            Symbol *symbol = symbol_table_lookup(table, node->value.call.name);
            if (symbol == NULL) {
                fprintf(stderr, "Undeclared function: %s\n", node->value.call.name);
                exit(1);
            }
            
            if (symbol->kind != SYMBOL_FUNCTION) {
                fprintf(stderr, "%s is not a function\n", node->value.call.name);
                exit(1);
            }
            
            for (int i = 0; i < node->value.call.argument_count; i++) {
                semantic_check_expression(node->value.call.arguments[i], table);
            }
            break;
        }
        
        case NODE_ASSIGNMENT:
            semantic_check_expression(node->value.assignment.target, table);
            semantic_check_expression(node->value.assignment.value, table);
            break;
            
        default:
            break;
    }
}

// Check a statement node for semantic errors
void semantic_check_statement(ASTNode *node, SymbolTable *table) {
    if (node == NULL) return;
    
    switch (node->type) {
        case NODE_BLOCK: {
            SymbolTable *block_table = symbol_table_create(table);
            
            for (int i = 0; i < node->value.block.statement_count; i++) {
                semantic_check_statement(node->value.block.statements[i], block_table);
            }
            break;
        }
        
        case NODE_DECLARATION: {
            // Check for redeclaration
            Symbol *existing = symbol_table_lookup(table, node->value.declaration.name);
            if (existing != NULL && existing->next == table->symbols) {
                fprintf(stderr, "Redeclaration of '%s'\n", node->value.declaration.name);
                exit(1);
            }
            
            // Add to symbol table
            symbol_table_add(table, node->value.declaration.name, 
                             node->value.declaration.type, SYMBOL_VARIABLE);
            
            // Check initializer
            if (node->value.declaration.initializer != NULL) {
                semantic_check_expression(node->value.declaration.initializer, table);
            }
            break;
        }
        
        case NODE_IF:
            semantic_check_expression(node->value.if_stmt.condition, table);
            semantic_check_statement(node->value.if_stmt.then_branch, table);
            if (node->value.if_stmt.else_branch != NULL) {
                semantic_check_statement(node->value.if_stmt.else_branch, table);
            }
            break;
            
        case NODE_WHILE:
            semantic_check_expression(node->value.while_loop.condition, table);
            semantic_check_statement(node->value.while_loop.body, table);
            break;
            
        case NODE_FOR: {
            SymbolTable *for_table = symbol_table_create(table);
            
            if (node->value.for_loop.initializer != NULL) {
                semantic_check_statement(node->value.for_loop.initializer, for_table);
            }
            
            if (node->value.for_loop.condition != NULL) {
                semantic_check_expression(node->value.for_loop.condition, for_table);
            }
            
            if (node->value.for_loop.increment != NULL) {
                semantic_check_expression(node->value.for_loop.increment, for_table);
            }
            
            semantic_check_statement(node->value.for_loop.body, for_table);
            break;
        }
        
        case NODE_RETURN:
            if (node->value.return_stmt.value != NULL) {
                semantic_check_expression(node->value.return_stmt.value, table);
            }
            break;
            
        case NODE_ASSIGNMENT:
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
        case NODE_CALL:
        case NODE_IDENTIFIER:
        case NODE_INTEGER:
        case NODE_STRING:
            semantic_check_expression(node, table);
            break;
            
        default:
            break;
    }
}

// Check a function node for semantic errors
void semantic_check_function(ASTNode *node, SymbolTable *global_table) {
    // Create function symbol table
    SymbolTable *function_table = symbol_table_create(global_table);
    
    // Add parameters to symbol table
    for (int i = 0; i < node->value.function.parameter_count; i++) {
        ASTNode *param = node->value.function.parameters[i];
        symbol_table_add(function_table, param->value.declaration.name,
                         param->value.declaration.type, SYMBOL_VARIABLE);
    }
    
    // Check function body
    semantic_check_statement(node->value.function.body, function_table);
}

// Check an entire program for semantic errors
void semantic_check_program(ASTNode *node) {
    // Create global symbol table
    SymbolTable *global_table = symbol_table_create(NULL);
    
    // Add built-in functions
    symbol_table_add(global_table, "print", "void", SYMBOL_FUNCTION);
    
    // First pass: add all functions to global symbol table
    for (int i = 0; i < node->value.program.function_count; i++) {
        ASTNode *function = node->value.program.functions[i];
        symbol_table_add(global_table, function->value.function.name,
                         function->value.function.return_type, SYMBOL_FUNCTION);
    }
    
    // Second pass: check each function
    for (int i = 0; i < node->value.program.function_count; i++) {
        semantic_check_function(node->value.program.functions[i], global_table);
    }
}

/*
 * CODE GENERATION
 *
 * The code generator converts the AST into target code.
 * In this simple compiler, we'll generate code for a virtual machine.
 */

// Virtual machine opcodes
typedef enum {
    OP_PUSH,    // Push literal value
    OP_LOAD,    // Load variable value
    OP_STORE,   // Store to variable
    OP_ADD,     // Add top two values
    OP_SUB,     // Subtract top two values
    OP_MUL,     // Multiply top two values
    OP_DIV,     // Divide top two values
    OP_MOD,     // Modulo top two values
    OP_EQ,      // Equality comparison
    OP_NE,      // Inequality comparison
    OP_LT,      // Less than comparison
    OP_LE,      // Less than or equal comparison
    OP_GT,      // Greater than comparison
    OP_GE,      // Greater than or equal comparison
    OP_AND,     // Logical AND
    OP_OR,      // Logical OR
    OP_NOT,     // Logical NOT
    OP_NEG,     // Negate top value
    OP_JMP,     // Unconditional jump
    OP_JMPF,    // Jump if false
    OP_CALL,    // Call function
    OP_RET,     // Return from function
    OP_PRINT    // Print top value
} Opcode;

// Instruction structure
typedef struct {
    Opcode opcode;
    int operand;
} Instruction;

// Code generator state
typedef struct {
    Instruction *code;
    int code_size;
    int code_capacity;
    
    // Symbol table for variable offsets
    SymbolTable *globals;
    SymbolTable *locals;
    int local_offset;
    
    // Function addresses
    char **function_names;
    int *function_addrs;
    int function_count;
} CodeGen;

// Initialize the code generator
void codegen_init(CodeGen *gen) {
    gen->code_capacity = 1000;
    gen->code = malloc(sizeof(Instruction) * gen->code_capacity);
    gen->code_size = 0;
    
    gen->globals = symbol_table_create(NULL);
    gen->locals = NULL;
    gen->local_offset = 0;
    
    gen->function_names = malloc(sizeof(char*) * 100);
    gen->function_addrs = malloc(sizeof(int) * 100);
    gen->function_count = 0;
}

// Add an instruction to the code
void codegen_add(CodeGen *gen, Opcode opcode, int operand) {
    if (gen->code_size >= gen->code_capacity) {
        gen->code_capacity *= 2;
        gen->code = realloc(gen->code, sizeof(Instruction) * gen->code_capacity);
    }
    
    gen->code[gen->code_size].opcode = opcode;
    gen->code[gen->code_size].operand = operand;
    gen->code_size++;
}

// Get the current instruction address
int codegen_get_address(CodeGen *gen) {
    return gen->code_size;
}

// Patch the operand of an instruction at a specific address
void codegen_patch(CodeGen *gen, int addr, int operand) {
    gen->code[addr].operand = operand;
}

// Generate code for an expression
void codegen_expression(CodeGen *gen, ASTNode *node) {
    switch (node->type) {
        case NODE_INTEGER:
            codegen_add(gen, OP_PUSH, node->value.integer.value);
            break;
            
        case NODE_IDENTIFIER: {
            // Check if it's a local or global variable
            Symbol *symbol = NULL;
            if (gen->locals != NULL) {
                symbol = symbol_table_lookup(gen->locals, node->value.identifier.name);
            }
            
            if (symbol != NULL) {
                // Local variable
                int offset = symbol->next == NULL ? 0 : symbol->next->next == NULL ? 1 : 2;
                codegen_add(gen, OP_LOAD, offset);
            } else {
                // Global variable
                Symbol *global = symbol_table_lookup(gen->globals, node->value.identifier.name);
                codegen_add(gen, OP_LOAD, global->next == NULL ? 0 : 1);
            }
            break;
        }
        
        case NODE_BINARY_OP:
            // Generate code for left and right operands
            codegen_expression(gen, node->value.binary_op.left);
            codegen_expression(gen, node->value.binary_op.right);
            
            // Generate operation code
            switch (node->value.binary_op.operator) {
                case '+': codegen_add(gen, OP_ADD, 0); break;
                case '-': codegen_add(gen, OP_SUB, 0); break;
                case '*': codegen_add(gen, OP_MUL, 0); break;
                case '/': codegen_add(gen, OP_DIV, 0); break;
                case '%': codegen_add(gen, OP_MOD, 0); break;
                case 'E': codegen_add(gen, OP_EQ, 0); break;  // ==
                case 'N': codegen_add(gen, OP_NE, 0); break;  // !=
                case '<': codegen_add(gen, OP_LT, 0); break;
                case '>': codegen_add(gen, OP_GT, 0); break;
                case 'L': codegen_add(gen, OP_LE, 0); break;  // <=
                case 'G': codegen_add(gen, OP_GE, 0); break;  // >=
                case 'A': codegen_add(gen, OP_AND, 0); break; // &&
                case 'O': codegen_add(gen, OP_OR, 0); break;  // ||
            }
            break;
            
        case NODE_UNARY_OP:
            // Generate code for operand
            codegen_expression(gen, node->value.unary_op.operand);
            
            // Generate operation code
            switch (node->value.unary_op.operator) {
                case '-': codegen_add(gen, OP_NEG, 0); break;
                case '!': codegen_add(gen, OP_NOT, 0); break;
            }
            break;
            
        case NODE_ASSIGNMENT:
            // Generate code for value
            codegen_expression(gen, node->value.assignment.value);
            
            // Store to variable
            char *var_name = node->value.assignment.target->value.identifier.name;
            
            // Check if it's a local or global variable
            Symbol *symbol = NULL;
            if (gen->locals != NULL) {
                symbol = symbol_table_lookup(gen->locals, var_name);
            }
            
            if (symbol != NULL) {
                // Local variable
                int offset = symbol->next == NULL ? 0 : symbol->next->next == NULL ? 1 : 2;
                codegen_add(gen, OP_STORE, offset);
            } else {
                // Global variable
                Symbol *global = symbol_table_lookup(gen->globals, var_name);
                codegen_add(gen, OP_STORE, global->next == NULL ? 0 : 1);
            }
            break;
            
        case NODE_CALL:
            // Generate code for arguments
            for (int i = 0; i < node->value.call.argument_count; i++) {
                codegen_expression(gen, node->value.call.arguments[i]);
            }
            
            // Find function address
            int addr = -1;
            for (int i = 0; i < gen->function_count; i++) {
                if (strcmp(gen->function_names[i], node->value.call.name) == 0) {
                    addr = gen->function_addrs[i];
                    break;
                }
            }
            
            if (addr == -1) {
                // For builtin functions like 'print'
                if (strcmp(node->value.call.name, "print") == 0) {
                    codegen_add(gen, OP_PRINT, 0);
                } else {
                    fprintf(stderr, "Function not found: %s\n", node->value.call.name);
                    exit(1);
                }
            } else {
                codegen_add(gen, OP_CALL, addr);
            }
            break;
            
        default:
            fprintf(stderr, "Unsupported node type in codegen_expression\n");
            exit(1);
    }
}

// Generate code for a statement
void codegen_statement(CodeGen *gen, ASTNode *node) {
    switch (node->type) {
        case NODE_BLOCK: {
            // Create a new symbol table for the block
            SymbolTable *parent = gen->locals;
            gen->locals = symbol_table_create(parent);
            
            // Generate code for each statement in the block
            for (int i = 0; i < node->value.block.statement_count; i++) {
                codegen_statement(gen, node->value.block.statements[i]);
            }
            
            // Restore the parent symbol table
            gen->locals = parent;
            break;
        }
        
        case NODE_DECLARATION: {
            // Add variable to symbol table
            symbol_table_add(gen->locals == NULL ? gen->globals : gen->locals,
                           node->value.declaration.name, node->value.declaration.type,
                           SYMBOL_VARIABLE);
            
            // Generate code for initializer if present
            if (node->value.declaration.initializer != NULL) {
                //codegen_expression(gen, node->value.declaration);
                codegen_expression(gen, node->value.declaration.initializer);
            }
            break;
        }
        
        case NODE_ASSIGNMENT: {
            // Generate code for the right-hand side expression
            codegen_expression(gen, node->value.assignment.value);
            
            // Store the result in the variable
            if (node->value.assignment.target->type == NODE_IDENTIFIER) {
                const char *name = node->value.assignment.target->value.identifier.name;
                Symbol *symbol = symbol_table_lookup(gen->locals, name);
                if (symbol == NULL) {
                    symbol = symbol_table_lookup(gen->globals, name);
                    if (symbol == NULL) {
                        fprintf(stderr, "Undefined variable: %s\n", name);
                        exit(1);
                    }
                    codegen_add(gen, OP_STORE, symbol->offset);
                } else {
                    codegen_add(gen, OP_STORE, symbol->offset);
                }
            } else {
                fprintf(stderr, "Invalid assignment target\n");
                exit(1);
            }
            break;
        }
        
        case NODE_IF: {
            // Generate code for condition
            codegen_expression(gen, node->value.if_stmt.condition);
            
            // Jump to else branch if condition is false
            int jmp_to_else = codegen_get_address(gen);
            codegen_add(gen, OP_JMPF, 0);  // Placeholder address
            
            // Generate code for then branch
            codegen_statement(gen, node->value.if_stmt.then_branch);
            
            // Jump over else branch
            int jmp_over_else = codegen_get_address(gen);
            codegen_add(gen, OP_JMP, 0);  // Placeholder address
            
            // Patch jump to else
            codegen_patch(gen, jmp_to_else, codegen_get_address(gen));
            
            // Generate code for else branch if present
            if (node->value.if_stmt.else_branch != NULL) {
                codegen_statement(gen, node->value.if_stmt.else_branch);
            }
            
            // Patch jump over else
            codegen_patch(gen, jmp_over_else, codegen_get_address(gen));
            break;
        }
        
        case NODE_WHILE: {
            // Save start of loop address
            int loop_start = codegen_get_address(gen);
            
            // Generate code for condition
            codegen_expression(gen, node->value.while_loop.condition);
            
            // Jump out of loop if condition is false
            int jmp_out = codegen_get_address(gen);
            codegen_add(gen, OP_JMPF, 0);  // Placeholder address
            
            // Generate code for loop body
            codegen_statement(gen, node->value.while_loop.body);
            
            // Jump back to start of loop
            codegen_add(gen, OP_JMP, loop_start);
            
            // Patch jump out of loop
            codegen_patch(gen, jmp_out, codegen_get_address(gen));
            break;
        }
        
        case NODE_FOR: {
            // Generate code for initializer
            if (node->value.for_loop.initializer != NULL) {
                codegen_statement(gen, node->value.for_loop.initializer);
            }
            
            // Save start of loop address
            int loop_start = codegen_get_address(gen);
            
            // Generate code for condition
            if (node->value.for_loop.condition != NULL) {
                codegen_expression(gen, node->value.for_loop.condition);
            } else {
                // If no condition, use true (non-zero)
                codegen_add(gen, OP_PUSH, 1);
            }
            
            // Jump out of loop if condition is false
            int jmp_out = codegen_get_address(gen);
            codegen_add(gen, OP_JMPF, 0);  // Placeholder address
            
            // Generate code for loop body
            codegen_statement(gen, node->value.for_loop.body);
            
            // Generate code for increment
            if (node->value.for_loop.increment != NULL) {
                codegen_expression(gen, node->value.for_loop.increment);
                // Discard the result of the increment expression
                // (We could add a POP instruction for this)
            }
            
            // Jump back to start of loop
            codegen_add(gen, OP_JMP, loop_start);
            
            // Patch jump out of loop
            codegen_patch(gen, jmp_out, codegen_get_address(gen));
            break;
        }
        
        case NODE_RETURN: {
            // Generate code for return value
            if (node->value.return_stmt.value != NULL) {
                codegen_expression(gen, node->value.return_stmt.value);
            } else {
                // Return 0 by default
                codegen_add(gen, OP_PUSH, 0);
            }
            
            // Return from function
            codegen_add(gen, OP_RET, 0);
            break;
        }
        //case NODE_EXPRESSION:
        default:
            // Assume it's an expression statement
            codegen_expression(gen, node);
            break;
            
        //default:
        //    fprintf(stderr, "Unsupported node type in codegen_statement\n");
        //    exit(1);
    }
}

// Generate code for a function
void codegen_function(CodeGen *gen, ASTNode *node) {
    // Save the function address
    int func_addr = codegen_get_address(gen);
    
    // Add function to the function table
    gen->function_names = realloc(gen->function_names, (gen->function_count + 1) * sizeof(char *));
    gen->function_addrs = realloc(gen->function_addrs, (gen->function_count + 1) * sizeof(int));
    gen->function_names[gen->function_count] = strdup(node->value.function.name);
    gen->function_addrs[gen->function_count] = func_addr;
    gen->function_count++;
    
    // Create a new symbol table for local variables
    SymbolTable *old_locals = gen->locals;
    gen->locals = symbol_table_create(gen->globals);
    
    // Reset local offset for parameters and local variables
    int old_offset = gen->local_offset;
    gen->local_offset = 0;
    
    // Add parameters to the symbol table
    for (int i = 0; i < node->value.function.parameter_count; i++) {
        ASTNode *param = node->value.function.parameters[i];
        symbol_table_add(gen->locals, param->value.declaration.name, 
                        param->value.declaration.type, SYMBOL_VARIABLE);
        Symbol *symbol = symbol_table_lookup(gen->locals, param->value.declaration.name);
        symbol->offset = gen->local_offset++;
    }
    
    // Generate code for the function body
    codegen_statement(gen, node->value.function.body);
    
    // Add a default return if not present
    // Check if the last instruction is a return
    if (gen->code_size == 0 || gen->code[gen->code_size - 1].opcode != OP_RET) {
        codegen_add(gen, OP_PUSH, 0);  // Default return value
        codegen_add(gen, OP_RET, 0);
    }
    
    // Restore the previous symbol table and local offset
    gen->locals = old_locals;
    gen->local_offset = old_offset;
}

// Generate code for the entire program
void codegen_program(CodeGen *gen, ASTNode *node) {
    // Initialize the code generator
    codegen_init(gen);
    
    // Create global symbol table
    gen->globals = symbol_table_create(NULL);
    
    // Generate code for each function
    for (int i = 0; i < node->value.program.function_count; i++) {
        codegen_function(gen, node->value.program.functions[i]);
    }
}

// Virtual Machine implementation
typedef struct {
    int *stack;
    int stack_size;
    int stack_capacity;
    int pc;  // Program counter
    int fp;  // Frame pointer
} VM;

// Initialize the virtual machine
void vm_init(VM *vm) {
    vm->stack_capacity = 1024;
    vm->stack = malloc(vm->stack_capacity * sizeof(int));
    vm->stack_size = 0;
    vm->pc = 0;
    vm->fp = 0;
}

// Push a value onto the stack
void vm_push(VM *vm, int value) {
    if (vm->stack_size >= vm->stack_capacity) {
        vm->stack_capacity *= 2;
        vm->stack = realloc(vm->stack, vm->stack_capacity * sizeof(int));
    }
    vm->stack[vm->stack_size++] = value;
}

// Pop a value from the stack
int vm_pop(VM *vm) {
    if (vm->stack_size <= 0) {
        fprintf(stderr, "Stack underflow\n");
        exit(1);
    }
    return vm->stack[--vm->stack_size];
}

// Execute the bytecode
void vm_execute(VM *vm, CodeGen *gen) {
    // Start execution at the main function
    for (int i = 0; i < gen->function_count; i++) {
        if (strcmp(gen->function_names[i], "main") == 0) {
            vm->pc = gen->function_addrs[i];
            break;
        }
    }
    
    while (vm->pc < gen->code_size) {
        Instruction instr = gen->code[vm->pc++];
        
        switch (instr.opcode) {
            case OP_PUSH:
                vm_push(vm, instr.operand);
                break;
                
            case OP_LOAD: {
                int offset = instr.operand;
                vm_push(vm, vm->stack[vm->fp + offset]);
                break;
            }
            
            case OP_STORE: {
                int offset = instr.operand;
                vm->stack[vm->fp + offset] = vm_pop(vm);
                break;
            }
            
            case OP_ADD: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a + b);
                break;
            }
            
            case OP_SUB: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a - b);
                break;
            }
            
            case OP_MUL: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a * b);
                break;
            }
            
            case OP_DIV: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                if (b == 0) {
                    fprintf(stderr, "Division by zero\n");
                    exit(1);
                }
                vm_push(vm, a / b);
                break;
            }
            
            case OP_MOD: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                if (b == 0) {
                    fprintf(stderr, "Modulo by zero\n");
                    exit(1);
                }
                vm_push(vm, a % b);
                break;
            }
            
            case OP_EQ: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a == b ? 1 : 0);
                break;
            }
            
            case OP_NE: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a != b ? 1 : 0);
                break;
            }
            
            case OP_LT: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a < b ? 1 : 0);
                break;
            }
            
            case OP_LE: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a <= b ? 1 : 0);
                break;
            }
            
            case OP_GT: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a > b ? 1 : 0);
                break;
            }
            
            case OP_GE: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a >= b ? 1 : 0);
                break;
            }
            
            case OP_AND: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a && b ? 1 : 0);
                break;
            }
            
            case OP_OR: {
                int b = vm_pop(vm);
                int a = vm_pop(vm);
                vm_push(vm, a || b ? 1 : 0);
                break;
            }
            
            case OP_NOT: {
                int a = vm_pop(vm);
                vm_push(vm, !a ? 1 : 0);
                break;
            }
            
            case OP_NEG: {
                int a = vm_pop(vm);
                vm_push(vm, -a);
                break;
            }
            
            case OP_JMP:
                vm->pc = instr.operand;
                break;
                
            case OP_JMPF: {
                int condition = vm_pop(vm);
                if (!condition) {
                    vm->pc = instr.operand;
                }
                break;
            }
            
            case OP_CALL: {
                int old_fp = vm->fp;
                vm->fp = vm->stack_size;
                
                // Save return address and old frame pointer
                vm_push(vm, vm->pc);
                vm_push(vm, old_fp);
                
                // Jump to function
                vm->pc = instr.operand;
                break;
            }
            
            case OP_RET: {
                int return_value = vm_pop(vm);
                
                // Restore frame pointer and return address
                vm->stack_size = vm->fp;
                
                // Check if we're at the top level (main function)
                if (vm->stack_size <= 0) {
                    // We're returning from main, just exit
                    printf("Program exited with code: %d\n", return_value);
                    return;
                }
                
                int old_fp = vm_pop(vm);
                int return_addr = vm_pop(vm);
                
                // Restore stack frame
                vm->fp = old_fp;
                vm->pc = return_addr;
                
                // Push return value
                vm_push(vm, return_value);
                break;
            }
            
            case OP_PRINT: {
                int value = vm_pop(vm);
                printf("%d\n", value);
                break;
            }
            
            default:
                fprintf(stderr, "Unknown opcode: %d\n", instr.opcode);
                exit(1);
        }
        
        // For expression statements, we need to clean up the stack
        // after each statement to prevent stack overflow/underflow
        if (vm->pc < gen->code_size) {
            Instruction next_instr = gen->code[vm->pc];
            // If the next instruction is a jump or call, don't clean up
            if (next_instr.opcode != OP_JMP && 
                next_instr.opcode != OP_JMPF && 
                next_instr.opcode != OP_CALL && 
                next_instr.opcode != OP_RET) {
                // Check if we just executed an expression that left a value on the stack
                // that isn't going to be used (e.g., a function call with no assignment)
                if (instr.opcode == OP_ADD || instr.opcode == OP_SUB || 
                    instr.opcode == OP_MUL || instr.opcode == OP_DIV || 
                    instr.opcode == OP_MOD || instr.opcode == OP_EQ || 
                    instr.opcode == OP_NE || instr.opcode == OP_LT || 
                    instr.opcode == OP_LE || instr.opcode == OP_GT || 
                    instr.opcode == OP_GE || instr.opcode == OP_AND || 
                    instr.opcode == OP_OR || instr.opcode == OP_NOT || 
                    instr.opcode == OP_NEG || instr.opcode == OP_CALL) {
                    // Pop the result if it's not going to be used
                    if (vm->stack_size > 0) {
                        vm->stack_size--;
                    }
                }
            }
        }
    }
}

// Free resources used by the virtual machine
void vm_cleanup(VM *vm) {
    free(vm->stack);
}

// Main function
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }
    
    // Read source file
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Could not open file: %s\n", argv[1]);
        return 1;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Read file contents
    char *source = malloc(file_size + 1);
    fread(source, 1, file_size, file);
    source[file_size] = '\0';
    fclose(file);
    
    // Initialize lexer
    Lexer lexer;
    lexer_init(&lexer, source);
    
    // Initialize parser
    Parser parser;
    parser_init(&parser, source);
    
    // Parse program
    ASTNode *program = parser_parse_program(&parser);
    
    // Semantic analysis
    semantic_check_program(program);
    
    // Code generation
    CodeGen gen;
    codegen_program(&gen, program);
    
    // Execute bytecode
    VM vm;
    vm_init(&vm);
    vm_execute(&vm, &gen);
    vm_cleanup(&vm);
    
    // Free resources
    // TODO: Free AST, symbol tables, and code generator resources
    
    free(source);
    return 0;
}