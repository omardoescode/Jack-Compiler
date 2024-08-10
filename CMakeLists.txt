cmake_minimum_required(VERSION 3.8)
project(compiler)

set(HEADERS
    include/JackCompiler.h
    include/SymbolTable.h
    include/tokenizer.h
    include/hack_map.h
    include/compilation_engine.h
    include/vm_writer.h
    include/label_generator.h)

set(SOURCES
    src/JackCompiler.cpp
    src/SymbolTable.cpp
    src/tokenizer.cpp
    src/hack_map.cpp
    src/compilation_engine.cpp
    src/vm_writer.cpp
    src/label_generator.cpp)

add_executable(compiler src/main.cpp ${HEADERS} ${SOURCES})

target_include_directories(compiler PRIVATE ${CMAKE_HOME_DIRECTORY}/include)
