#include "lexer.hpp"
#include <map>
#include <sstream>
#include <vector>
#include <string>

enum Mode {
    normal,
    string,
    comment1,
    comment2
};

static std::vector<char> sytnax_operator = {
    ':',
    '+',
    '-',
    '*',
    '/',
    '=',
    '!',
    ',',
    '(',
    ')'
};

static bool isOperator(char& op) {
    bool equal = false;
    for (const char& sytnax : sytnax_operator) if (sytnax == op) { equal = true; break; }
    return equal;
}

std::map<int, std::vector<Lexer>> lexerfunc(std::stringstream& code) {
    std::map<int, std::vector<std::string>> lexerlayer1;
    std::vector<std::string> tokens;
    std::string token;
    int line = 0;

    std::string kod = code.str();
    Mode mode = Mode::normal;

    for (size_t size = 0; size < kod.size(); size++) {
        char i = kod[size];

        if (i == '\"' && (mode != Mode::comment1 && mode != Mode::comment2)) {
            if (mode == Mode::string) {  
                token += i;
                if (!token.empty()) {
                    tokens.push_back(token);
                    token = "";
                }
                mode = Mode::normal;
            } else {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token = "";
                }
                token += i;
                mode = Mode::string;
            }
            continue;
        }

        if (mode == Mode::comment1 || mode == Mode::comment2) {
            if (i == '\n' && mode == Mode::comment1) {
                mode = Mode::normal;
                continue;
            }
            else if (i == '*' && mode == Mode::comment2) {
                size++;
                if (kod[size] == '/' && kod.size() > 1 + size) {
                    mode = Mode::normal;
                    continue;
                }
                else size--;
            }
        }
        
        if (mode == Mode::string) {
            if (i == '\\' && kod.size() > 1 + size) {
                size++;
                token += i;
                token += kod[size];
            } else token += i;
        }
        
        if (mode == Mode::normal) {
            if (i == ';') {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token = "";
                }
                lexerlayer1[line] = tokens;
                line++;
                tokens.clear();
            } else {
                if (mode != Mode::comment1 && mode != Mode::comment2) {
                    if (i == '/' && kod.size() > 1 + size) {
                        size++;
                        if (kod[size] == '/') {
                            mode = Mode::comment1;
                            continue;
                        } else if (kod[size] == '*') {
                            mode = Mode::comment2;
                            continue;
                        } else {
                            size--;
                        }
                    }
                } 

                if (i != ' ' && i != '\n' && i != '\t') token += i;
                else {
                    if (!token.empty()) {
                        tokens.push_back(token);
                        token = "";
                    }
                }
            }
        }
    }

    if (!token.empty()) {
        if (mode == Mode::string) token += '\"';
        tokens.push_back(token);
        token = "";
        lexerlayer1[line] = tokens;
        line++;
    }



    tokens.clear();
    std::map<int, std::vector<Lexer>> lexerlayer2;
    line = 0;

    for (const auto& [line, kod] : lexerlayer1) {
        std::vector<Lexer> tokensout;
        Mode mode = Mode::normal;

        for (const std::string& i : kod) {
            Lexer::Op op = Lexer::Op::noop;
            std::string k;

            for (size_t idx = 0; idx < i.size(); idx++) {
                Lexer::Op newtype;
                char j = i[idx];

                if (isOperator(j)) newtype = Lexer::Op::yesop;
                else newtype = Lexer::Op::noop;

                if (mode == Mode::normal && op != newtype) {
                    if (!k.empty()) {
                        tokensout.push_back({.lexerop = op, .lexerdata = k});
                        k = "";
                    }
                    op = newtype;
                }

                if (j == '\"') {
                    if (mode == Mode::normal) mode = Mode::string;
                    else mode = Mode::normal;
                }

                k += j;
            }

            if (!k.empty()) tokensout.push_back({.lexerop = op, .lexerdata = k});
        }

        lexerlayer2[line] = tokensout;
    }



    std::map<int, std::vector<Lexer>> lexerlayer3;

    for (const auto& [line, kod] : lexerlayer2) {
        std::vector<Lexer> tokens;

        for (size_t size = 0; size < kod.size(); size++) {
            Lexer token = kod[size];

            if (token.lexerop == Lexer::Op::noop) {
                if (token.lexerdata.size() > 2 && token.lexerdata.substr(0, 2) == "0x") {
                    token.lexerdata.erase(0, 2);

                    bool error = false;
                    for (const char& i : token.lexerdata) if (!((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9'))) error = true;

                    if (error) continue;

                    tokens.push_back({
                        .lexertype = Lexer::Type::Number,
                        .lexerop = Lexer::Op::noop,
                        .lexerdata = std::to_string(std::stoi(token.lexerdata, 0, 16))
                    });
                    continue;
                }

                else if (token.lexerdata.size() > 2 && token.lexerdata.substr(0, 2) == "0b") {
                    token.lexerdata.erase(0, 2);

                    bool error = false;
                    for (const char& i : token.lexerdata) if (i != '0' && i != '1') error = true;

                    if (error) continue;

                    tokens.push_back({
                        .lexertype = Lexer::Type::Number,
                        .lexerop = Lexer::Op::noop,
                        .lexerdata = std::to_string(std::stoi(token.lexerdata, 0, 2))
                    });
                    continue;
                }

                else if (token.lexerdata.size() > 1 && token.lexerdata.substr(0, 2).front() == '0') {
                    token.lexerdata.erase(token.lexerdata.begin());

                    bool error = false;
                    for (const char& i : token.lexerdata) if (!(i >= '0' && i <= '7')) error = true;

                    if (error) continue;

                    tokens.push_back({
                        .lexertype = Lexer::Type::Number,
                        .lexerop = Lexer::Op::noop,
                        .lexerdata = std::to_string(std::stoi(token.lexerdata, 0, 8))});
                    continue;
                }

                else {
                    bool error = false;
                    for (const char& i : token.lexerdata) if (!(i >= '0' && i <= '9')) error = true;

                    if (error) {
                        tokens.push_back({
                            .lexertype = Lexer::Type::Keyword,
                            .lexerop = Lexer::Op::noop,
                            .lexerdata = token.lexerdata});
                        continue;
                    }

                    tokens.push_back({
                        .lexertype = Lexer::Type::Number,
                        .lexerop = Lexer::Op::noop,
                        .lexerdata = std::to_string(std::stoi(token.lexerdata, 0, 10))});
                    continue;
                }
            }

            else {
                tokens.push_back({
                    .lexertype = Lexer::Type::Operator,
                    .lexerop = Lexer::Op::yesop,
                    .lexerdata = token.lexerdata
                });
            }
        }

        lexerlayer3[line] = tokens;
    }

    return lexerlayer3;
}
