#include "lexer.hpp"
#include "parser.hpp"
#include <ios>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

static int ret = 0;
static std::string spesifik_ret = "?";
static bool error;
static std::string error_info;

static unsigned long int ifade_no = 1;

static void return_error(const std::string& error_information, const int returns, const std::string& spesifik_returns = "?BELİRTİLMEMİŞ?") {
    error = true;
    error_info = "İfade: " + std::to_string(ifade_no) + " - (" + error_information + ")";
    ret = returns;
    spesifik_ret = spesifik_returns;
}

static std::string lexertypeto(Lexer::Type lexertype) {
    if (lexertype == Lexer::Type::noop) return "OPERATÖR DEĞİL";
    else return "OPERATÖR";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Kullanım: " << argv[0] << " <girdi_dosyası> <çıktı_dosyası>" << std::endl;
        return -1;
    }

    std::ifstream input(argv[1], std::ios::binary);
    if (!input.is_open()) {
        std::cerr << "Girdi dosyası açılamadı!" << std::endl;
        return -1;
    }
    
    std::ofstream output(argv[2], std::ios::binary);
    if (!output.is_open()) {
        std::cerr << "Çıktı dosyası açılamadı!" << std::endl;
        return -1;
    }

    std::stringstream ss;
    ss << input.rdbuf();

    std::map<int, std::vector<Lexer::Out>> code1 = lexerfunc(ss);

    output << "LEXER: " << "\n\n";
    for (const auto& [line, out] : code1) {
        output << "Line: " << line+1 << "\n\n";
        for (const auto& [type, data] : out) {
            output << "Type: " << lexertypeto(type) << "\n"
                   << "Data: " << data << "\n\n";
        }
    }
    output << "---\n" << std::endl;

    std::map<int, std::vector<std::string>> code2 = parserfunc(code1);

    output << "PARSER: " << "\n\n";
    for (const auto& [line, data] : code2) {
        output << "Line: " << line+1 << "\n";
        for (size_t size = 0; size < data.size(); size++) output << data[size] << "\n";
        output << "\n";
    }
    output << "---";

    input.close();
    output.close();

    if (error) {
        std::cerr << "MQuaLix DBG: " << error_info << std::endl;
        std::cerr << "MQuaLix: " << ret << " döndü. " << spesifik_ret << std::endl << std::endl;
        return ret;
    } else {
        std::cout << "MQuaLix DBG: Hata bulunamadı." << std::endl;
        std::cout << "MQuaLix: 0 döndü." << std::endl;
        return 0;
    }
}
