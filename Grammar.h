// Copyright (C) 2025 Pablo Macias

#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include "Token.h"
#include "Production.h"
#include <vector>
#include <iostream>

class Grammar {

    private:
        Token startSymbol;
        std::vector<Production> productions;

    public:
        Grammar() {}

        Grammar(Token i, std::vector<Production> p) {
            startSymbol = i;
            productions = p;
        }

        Token getStartSymbol() {
            return startSymbol;
        }

        std::vector<Production> getProductions() {
            return productions;
        }

        void setStartSymbol(Token i) {
            startSymbol = i;
        }

        void setProductions(std::vector<Production> d) {
            productions = d;
        }

        void addProduction(Production d) {
            productions.push_back(d);
        }

        Production productionOnIndex(int i) {
            return productions[i];
        }

        size_t getGrammarSize() {
            return productions.size();
        }

        void augment() {
            Token augmentedStartSymbol = Token(TokenType::NONTERMINAL, "S'");
            Token acceptingEndSymbol = Token(TokenType::FINAL, "$");
            std::vector<Token> body = {startSymbol, acceptingEndSymbol};
            Production aumentada = Production(augmentedStartSymbol, body);
            
            productions.insert(productions.begin(), aumentada);
            startSymbol = augmentedStartSymbol;
        }
};

#endif