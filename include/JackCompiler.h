#pragma once
#include <filesystem>
class JackCompiler {
public:
    JackCompiler (std::filesystem::path in_file);

    void compile ();

private:
    void compile_file ();
    void compile_dir ();

private:
    std::filesystem::path in_file;
};
