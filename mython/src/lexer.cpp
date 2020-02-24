#include "lexer.h"

#include <iostream>
#include <algorithm>
//#include <charconv>
#include <unordered_map>
#include <ctype.h>

using namespace std;

namespace Parse {

bool operator ==(const Token& lhs, const Token& rhs) {
    using namespace TokenType;

    if (lhs.index() != rhs.index()) {
        return false;
    }
    if (lhs.Is<Char>()) {
        return lhs.As<Char>().value == rhs.As<Char>().value;
    } else if (lhs.Is<Number>()) {
        return lhs.As<Number>().value == rhs.As<Number>().value;
    } else if (lhs.Is<String>()) {
        return lhs.As<String>().value == rhs.As<String>().value;
    } else if (lhs.Is<Id>()) {
        return lhs.As<Id>().value == rhs.As<Id>().value;
    } else {
        return true;
    }
}

std::ostream& operator <<(std::ostream& os, const Token& rhs) {
    using namespace TokenType;

#define VALUED_OUTPUT(type) \
    if (auto p = rhs.TryAs<type>()) return os << #type << '{' << p->value << '}';

    VALUED_OUTPUT(Number);
    VALUED_OUTPUT(Id);
    VALUED_OUTPUT(String);
    VALUED_OUTPUT(Char);

#undef VALUED_OUTPUT

#define UNVALUED_OUTPUT(type) \
    if (rhs.Is<type>()) return os << #type;

    UNVALUED_OUTPUT(Class);
    UNVALUED_OUTPUT(Return);
    UNVALUED_OUTPUT(If);
    UNVALUED_OUTPUT(Else);
    UNVALUED_OUTPUT(Def);
    UNVALUED_OUTPUT(Newline);
    UNVALUED_OUTPUT(Print);
    UNVALUED_OUTPUT(Indent);
    UNVALUED_OUTPUT(Dedent);
    UNVALUED_OUTPUT(And);
    UNVALUED_OUTPUT(Or);
    UNVALUED_OUTPUT(Not);
    UNVALUED_OUTPUT(Eq);
    UNVALUED_OUTPUT(NotEq);
    UNVALUED_OUTPUT(LessOrEq);
    UNVALUED_OUTPUT(GreaterOrEq);
    UNVALUED_OUTPUT(None);
    UNVALUED_OUTPUT(True);
    UNVALUED_OUTPUT(False);
    UNVALUED_OUTPUT(Eof);

#undef UNVALUED_OUTPUT

    return os << "Unknown token :(";
}

Lexer::Lexer(istream& input)
    : is(input >> noskipws)
{
    NextToken();
}

const Token& Lexer::CurrentToken() const {
    return curr_token;
}

namespace {

Token ReadString(istream& is, char delim) {
    is.ignore(); // delim assumed
    string res;
    getline(is, res, delim);
    return TokenType::String{move(res)};
}

Token ReadIdOrKeyword(istream& is) {
    string res;
    do {
        if (char c = is.peek(); c == '_' || isalnum(c)) {
            is.ignore();
            res += c;
        } else {
            break;
        }
    } while (true);
    static const unordered_map<string_view, Token> keywords = {
        {"class", TokenType::Class{}},
        {"return", TokenType::Return{}},
        {"if", TokenType::If{}},
        {"else", TokenType::Else{}},
        {"def", TokenType::Def{}},
        {"print", TokenType::Print{}},
        {"None", TokenType::None{}},
        {"True", TokenType::True{}},
        {"False", TokenType::False{}},
        {"and", TokenType::And{}},
        {"or", TokenType::Or{}},
        {"not", TokenType::Not{}},
    };
    if (auto it = keywords.find(res); it != end(keywords)) {
        return (*it).second;
    }
    return TokenType::Id{move(res)};
}

Token ReadNumber(istream& is) {
    int res;
    is >> res;
    return TokenType::Number{res};
}

Token ReadComparisionOrChar(istream& is) {
    char c1;
    is >> c1;
    if (char c2 = is.peek(); c2 == '=') {
        if (c1 == '=') {
            is.ignore();
            return TokenType::Eq{};
        } else if (c1 == '<') {
            is.ignore();
            return TokenType::LessOrEq{};
        } else if (c1 == '>') {
            is.ignore();
            return TokenType::GreaterOrEq{};
        } else if (c1 == '!') {
            is.ignore();
            return TokenType::NotEq{};
        }
    }
    return TokenType::Char{c1};
}

} // end namespace

Token Lexer::NextToken() {
    curr_token = NextTokenImpl();
    return curr_token;
}

Token Lexer::NextTokenImpl() {
    if (dedents_left > 0) {
        --dedents_left;
        return TokenType::Dedent{};
    }
    size_t space_count = 0;
    do {
        const char c = is.peek();
        if (c == char_traits<char>::eof()) {
            if (!(curr_token == TokenType::Eof{})) {
                if (curr_token == TokenType::Dedent{}) {
                    return TokenType::Eof{};
                }
                if (curr_token == TokenType::Newline{}) {
                    if (indent > 0) {
                        dedents_left = indent - 1;
                        indent = 0;
                        return TokenType::Dedent{};
                    }
                    return TokenType::Eof{};
                }
                return TokenType::Newline{};
            }
            return curr_token;
        }
        if (!isspace(c)) {
            if (curr_token == TokenType::Newline{}) {
                if (space_count < indent * 2) {
                    dedents_left = indent - space_count / 2 - 1;
                    indent = space_count / 2;
                    return TokenType::Dedent{};
                }
                if (space_count > indent * 2) {
                    indent = space_count / 2;
                    return TokenType::Indent{};
                }
            }
            if (c == '"' || c == '\'') {
                return ReadString(is, c);
            } else if (isdigit(c)) {
                return ReadNumber(is);
            } else if (isalpha(c) || c == '_') {
                return ReadIdOrKeyword(is);
            }
            return ReadComparisionOrChar(is);
        }
        if (c == '\n') {
            is.ignore();
            if (!(curr_token == TokenType::Newline{})) {
                return TokenType::Newline{};
            }
            space_count = 0;
        } else if (c == ' ') {
            is.ignore();
            ++space_count;
        }
    } while (true);
    return curr_token;
}

}
