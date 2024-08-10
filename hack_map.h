#pragma once
#include "enums.h"
#include <map>
#include <string>
#include <vector>
class HackMap {
public:
    std::string get_keyword (Keyword);
    Keyword get_keyword (const std::string&);

    std::string get_token (TokenType);

    bool contains_keyword (std::string);

    bool contains_keyword_constant (std::string);

    bool contains_symbol (char);

    bool contains_binary_operator (char);
    bool contains_unary_operator (char);

    std::string get_variable_kind (VariableKind);
    VariableKind get_variable_kind (std::string);

    std::string get_segment (Segment);
    Segment get_segment (std::string);

private:
    template <typename T> bool contains (T value, std::vector<T> values);
    template <typename T, typename D>
    T lookup_key (std::map<T, D> map, D target, std::string&& error_emssage);
    template <typename T, typename D>
    D lookup_value (std::map<T, D> map, T key);


private:
    static std::map<Keyword, std::string> keywords;
    static std::vector<std::string> keyword_constants;
    static std::map<TokenType, std::string> tokens; // This is just used for debugging
    static std::vector<char> symbols;
    static std::vector<char> binary_operators;
    static std::vector<char> unary_operators;
    static std::map<VariableKind, std::string> variable_kinds;
    static std::map<Segment, std::string> segments;
};
