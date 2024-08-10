#include "SymbolTable.h"
#include "enums.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

SymbolTable::SymbolTable () = default;

void SymbolTable::reset () {
    std::cout << "made it " << std::endl;
    table.clear ();
    count.clear ();
    std::cout << "made it " << std::endl;
}

void SymbolTable::define (const std::string& name, const std::string& type, VariableKind var_kind) {
    table[name] = { type, var_kind, count[var_kind]++ };
}

int SymbolTable::get_var_count (VariableKind var_kind) {
    return count[var_kind];
}


bool SymbolTable::contains (const std::string& name) {
    return std::any_of (
    begin (), end (), [&] (auto& pair) { return pair.first == name; });
}


const SymbolTable::Value& SymbolTable::operator[] (const std::string& key) const {
    auto it = table.find (key);
    if (it != table.end ()) {
        return it->second;
    }

    throw std::out_of_range ("Key not found");
}


const SymbolTable::Value& SymbolTable::at (const std::string& key) const {
    return operator[] (key);
}


Segment SymbolTable::kind_to_segment (const SymbolTable& table, const std::string& name) {
    return kind_to_segment (table[name]);
}

Segment SymbolTable::kind_to_segment (const SymbolTable::Value& value) {
    switch (value.kind) {
    case VariableKind::FIELD:
    case VariableKind::LOCAL: return Segment::LOCAL;
    case VariableKind::STATIC: return Segment::STATIC;
    case VariableKind::ARG: return Segment::ARG;
    }
}
