// Copyright (C) 2025 Pablo Macias

#ifndef NODE_H_
#define NODE_H_

#include "Token.h"
#include "Production.h"
#include "Grammar.h"
#include <set>

class Node {
    private:
        Token reduction;
        int value;
        std::vector<std::pair<Production, int>> startingProductions;
        std::vector<std::pair<Production, int>> obtainedProductions;
        std::vector<std::pair<Node*, Token>> adjacentNodes;
        std::set<Token> adjacentTokens;

    public:
        Node() {}

        Node(std::vector<std::pair<Production, int>> initial, Grammar& g, int v) {
            reduction = Token();
            startingProductions = initial;
            value = v;

            try {
                generateObtainedProductions(g);
            } catch(const std::runtime_error& e) {
                std::cout<<e.what()<<std::endl;
                exit(EXIT_FAILURE);
            }
        }

        void setReduction(Token t) {
            reduction = t;
        }

        std::set<Token> getAdjacentTokens() {
            return adjacentTokens;
        }

        Token getReduction() {
            return reduction;
        }

        std::vector<std::pair<Production, int>> getStartingProductions() {
            return startingProductions;
        }

        std::vector<std::pair<Production, int>> getObtainedProductions() {
            return obtainedProductions;
        }

        std::vector<std::pair<Node*, Token>> getAdjacentNodes() {
            return adjacentNodes;
        }

        int getValue() {
            return value;
        }

        void addAdjacentNode(Node* n, Token t) {
            adjacentNodes.push_back({n, t});
        }

        void generateObtainedProductions(Grammar& g) {
            std::vector<Production> productions = g.getProductions();
            std::set<Token> addedTokens;


            // Add starting productions
            for(auto initial : startingProductions) {
                std::vector<Token> body = initial.first.getBody();
                Token analyzedToken = body[initial.second]; // Token "touched" by the "point"

                if(analyzedToken.getType() == TokenType::FINAL) {
                    if(reduction.getType() == TokenType::EMPTY || reduction == initial.first.getLeftSide()) {
                        reduction = initial.first.getLeftSide();
                    } else {
                        // Collisions, or multiple acceptance states
                        throw std::runtime_error("ERROR: Grammar is not LR(0)");
                    }
                }

                if(addedTokens.count(analyzedToken)) continue; // If token was added

                for(Production d : productions) {
                    if(analyzedToken == d.getLeftSide()) {
                        obtainedProductions.push_back({d, 0});
                    }
                }

                addedTokens.insert(analyzedToken);
            }

            int obtainedProductionsSize = obtainedProductions.size();

            for(int i = 0; i < obtainedProductionsSize; i++) {
                std::vector<Token> body = obtainedProductions[i].first.getBody();
                Token analyzedToken = body[obtainedProductions[i].second]; // Token "touched" by the "point"

                if(addedTokens.count(analyzedToken)) continue;

                for(Production d : productions) {
                    if(addedTokens.count(d.getLeftSide())) continue;

                    if(analyzedToken == d.getLeftSide()) {
                        obtainedProductions.push_back({d, 0});
                        obtainedProductionsSize++;
                    }
                }

                addedTokens.insert(analyzedToken);
            }

            adjacentTokens = addedTokens;
        }
};

#endif