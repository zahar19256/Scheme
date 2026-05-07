#pragma once

#include <memory>
#include "tokenizer.h"
#include <vector>
#include "error.h"

class Enviroment;

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual std::shared_ptr<Object> Eval() = 0;
    bool IsFunctional() {
        return false;
    }
    virtual ~Object() = default;
};

class Functional : public Object {
public:
    virtual std::shared_ptr<Object> F(std::vector<std::shared_ptr<Object>> args) = 0;
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
    bool IsFunctional() {
        return true;
    }
    virtual ~Functional() = default;
};

class OpenBracket : public Object {};

class CloseBracket : public Object {
public:
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
};

class Dot : public Object {
public:
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
};

class Quote : public Object {
public:
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
};
class Number : public Object {
public:
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
    Number(int64_t value) : value_(value) {
    }
    int64_t GetValue() const {
        return value_;
    }

private:
    int64_t value_;
};

class Symbol : public Object {
public:
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
    Symbol(const std::string& name) : name_(name) {
    }
    const std::string& GetName() const {
        return name_;
    }

private:
    std::string name_;
};

class Boolean : public Object {
public:
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
    Boolean(bool flag) : flag_(flag) {
    }
    bool Get() const {
        return flag_;
    }

private:
    bool flag_;
};

class EmptyList : public Object {
public:
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
};

class Cell : public Object {
public:
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
        : first_(first), second_(second) {
    }
    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }

private:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
};

///////////////////////////////////////////////////////////////////////////////
// Now we ready to add some more functions that's we what to use

///////////////////////////////////////////////////////////////////////////////

// Runtime type checking and conversion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return dynamic_cast<T*>(obj.get()) != nullptr;
}
