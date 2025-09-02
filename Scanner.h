// Copyright (C) 2025 Pablo Macias

#ifndef SCANNER_H_
#define SCANNER_H_

#include "Token.h"
#include "TokenType.h"
#include <vector>
#include <string>
#include <cctype>
#include <regex>
#include <iterator>

class Scanner {
    private:
        std::string source;
        std::string analog; // Works as an analog to the source, but easily analyzable through regex
    
    public:
        Scanner() {}

        Scanner(std::string s) {
            source = s;
            analog = "";
        }

        std::pair<Token, std::vector<Token>> scan() {
            std::vector<Token> tokens;
            int state = 0;
            std::string lexema = "";

            for(int i = 0; i < (int)source.size(); i++) {
                char c = source[i];
                switch(state) {
                    case 0:
                        if(isupper(c)) { // Terminals
                            state = 1;
                            lexema += c;
                        } else if(islower(c)) { // Non-Terminals
                            state = 5;
                            lexema += c;
                        } else if(c == ':') { // Equality
                            Token t = Token(TokenType::EQUALITY, ":");
                            tokens.push_back(t);
                            analog += ":";
                            lexema = "";
                        } else if (std::isspace(static_cast<unsigned char>(c))) { // Whitespaces
                            state = 7;
                            lexema = "";
                        } else { // Symbols
                            std::string s(1, c);
                            Token t = Token(TokenType::TERMINAL, s);
                            tokens.push_back(t);
                            analog += "T";
                            lexema = "";
                        }
                        break;
                    case 1:
                        if(isupper(c)) {
                            lexema += c;
                        } else {
                            Token t = Token(TokenType::NONTERMINAL, lexema);
                            tokens.push_back(t);
                            analog += "N";
                            lexema = "";
                            state = 0;
                            i--;
                        }
                        break;
                    case 5:
                        if(islower(c)) {
                            lexema += c;
                        } else {
                            Token t = Token(TokenType::TERMINAL, lexema);
                            tokens.push_back(t);
                            analog += "T";
                            lexema = "";
                            state = 0;
                            i--;
                        }
                        break;
                    case 7:
                        if(!std::isspace(static_cast<unsigned char>(c))) {
                            state = 0;
                            i--;
                        }
                        break;
                    default:
                        break;
                }
            }

            // Clean up last lexema (in case it's not a symbol)
            if(state == 1) {
                Token t = Token(TokenType::NONTERMINAL, lexema);
                tokens.push_back(t);
                analog += "N";
                lexema = "";
            } else if(state == 5) {
                Token t = Token(TokenType::TERMINAL, lexema);
                tokens.push_back(t);
                analog += "T";
                lexema = "";
            }

            Token exitSymbol = Token(TokenType::FINAL, "$");
            tokens.push_back(exitSymbol);

            // Check if production (in analog) conforms to N:(N|T)+
            std::regex reg("^N:(N|T)+$");
            std::smatch m;

            if(!std::regex_match(analog.begin(), analog.end(), reg)) {
                throw std::runtime_error("ERROR: Invalid String: " + source);
                exit(EXIT_FAILURE);
            }

            // Return {LeftSide, RightSide}
            std::vector<Token> rightSide(tokens.begin() + 2, tokens.end());
            return {tokens[0], rightSide};
        }
};

#endif