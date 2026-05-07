#pragma once

#include <variant>
#include <istream>

struct SymbolToken {
    std::string name;
    SymbolToken(const std::string& current) : name(current) {
    }

    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;
    ConstantToken(int current) : value(current) {
    }
    bool operator==(const ConstantToken& other) const;
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    bool IsEnd();

    void Next();

    Token GetToken();

private:
    std::istream* src_;
    Token token_;
    bool scaned_ = false;
    bool is_end_ = false;
};