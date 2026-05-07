#include <memory>
#include "object.h"
#include "error.h"
#include <functional>
#include <set>
#include <iostream>

////////////////////////////////////////////////////////////////////////
// Build all functions thats we need for Intergers
////////////////////////////////////////////////////////////////////////
using ArgsList = std::vector<std::shared_ptr<Object>>;

class BinaryPredicat : public Functional {
public:
    BinaryPredicat(std::shared_ptr<Symbol> func) {
        if (func->GetName() == "=") {
            predicat_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Compare not Numbers");
                }
                return As<Number>(first)->GetValue() == As<Number>(second)->GetValue();
            };
            return;
        }
        if (func->GetName() == "<") {
            predicat_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Compare not Numbers");
                }
                return As<Number>(first)->GetValue() < As<Number>(second)->GetValue();
            };
            return;
        }
        if (func->GetName() == "<=") {
            predicat_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Compare not Numbers");
                }
                return As<Number>(first)->GetValue() <= As<Number>(second)->GetValue();
            };
            return;
        }
        if (func->GetName() == ">") {
            predicat_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Compare not Numbers");
                }
                return As<Number>(first)->GetValue() > As<Number>(second)->GetValue();
            };
            return;
        }
        if (func->GetName() == ">=") {
            predicat_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Compare not Numbers");
                }
                return As<Number>(first)->GetValue() >= As<Number>(second)->GetValue();
            };
            return;
        }
        throw NameError("Binary Predicat don't know" + func->GetName());
    }
    std::shared_ptr<Object> F(ArgsList args) {
        bool result = true;
        for (size_t i = 0; i + 1 < args.size(); ++i) {
            if (!predicat_(args[i], args[i + 1])) {
                result = false;
                break;
            }
        }
        return std::make_shared<Boolean>(result);
    }
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }

private:
    std::function<bool(std::shared_ptr<Object>, std::shared_ptr<Object>)> predicat_;
};

class UnaryPredicat : public Functional {
public:
    UnaryPredicat(std::shared_ptr<Symbol> func) {
        if (func->GetName() == "number?") {
            predicat_ = [](std::shared_ptr<Object> first) {
                if (!Is<Number>(first)) {
                    return false;
                }
                return true;
            };
            return;
        }
        if (func->GetName() == "boolean?") {
            predicat_ = [](std::shared_ptr<Object> first) {
                if (!Is<Boolean>(first)) {
                    return false;
                }
                return true;
            };
            return;
        }
    }
    std::shared_ptr<Object> F(ArgsList args) {
        bool result = true;
        for (size_t i = 0; i < args.size(); ++i) {
            if (!predicat_(args[i])) {
                result = false;
                break;
            }
        }
        return std::make_shared<Boolean>(result);
    }
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }

private:
    std::function<bool(std::shared_ptr<Object>)> predicat_;
};

class BinaryOperation : public Functional {
public:
    BinaryOperation(std::shared_ptr<Symbol> func) {
        if (func->GetName() == "max") {
            start_ = 1;
            f_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Take Max not from Number");
                }
                if (As<Number>(first)->GetValue() <= As<Number>(second)->GetValue()) {
                    return second;
                } else {
                    return first;
                }
            };
            return;
        }
        if (func->GetName() == "min") {
            start_ = 1;
            f_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Take Min not from Number");
                }
                if (As<Number>(first)->GetValue() >= As<Number>(second)->GetValue()) {
                    return second;
                } else {
                    return first;
                }
            };
            return;
        }
        if (func->GetName() == "+") {
            f_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Take PLUS not from Number");
                }
                return std::make_shared<Number>(As<Number>(first)->GetValue() +
                                                As<Number>(second)->GetValue());
            };
            return;
        }
        if (func->GetName() == "*") {
            standart_ = 1;
            f_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Take MUL not from Number");
                }
                return std::make_shared<Number>(As<Number>(first)->GetValue() *
                                                As<Number>(second)->GetValue());
            };
            return;
        }
        if (func->GetName() == "/") {
            start_ = 1;
            f_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Take DEL not from Number");
                }
                if (As<Number>(second)->GetValue() == 0) {
                    throw RuntimeError("Devide ZERO!");
                }
                return std::make_shared<Number>(As<Number>(first)->GetValue() /
                                                As<Number>(second)->GetValue());
            };
            return;
        }
        if (func->GetName() == "-") {
            start_ = 1;
            f_ = [](std::shared_ptr<Object> first, std::shared_ptr<Object> second) {
                if (!Is<Number>(first) || !Is<Number>(second)) {
                    throw RuntimeError("Take SUB not from Number");
                }
                return std::make_shared<Number>(As<Number>(first)->GetValue() -
                                                As<Number>(second)->GetValue());
            };
            return;
        }
        throw NameError("Binary Func don't know" + func->GetName());
    }
    std::shared_ptr<Object> F(ArgsList args) {
        std::shared_ptr<Object> result = std::make_shared<Number>(standart_);
        if (start_ != 0) {
            if (!args.empty()) {
                if (!Is<Number>(args[0])) {
                    throw RuntimeError("Start args isn't Number!");
                }
                result = std::make_shared<Number>(As<Number>(args[0])->GetValue());
            } else {
                throw RuntimeError("Don't enought args!");
            }
        }
        for (size_t i = start_; i < args.size(); ++i) {
            result = f_(result, args[i]);
        }
        return result;
    }
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }

