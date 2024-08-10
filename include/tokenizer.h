#pragma once
#include "Token.h"
#include "enums.h"
#include "hack_map.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
class Tokenizer {
public:
    Tokenizer (std::filesystem::path, std::shared_ptr<HackMap>);

    bool has_more_tokens ();
    Token advance ();
    Token peek_next (int count = 1);
    TokenType token_type ();

private:
    void tokenize ();
    void trim_codeline (std::string&);
    void process_word (const std::string&);
    void identify_type (const std::string&, bool is_string = false);

private:
    std::vector<Token> tokens_vec;
    std::ifstream inp_file;
    bool in_comment_block, in_string;
    std::shared_ptr<HackMap> hack_map;
    int current = -1;
};
