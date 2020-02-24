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

void Print(const Node& node, ostream& output, int indent = 0, bool start_indent = false);

void Print(const String& str, ostream& output, int indent, bool start_indent) {
    if (start_indent) {
        Indent(output, indent);
    }
    output << '"';
    for (auto c : str) {
        if (c == '"' || c == '\\') {
            output << '\\';
        }
        output << c;
    }
    output << '"';
}

void Print(Number num, ostream& output, int indent, bool start_indent) {
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

void Print(Bool b, ostream& output, int indent, bool start_indent) {
    if (start_indent) {
        Indent(output, indent);
    }
    output << boolalpha << b;
}

void Print(const Array& array, ostream& output, int indent, bool start_indent) {
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
        Print(n, output, indent + 4, true);
    }
    if (!array.empty()) {
        output << '\n';
        Indent(output, indent);
    }
    output << ']';
}

void Print(const Dict& dict, ostream& output, int indent, bool start_indent) {
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
        Print(k, output, indent + 4, true);
        output << ": ";
        Print(v, output, indent + 4, false);
    }
    if (!dict.empty()) {
        output << '\n';
        Indent(output, indent);
    }
    output << '}';
}

void Print(const Node& node, ostream& output, int indent, bool start_indent) {
    node.Visit([&output, indent, start_indent](const auto& n) { Print(n, output, indent, start_indent); });
}

void Save(ostream& output, const Document& doc) {
    Print(doc.GetRoot(), output);
}

}
