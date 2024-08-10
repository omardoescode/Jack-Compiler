cmake_minimum_required(VERSION 3.8)
project(compiler)

set(HEADERS
    JackCompiler.h
    SymbolTable.h
    tokenizer.h
    hack_map.h
    compilation_engine.h
    vm_writer.h
    label_generator.h)

set(SOURCES
    JackCompiler.cpp
    SymbolTable.cpp
    tokenizer.cpp
    hack_map.cpp
    compilation_engine.cpp
    vm_writer.cpp
    label_generator.cpp)

add_executable(compiler main.cpp ${HEADERS} ${SOURCES})

target_include_directories(compiler PRIVATE ${CMAKE_HOME_DIRECTORY})
