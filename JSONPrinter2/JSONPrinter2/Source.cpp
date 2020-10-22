#include "test_runner.h"

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <iostream>

template<typename T> class ArrayContext;
template <typename T> class ObjectContextKCExpected;
template <typename T> class ObjectContextVExpected;

using namespace std;

class EmptyContext {

};

EmptyContext EMPTY_CONT;

void PrintJsonString(std::ostream& out, string_view str) {
    out << "\"";
    for (auto i : str) {
        if (i == '\"' || i == '\\') {
            out << "\\" << i;
        }
        else {
            out << i;
        }
    }
    out << "\"";
}

template<typename T> 
class ArrayContext {
public:
    ArrayContext(ostream& stream, T* parent) : stream_(stream), parent_(parent) {
        stream_ << "[";
    }

    ArrayContext<T>& Null() {
        if (hasElemBehind) {
            stream_ << ",";
        }
        stream_ << "null";
        hasElemBehind = true;
        return *this;
    }

    ArrayContext<T>& String(string_view sv) {
        if (hasElemBehind) {
            stream_ << ",";
        }
        PrintJsonString(stream_, sv);
        hasElemBehind = true;
        return *this;
    }

    ArrayContext<T>& Boolean(bool boolVal) {
        if (hasElemBehind) {
            stream_ << ",";
        }
        stream_ << std::boolalpha << boolVal;
        hasElemBehind = true;
        return *this;
    }

    ArrayContext<T>& Number(int64_t intVal) {
        if (hasElemBehind) {
            stream_ << ",";
        }
        stream_ << intVal;
        hasElemBehind = true;
        return *this;
    }

    T& EndArray() {
        stream_ << "]";
        toClose = false;
        return *parent_;
    }

    auto BeginArray() {
        if (hasElemBehind) {
            stream_ << ",";
        }
        hasElemBehind = true;
        return ArrayContext<ArrayContext<T>>(stream_, this);
    }

    ~ArrayContext() {
        if (toClose) {
            stream_ << "]";
        }
    }

    auto BeginObject() {
        if (hasElemBehind) {
            stream_ << ",";
        }
        hasElemBehind = true;
        return ObjectContextKCExpected<ArrayContext<T>>(stream_, this);
    }

private:
    ostream& stream_;
    T* parent_;
    bool hasElemBehind = false;
    bool toClose = true;
};

template <typename T>
class ObjectContextVExpected {
public:
    ObjectContextVExpected(ostream& stream, T* parent) : stream_(stream), parent_(parent) {}

    auto BeginArray() {
        stream_ << ":";
        hasValue = true;
        return ArrayContext(stream_, parent_);
    }

    auto BeginObject() {
        stream_ << ":";
        hasValue = true;
        return ObjectContextKCExpected(stream_, parent_);
    }

    ~ObjectContextVExpected() {
        if (!hasValue) {
            stream_ << ":null";
        }
    }

    T& Null() {
        stream_ << ":" << "null";
        hasValue = true;
        return *parent_;
    }

    T& String(string_view sv) {
        stream_ << ":";
        PrintJsonString(stream_, sv);
        hasValue = true;
        return *parent_;
    }

    T& Boolean(bool boolVal) {
        stream_ << ":" << std::boolalpha << boolVal;
        hasValue = true;
        return *parent_;
    }

    T& Number(int64_t intVal) {
        stream_ << ":" << intVal;
        hasValue = true;
        return *parent_;
    }
private:
    ostream& stream_;
    T* parent_;
    bool hasValue = false;
};


template <typename T>
class ObjectContextKCExpected {
public:
    ObjectContextKCExpected(ostream& stream, T* parent) : stream_(stream), parent_(parent) {
        stream_ << "{";
    }

    auto Key(string_view sv) {
        if (hasElemBehind) {
            stream_ << ",";
        }
        PrintJsonString(stream_, sv);
        hasElemBehind = true;
        return ObjectContextVExpected(stream_, this);
    }

    ~ObjectContextKCExpected() {
        if (toClose) {
            stream_ << "}";
        }
    }

    T& EndObject() {
        stream_ << "}";
        toClose = false;
        return *parent_;
    }

private:
    ostream& stream_;
    T* parent_;
    bool hasElemBehind = false;
    bool toClose = true;
};

ArrayContext<EmptyContext> PrintJsonArray(std::ostream& out) {
    return ArrayContext(out, &EMPTY_CONT);
}

ObjectContextKCExpected<EmptyContext> PrintJsonObject(std::ostream& out) {
    return ObjectContextKCExpected(out, &EMPTY_CONT);
}

void TestArray() {
    std::ostringstream output;

    {
        auto json = PrintJsonArray(output);
        json
            .Number(5)
            .Number(6)
            .BeginArray()
            .Number(7)
            .EndArray()
            .Number(8)
            .String("bingo!");
    }

    ASSERT_EQUAL(output.str(), R"([5,6,[7],8,"bingo!"])");
}

void TestObject() {
    std::ostringstream output;

    {
        auto json = PrintJsonObject(output);
        json
            .Key("id1").Number(1234)
            .Key("id2").Boolean(false)
            .Key("").Null()
            .Key("\"").String("\\");
    }

    ASSERT_EQUAL(output.str(), "{\"id1\":1234,\"id2\":false,\"\":null,\"\\\"\":\"\\\\\"}");
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
    //RUN_TEST(tr, TestArray);
    //RUN_TEST(tr, TestObject);
    //RUN_TEST(tr, TestAutoClose);

    return 0;
}