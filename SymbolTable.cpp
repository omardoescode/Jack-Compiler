#include "SymbolTable.h"
#include <string>

SymbolTable::SymbolTable () = default;

void SymbolTable::reset () {
    table.clear ();
    count.clear ();
}

void SymbolTable::define (const std::string&& name, const std::string& type, VariableKind var_kind) {
    table[name] = { type, var_kind, count[var_kind]++ };
}

int SymbolTable::get_var_count (VariableKind var_kind) {
    return count[var_kind];
}

std::string SymbolTable::type_of_var (const std::string& var_name) {
    return std::get<0> (table[var_name]);
}
VariableKind SymbolTable::kind_of_var (const std::string& var_name) {
    return std::get<1> (table[var_name]);
}
int SymbolTable::index_of_var (const std::string& var_name) {
    return std::get<2> (table[var_name]);
}
