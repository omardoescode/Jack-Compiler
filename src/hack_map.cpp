#include "hack_map.h"
#include "Error.h"
#include <algorithm>
#include <utility>


std::string HackMap::get_keyword (Keyword value) {
    return keywords[value];
}

std::string HackMap::get_token (TokenType value) {
    return tokens[value];
}

template <typename T> bool HackMap::contains (T target, std::vector<T> values) {
    return std::any_of (values.begin (), values.end (),
    [&] (const auto& value) { return value == target; });
}

template <typename T, typename D>
T HackMap::lookup_key (std::map<T, D> map, D target, std::string&& error_message) {
    for (auto& [key, value] : map)
        if (value == target)
            return key;

    throw Error (std::move (error_message));
}
template <typename T, typename D>
D HackMap::lookup_value (std::map<T, D> map, T key) {
    return map[key];
}

Keyword HackMap::get_keyword (const std::string& target) {
    return lookup_key (keywords, target, "HackMap::get_keyword: Keyword not found");
}

bool HackMap::contains_keyword (std::string target) {
    return std::any_of (keywords.begin (), keywords.end (),
    [&] (const auto& pair) { return pair.second == target; });
}

bool HackMap::contains_symbol (char test) {
    return contains (test, symbols);
}

bool HackMap::contains_binary_operator (char test) {
    return contains (test, binary_operators);
}

bool HackMap::contains_unary_operator (char test) {
    return contains (test, unary_operators);
}
bool HackMap::contains_keyword_constant (std::string target) {
    return contains (std::move (target), keyword_constants);
}


std::string HackMap::get_variable_kind (VariableKind kind) {
    return lookup_value (variable_kinds, kind);
}

VariableKind HackMap::get_variable_kind (std::string target) {
    return lookup_key (variable_kinds, target, "HackMap::get_variable_kind: target not found");
}

std::string HackMap::get_segment (Segment seg) {
    return lookup_value (segments, seg);
}

Segment HackMap::get_segment (std::string target) {
    return lookup_key (segments, target, "HackMap::get_segment: target not found");
}

std::map<Keyword, std::string> HackMap::keywords = {
    { Keyword::CLASS, "class" },
    { Keyword::METHOD, "method" },
    { Keyword::FUNCTION, "function" },
    { Keyword::CONSTRUCTOR, "constructor" },
    { Keyword::INT, "int" },
    { Keyword::BOOL, "bool" },
    { Keyword::CHAR, "char" },
    { Keyword::VOID, "void" },
    { Keyword::VAR, "var" },
    { Keyword::STATIC, "static" },
    { Keyword::FIELD, "field" },
    { Keyword::LET, "let" },
    { Keyword::DO, "do" },
    { Keyword::IF, "if" },
    { Keyword::ELSE, "else" },
    { Keyword::WHILE, "while" },
    { Keyword::RETURN, "return" },
    { Keyword::TRUE, "true" },
    { Keyword::FALSE, "false" },
    { Keyword::THIS, "this" },
    { Keyword::NULL_keyword, "null" },
};

std::map<TokenType, std::string> HackMap::tokens = {
    { TokenType::SYMBOL, "symbol" },
    { TokenType::IDENTIFIER, "identifier" },
    { TokenType::KEYWORD, "keyword" },
    { TokenType::INT_CONST, "int_constant" },
    { TokenType::STRING_CONST, "string_constant" },
};

std::vector<char> HackMap::symbols = { '(', ')', '{', '}', '[', ']', ',', ';',
    '.', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~' };

std::vector<char> HackMap::binary_operators = { '+', '-', '*', '/', '&', '|', '<', '>', '=' };

std::vector<char> HackMap::unary_operators = { '-', '~' };

std::vector<std::string> HackMap::keyword_constants = { "true", "false", "null", "this" };

std::map<VariableKind, std::string> HackMap::variable_kinds = {
    { VariableKind::ARG, "" },
    { VariableKind::LOCAL, "var" },
    { VariableKind::FIELD, "field" },
    { VariableKind::STATIC, "STATIC" },
};

std::map<Segment, std::string> HackMap::segments = {
    { Segment::ARG, "argument" },
    { Segment::LOCAL, "local" },
    { Segment::CONST, "constant" },
    { Segment::THAT, "that" },
    { Segment::THIS, "this" },
    { Segment::POINTER, "pointer" },
    { Segment::TEMP, "temp" },
    { Segment::STATIC, "static" },
};
