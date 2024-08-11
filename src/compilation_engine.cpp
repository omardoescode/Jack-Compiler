#include "compilation_engine.h"
#include "Error.h"
#include "Token.h"
#include "enums.h"
#include "vm_writer.h"
#include <iostream>
#include <memory>
#include <regex>
#include <string>

/*
 * Decisions for the temp segment
 * temp 0: for the rhs expressions in let in case of the array
 * temp 1: for the strings created
 */

CompilationEngine::CompilationEngine (std::shared_ptr<HackMap> hack_map)
: hack_map (hack_map) {
    label_generator  = std::make_shared<LabelGenerator> ();
    class_table      = std::make_shared<SymbolTable> ();
    subroutine_table = std::make_shared<SymbolTable> ();
}

void CompilationEngine::set_vm_writer (std::shared_ptr<VMWriter> new_writer) {
    vm_writer = new_writer;
}
void CompilationEngine::set_tokenizer (std::shared_ptr<Tokenizer> new_tokenizer) {
    tokenizer = new_tokenizer;
}

void CompilationEngine::compile () {
    compile_class ();
}

Token CompilationEngine::advance (TokenType type) {
    Token token = tokenizer->advance ();

    std::cout << token.value << std::endl;
    if (token.type != type)
        throw Error ("Invalid token type");

    return token;
}

Token CompilationEngine::advance (std::vector<TokenType> types) {
    Token token = tokenizer->advance ();

    std::cout << token.value << std::endl;
    for (auto& type : types) {
        if (type == token.type) {
            return token;
        }
    }

    throw Error ("Invalid type");
}
Token CompilationEngine::advance (TokenType type, const std::string& value) {
    Token token = tokenizer->advance ();

    std::cout << token.value << std::endl;
    if (token.type != type) {
        throw Error ("Invalid token type");
    }

    const std::regex pattern (value);
    if (!std::regex_match (token.value, pattern)) {
        throw Error ("Invalid token value");
    }

    return token;
}

void CompilationEngine::compile_class () {
    class_table->reset ();

    std::cout << "made it here" << std::endl;

    Token token;
    advance (TokenType::KEYWORD, "\\bclass\\b");

    token       = advance (TokenType::IDENTIFIER);
    _class_name = token.value;

    advance (TokenType::SYMBOL, "\\{");

    compile_class_var_dec ();

    compile_subroutine_dec ();
}

void CompilationEngine::compile_class_var_dec () {
    Token token;
    token = tokenizer->peek_next ();
    std::regex pattern ("\\b(static|field)\\b");

    while (std::regex_match (token.value, pattern)) {
        // static|field
        Token kind_token  = advance (TokenType::KEYWORD);
        VariableKind kind = hack_map->get_variable_kind (kind_token.value);

        _compile_multiple_dec (kind);

        advance (TokenType::SYMBOL, ";");

        token = tokenizer->peek_next ();
    }
}


// Write functions
void CompilationEngine::compile_subroutine_dec () {
    subroutine_table->reset ();

    Token token;

    token = tokenizer->peek_next ();
    std::regex pattern ("\\b(constructor|method|function)\\b");

    while (std::regex_match (token.value, pattern)) {
        // constructor|method|function
        Token function_type =
        advance (TokenType::KEYWORD, "\\b(constructor|method|function)\\b");

        // (void|type)
        advance ({ TokenType::IDENTIFIER, TokenType::KEYWORD });

        // subroutine name
        Token function_name = advance (TokenType::IDENTIFIER);
        label_generator->reset (function_name.value);

        // (
        advance (TokenType::SYMBOL, "\\(");

        // parameter_list
        compile_parameter_list ();

        // )
        advance (TokenType::SYMBOL, "\\)");


        // subroutine body
        // {
        advance (TokenType::SYMBOL, "\\{");

        compile_var_dec ();

        // write function
        vm_writer->write_function (
        _class_name + "." + function_name.value, calculate_function_n_locals ());

        if (function_type.value == "constructor") {
            vm_writer->write_push (Segment::CONST, calculate_class_size ());
            vm_writer->write_call ("Memory.alloc", 1);
            vm_writer->write_pop (Segment::POINTER, 0);
        } else if (token.value == "method") {
            vm_writer->write_push (Segment::ARG, 0);
            vm_writer->write_pop (Segment::POINTER, 0);
        }

        compile_statements ();

        // }
        advance (TokenType::SYMBOL, "\\}");


        token = tokenizer->peek_next ();
        // output_engine->close_non_terminal ();
    }
}

