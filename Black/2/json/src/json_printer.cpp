#include "test_runner.h"

#include <vector>
#include <utility>
#include <variant>
#include <sstream>
#include <string>
#include <cstdint>
#include <iostream>

struct Value;

using Obj = std::vector<std::pair<std::string, Value>>;
using Arr = std::vector<Value>;
using Str = std::string;
using Num = int64_t;
using Bool = bool;
using Nil = std::monostate;

struct Value : std::variant<Nil,
                            Bool,
                            Num,
                            Str,
                            Arr,
                            Obj> {
    using variant::variant;
    
    Nil&  AsNull()   { return std::get<Nil>(*this); }
    Bool& AsBool()   { return std::get<Bool>(*this); }
    Num&  AsNumber() { return std::get<Num>(*this); }
    Str&  AsString() { return std::get<Str>(*this); }
    Arr&  AsArray()  { return std::get<Arr>(*this); }
    Obj&  AsObject() { return std::get<Obj>(*this); }
    
    template <typename F>
    auto Visit(F f) const {
        return std::visit(f, static_cast<const variant&>(*this));
    }
};

void Print(const Value& val, std::ostream& out);

void Print(Nil val, std::ostream& out) {
    out << "null";
}

void Print(Bool val, std::ostream& out) {
    out << std::boolalpha << val;
}

void Print(Num val, std::ostream& out) {
    out << val;
}

void Print(const Str& val, std::ostream& out) {
    out << '"';
    for (auto c : val) {
        if (c == '"' || c == '\\') {
            out << '\\';
        }
        out << c;
    }
    out << '"';
}

void Print(const Arr& val, std::ostream& out) {
    out << '[';
    bool first = true;
    for (const auto& v : val) {
        if (!first) {
            out << ',';
        }
        first = false;
        Print(v, out);
    }
    out << ']';
}

void Print(const Obj& val, std::ostream& out) {
    out << '{';
    bool first = true;
    for (const auto& [k, v] : val) {
        if (!first) {
            out << ',';
        }
        first = false;
        Print(k, out);
        out << ':';
        Print(v, out);
    }
    out << '}';
}

void Print(const Value& val, std::ostream& out) {
    val.Visit([&out](const auto& v) { Print(v, out); });
}

//

template <typename Ctx> class ObjectProxy;
template <typename Ctx> class ArrayProxy;

struct VoidCtx {};
static VoidCtx void_ctx;

// ObjectContext

template <typename Ctx>
class ObjectProxy {
    using Self = ObjectProxy<Ctx>;
public:
    class ValueProxy {
    public:
        ValueProxy(Ctx& _ctx, Value& _val) : ctx(_ctx), val(_val) {}
        
        Ctx& Null() {
            val = Nil();
            return ctx;
        }
        Ctx& Boolean(Bool _val) {
            val = _val;
            return ctx;
        }
        Ctx& Number(Num _val) {
            val = _val;
            return ctx;
        }
        Ctx& String(const Str& _val) {
            val = _val;
            return ctx;
        }
        auto BeginArray() {
            val = Arr();
            return ArrayProxy(ctx, val.AsArray());
        }
        auto BeginObject() {
            val = Obj();
            return ObjectProxy(ctx, val.AsObject());
        }
        
    private:
        Ctx& ctx;
        Value& val;
    };

    ObjectProxy(Ctx& _ctx, Obj& _obj) : ctx(_ctx), obj(_obj) {}
    
    auto Key(std::string_view k) {
        obj.emplace_back(k, Nil());
        return typename ObjectProxy<Self>::ValueProxy(*this, obj.back().second);
    }
    
    Ctx& EndObject() { return ctx; }
    
private:
    Ctx& ctx;
    Obj& obj;
};

class ObjectContext : public ObjectProxy<VoidCtx> {
public:
    explicit ObjectContext(std::ostream& _out)
        : ObjectProxy(void_ctx, obj)
        , out(_out)
    {}
    
    ~ObjectContext() {
        Print(obj, out);
    }
    
private:
    std::ostream& out;
    Obj obj;
};

// ArrayContext

template <typename Ctx>
class ArrayProxy {
    using Self = ArrayProxy<Ctx>;
public:
    ArrayProxy(Ctx& _ctx, Arr& _arr) : ctx(_ctx), arr(_arr) {}
    
    Self& Null() {
        arr.emplace_back(Nil());
        return *this;
    }
    Self& Boolean(Bool val) {
        arr.emplace_back(val);
        return *this;
    }
    Self& Number(Num val) {
        arr.emplace_back(val);
        return *this;
    }
    Self& String(const Str& val) {
        arr.emplace_back(val);
        return *this;
    }
    auto BeginArray() {
        arr.emplace_back(Arr());
        return ArrayProxy<Self>(*this, arr.back().AsArray());
    }
    auto BeginObject() {
        arr.emplace_back(Obj());
        return ObjectProxy<Self>(*this, arr.back().AsObject());
    }
    
    Ctx& EndArray() { return ctx; }
    
private:
    Ctx& ctx;
    Arr& arr;
};

class ArrayContext : public ArrayProxy<VoidCtx> {
public:
    explicit ArrayContext(std::ostream& _out)
        : ArrayProxy(void_ctx, arr)
        , out(_out)
    {}
    
    ~ArrayContext() {
        Print(arr, out);
    }
    
private:
    std::ostream& out;
    Arr arr;
};

//

void PrintJsonString(std::ostream& out, const Str& str) {
    Print(str, out);
}

ObjectContext PrintJsonObject(std::ostream& out) {
    return ObjectContext(out);
}

ArrayContext PrintJsonArray(std::ostream& out) {
    return ArrayContext(out);
}

// Tests

void TestArray() {
    std::ostringstream output;

    {
        auto json = PrintJsonArray(output);
        json.Number(5)
            .Number(6)
            .BeginArray()
                .Number(7)
                .BeginArray()
                    .Number(10)
                .EndArray()
                .BeginArray()
                    .BeginArray()
                    .EndArray()
                .EndArray()
            .EndArray()
            .Number(8)
            .String("bingo!");
    }

    ASSERT_EQUAL(output.str(), R"([5,6,[7,[10],[[]]],8,"bingo!"])");
}

void TestObject() {
    std::ostringstream output;

    {
        auto json = PrintJsonObject(output);
        json.Key("id1").Number(1234)
            .Key("id2").Boolean(false)
            .Key("").Null()
            .Key("\"").String("\\");
    }

    ASSERT_EQUAL(output.str(), R"({"id1":1234,"id2":false,"":null,"\"":"\\"})");
}

void TestAutoClose() {
    std::ostringstream output;

    {
        auto json = PrintJsonArray(output);
        json.BeginArray().BeginObject();
    }

    ASSERT_EQUAL(output.str(), R"([[{}]])");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestArray);
    RUN_TEST(tr, TestObject);
    RUN_TEST(tr, TestAutoClose);

    return 0;
}
