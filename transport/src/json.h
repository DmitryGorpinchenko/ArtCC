#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Json {

class Node;

using Array = std::vector<Node>;
using Dict = std::map<std::string, Node>;
using String = std::string;
using Number = double;
using Bool = bool;

struct Node : std::variant<Array,
                           Dict,
                           String,
                           Number,
                           Bool> {
    using variant::variant;

    const Array& AsArray() const {
        return std::get<Array>(*this);
    }
    const Dict& AsDict() const {
        return std::get<Dict>(*this);
    }
    const String& AsString() const {
        return std::get<String>(*this);
    }
    Number AsNumber() const {
        return std::get<Number>(*this);
    }
    Bool AsBool() const {
        return std::get<Bool>(*this);
    }

    template <typename F>
    auto Visit(F f) const {
        return std::visit(f, static_cast<const variant&>(*this));
    }
};

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root;
};

Document Load(std::istream& input);
void Save(std::ostream& output, const Document& doc);

}
