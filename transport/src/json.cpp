#include "json.h"

#include <iostream>
#include <iomanip>
using namespace std;

namespace Json {

Document::Document(Node root) : root(move(root)) {}

const Node& Document::GetRoot() const {
    return root;
}

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
    vector<Node> result;
    for (char c; input >> c && c != ']'; ) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }
    return Node(move(result));
}

Node LoadDouble(istream& input) {
    double result = 0;
    input >> result;
    return Node(result);
}

Node LoadBool(istream& input) {
    bool result = false;
    input >> boolalpha >> result;
    return Node(result);
}

Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
}

Node LoadDict(istream& input) {
    map<string, Node> result;
    for (char c; input >> c && c != '}'; ) {
        if (c == ',') {
            input >> c;
        }
        string key = LoadString(input).AsString();
        input >> c;
        result.emplace(move(key), LoadNode(input));
    }
    return Node(move(result));
}

Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '"') {
        return LoadString(input);
    } else if (c == 't' || c == 'f') {
        input.putback(c);
        return LoadBool(input);
    } else {
        input.putback(c);
        return LoadDouble(input);
    }
}

Document Load(istream& input) {
    return Document{LoadNode(input)};
}

//

void Indent(ostream& output, int indent) {
    while (indent > 0) {
        output << ' '; 
        --indent;
    }
}

void PrintNode(ostream& output, const Node& node, int indent = 0, bool start_indent = false);

void PrintString(ostream& output, const String& str, int indent, bool start_indent) {
    if (start_indent) {
        Indent(output, indent);
    }
    output << '"' << str << '"';
}

void PrintNumber(ostream& output, Number num, int indent, bool start_indent) {
    if (start_indent) {
        Indent(output, indent);
    }
    bool is_integral = int(num) == num;
    if (is_integral) {
        output << int(num);
    } else {
        output << setprecision(6) << num;
    }
}

void PrintBool(ostream& output, Bool b, int indent, bool start_indent) {
    if (start_indent) {
        Indent(output, indent);
    }
    output << boolalpha << b;
}

void PrintArray(ostream& output, const Array& array, int indent, bool start_indent) {
    if (start_indent) {
        Indent(output, indent);
    }
    output << '[';
    bool first = true;
    for (const auto& n : array) {
        if (!first) {
            output << ',';
        }
        first = false;
        output << '\n';
        PrintNode(output, n, indent + 4, true);
    }
    if (!array.empty()) {
        output << '\n';
        Indent(output, indent);
    }
    output << ']';
}

void PrintDict(ostream& output, const Dict& dict, int indent, bool start_indent) {
    if (start_indent) {
        Indent(output, indent);
    }
    output << '{';
    bool first = true;
    for (const auto& [k, v] : dict) {
        if (!first) {
            output << ',';
        }
        first = false;
        output << '\n';
        PrintString(output, k, indent + 4, true);
        output << ": ";
        PrintNode(output, v, indent + 4, false);
    }
    if (!dict.empty()) {
        output << '\n';
        Indent(output, indent);
    }
    output << '}';
}

void PrintNode(ostream& output, const Node& node, int indent, bool start_indent) {
    if (node.IsArray()) {
        PrintArray(output, node.AsArray(), indent, start_indent);
    } else if (node.IsDict()) {
        PrintDict(output, node.AsDict(), indent, start_indent);
    } else if (node.IsString()) {
        PrintString(output, node.AsString(), indent, start_indent);
    } else if (node.IsNumber()) {
        PrintNumber(output, node.AsNumber(), indent, start_indent);
    } else if (node.IsBool()) {
        PrintBool(output, node.AsBool(), indent, start_indent);
    }
}

void Save(ostream& output, const Document& doc) {
    PrintNode(output, doc.GetRoot());
}

}
