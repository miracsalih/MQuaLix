#include "parser.hpp"
#include "lexer.hpp"
#include <vector>
#include <map>

std::vector<Parser> parserfunc(std::map<int, std::vector<Lexer::Out>> code) {
    std::vector<Parser> out;
    int line = 0;

    for (; line < code.size(); line++) {
        std::vector<Lexer::Out> kod = code[line];

        // here...
    }

    return out;
}
