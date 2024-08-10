#pragma once

#include <enums.h>
#include <map>
#include <string>

class SymbolTable {
public:
    typedef struct {
        std::string type;
        VariableKind kind;
        int index;
    } Value;

    typedef std::map<std::string, Value> SymbolTableType;
    typedef SymbolTableType::iterator iterator;
    typedef SymbolTableType::const_iterator const_iterator;
    typedef SymbolTableType::value_type pair;

    SymbolTable ();

    void reset ();
    void define (const std::string& name, const std::string& type, VariableKind var_kind);
    int get_var_count (VariableKind var_kind);
    bool contains (const std::string& name);

    const Value& operator[] (const std::string& key) const;
    const Value& at (const std::string& key) const;

    static Segment kind_to_segment (const SymbolTable& table, const std::string& name);
    static Segment kind_to_segment (const Value& value);


    // Iterators in case needed
    iterator begin () {
        return table.begin ();
    }
    const_iterator begin () const {
        return table.begin ();
    }
    iterator end () {
        return table.end ();
    }
    const_iterator end () const {
        return table.end ();
    }

private:
    SymbolTableType table;
    std::map<VariableKind, int> count;
};
