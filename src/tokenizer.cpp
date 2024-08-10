#include "tokenizer.h"
#include "Error.h"
#include "enums.h"
#include "hack_map.h"
#include <algorithm>
#include <regex>
#include <sstream>
#include <string>

Tokenizer::Tokenizer (std::filesystem::path inp_file, std::shared_ptr<HackMap> hack_map)
: inp_file (inp_file), in_string{ false }, in_comment_block{ false },
  hack_map{ hack_map } {
    tokenize ();
}

bool Tokenizer::has_more_tokens () {
    return (current + 1) < tokens_vec.size ();
}

Token Tokenizer::advance () {
    current++;
    return tokens_vec[current];
}

Token Tokenizer::peek_next (int count) {
    return tokens_vec[count + current];
}

void Tokenizer::tokenize () {
    in_string = false;
    std::string current_line;
    while (std::getline (inp_file, current_line)) {
        if (inp_file.bad ()) {
            throw Error ("Tokenizer::tokenize: Error reading input file");
        }

        trim_codeline (current_line);

        if (current_line.empty ()) {
            continue;
        }

        std::istringstream codeline_stream{ current_line };
        char next;
        std::string value;
        while (codeline_stream.get (next)) {
            if (next == '"') {
                if (in_string) {
                    // End of string literal
                    value += next;
                    process_word (value);
                    value.clear ();
                    in_string = false;
                } else {

                    // Start of string literal
                    if (!value.empty ()) {
                        process_word (value);
                        value.clear ();
                    }
                    value += next;
                    in_string = true;
                }
            } else if (in_string) {
                // Inside a string literal
                value += next;
            } else if (isspace (next) != 0) {
                // Delimiters outside of string literals
                if (!value.empty ()) {
                    process_word (value);
                    value.clear ();
                }
            } else {
                // Regular characters
                value += next;
            }
        }

        // Process any remaining value after end of line
        if (!value.empty ()) {
            process_word (value);
        }
    }
}

void Tokenizer::trim_codeline (std::string& line) {
    // Remove Comment
    auto comment = line.find ("//");
    if (comment != std::string::npos) {
        line.erase (comment);
    }

    // Remove leading and trailing spaces
    const std::regex pattern (R"(^\s+|\s+$)");
    line = std::regex_replace (line, pattern, "");
}

void Tokenizer::process_word (const std::string& word) {
    std::string token;

    if (word.at (0) == '"') {
        identify_type (word, true);
        return;
    }
    for (int i = 0, _end = static_cast<int> (word.length ()); i < _end; i++) {
        const auto& c = word[i];

        if (in_comment_block) {
            if (word.substr (i, 2) == "*/") {
                i++;
                in_comment_block = false;
            }
        } else if (word.substr (i, 2) == "/*") {
            i++;
            in_comment_block = true;
            // handle standalone symbols
        } else if (hack_map->contains_symbol (c)) {
            if (!token.empty ())
                identify_type (token);
            token = "";
            std::string value (1, c);
            identify_type (value);
            // otherwise, keep adding to the token
        } else {
            token += c;
        }
    }

    if (!token.empty () && !in_string)
        identify_type (token);
}

void Tokenizer::identify_type (const std::string& value, bool is_string) {

    if (is_string)
        tokens_vec.push_back ({ value, TokenType::STRING_CONST });
    else if (hack_map->contains_keyword (value))
        tokens_vec.push_back ({ value, TokenType::KEYWORD });
    else if (std::all_of (value.begin (), value.end (), ::isdigit))
        tokens_vec.push_back ({ value, TokenType::INT_CONST });
    else if (value.length () == 1 && hack_map->contains_symbol (value[0]))
        tokens_vec.push_back ({ value, TokenType::SYMBOL });
    else
        tokens_vec.push_back ({ value, TokenType::IDENTIFIER });
}


TokenType Tokenizer::token_type () {
    if (current < 0)
        throw Error ("Tokenizer::token_type: No current token");
    return tokens_vec[current].type;
}
