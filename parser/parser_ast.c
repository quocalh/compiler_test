#include "parser_ast.h"
#include "../misc/error.h"

Expr* parser_arena_expr_allocate_expression(Status* status, Parser* parser, Expr* expr)
{
    Expr* expression = arena_allocate(status, parser->expression_arena, sizeof(Expr));
    if (!STATUS_OK(status))
        return NULL;
    
    *expression = (Expr){
        .expr = expr,
        .type = EXPRESSION
    };
    return expression;
}

Binary* parser_arena_expr_allocate_binary(Status* status, Parser* parser, 
    Expr* left, Expr* right, TokenType op)
{
    Binary* bin = arena_allocate(status, parser->expression_arena, sizeof(Binary));
    if (!STATUS_OK(status))
        return NULL;
    
    *bin = (Binary){
        .left = left,
        .right = right,
        .op = op,
        .type = BINARY,
    };
    return bin;
}

Unary* parser_arena_expr_allocate_unary(Status* status, Parser* parser,
    Expr* expr, TokenType op)
{
    Unary* una = arena_allocate(status, parser->expression_arena, sizeof(Unary));
    if (!STATUS_OK(status))
        return NULL;
    
    *una = (Unary){
        .expr = expr,
        .op = op,
        .type = UNARY
    };
    return una;
}

Grouping* parser_arena_expr_allocate_grouping(Status* status, Parser* parser,
    Expr* expr)
{
    Grouping* group = arena_allocate(status, parser->expression_arena, sizeof(Grouping));
    if (!STATUS_OK(status))
        return NULL;
    
    *group = (Grouping){
        .type = GROUPING,
        .expr = expr,
    };
    return group;
}

Literal* parser_arena_expr_allocate_literal(Status* status, Parser* parser, 
    const char* _lexeme, TokenType dtype)
{
    Literal* lit = arena_allocate(status, parser->expression_arena, sizeof(Literal));
    if (!STATUS_OK(status))
        return NULL;

    *lit = (Literal){
        ._lexeme = _lexeme,
        .dtype = dtype,
        .type = LITERAL
    };
    return lit;
}

// helper functions
static Token* parser_peek(Status* status, Parser* parser, size_t index)
{
    if (index >= parser->_tokens->length)
    {
        SET_STATUS(status, INDEX_ERROR, 
        "index error (index of %zu, exceeding the allocated of %zu + 1)", 
        index, parser->_tokens->length);
        return NULL;
    }

    return (Token*)parser->_tokens->ptr + index;
}

// build ast tree
// assumes the current token is the first token to process
// on return, the current token points to the next unprocessed token, 
// for next parser unit to process
Expr* parser_expression(Status* status, Parser* parser)
{
    return parser_equality(status, parser);
}

// return NULL is justified as the release routine is 
// handled by the parser's expresison_arena
Expr* parser_equality(Status* status, Parser* parser)
{
    Expr* left = parser_comparision(status, parser);
    if (!STATUS_OK(status)) return NULL;

    while (parser->current < parser->_tokens->length)
    {
        Token* token = parser_peek(status, parser, parser->current);
        if (!STATUS_OK(status)) return NULL;
        TokenType op = token->type;

        if (op == EQUAL_EQUAL ||
            op == BANG_EQUAL)
            parser->current++;
        else
            break;
        
        Expr* right = parser_comparision(status, parser);
        if (!STATUS_OK(status))
            return NULL;

        Binary* binary = parser_arena_expr_allocate_binary(status, parser, 
            left, 
            right,
            op
        );
        if (!STATUS_OK(status)) return NULL;

        left = (Expr*)binary;
    }

    return left;
}

Expr* parser_comparision(Status* status, Parser* parser)
{
    Expr* left = parser_term(status, parser);
    if (!STATUS_OK(status)) return NULL;
    
    while (parser->current < parser->_tokens->length)
    {
        Token* token = parser_peek(status, parser, parser->current);
        if (!STATUS_OK(status)) return NULL;
        TokenType op = token->type;
        
        if (op == GREATER_EQUAL ||
            op == LESS_EQUAL ||
            op == GREATER ||
            op == LESS)
            parser->current++;
        else
            break;
        
        Expr* right = parser_term(status, parser);
        if (!STATUS_OK(status)) return NULL;

        left = (Expr*)parser_arena_expr_allocate_binary(status, parser, 
            left, 
            right,
            op
        );
        if (!STATUS_OK(status)) return NULL;
    }

    return left;
}