private:
    int64_t standart_ = 0;
    size_t start_ = 0;
    std::function<std::shared_ptr<Object>(std::shared_ptr<Object>, std::shared_ptr<Object>)> f_;
};

class UnaryOperation : public Functional {
public:
    UnaryOperation(std::shared_ptr<Symbol> func) {
        if (func->GetName() == "abs") {
            f_ = [](std::shared_ptr<Object> first) {
                if (!Is<Number>(first)) {
                    throw RuntimeError("Not Number in ABS");
                }
                return std::make_shared<Number>(std::abs(As<Number>(first)->GetValue()));
            };
            return;
        }
        if (func->GetName() == "not") {
            f_ = [](std::shared_ptr<Object> first) {
                bool result = true;
                if (Is<Boolean>(first)) {
                    result = As<Boolean>(first)->Get();
                }
                return std::make_shared<Boolean>(result ^ 1);
            };
            return;
        }
    }
    std::shared_ptr<Object> F(ArgsList args) {
        if (args.size() > 1) {
            throw RuntimeError("To much args for Unary function");
        }
        if (args.empty()) {
            throw RuntimeError("NO args for Unary function");
        }
        return f_(args[0]);
    }
    std::shared_ptr<Object> Eval() {
        return shared_from_this();
    }

private:
    std::function<std::shared_ptr<Object>(std::shared_ptr<Object>)> f_;
};

class ListPredicat : public Functional {
public:
    ListPredicat(std::shared_ptr<Symbol> func) {
        if (func->GetName() == "pair?") {
            predicat_ = [](std::shared_ptr<Object> obj) {
                if (Is<Cell>(obj)) {
                    return true;
                }
                return false;
            };
        } else if (func->GetName() == "null?") {
            predicat_ = [](std::shared_ptr<Object> obj) { return obj == nullptr; };
        } else if (func->GetName() == "list?") {
            predicat_ = [](std::shared_ptr<Object> obj) {
                if (obj == nullptr) {
                    return true;
                }
                if (!Is<Cell>(obj)) {
                    return false;
                }
                std::shared_ptr<Object> curr = obj;
                while (Is<Cell>(curr)) {
                    curr = As<Cell>(curr)->GetSecond();
                }
                return curr == nullptr;
            };
        }
    }
    std::shared_ptr<Object> F(ArgsList args) override {
        if (args.size() != 1) {
            throw RuntimeError("Incorrect size of args");
        }
        return std::make_shared<Boolean>(predicat_(args[0]));
    }
    std::shared_ptr<Object> Eval() override {
        return shared_from_this();
    }

private:
    std::function<bool(std::shared_ptr<Object>)> predicat_;
};

