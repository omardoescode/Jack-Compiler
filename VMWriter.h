#pragma once
#include "enums.h"
#include <fstream>
#include <map>
#include <string>
class VMWriter {
public:
    VMWriter (const std::string out_file);

    void write_push (Segment seg, int value);
    void write_pop (Segment seg, int value);
    void write_arithmetic (Arithmetic ari);
    void write_label (std::string label);
    void write_if (std::string label);
    void write_goto (std::string label);
    void write_call (std::string name, int nArgs);
    void write_function (std::string name, int nLocals);
    void write_return ();

private:
    std::ofstream out_file;
    static std::map<Segment, std::string> segments;
    static std::map<Arithmetic, std::string> arithmetics;
};
