#include "VMWriter.h"
#include "enums.h"

VMWriter::VMWriter (const std::string& out_file) : out_file (out_file) {
}

void VMWriter::write_push (Segment seg, int value) {
    out_file << "push " << segments[seg] << " " << value << "\n";
}

void VMWriter::write_pop (Segment seg, int value) {
    out_file << "pop " << segments[seg] << " " << value << "\n";
}

void VMWriter::write_arithmetic (Arithmetic ari) {
    out_file << arithmetics[ari] << "\n";
}

void VMWriter::write_label (std::string label) {
    out_file << "label " << label << "\n";
}

void VMWriter::write_goto (std::string label) {
    out_file << "goto " << label << "\n";
}

void VMWriter::write_if (std::string label) {
    out_file << "if-goto " << label << "\n";
}

void VMWriter::write_call (std::string name, int nArgs) {
    out_file << "call " << name << " " << nArgs << "\n";
}

void VMWriter::write_function (std::string name, int nLocals) {
    out_file << "call " << name << " " << nLocals << "\n";
}

void VMWriter::write_return () {
    out_file << "return\n";
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
