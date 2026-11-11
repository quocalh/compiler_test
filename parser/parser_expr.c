#include "parser_expr.h"
#include "parser.h"
#include "../misc/error.h"

Expr* parser_arena_expr_allocate_expression(Status* status, Parser* parser, 
    Expr* expr)
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

Assign* parser_arena_expr_allocate_assign(Status* status, Parser* parser, 
    ExpressionType lvalue_type, const char* _name, Expr* rvalue)
{
    Assign* ass = arena_allocate(status, parser->expression_arena, sizeof(*ass));
    if (!STATUS_OK(status)) return NULL;

    *ass = (Assign){
        .lvalue_type = lvalue_type,
        ._name = _name,
        .rvalue = rvalue,
        .type = ASSIGN,
    };
    return ass;
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

PostFix* parser_arena_expr_allocate_postfix(Status* status, Parser* parser, 
    Expr* array, Expr* index)
{
    PostFix* postfix = arena_allocate(status, parser->expression_arena, sizeof(PostFix));
    if (!STATUS_OK(status)) return NULL;
    
    *postfix = (PostFix){
        .type = POSTFIX, 
        .array = array,
        .index = index,
    };
    return postfix;
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

ListLiteral* parser_arena_expr_allocate_list_literal(Status* status, Parser* parser, 
    Expr* previous, Expr* current)
{
    ListLiteral* list = arena_allocate(status, parser->expression_arena, sizeof(ListLiteral));
    if (!STATUS_OK(status)) return NULL;

    *list = (ListLiteral){
        .type = LIST_LITERAL,
        .previous = previous,
        .current = current
    };    
    return list;
}

Variable* parser_arena_expr_allocate_variable(Status* status, Parser* parser,
    const char* _name)
{
    Variable* variable = arena_allocate(status, parser->expression_arena, sizeof(*variable));
    if (!STATUS_OK(status)) return NULL;

    *variable = (Variable){
        ._name = _name,
        .type = VARIABLE,
    };
    return variable;
}



// build ast tree
// assumes the current token is the first token to process
// on return, the current token points to the next unprocessed token, 
// for next parser unit to process
Expr* parser_expression(Status* status, Parser* parser)
{
    // return parser_equality(status, parser);
    return parser_assignment(status, parser);
}

Expr* parser_assignment(Status* status, Parser* parser)
{
    // get the lvalue
    Expr* lvalue = parser_equality(status, parser);
    if (!STATUS_OK(status)) return NULL;

    Token* token = parser_peek_token(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;

    // consume the equal
    if (token->type == EQUAL)
    {
        parser->current++;

        // allow the a = b = c = 5; syntax
        Expr* rvalue = parser_assignment(status, parser); 
        if (!STATUS_OK(status)) return NULL;

        if (lvalue->type == VARIABLE) 
        {
            const char* name = AS(Variable*, lvalue)->_name;
            ExpressionType lvalue_type = AS(Variable*, lvalue)->type;
            return (Expr*)parser_arena_expr_allocate_assign(status, parser, lvalue_type, name, rvalue);
        }

        SET_STATUS(status, AST_ASSIGN_INVALID_ASSIGNMENT_TARGET,
            "invalid assignment target (invalid lvalue).");

        return NULL;
    }

    return lvalue;
}

// return NULL is justified as the release routine is 
// handled by the parser's expresison_arena
Expr* parser_equality(Status* status, Parser* parser)
{
    Expr* left = parser_comparision(status, parser);
    if (!STATUS_OK(status)) return NULL;

    while (parser->current < parser->_tokens->index)
    {
        Token* token = parser_peek_token(status, parser, parser->current);
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
    
    while (parser->current < parser->_tokens->index)
    {
        Token* token = parser_peek_token(status, parser, parser->current);
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
    
    while (parser->current < parser->_tokens->index)
    {
        Token* token = parser_peek_token(status, parser, parser->current);
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
    
    while (parser->current < parser->_tokens->index)
    {
        Token* token = parser_peek_token(status, parser, parser->current);
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

    Token* token = parser_peek_token(status, parser, parser->current);
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
    Token* token = parser_peek_token(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;
    TokenType type = token->type;
    if (!STATUS_OK(status)) return NULL;

    if (type != LEFT_PAREN)
        return parser_postfix(status, parser);

    // consume the left paren
    parser->current++;

    // consume an expression
    Expr* group;
    group = parser_expression(status, parser);
    if (!STATUS_OK(status)) return NULL;
    
    // consume a right paren and current++ afterward
    token = parser_peek_token(status, parser, parser->current);
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

Expr* parser_postfix(Status* status, Parser* parser)
{
    // arr[5][6]
    Expr* post_fix = parser_primary(status, parser);
    if (!STATUS_OK(status)) return NULL;

    while (parser->current < parser->_tokens->index)
    {
        Token* token = parser_peek_token(status, parser, parser->current);
        if (!STATUS_OK(status)) return NULL;
        
        // consume a left bracket
        if (token->type == LEFT_BRACKET)
        {
            parser->current++;
        }
        else break;

        // consume an another [expression]
        Expr* index = parser_expression(status, parser);
        if (!STATUS_OK(status)) return NULL;

        post_fix = (Expr*)parser_arena_expr_allocate_postfix(status, parser, 
            post_fix, index
        );
        if (!STATUS_OK(status)) return NULL;
        

        // consume a right bracket
        token = parser_peek_token(status, parser, parser->current);
        if (!STATUS_OK(status)) return NULL;

        if (token->type == RIGHT_BRACKET)
            parser->current++; // next unprocessed character
        else {
            SET_STATUS(status, PARSER_PARSE_EXPECT_CHARACTER, 
                "expect a right bracket ']'.");
            return NULL;
        }
    }
    return post_fix;
}

Expr* parser_primary(Status* status, Parser* parser)
{
    Token* token = parser_peek_token(status, parser, parser->current);
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
            parser->current++;
            // borrowing
            _lexeme = token->lexeme;
            break;
        }

        case IDENTIFIER:
        {
            parser->current++;
            Variable* variable = parser_arena_expr_allocate_variable(
                status, parser, token->lexeme);
            return (Expr*) variable;
        }

        case LEFT_BRACKET:
            Expr* list = parser_list_creation(status, parser);
            return list;

        default:
        {
            SET_STATUS(status, PARSER_BUILD_AST_UNKNOWN_LITERAL_TYPE, 
            "parser build ast tree encounter unknown literal type. (tokentype id: %d)", token->type);
            return NULL;
        }
    }


    Literal* literal = parser_arena_expr_allocate_literal(status, parser, 
        _lexeme, token->type);
    if (!STATUS_OK(status)) return NULL;

    return (Expr*)literal;
}

Expr* parser_list_creation(Status* status, Parser* parser)
{
    // [1, 2, 3, 4]
    // [1, 2, 3, 4, ]
    // [, ] is not a valid syntax
    // result
    // (((1, 2), 3), 4)
    

    Token* token;
    // consume a [ 
    token = parser_peek_token(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;
    if (token->type != LEFT_BRACKET) // 100% but just to safecheck here
    {
        SET_STATUS(status, PARSER_PARSE_EXPECT_CHARACTER,
            "(i don't even know you reach here), expect a left bracket '['.");
        return NULL;
    } 
    else
        parser->current++;
    
    // if a right bracket -> return an empty list
    token = parser_peek_token(status, parser, parser->current);
    if (token->type == RIGHT_BRACKET)
    {
        parser->current++; // consume the right bracket
        return (Expr*)parser_arena_expr_allocate_list_literal(status, parser, 
            NULL, NULL
        );
    }

    // get the first element
    Expr* list = parser_expression(status, parser);
    if (!STATUS_OK(status)) return NULL;

    while (parser->current < parser->_tokens->index)
    {

        // consume a coma
        token = parser_peek_token(status, parser, parser->current);
        if (!STATUS_OK(status)) return NULL;
        if (token->type == COMA) parser->current++;
        else break;

        // feature [1, 2, 3, ]
        // if spot a trailing bracket, break also
        token = parser_peek_token(status, parser, parser->current);
        if (!STATUS_OK(status)) return NULL;
        if (token->type == RIGHT_BRACKET) break;

        Expr* element = parser_expression(status, parser);
        if (!STATUS_OK(status)) return NULL;

        list = (Expr*)parser_arena_expr_allocate_list_literal(status, 
            parser, 
            list, // previous
            element // current
        );
        if (!STATUS_OK(status)) return NULL;
    }

    // consume a ]
    token = parser_peek_token(status, parser, parser->current);
    if (!STATUS_OK(status)) return NULL;
    if (token->type != RIGHT_BRACKET)
    {
        SET_STATUS(status, PARSER_PARSE_EXPECT_CHARACTER,
            "expect a right bracket ']'.");
        return NULL;
    } 
    else parser->current++;

    return list;
}