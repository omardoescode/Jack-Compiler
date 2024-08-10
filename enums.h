#pragma once

enum class TokenType { KEYWORD, SYMBOL, INT_CONST, STRING_CONST, IDENTIFIER };

enum class Keyword {
    CLASS,
    METHOD,
    CONSTRUCTOR,
    FUNCTION,
    INT,
    BOOL,
    CHAR,
    VOID,
    VAR,
    STATIC,
    FIELD,
    LET,
    DO,
    IF,
    ELSE,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    NULL_keyword,
    THIS,
};

enum class NonTerminalRules {
    CLASS,
    CLASS_VAR_DEC,
    SUBROUTINE_DEC,
    SUBROUTINE_BODY,
    VAR_DEC,
    STATEMENTS,
    LET_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    RETURN_STATEMENT,
    DO_STATEMENT,
    PARAMETER_LIST,
    EXPRESSION_LIST,
    TERM,
    EXPRESSION,
};

enum class VariableKind { FIELD, STATIC, ARG, LOCAL };

enum class Segment { CONST, ARG, LOCAL, STATIC, THIS, THAT, POINTER, TEMP };

enum class Arithmetic { ADD, SUB, NEG, EQ, GT, LT, AND, OR, NOT };
