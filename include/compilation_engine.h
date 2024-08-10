#pragma once

#include "SymbolTable.h"
#include "enums.h"
#include "hack_map.h"
#include "label_generator.h"
#include "tokenizer.h"
#include "vm_writer.h"

class CompilationEngine {
public:
    CompilationEngine (std::shared_ptr<HackMap>);

    // Setters
    void set_tokenizer (std::shared_ptr<Tokenizer>);
    void set_vm_writer (std::shared_ptr<VMWriter>);
    void compile ();

private:
    // Compilers
    void compile_class ();
    void compile_class_var_dec ();
    void compile_subroutine_dec ();
    void compile_parameter_list ();
    void compile_var_dec ();
    void compile_let ();
    void compile_if ();
    void compile_while ();
    void compile_return ();
    void compile_do ();
    void compile_expression ();
    void compile_term ();
    int compile_expression_list (std::string end);
    void compile_statements ();

    // Helpers
    Token advance (TokenType);
    Token advance (std::vector<TokenType>);
    Token advance (TokenType type, const std::string& value);
    void _compile_multiple_dec (VariableKind kind);
    void _compile_identifier (bool must = true);
    void _compile_array_item ();
    void _compile_var_name ();
    void _compile_call_from_class ();
    void _compile_call_from_outside ();

    // Class Specific helpers
    int calculate_class_size ();
    int calculate_function_n_locals ();

    // symbol table operations
    bool _push_pop_identifier (const std::string& ident, bool push = true);
    /*
     * Checks if the idenfier is in subroutine table or class table
     * 0 -> not found
     * 1 -> in subroutine table
     * 2 -> in class table
     */
    int has_identifier (const std::string& identifier);

    void _handle_string_const ();
    void _handle_int_const ();
    void _handle_keyword_const ();
    void _compile_binary_operator (const std::string& op);
    void _compile_unary_operator (const std::string& op);

private:
    std::shared_ptr<Tokenizer> tokenizer;
    std::shared_ptr<HackMap> hack_map;
    std::shared_ptr<VMWriter> vm_writer;
    std::shared_ptr<SymbolTable> class_table;
    std::shared_ptr<SymbolTable> subroutine_table;
    std::shared_ptr<LabelGenerator> label_generator;

    // Info in compilation
    std::string _class_name;
};