Expr* parser_term(Status* status, Parser* parser)
{
    Expr* left = parser_factor(status, parser);
    if (!STATUS_OK(status)) return NULL;
    
    while (parser->current < parser->_tokens->length)
    {
        Token* token = parser_peek(status, parser, parser->current);
        if (!STATUS_OK(status)) return NULL;
        TokenType op = token->type;

        if (op == PLUS ||
            op == SIGN)
            parser->current++;
        else
            break;
        
        Expr* right = parser_factor(status, parser);
        if (!STATUS_OK(status)) return NULL;
        
        left = (Expr*)parser_arena_expr_allocate_binary(status, parser, 
            left,
            right,
            op
        );
        if (!STATUS_OK(status)) return NULL;

    }
    return left;
}

Expr* parser_factor(Status* status, Parser* parser)
{
    Expr* left = parser_unary(status, parser);
    if (!STATUS_OK(status)) return NULL;
    
    while (parser->current < parser->_tokens->length)
    {
        Token* token = parser_peek(status, parser, parser->current);
        if (!STATUS_OK(status)) return NULL;
        TokenType op = token->type;
        
        if (op == STAR || 
            op == SLASH) 
            parser->current++;
        else 
            break;
        
        Expr* right = parser_unary(status, parser);
        if (!STATUS_OK(status)) return NULL;

        left = (Expr*)parser_arena_expr_allocate_binary(status, parser,
            left, 
            right,
            op
        );
        if (!STATUS_OK(status)) return NULL;
    }

    return left;
}

// until now the unary type is always SIGN (e.g. -5)
Expr* parser_unary(Status* status, Parser* parser)
{
    Expr* unary;

    Token* token = parser_peek(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;
    TokenType type = token->type;

    if (type == SIGN ||
        type == BANG) 
    {
        // consume the sign
        parser->current++;
        
        // comsume the next expression
        Expr* expr = parser_unary(status, parser);
        if (!STATUS_OK(status)) return NULL;

        // to handle --5 (unary in unary)
        unary = (Expr*)parser_arena_expr_allocate_unary(status, parser, 
            expr, type);
        if (!STATUS_OK(status)) return NULL;
    }
    else
    {
        unary = (Expr*)parser_grouping(status, parser);
    }
    return unary;
}

Expr* parser_grouping(Status* status, Parser* parser)
{
    Token* token = parser_peek(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;
    TokenType type = token->type;
    if (!STATUS_OK(status)) return NULL;

    if (type != LEFT_PAREN)
        return parser_primary(status, parser);

    // consume the left paren
    parser->current++;

    // consume an expression
    Expr* group;
    group = parser_expression(status, parser);
    if (!STATUS_OK(status)) return NULL;
    
    // consume a right paren and current++ afterward
    token = parser_peek(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;
    type = token->type;
    if (!STATUS_OK(status)) return NULL;

    if (type != RIGHT_PAREN)
    {
        SET_STATUS(status, PARSER_BUILD_AST_EXPECT_A_RIGHT_PAREN,
            "expect a right paren."); return NULL;
    }
    parser->current++;

    return group;
}

Expr* parser_primary(Status* status, Parser* parser)
{
    Token* token = parser_peek(status, parser, parser->current);
    if (!STATUS_OK(status))
        return NULL;

    char* _lexeme;

    switch (token->type)
    {
        case INT:
        case DOUBLE:
        case STRING:
        case TRUE:
        case FALSE:
        case NIL:
        {
            // borrowing
            _lexeme = token->lexeme;
            break;
        }

        default:
        {
            SET_STATUS(status, PARSER_BUILD_AST_UNKNOWN_LITERAL_TYPE, 
            "parser build ast tree encounter unknown literal type. (tokentype id: %d)", token->type);
            return NULL;
        }
    }

    parser->current++;

    Literal* literal = parser_arena_expr_allocate_literal(status, parser, 
        _lexeme, token->type);
    if (!STATUS_OK(status)) return NULL;

    return (Expr*)literal;
}