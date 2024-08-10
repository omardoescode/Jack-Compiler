#include "vm_writer.h"
#include "enums.h"

VMWriter::VMWriter (const std::filesystem::path& out_file, int tab_width)
: _out_file (out_file), _tab_width (tab_width) {
}

void VMWriter::write_spaces () {
    int spaces = _tab_width;
    while (spaces--)
        _out_file << " ";
}

void VMWriter::write_push (Segment seg, int value) {
    write_spaces ();
    _out_file << "push " << segments[seg] << " " << value << "\n";
}

void VMWriter::write_pop (Segment seg, int value) {
    write_spaces ();
    _out_file << "pop " << segments[seg] << " " << value << "\n";
}

void VMWriter::write_arithmetic (Arithmetic ari) {
    write_spaces ();
    _out_file << arithmetics[ari] << "\n";
}

void VMWriter::write_label (std::string label) {
    write_spaces ();
    _out_file << "label " << label << "\n";
}

void VMWriter::write_goto (std::string label) {
    write_spaces ();
    _out_file << "goto " << label << "\n";
}

void VMWriter::write_if (std::string label) {
    write_spaces ();
    _out_file << "if-goto " << label << "\n";
}

void VMWriter::write_call (std::string name, int nArgs) {
    write_spaces ();
    _out_file << "call " << name << " " << nArgs << "\n";
}

void VMWriter::write_function (std::string name, int nLocals) {
    _out_file << "function " << name << " " << nLocals << "\n";
}

void VMWriter::write_return () {
    write_spaces ();
    _out_file << "return\n\n";
}

std::map<Segment, std::string> VMWriter::segments = {
    { Segment::POINTER, "pointer" },
    { Segment::LOCAL, "local" },
    { Segment::ARG, "argument" },
    { Segment::THIS, "this" },
    { Segment::THAT, "that" },
    { Segment::STATIC, "static" },
    { Segment::CONST, "constant" },
    { Segment::TEMP, "temp" },
};

std::map<Arithmetic, std::string> VMWriter::arithmetics = {
    { Arithmetic::ADD, "add" },
    { Arithmetic::SUB, "sub" },
    { Arithmetic::NEG, "neg" },
    { Arithmetic::EQ, "eq" },
    { Arithmetic::GT, "gt" },
    { Arithmetic::LT, "lt" },
    { Arithmetic::AND, "and" },
    { Arithmetic::OR, "or" },
    { Arithmetic::NOT, "not" },
};