void CompilationEngine::compile_var_dec () {
    Token token;

    std::regex pattern ("\\b(var)\\b");
    token = tokenizer->peek_next ();

    while (std::regex_match (token.value, pattern)) {
        // output_engine->write_non_terminal (NonTerminalRules::VAR_DEC);
        // var
        Token kind_token  = advance (TokenType::KEYWORD, "\\bvar\\b");
        VariableKind kind = hack_map->get_variable_kind (kind_token.value);

        // type
        Token type_token = advance ({ TokenType::KEYWORD, TokenType::IDENTIFIER });
        std::string type = type_token.value;

        // name
        Token name_token = advance (TokenType::IDENTIFIER);
        std::string name = name_token.value;

        subroutine_table->define (name, type, kind);

        std::regex pattern (",");
        token = tokenizer->peek_next ();
        while (std::regex_match (token.value, pattern)) {
            // ,
            advance (TokenType::SYMBOL, ",");

            // name
            Token name_token = advance (TokenType::IDENTIFIER);
            std::string name = name_token.value;

            subroutine_table->define (name, type, kind);

            // either , or ;
            token = tokenizer->peek_next ();
        }

        // ;
        advance (TokenType::SYMBOL, ";");

        // output_engine->close_non_terminal ();

        token = tokenizer->peek_next ();
    }
}

void CompilationEngine::compile_statements () {
    Token token;
    std::regex pattern ("\\b(let|do|return|while|if)\\b");

    token = tokenizer->peek_next ();
    while (std::regex_match (token.value, pattern)) {
        if (token.value == "let")
            compile_let ();
        else if (token.value == "if")
            compile_if ();
        else if (token.value == "return")
            compile_return ();
        else if (token.value == "do")
            compile_do ();
        else if (token.value == "while")
            compile_while ();

        token = tokenizer->peek_next ();
    }
}

void CompilationEngine::compile_let () {
    Token token;
    // let
    advance (TokenType::KEYWORD, "let");

    // varName
    Token ident = advance (TokenType::IDENTIFIER);


    token         = tokenizer->peek_next ();
    bool to_index = token.value == "[";
    if (to_index) {
        _push_pop_identifier (ident.value);
        advance (TokenType::SYMBOL, "\\[");
        compile_expression ();
        advance (TokenType::SYMBOL, "\\]");

        vm_writer->write_arithmetic (Arithmetic::ADD);
    }
    // =
    advance (TokenType::SYMBOL, "=");

    // expression
    compile_expression ();

    if (to_index) {
        vm_writer->write_pop (Segment::TEMP, 0);
        vm_writer->write_pop (Segment::POINTER, 1);
        vm_writer->write_push (Segment::TEMP, 0);
        vm_writer->write_pop (Segment::THAT, 0);
    } else {
        _push_pop_identifier (ident.value, 0);
    }

    advance (TokenType::SYMBOL, ";");
}

void CompilationEngine::compile_if () {
    Token token;
    advance (TokenType::KEYWORD, "if");

    advance (TokenType::SYMBOL, "\\(");

    compile_expression ();

    std::string label_1 = label_generator->get_next_label ();
    std::string label_2 = label_generator->get_next_label ();

    vm_writer->write_arithmetic (Arithmetic::NOT);
    vm_writer->write_if (label_1);

    advance (TokenType::SYMBOL, "\\)");

    advance (TokenType::SYMBOL, "\\{");

    compile_statements ();

    vm_writer->write_goto (label_2);

    advance (TokenType::SYMBOL, "\\}");

    token = tokenizer->peek_next ();

    vm_writer->write_label (label_1);

    if (token.value == "else") {
        advance (TokenType::KEYWORD, "else");

        advance (TokenType::SYMBOL, "\\{");

        compile_statements ();

        advance (TokenType::SYMBOL, "\\}");
    }

    vm_writer->write_label (label_2);
}

void CompilationEngine::compile_while () {

    std::string label_1 = label_generator->get_next_label ();
    std::string label_2 = label_generator->get_next_label ();

    vm_writer->write_label (label_1);

    advance (TokenType::KEYWORD, "while");

    advance (TokenType::SYMBOL, "\\(");

    compile_expression ();

    vm_writer->write_arithmetic (Arithmetic::NOT);
    vm_writer->write_if (label_2);

    advance (TokenType::SYMBOL, "\\)");

    advance (TokenType::SYMBOL, "\\{");

    compile_statements ();

    vm_writer->write_goto (label_1);

    advance (TokenType::SYMBOL, "\\}");
    vm_writer->write_label (label_2);
}

