#pragma once

#include "enums.h"
#include <map>
#include <string>
#include <tuple>

class SymbolTable {
public:
    SymbolTable ();

    void reset ();
    void define (const std::string&& name, const std::string& type, VariableKind var_kind);
    int get_var_count (VariableKind var_kind);
    std::string type_of_var (const std::string& var_name);
    VariableKind kind_of_var (const std::string& var_name);
    int index_of_var (const std::string& var_name);

private:
    std::map<std::string, std::tuple<std::string, VariableKind, int>> table;
    std::map<VariableKind, int> count;
};
