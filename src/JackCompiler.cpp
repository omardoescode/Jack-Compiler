#include "JackCompiler.h"
#include "Error.h"
#include "compilation_engine.h"
#include "hack_map.h"
#include <filesystem>
#include <iostream>
#include <memory>

JackCompiler::JackCompiler (std::filesystem::path in_path) : in_file (in_path) {
}

void JackCompiler::compile () {
    if (std::filesystem::is_directory (in_file)) {
        compile_dir ();
    } else {
        compile_file ();
    }
}

void JackCompiler::compile_dir () {
    std::shared_ptr<HackMap> hack_map = std::make_shared<HackMap> ();
    CompilationEngine engine (hack_map);

    bool found_one = false;
    for (auto& file : std::filesystem::recursive_directory_iterator (in_file)) {
        if (file.path ().extension () != ".jack")
            continue;

        found_one = true;

        auto out_file = file.path ();
        out_file.replace_extension (".vm");

        std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer> (file, hack_map);
        std::shared_ptr<VMWriter> vm_writer = std::make_shared<VMWriter> (out_file);

        engine.set_tokenizer (tokenizer);
        engine.set_vm_writer (vm_writer);

        engine.compile ();
    }

    if (!found_one)
        throw Error ("No jack file in directory");
}

void JackCompiler::compile_file () {
    auto out_file = in_file;
    out_file.replace_extension (".vm");

    std::shared_ptr<HackMap> hack_map = std::make_shared<HackMap> ();
    std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer> (in_file, hack_map);
    std::shared_ptr<VMWriter> vm_writer = std::make_shared<VMWriter> (out_file);
    CompilationEngine engine (hack_map);
    engine.set_tokenizer (tokenizer);
    engine.set_vm_writer (vm_writer);

    engine.compile ();
}