class ListOperation : public Functional {
public:
    ListOperation(std::shared_ptr<Symbol> func) : name_(func->GetName()) {
    }
    std::shared_ptr<Object> F(ArgsList args) override {
        if (name_ == "cons") {
            if (args.size() != 2) {
                throw RuntimeError("Cons requires 2 args");
            }
            return std::make_shared<Cell>(args[0], args[1]);
        }
        if (name_ == "car") {
            if (args.size() != 1) {
                throw RuntimeError("Car requires 1 arg");
            }
            if (!Is<Cell>(args[0])) {
                throw RuntimeError("Car argument must be a pair");
            }
            return As<Cell>(args[0])->GetFirst();
        }
        if (name_ == "cdr") {
            if (args.size() != 1) {
                throw RuntimeError("Cdr requires 1 arg");
            }
            if (!Is<Cell>(args[0])) {
                throw RuntimeError("Cdr argument must be a pair");
            }
            return As<Cell>(args[0])->GetSecond();
        }
        if (name_ == "list") {
            std::shared_ptr<Object> result = nullptr;
            for (int i = args.size() - 1; i >= 0; --i) {
                result = std::make_shared<Cell>(args[i], result);
            }
            return result;
        }
        if (name_ == "list-ref") {
            if (args.size() != 2) {
                throw RuntimeError("list-ref requires 2 arguments");
            }
            if (!Is<Number>(args[1])) {
                throw RuntimeError("index must be a number");
            }
            int64_t idx = As<Number>(args[1])->GetValue();
            std::shared_ptr<Object> curr = args[0];
            for (int64_t i = 0; i < idx; ++i) {
                if (!Is<Cell>(curr)) {
                    throw RuntimeError("Index out of bounds");
                }
                curr = As<Cell>(curr)->GetSecond();
            }
            if (!Is<Cell>(curr)) {
                throw RuntimeError("Index out of bounds");
            }
            return As<Cell>(curr)->GetFirst();
        }
        if (name_ == "list-tail") {
            if (args.size() != 2) {
                throw RuntimeError("list-tail requires 2 arguments");
            }
            if (!Is<Number>(args[1])) {
                throw RuntimeError("index must be a number");
            }
            int64_t idx = As<Number>(args[1])->GetValue();
            std::shared_ptr<Object> curr = args[0];
            for (int64_t i = 0; i < idx; ++i) {
                if (!Is<Cell>(curr)) {
                    throw RuntimeError("Index out of bounds");
                }
                curr = As<Cell>(curr)->GetSecond();
            }
            return curr;
        }
        throw RuntimeError("Unknown List Operation");
    }
    std::shared_ptr<Object> Eval() override {
        return shared_from_this();
    }

private:
    std::string name_;
};

///////////////////////////////////////////////////////////////////////////
// Final Function that's make a choice of what type of Functional Object we need
///////////////////////////////////////////////////////////////////////////

std::shared_ptr<Object> MakeFunction(std::shared_ptr<Object> root) {
    std::set<std::string> compare_class = {"=", "<=", ">=", ">", "<"};
    std::set<std::string> arithmetics_class = {"+", "-", "*", "/"};
    std::set<std::string> list_ops_class = {"cons", "car", "cdr", "list", "list-ref", "list-tail"};
    std::set<std::string> list_preds_class = {"pair?", "null?", "list?"};
    if (Is<Symbol>(root)) {
        std::shared_ptr<Symbol> value = As<Symbol>(root);
        if (compare_class.count(value->GetName())) {
            return std::make_shared<BinaryPredicat>(value);
        }
        if (value->GetName() == "number?") {
            return std::make_shared<UnaryPredicat>(value);
        }
        if (value->GetName() == "min" || value->GetName() == "max") {
            return std::make_shared<BinaryOperation>(value);
        }
        if (arithmetics_class.count(value->GetName())) {
            return std::make_shared<BinaryOperation>(value);
        }
        if (value->GetName() == "abs") {
            return std::make_shared<UnaryOperation>(value);
        }
        if (value->GetName() == "boolean?") {
            return std::make_shared<UnaryPredicat>(value);
        }
        if (value->GetName() == "not") {
            return std::make_shared<UnaryOperation>(value);
        }
        if (list_ops_class.count(value->GetName())) {
            return std::make_shared<ListOperation>(value);
        }
        if (list_preds_class.count(value->GetName())) {
            return std::make_shared<ListPredicat>(value);
        }
        throw NameError("NEW FUNCTION WHAT: " + value->GetName());
    }
    throw RuntimeError("It's not a Symbol in MakeFunction");
}