#include <parser.h>
#include <memory>
#include <vector>
#include "error.h"
#include "object.h"
#include "tokenizer.h"

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);

std::shared_ptr<Object> Read(Tokenizer* tokenizer, bool in_list) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("END of stream!");
    }
    Token current = tokenizer->GetToken();
    tokenizer->Next();
    if (std::get_if<BracketToken>(&current)) {
        if (std::get<BracketToken>(current) == BracketToken::OPEN) {
            return ReadList(tokenizer);
        }
        if (in_list) {
            return std::make_shared<CloseBracket>();
        } else {
            throw SyntaxError("Unexpected closing bracket");
        }
    }
    if (std::get_if<ConstantToken>(&current)) {
        int value = std::get<ConstantToken>(current).value;
        return std::make_shared<Number>(value);
    }
    if (std::get_if<SymbolToken>(&current)) {
        std::string name = std::get<SymbolToken>(current).name;
        if (name == "#t") {
            return std::make_shared<Boolean>(true);
        }
        if (name == "#f") {
            return std::make_shared<Boolean>(false);
        }
        return std::make_shared<Symbol>(name);
    }
    if (std::get_if<DotToken>(&current)) {
        if (in_list) {
            return std::make_shared<Dot>();
        } else {
            throw SyntaxError("Dot used outside of list");
        }
    }
    if (std::get_if<QuoteToken>(&current)) {
        std::shared_ptr<Object> next = Read(tokenizer, in_list);
        std::shared_ptr<Object> quote_sym = std::make_shared<Symbol>("quote");
        std::shared_ptr<Object> tail = std::make_shared<Cell>(next, nullptr);
        return std::make_shared<Cell>(quote_sym, tail);
    }
    throw SyntaxError("Unknown token");
}

std::shared_ptr<Object> MakeList(const std::vector<std::shared_ptr<Object>>& buf,
                                 std::shared_ptr<Object> tail = nullptr) {
    std::shared_ptr<Object> result = tail;
    for (ssize_t i = buf.size() - 1; i >= 0; --i) {
        result = std::make_shared<Cell>(buf[i], result);
    }
    return result;
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    std::vector<std::shared_ptr<Object>> buf;
    while (true) {
        if (tokenizer->IsEnd()) {
            throw SyntaxError("Unclosed list");
        }
        std::shared_ptr<Object> current = Read(tokenizer, true);
        if (Is<CloseBracket>(current)) {
            return MakeList(buf);
        }
        if (Is<Dot>(current)) {
            if (buf.empty()) {
                throw SyntaxError("Dot cann't be the first elem");
            }
            std::shared_ptr<Object> tail = Read(tokenizer, true);
            if (Is<CloseBracket>(tail) || tokenizer->IsEnd()) {
                throw SyntaxError("Expected elem after Dot");
            }
            std::shared_ptr<Object> check_close = Read(tokenizer, true);
            if (!Is<CloseBracket>(check_close)) {
                throw SyntaxError("Expected ')' after Dot part");
            }
            return MakeList(buf, tail);
        }
        buf.push_back(current);
    }
}