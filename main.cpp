/*
    LR(0) Automaton Generator
    Copyright (C) 2025 Pablo Macias

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    To run:
    .\'<executable>' $(cat '<route to grammar .txt file>')
*/

#include "Scanner.h"
#include "Production.h"
#include "Grammar.h"
#include "Node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>

#define initialProductions std::vector<std::pair<Production, int>>

// Global Variables
std::vector<std::pair<initialProductions, Node*>> existingNodes;
Grammar grammar;
int nodeID;

// Function declarations
void run(int argc, char **argv);
void analyzeGrammarFromFile(int size, char** argv);
Node* generateAutomaton();
Node* generateNode(std::vector<std::pair<Production, int>>);
void printAutomaton(Node*, std::vector<bool>&);
void printNode(Node*, std::vector<bool>&);

int main(int argc, char **argv) {
    try {
        run(argc, argv);
    } catch(const std::runtime_error& e) {
        std::cout<<e.what()<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void run(int argc, char **argv) {
    if(argc == 1) {
        throw std::runtime_error("Pass .txt as parameter\n");
    }

    // Get Grammar from txt file
    try {
        analyzeGrammarFromFile(argc, argv);
    } catch(const std::runtime_error& e) {
        throw e;
    }

    // Generate Automaton   
    Node* initialNode = generateAutomaton();

    std::vector<bool> visited(existingNodes.size(), false); // Avoid visiting node more than once
    printAutomaton(initialNode, visited);
}

void analyzeGrammarFromFile(int size, char** argv) {
    for(int i = 1; i < size; i++) {
        Scanner scanner = Scanner(argv[i]);
        std::pair<Token, std::vector<Token>> completeProduction;

        try {
            completeProduction = scanner.scan(); // Syntax Analysis
        } catch(const std::runtime_error& e) {
            throw e;
        }

        Production production = Production(completeProduction.first, completeProduction.second); // Formalize Production structure
        
        if(i == 1) grammar.setStartSymbol(completeProduction.first);

        grammar.addProduction(production);
    }
    grammar.augment();
}

Node* generateAutomaton() {
    initialProductions augmented = {{grammar.productionOnIndex(0), 0}}; // 0 represents the "point" position
    nodeID = 0;
    return generateNode(augmented);
}

Node* generateNode(initialProductions initial) {
    for(auto checker : existingNodes) {
        if(checker.first == initial) {
            return checker.second;
        }
    }

    Node* newNode = new Node(initial, grammar, nodeID);
    nodeID++;
    existingNodes.push_back({initial, newNode});

    // Add connections recursively
    std::set<Token> tokensToCheck = newNode->getAdjacentTokens();

    for(auto t : tokensToCheck) {
        if(t.getType() == TokenType::FINAL) {
            continue;
        }

        initialProductions newNodeInitials;

        for(auto d : newNode->getStartingProductions()) {
            if(d.first.getBody()[d.second] == t) {
                newNodeInitials.push_back({d.first, d.second + 1}); // Advance "point"
            }
        }

        for(auto d : newNode->getObtainedProductions()) {
            if(d.first.getBody()[d.second] == t) {
                newNodeInitials.push_back({d.first, d.second + 1});
            }
        }

        newNode->addAdjacentNode(generateNode(newNodeInitials), t);
    }

    return newNode;
}

void printAutomaton(Node* initialNode, std::vector<bool>& initialNodeVisited) {
    printNode(initialNode, initialNodeVisited);
}

void printNode(Node* n, std::vector<bool>& visited) {
    if(visited[n->getValue()]) return;

    visited[n->getValue()] = true;

    std::cout<<"\n\n\n=====NODE I"<<n->getValue()<<"\n\n";
    std::cout<<"REDUCTION: "<<n->getReduction().getName()<<"\n";
    std::cout<<"INITIALS:\n";
    for(int i = 0; i < (int)n->getStartingProductions().size(); i++) {
        std::pair<Production, int> t = n->getStartingProductions()[i];
        std::cout<<t.first.getLeftSide().getName()<<" : ";
        for(int j = 0; j < (int)t.first.getBody().size(); j++)  {
            Token t2 = t.first.getBody()[j];
            if(j == t.second) std::cout<<" . ";
            std::cout<<t2.getName()<<" ";
        }
        std::cout<<"\n";
    }
    
    std::cout<<"OBTAINED:\n";
    for(int i = 0; i < (int)n->getObtainedProductions().size(); i++) {
        std::pair<Production, int> t = n->getObtainedProductions()[i];
        std::cout<<t.first.getLeftSide().getName()<<" : ";
        for(int j = 0; j < (int)t.first.getBody().size(); j++)  {
            Token t2 = t.first.getBody()[j];
            if(j == t.second) std::cout<<" . ";
            std::cout<<t2.getName()<<" ";
        }
        std::cout<<"\n";
    }

    for(auto ad : n->getAdjacentNodes()) {
        std::cout<<"SYMBOL "<<ad.second.getName()<<" MOVES TO I"<<ad.first->getValue()<<"\n";
    }
    if(n->getReduction().getName() == "S'") {
        std::cout<<"SYMBOL $ ACCEPTS\n";
    }

    for(auto ad : n->getAdjacentNodes()) {
        printNode(ad.first, visited);
    }
}