void CompilationEngine::compile_do () {
    // output_engine->write_non_terminal (NonTerminalRules::DO_STATEMENT);

    advance (TokenType::KEYWORD, "do");

    Token del = tokenizer->peek_next (2);
    if (del.value == ".") {
        _compile_call_from_outside ();
    } else if (del.value == "(") {
        _compile_call_from_class ();
    }

    // clean up
    vm_writer->write_pop (Segment::TEMP, 0);

    advance (TokenType::SYMBOL, ";");
}

void CompilationEngine::compile_return () {
    Token token;

    advance (TokenType::KEYWORD, "return");

    token = tokenizer->peek_next ();
    if (token.value != ";") {
        compile_expression ();
    } else {
        vm_writer->write_push (Segment::CONST, 0);
    }

    vm_writer->write_return ();

    advance (TokenType::SYMBOL, ";");
}

void CompilationEngine::compile_parameter_list () {

    Token token;
    token = tokenizer->peek_next ();
    while (token.value != ")") {
        Token type_token = advance ({ TokenType::IDENTIFIER, TokenType::KEYWORD });
        std::string type = type_token.value;

        Token name_token = advance (TokenType::IDENTIFIER);
        std::string name = name_token.value;

        subroutine_table->define (name, type, VariableKind::ARG);

        token = tokenizer->peek_next ();

        if (token.value == ",")
            advance (TokenType::SYMBOL, ",");
    }
}

int CompilationEngine::compile_expression_list (std::string end) {
    int count = 0;

    Token token = tokenizer->peek_next ();
    std::regex pattern (end);
    while (!std::regex_match (token.value, pattern)) {
        compile_expression ();

        token = tokenizer->peek_next ();
        if (token.value == ",") {
            advance (TokenType::SYMBOL, "\\,");
        }

        token = tokenizer->peek_next ();
        count++;
    }

    return count;
}
void CompilationEngine::compile_expression () {
    Token token;

    compile_term ();

    token = tokenizer->peek_next ();
    if (token.type == TokenType::SYMBOL &&
    hack_map->contains_binary_operator (token.value.at (0))) {
        // operators
        Token op = advance (TokenType::SYMBOL);

        compile_expression ();

        _compile_binary_operator (op.value);
    }
}

void CompilationEngine::compile_term () {
    Token token;

    token = tokenizer->peek_next ();

    switch (token.type) {
    case TokenType::INT_CONST: {
        Token num = advance (TokenType::INT_CONST);
        vm_writer->write_push (Segment::CONST, std::stoi (num.value));
        break;
    }

    case TokenType::STRING_CONST: _handle_string_const (); break;
    case TokenType::KEYWORD: _handle_keyword_const (); break;
    case TokenType::SYMBOL:
        if (hack_map->contains_unary_operator (token.value.at (0))) {
            token = tokenizer->advance ();
            _compile_unary_operator (token.value);
            compile_expression ();
        } else if (token.value == "(") {
            advance (TokenType::SYMBOL, "\\(");
            compile_expression ();
            advance (TokenType::SYMBOL, "\\)");
        }
        break;
    case TokenType::IDENTIFIER: {
        _compile_identifier ();
    }
    }
}


void CompilationEngine::_compile_multiple_dec (VariableKind kind) {

    Token token;


    Token type_token = advance ({ TokenType::KEYWORD, TokenType::IDENTIFIER });
    std::string type = type_token.value;

    Token name_token = advance (TokenType::IDENTIFIER);
    std::string name = name_token.value;

    class_table->define (name, type, kind);

    token = tokenizer->peek_next ();
    std::regex pattern (",");

    while (std::regex_match (token.value, pattern)) {
        // ,
        advance (TokenType::SYMBOL, ",");

        Token name_token = advance (TokenType::IDENTIFIER);
        std::string name = name_token.value;

        class_table->define (name, type, kind);

        token = tokenizer->peek_next ();
    }
}

void CompilationEngine::_compile_identifier (bool must) {
    Token token;

    Token identifier = tokenizer->peek_next ();
    Token next       = tokenizer->peek_next (2);

    if (next.value == "[")
        _compile_array_item ();
    else if (next.value == "(")
        _compile_call_from_class ();
    else if (next.value == ".")
        _compile_call_from_outside ();
    else
        _compile_var_name ();
}

int CompilationEngine::calculate_function_n_locals () {
    return subroutine_table->get_var_count (VariableKind::LOCAL);
}
int CompilationEngine::calculate_class_size () {
    return class_table->get_var_count (VariableKind::FIELD);
}

void CompilationEngine::_compile_array_item () {
    Token identifier = advance (TokenType::IDENTIFIER);

    _push_pop_identifier (identifier.value);

    advance (TokenType::SYMBOL, "\\[");

    compile_expression ();

    advance (TokenType::SYMBOL, "\\]");

    // Finish code
    vm_writer->write_arithmetic (Arithmetic::ADD);
}

