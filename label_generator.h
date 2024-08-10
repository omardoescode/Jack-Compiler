#pragma once

#include <string>
class LabelGenerator {
public:
    LabelGenerator ();
    LabelGenerator (const std::string&);
    LabelGenerator (std::string&&);

    std::string get_next_label ();

    void reset (const std::string&);

private:
    std::string _prefix;
    int _count;
};
