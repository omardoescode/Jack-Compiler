# Jack Compiler

This is my first ever compiler, compiling the Jack language to the Virtual machine itermediary language, from which we use a VM translator to translate this to Hack assembly language and run right into the machine language

This project is Module's 11 project and the best one so far

# Usage

1. Write a program in the Jack language
2. Clone this repo
3. Build this project

```bash
mkdir build
cmake -S . -B ./build
cmake --build ./build/
```

4. Run the program

```
./build/compiler <path-to-file>
```

# Related projects

- [Jack Syntax Analyzer](https://github.com/omardoescode/Hack-Syntax-Analyzer/): This tool tests the tokenizer and syntax analyzsis to prepare for this compiler
- [Hack VM Translator](https://github.com/omardoescode/hack-vm-translator): This tool compiles from the VM machine language to the hack assembly language
- [Hack Assembler](https://github.com/omardoescode/Hack-Assembler): This tool converts the hack assembly language to hack machine language
