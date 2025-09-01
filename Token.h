// Copyright (C) 2025 Pablo Macias

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include "TokenType.h"

class Token {
    private:
        TokenType type;
        std::string name;

    public:
        Token() {
            type = TokenType::EMPTY;
            name = "";
        }

        Token(TokenType t, std::string n) {
            type = t;
            name = n;
        }

        TokenType getType() {
            return type;
        }

        std::string getName() {
            return name;
        }

        void setType(TokenType t) {
            type = t;
        }

        void setName(std::string n) {
            name = n;
        }

        bool operator==(const Token& rhs) const {
            return (type == rhs.type) && !(name.compare(rhs.name));
        }

        bool operator!=(const Token& rhs) const {
            return (type != rhs.type) || (name.compare(rhs.name));
        }

        // Needed for set operations, although the set ordering is unimportant
        bool operator<(const Token& rhs) const {
            return rhs.name < name;
        }
};

#endif