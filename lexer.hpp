#pragma once
#include <map>
#include <vector>
#include <string>

struct Lexer {
    enum Type {
        Number,
        Keyword,
        Operator
    };

    enum Op {
        yesop,
        noop
    };

    Lexer::Type lexertype;        // TYPE
    Lexer::Op lexerop;            // OPER
    std::string lexerdata;        // DATA
};

std::map<int, std::vector<Lexer>> lexerfunc(std::stringstream& code);
