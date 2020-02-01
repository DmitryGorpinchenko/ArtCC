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

class Node : std::variant<Array,
                          Dict,
                          String,
                          Number,
                          Bool> {
public:
    using variant::variant;

    bool IsArray() const {
        return std::holds_alternative<Array>(*this);
    }
    bool IsDict() const {
        return std::holds_alternative<Dict>(*this);
    }
    bool IsString() const {
        return std::holds_alternative<String>(*this);
    }
    bool IsNumber() const {
        return std::holds_alternative<Number>(*this);
    }
    bool IsBool() const {
        return std::holds_alternative<Bool>(*this);
    }

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
