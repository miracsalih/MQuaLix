#include "lexer.hpp"
#include <vector>
#include <map>

struct Parser {
    enum Type {
        /*VarAdd,
        If*/
        Error,
        Test
    };

    Parser::Type what_is;
    std::vector<std::string> args;
};

std::vector<Parser> parserfunc(std::map<int, std::vector<Lexer::Out>> code);
