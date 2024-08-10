#include "JackCompiler.h"
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
        compile_file (in_file);
    }
}

void JackCompiler::compile_dir () {
    for (auto& file : std::filesystem::recursive_directory_iterator (in_file)) {
        if (file.path ().extension () == ".jack")
            compile_file (file.path ());
    }
}

void JackCompiler::compile_file (std::filesystem::path file) {
    auto out_file = file;
    out_file.replace_extension (".vm");

    std::shared_ptr<HackMap> hack_map = std::make_shared<HackMap> ();
    std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer> (file, hack_map);
    std::shared_ptr<VMWriter> vm_writer = std::make_shared<VMWriter> (out_file);
    CompilationEngine engine (hack_map);
    engine.set_tokenizer (tokenizer);
    engine.set_vm_writer (vm_writer);

    engine.compile ();
}
