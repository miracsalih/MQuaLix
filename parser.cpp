#include "parser.hpp"
#include "lexer.hpp"
#include <string>
#include <vector>
#include <map>

struct Parserlayer1 {
    enum Type {
        Number,
        Keyword,
        Operator
    };

    Parserlayer1::Type type;
    std::string data;
};

std::vector<Parser> parserfunc(std::map<int, std::vector<Lexer::Out>> code) {
    std::vector<Parser> out;
    int line = 0;

    std::map<int, std::vector<Parserlayer1>> parserlayer1;

    for (const auto& [line, kod] : code) {
        std::vector<Parserlayer1> tokens;

        for (size_t size = 0; size < kod.size(); size++) {
            Lexer::Out token = kod[size];

            if (token.lexertype == Lexer::Type::noop) {
                if (token.lexerdata.size() > 2 && token.lexerdata.substr(0, 2) == "0x") {
                    token.lexerdata.erase(0, 2);

                    bool error = false;
                    for (const char& i : token.lexerdata) if (!((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z') || (i >= '0' && i <= '9'))) error = true;

                    if (error) {
                        std::vector<Parser> errormessage;
                        errormessage.push_back({.what_is = Parser::Type::Error, .args = {"ONALTILIK YANLIŞ."}});
                        return errormessage;
                    }

                    tokens.push_back({
                        .type = Parserlayer1::Type::Number,
                        .data = std::to_string(std::stoi(token.lexerdata, 0, 16))
                    });
                    continue;
                }

                else if (token.lexerdata.size() > 2 && token.lexerdata.substr(0, 2) == "0b") {
                    token.lexerdata.erase(0, 2);

                    bool error = false;
                    for (const char& i : token.lexerdata) if (i != '0' && i != '1') error = true;

                    if (error) {
                        std::vector<Parser> errormessage;
                        errormessage.push_back({.what_is = Parser::Type::Error, .args = {"İKİLİK YANLIŞ."}});
                        return errormessage;
                    }

                    tokens.push_back({
                        .type = Parserlayer1::Type::Number,
                        .data = std::to_string(std::stoi(token.lexerdata, 0, 2))
                    });
                    continue;
                }

                else if (token.lexerdata.size() > 1 && token.lexerdata.substr(0, 2).front() == '0') {
                    token.lexerdata.erase(token.lexerdata.begin());

                    bool error = false;
                    for (const char& i : token.lexerdata) if (!(i >= '0' && i <= '7')) error = true;

                    if (error) {
                        std::vector<Parser> errormessage;
                        errormessage.push_back({.what_is = Parser::Type::Error, .args = {"SEKİZLİK YANLIŞ."}});
                        return errormessage;
                    }

                    tokens.push_back({
                        .type = Parserlayer1::Type::Number,
                        .data = std::to_string(std::stoi(token.lexerdata, 0, 8))});
                    continue;
                }

                else {
                    bool error = false;
                    for (const char& i : token.lexerdata) if (!(i >= '0' && i <= '9')) error = true;

                    if (error) {
                        tokens.push_back({.type = Parserlayer1::Type::Keyword, .data = token.lexerdata});
                        continue;
                    }

                    tokens.push_back({.type = Parserlayer1::Type::Number, .data = std::to_string(std::stoi(token.lexerdata, 0, 10))});
                    continue;
                }
            }

            else {
                tokens.push_back({
                    .type = Parserlayer1::Type::Operator,
                    .data = token.lexerdata
                });
            }
        }

        parserlayer1[line] = tokens;
    }

    for (const auto& [line, kod] : parserlayer1) {
        std::vector<std::string> tokens;
        for (const auto& [type, data] : kod) tokens.push_back(data);
        out.push_back({.what_is = Parser::Type::Test, .args = tokens});
    }

    return out;
}
