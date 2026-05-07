#include <tokenizer.h>
#include <stdexcept>
#include <iostream>
#include <variant>

bool SymbolToken::operator==(const SymbolToken& other) const {
    return other.name == name;
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return other.value == value;
}

bool DotToken::operator==(const DotToken&) const {
    return true;
}

bool QuoteToken::operator==(const QuoteToken&) const {
    return true;
}

bool SkipSymbol(char cur) {
    std::string special = ".)(\')<>=*/#?!+-";
    if (cur >= 'a' && cur <= 'z') {
        return false;
    }
    if (cur >= 'A' && cur <= 'Z') {
        return false;
    }
    if (cur >= '0' && cur <= '9') {
        return false;
    }
    if (special.find(cur) != std::variant_npos) {
        return false;
    }
    return true;
}

bool IsSymbol(char cur, bool not_first) {
    if (cur >= 'a' && cur <= 'z') {
        return true;
    }
    if (cur >= 'A' && cur <= 'Z') {
        return true;
    }
    if (cur == '<' || cur == '=' || cur == '>' || cur == '*' || cur == '/' || cur == '#') {
        return true;
    }
    if (not_first) {
        if (cur >= '0' && cur <= '9') {
            return true;
        }
        if (cur == '?' || cur == '!') {
            return true;
        }
        if (cur == '-') {
            return true;
        }
    }
    return false;
}

Tokenizer::Tokenizer(std::istream* in) : token_(DotToken()) {
    src_ = in;
    Next();
}

bool Tokenizer::IsEnd() {
    return is_end_;
}

void Tokenizer::Next() {
    scaned_ = false;
    if (src_->eof()) {
        is_end_ = true;
        return;
    }
    char current = src_->peek();
    while (SkipSymbol(current)) {
        src_->get();
        if (src_->eof()) {
            is_end_ = true;
            return;
        }
        current = src_->peek();
    }
    GetToken();
}

Token Tokenizer::GetToken() {
    if (scaned_) {
        return token_;
    }
    if (IsEnd()) {
        throw std::runtime_error("Try to GetToken when stream is empty!");
    }
    char current = src_->peek();
    if (current == '\'') {
        scaned_ = true;
        token_ = QuoteToken();
        src_->get();
        return token_;
    }
    if (current == '.') {
        scaned_ = true;
        token_ = DotToken();
        src_->get();
        return token_;
    }
    if (current == '(') {
        scaned_ = true;
        token_ = BracketToken::OPEN;
        src_->get();
        return token_;
    }
    if (current == ')') {
        scaned_ = true;
        token_ = BracketToken::CLOSE;
        src_->get();
        return token_;
    }
    std::string name;
    name += current;
    if (IsSymbol(current, false)) {
        src_->get();
        while (!src_->eof()) {
            current = src_->peek();
            if (IsSymbol(current, true)) {
                name += current;
                src_->get();
            } else {
                break;
            }
        }
        scaned_ = true;
        token_ = SymbolToken(name);
        return token_;
    }
    src_->get();
    while (!src_->eof()) {
        current = src_->peek();
        if (current >= '0' && current <= '9') {
            name += current;
            src_->get();
        } else {
            break;
        }
    }
    scaned_ = true;
    if ((name[0] == '+' || name[0] == '-') && name.size() == 1) {
        token_ = SymbolToken(name);
    } else {
        token_ = ConstantToken(std::stoi(name));
    }
    return token_;
}