void CompilationEngine::_compile_call_from_class () {
    /*
     * This function has the same `this` so no need to anchor
     */
    Token ident = advance (TokenType::IDENTIFIER);

    advance (TokenType::SYMBOL, "\\(");

    int count = compile_expression_list ("\\)");

    advance (TokenType::SYMBOL, "\\)");

    vm_writer->write_call (_class_name + "." + ident.value, count);
}


/*
 * If the identifier is found, then it's a call to a method.
 * Otherwise, it's a call to a static function
 */
void CompilationEngine::_compile_call_from_outside () {
    Token ident = advance (TokenType::IDENTIFIER);

    advance (TokenType::SYMBOL, ".");

    // Anchor for `this`
    // If not found (return false), do not do a thing and just call the function
    int check = has_identifier (ident.value);
    if (check != 0) {
        _push_pop_identifier (ident.value);
    }

    Token method = advance (TokenType::IDENTIFIER);

    advance (TokenType::SYMBOL, "\\(");

    int count = compile_expression_list ("\\)");

    advance (TokenType::SYMBOL, "\\)");

    if (check == 0)
        vm_writer->write_call (ident.value + "." + method.value, count);
    else if (check == 1)
        vm_writer->write_call (
        subroutine_table->at (ident.value).type + "." + method.value, count + 1);
    else if (check == 2)
        vm_writer->write_call (
        class_table->at (ident.value).type + "." + method.value, count + 1);
}

bool CompilationEngine::_push_pop_identifier (const std::string& ident, bool push) {
    int check = has_identifier (ident);
    if (check == 0)
        return false;
    auto& entry = check == 1 ? subroutine_table->at (ident) : class_table->at (ident);

    push ? vm_writer->write_push (SymbolTable::kind_to_segment (entry), entry.index) :
           vm_writer->write_pop (SymbolTable::kind_to_segment (entry), entry.index);
    return true;
}

void CompilationEngine::_compile_var_name () {
    Token ident = advance (TokenType::IDENTIFIER);

    _push_pop_identifier (ident.value);
}

void CompilationEngine::_handle_string_const () {
    Token string = advance (TokenType::STRING_CONST);

    vm_writer->write_call ("String.new", string.value.size ());
    vm_writer->write_pop (Segment::TEMP, 1);

    for (char& c : string.value) {
        vm_writer->write_push (Segment::TEMP, 1);
        vm_writer->write_push (Segment::CONST, c);

        vm_writer->write_call ("String.appendChar",
        2); // 2 -> for the string (this) and for the character

        // cleanup
        vm_writer->write_pop (Segment::TEMP, 0);
    }

    // push the string after finishing
    vm_writer->write_push (Segment::TEMP, 1);
}

void CompilationEngine::_handle_keyword_const () {
    Token token = tokenizer->peek_next ();
    if (!hack_map->contains_keyword_constant (token.value))
        throw Error ("Invalid keyword");

    token = advance (TokenType::KEYWORD);
    if (token.value == "true") {
        vm_writer->write_push (Segment::CONST, -1);
    } else if (token.value == "false" || token.value == "null") {
        vm_writer->write_push (Segment::CONST, 0);
    } else if (token.value == "this") {
        vm_writer->write_push (Segment::POINTER, 0);
    }
}

void CompilationEngine::_compile_binary_operator (const std::string& op) {
    if (op == "+")
        vm_writer->write_arithmetic (Arithmetic::ADD);
    else if (op == "-")
        vm_writer->write_arithmetic (Arithmetic::SUB);
    else if (op == "*")
        vm_writer->write_call ("Maths.multiply", 2);
    else if (op == "/")
        vm_writer->write_call ("Maths.divide", 2);
    else if (op == "&")
        vm_writer->write_arithmetic (Arithmetic::AND);
    else if (op == "|")
        vm_writer->write_arithmetic (Arithmetic::OR);
    else if (op == ">")
        vm_writer->write_arithmetic (Arithmetic::GT);
    else if (op == "<")
        vm_writer->write_arithmetic (Arithmetic::LT);
    else if (op == "=")
        vm_writer->write_arithmetic (Arithmetic::EQ);
}

void CompilationEngine::_compile_unary_operator (const std::string& op) {
    if (op == "-")
        vm_writer->write_arithmetic (Arithmetic::NEG);
    else if (op == "~")
        vm_writer->write_arithmetic (Arithmetic::NOT);
}

int CompilationEngine::has_identifier (const std::string& ident) {
    if (subroutine_table->contains (ident)) {
        return 1;
    } else if (class_table->contains (ident)) {
        return 2;
    } else {
        return 0;
    }
}
