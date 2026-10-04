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

struct Func {
    static std::string set(std::vector<Parserlayer1> code) {
        if (code.empty()) return "";

        std::string out;
        bool right = false;
        bool name = false;
        std::string number1 = "0";
        out = "new ";

        for (size_t size = 0; size < code.size(); size++) {
            if (!right) out += code[size].data;

            if (code[size].data == "=" && (!right && name)) {
                out += ";\ndata{;\n\tmov %0 ";
                if (size + 1 < code.size()) {
                    size++;
                    out += code[size].data;
                }
                out += ";\n";
                right = true;
                continue;
            }

            if (code[size].data != ":" && (!right && !name)) {
                out += ";\nname ";
                name = true;
                continue;
            }

            if (right) {
                std::string number2 = "__ERR__";
                std::string op = "__ERR__";
                if (size + 1 < code.size()) {
                    op = code[size].data;
                    number2 = code[size+1].data;

                    if (op == "+") out += "\tadd %" + number1 + " " + number2 + ";\n";
                    else if (op == "-") out += "\tdec %" + number1 + " " + number2 + ";\n";
                    else if (op == "*") out += "\tmul %" + number1 + " " + number2 + ";\n";
                    else if (op == "/") out += "\tdiv %" + number1 + " " + number2 + ";\n";
                }
            }
        }
        
        out += "};";
        return out;
    }
} static Func;

static void leftright(std::map<int, std::vector<Parserlayer1>>& code) {
    std::map<int, std::vector<Parserlayer1>> out;
    size_t max = code.size();

    for (const auto& [line, kod] : code) {
        std::vector<Parserlayer1> l;
        std::vector<Parserlayer1> r;
        bool rb = false;

        for (const auto& [type, data] : kod) {
            if (type == Parserlayer1::Type::Operator) {
                if (data == "=") {
                    rb = true;
                    continue;
                } else if (data == "==") {
                    rb = true;
                    continue;
                }
            }
            
            if (rb) r.push_back({.type = type, .data = data});
            else l.push_back({.type = type, .data = data});
        }

        out[line] = l;
        out[line+1] = r;
    }

    code = out;
}

std::map<int, std::vector<std::string>> parserfunc(std::map<int, std::vector<Lexer::Out>> code) {
    std::map<int, std::vector<std::string>> out;
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

                    if (error) continue;

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

                    if (error) continue;

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

                    if (error) continue;

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

    for (const auto& [line, parser] : parserlayer1) {        
        std::vector<std::string> tokens;
        std::string token = "";

        token = Func::set(parser);
        tokens.push_back(token);

        out[line] = tokens;
    }

    return out;
}
