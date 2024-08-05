cmake_minimum_required(VERSION 3.8)
project(compiler)

set(HEADERS
    JackCompiler.h SymbolTable.h analyzer/tokenizer.h analyzer/hack_map.h
    analyzer/compilation_engine.h analyzer/OutputEngine.h)
set(SOURCES
    JackCompiler.cpp SymbolTable.cpp analyzer/tokenizer.cpp
    analyzer/hack_map.cpp analyzer/compilation_engine.cpp
    analyzer/OutputEngine.h)

add_executable(compiler main.cpp ${HEADERS} ${SOURCES})
