#include "lexer.hpp"
#include <vector>
#include <map>

struct Parser {
    enum Type {
        If,
        Var
    };

    Parser::Type what_is;
    std::vector<std::string> args;
};

std::vector<Parser> parserfunc(std::map<int, std::vector<Lexer::Out>> code);
