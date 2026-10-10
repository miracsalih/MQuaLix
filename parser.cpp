#include "parser.hpp"
#include "lexer.hpp"
#include <string>
#include <vector>
#include <map>

struct Func {
    struct Out {
        bool error;
        std::string out;
    };

    static Out set(std::vector<Lexer> code) {
        if (code.empty()) return {.error = true};
        bool right = false;
        bool name = false;
        std::string number1 = "0";

        std::string data[3] = {"", "", ""};

        for (size_t size = 0; size < code.size(); size++) {
            if (code[size].lexerdata == "=" && (!right)) {
                if (size + 1 < code.size()) {
                    data[2] = "\tmov %0 " + code[size+1].lexerdata + ";";
                    size++;
                }
                right = true;
                continue;
            }

            if (code[size].lexerdata == ":" && (!right && !name)) {
                name = true;
                continue;
            }

            if (!right) {
                if (name) data[1] = code[size].lexerdata;
                else data[0] += code[size].lexerdata;
            }

            if (right) {
                std::string number2 = "__ERR__";
                std::string op = "__ERR__";
                if (size + 1 < code.size()) {
                    op = code[size].lexerdata;
                    number2 = code[size+1].lexerdata;

                    if (op == "+") data[2] += "\n\tadd %" + number1 + " " + number2 + ";";
                    else if (op == "-") data[2] += "\n\tdec %" + number1 + " " + number2 + ";";
                    else if (op == "*") data[2] += "\n\tmul %" + number1 + " " + number2 + ";";
                    else if (op == "/") data[2] += "\n\tdiv %" + number1 + " " + number2 + ";";
                }
            }
        }

        std::string out = "";
        if (name) {
            if (data[0].empty() || data[1].empty()) return {.error = true};
            out += "new " + data[0] + " " + data[1] + ";";

            if (right) {
                if (data[2].empty() || (!right)) return {.error = true};
                out += "\nset " + data[1] + " {\n" + data[2] + "\n};";
            }
        } else {
            if (data[0].empty()) return {.error = true};

            if (data[0].back() == '~') out += "del " + data[0].substr(0, data[0].size() - 1) + ";";
            else {
                if (data[2].empty() || !right) return {.error = true};
                out += "set " + data[0] + " {\n" + data[2] + "\n};";
            }
        }

        return {.error = false, .out = out};
    }

    struct If {
        enum Operator {
            iseq,   // ==
            noteq,  // !=
            unk     // ?
        };

        If::Operator ifoperator;
        std::string left, right;
    };

    static Out equal(std::vector<Lexer> code) {
        if (code.empty()) return {.error = true};

        if (code[0].lexerdata != "if") return {.error = true};
        
        code.erase(code.begin());
        std::vector<Func::If> datas;
        Func::If data;
        bool right = false;

        for (size_t size = 0; size < code.size(); size++) {
            if (code[size].lexerdata == "&&" && right) {
                datas.push_back(data);
                data.ifoperator = If::Operator::unk;
                data.left = "";
                data.right = "";
                right = false;
                continue;
            }

            else if (code[size].lexertype == Lexer::Type::Operator && !right) {
                if (code[size].lexerdata == "==") data.ifoperator = If::Operator::iseq;
                else if (code[size].lexerdata == "!=") data.ifoperator = If::Operator::noteq;
                else continue;
                right = true;
                continue;
            }

            if (!right) data.left += code[size].lexerdata;
            else data.right += code[size].lexerdata;
        }

        if (!data.right.empty()) {
            datas.push_back(data);
            data.right = "";
        }

        std::string out = "if (";
        for (size_t size = 0; size < datas.size(); size++) {
            data = datas[size];

            std::string op;
            if (data.ifoperator == If::Operator::iseq) op = "==";
            else if (data.ifoperator == If::Operator::noteq) op = "!=";
            else return {.error = true};

            if (size > 0) {
                out += "&&(";
            }

            out += data.left + op + data.right + ")";
        }

        return {.error = false, .out = out};
    }
} static Func;

std::map<int, std::vector<std::string>> parserfunc(std::map<int, std::vector<Lexer>> code) {
    std::map<int, std::vector<std::string>> out;

    for (const auto& [line, parser] : code) {        
        std::vector<std::string> tokens;
        Func::Out token;

        token = Func::set(parser);
        if (!token.error) {
            tokens.push_back(token.out);
            out[line] = tokens;
            continue;
        }

        token = Func::equal(parser);
        if (!token.error) {
            tokens.push_back(token.out);
            out[line] = tokens;
            continue;
        }

        if (token.error) tokens.push_back("error(line/expression: " + std::to_string(line+1) + ");");

        out[line] = tokens;
    }

    return out;
}
