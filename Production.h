// Copyright (C) 2025 Pablo Macias

#ifndef PRODUCTION_H_
#define PRODUCTION_H_

#include "Token.h"
#include <vector>
#include <iostream>

class Production {
    private:
        Token leftSide;
        std::vector<Token> body;

    public:
        Production() {}
        
        Production(Token i, std::vector<Token> c) {
            leftSide = i;
            body = c;
        }

        Token getLeftSide() {
            return leftSide;
        }

        std::vector<Token> getBody() {
            return body;
        }

        void setLeftSide(Token li) {
            leftSide = li;
        }

        void setBody(std::vector<Token> c) {
            body = c;
        }

        bool operator==(const Production& rhs) const {
            return (rhs.body == body) && (rhs.leftSide == leftSide);
        }

};

#endif