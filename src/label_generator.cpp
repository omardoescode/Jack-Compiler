#include "label_generator.h"
#include <string>

LabelGenerator::LabelGenerator () = default;
LabelGenerator::LabelGenerator (const std::string& prefix)
: _prefix (prefix), _count (0) {
}

LabelGenerator::LabelGenerator (std::string&& prefix)
: _prefix (std::move (prefix)), _count (0) {
}

std::string LabelGenerator::get_next_label () {
    return _prefix + "." + std::to_string (_count++);
}

void LabelGenerator::reset (const std::string& prefix) {
    _prefix = prefix;
    _count  = 0;
}
