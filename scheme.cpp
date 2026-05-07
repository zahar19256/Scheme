#include "scheme.h"
#include "object.h"
#include "tokenizer.h"
#include "parser.h"
#include <memory>
#include <sstream>
#include <string>
#include "error.h"
#include "functor.h"

bool IsFalse(std::shared_ptr<Object> obj) {
    if (Is<Boolean>(obj)) {
        return !As<Boolean>(obj)->Get();
    }
    return false;
}

std::shared_ptr<Object> Calculate(std::shared_ptr<Object> root);

std::shared_ptr<Object> CalculateAnd(std::vector<std::shared_ptr<Object>> list) {
    std::shared_ptr<Object> result = std::make_shared<Boolean>(true);
    for (auto to : list) {
        std::shared_ptr<Object> now;
        if (to == nullptr) {
            throw RuntimeError("Null element in evaluation");
        }
        if (Is<Cell>(to)) {
            now = Calculate(to);
        } else {
            now = to->Eval();
        }
        result = now;
        if (IsFalse(result)) {
            return result;
        }
    }
    return result;
}

std::shared_ptr<Object> CalculateOr(std::vector<std::shared_ptr<Object>> list) {
    std::shared_ptr<Object> result = std::make_shared<Boolean>(false);
    for (auto to : list) {
        std::shared_ptr<Object> now;
        if (to == nullptr) {
            throw RuntimeError("Null element in evaluation");
        }
        if (Is<Cell>(to)) {
            now = Calculate(to);
        } else {
            now = to->Eval();
        }
        result = now;
        if (!IsFalse(result)) {
            return result;
        }
    }
    return result;
}

std::vector<std::shared_ptr<Object>> ListToVector(std::shared_ptr<Object> root) {
    std::vector<std::shared_ptr<Object>> result;
    while (root != nullptr) {
        if (!Is<Cell>(root)) {
            throw RuntimeError("Arguments must be a proper list");
        }
        auto cell = As<Cell>(root);
        result.push_back(cell->GetFirst());
        root = cell->GetSecond();
    }
    return result;
}

std::vector<std::shared_ptr<Object>> EvalArgs(std::vector<std::shared_ptr<Object>> list) {
    std::vector<std::shared_ptr<Object>> result;
    for (auto to : list) {
        result.push_back(Calculate(to));
    }
    return result;
}

std::shared_ptr<Object> Calculate(std::shared_ptr<Object> root) {
    if (root == nullptr) {
        throw RuntimeError("DONT eval empty list ()");
    }
    if (Is<Cell>(root)) {
        auto cell = As<Cell>(root);
        std::shared_ptr<Object> head = cell->GetFirst();
        std::shared_ptr<Object> tail = cell->GetSecond();

        if (Is<Symbol>(head)) {
            std::string name = As<Symbol>(head)->GetName();
            if (name == "quote") {
                if (!Is<Cell>(tail) || As<Cell>(tail)->GetSecond() != nullptr) {
                    throw SyntaxError("Quote requires ONE arg");
                }
                return As<Cell>(tail)->GetFirst();
            }
            if (name == "and") {
                return CalculateAnd(ListToVector(tail));
            }
            if (name == "or") {
                return CalculateOr(ListToVector(tail));
            }
        }
        if (!Is<Symbol>(head)) {
            throw RuntimeError("First element of list is not a function");
        }
        std::shared_ptr<Object> func = MakeFunction(head);
        std::vector<std::shared_ptr<Object>> ast = ListToVector(tail);
        std::vector<std::shared_ptr<Object>> args = EvalArgs(ast);
        return As<Functional>(func)->F(args);
    }
    return root->Eval();
}

std::string Sereal(std::shared_ptr<Object> current) {
    if (current == nullptr) {
        return "()";
    }
    if (Is<Number>(current)) {
        return std::to_string(As<Number>(current)->GetValue());
    }
    if (Is<Symbol>(current)) {
        return As<Symbol>(current)->GetName();
    }
    if (Is<Boolean>(current)) {
        return As<Boolean>(current)->Get() ? "#t" : "#f";
    }
    if (Is<Cell>(current)) {
        std::string res = "(";
        std::shared_ptr<Object> node = current;
        while (node != nullptr) {
            if (!Is<Cell>(node)) {
                res += " . ";
                res += Sereal(node);
                break;
            }
            auto cell = As<Cell>(node);
            res += Sereal(cell->GetFirst());
            node = cell->GetSecond();
            if (node != nullptr) {
                if (Is<Cell>(node)) {
                    res += " ";
                } else {
                    res += " . ";
                    res += Sereal(node);
                    break;
                }
            }
        }
        res += ")";
        return res;
    }
    return "";
}

std::string Interpreter::Run(const std::string &input) {
    std::stringstream ss{input};
    Tokenizer tokenizer{&ss};
    std::shared_ptr<Object> root = Read(&tokenizer);
    if (!root && tokenizer.IsEnd()) {
        throw RuntimeError("Empty input");
    }
    std::shared_ptr<Object> result = Calculate(root);
    return Sereal(result);
}