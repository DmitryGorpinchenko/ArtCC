#include "ini.h"

#include <sstream>

namespace Ini {

Section& Document::AddSection(std::string name) {
    return sections[std::move(name)];
}

const Section& Document::GetSection(const std::string& name) const {
    return sections.at(name);
}

size_t Document::SectionCount() const {
    return sections.size();
}

Document Load(std::istream& input) {
    Document res;
    Section* curr_section = nullptr;
    for (std::string line; getline(input, line); ) {
        if (line.empty()) {
            continue;
        } else if (line.front() == '[') {
            curr_section = &res.AddSection(line.substr(1, line.size() - 2));
        } else if (curr_section) {
            std::string_view view = line;
            const auto eq_pos = view.find('=');
            curr_section->emplace(view.substr(0, eq_pos), view.substr(eq_pos + 1));
        }
    }
    return res;
}

}
