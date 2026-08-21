// Max error's: -13 - 0

#include <string>
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

typedef struct Token {
    std::string type;
    std::string value;
} Token;

static std::vector<Token> tokens;
static std::vector<std::string> var_name;

static void tokenize() {
    if (tokens.size() == 2) {
        if (tokens[0].type == "KEYWORD") {
            if (tokens[0].value == "var") {
                if (tokens[1].type == "UNKNOW") var_name.push_back(tokens[1].value);
                else if (tokens[1].type == "VARNAME") return_error("Değişken zaten tanımlanmış!.", -8, "DEĞİŞKEN::ZATEN_TANIMLANMIŞ");
                else if (tokens[1].type == "HEXNUMBER") return_error("Değişken ismi onaltılık olamaz!", -9, "DEĞİŞKEN::ONALTILIK_OLAMAZ");
                else if (tokens[1].type == "DECNUMBER") return_error("Değişken ismi onluk olamaz!", -10, "DEĞİŞKEN::ONLUK_OLAMAZ");
                else if (tokens[1].type == "OCTNUMBER") return_error("Değişken ismi sekizlik olamaz!", -11, "DEĞİŞKEN::SEKİZLİK_OLAMAZ");
                else if (tokens[1].type == "BINNUMBER") return_error("Değişken ismi ikilik olamaz!", -12, "DEĞİŞKEN::İKİLİK_OLAMAZ");
            }
            
            else if (tokens[0].value == "delete") {
                if (tokens[1].type == "VARNAME") {
                    for (size_t size = 0; size < var_name.size();) {
                        if (tokens[1].value == var_name[size]) {
                            var_name.erase(var_name.begin() + size);
                            break;
                        }
                        size++;
                    }
                }
                else if (tokens[1].type == "UNKNOW") return_error("Tanınmayan değişken silinemez!", -14);
                else if (tokens[1].type == "HEXNUMBER") return_error("Değişken ismi onaltılık olamaz!", -9, "DEĞİŞKEN::ONALTILIK_OLAMAZ");
                else if (tokens[1].type == "DECNUMBER") return_error("Değişken ismi onluk olamaz!", -10, "DEĞİŞKEN::ONLUK_OLAMAZ");
                else if (tokens[1].type == "OCTNUMBER") return_error("Değişken ismi sekizlik olamaz!", -11, "DEĞİŞKEN::SEKİZLİK_OLAMAZ");
                else if (tokens[1].type == "BINNUMBER") return_error("Değişken ismi ikilik olamaz!", -12, "DEĞİŞKEN::İKİLİK_OLAMAZ");
            }
            
            else return_error("Anahtar kullanımı anlaşılamadı.", -13, "ANAHTAR::KULLANIM_ANLAŞILAMADI");
        }
    }
}

int main(const int argc, const char **argv) {
    if (argc < 3) {
        std::cerr << "MQuaLix DBG: Hazırlık aşaması - Yetersiz argüman." << std::endl;
        std::cerr << "MQuaLix: -1 döndü. (DERLEME::YETERSİZ_ARGÜMAN)" << std::endl;
        return -1;
    }

    std::ifstream input(argv[1]);

    if (!input.is_open()) {
        std::cerr << "MQuaLix DBG: Hazırlık aşaması - Girdi dosya açılamadı." << std::endl;
        std::cerr << "MQuaLix: -2 döndü. (DERLEME::GİRDİ_DOSYA_AÇILAMADI)" << std::endl;
        return -2;
    }

    std::ofstream output(argv[2]);

    if (!output.is_open()) {
        std::cerr << "MQuaLix DBG: Hazırlık aşaması - Çıktı dosya açılamadı." << std::endl;
        std::cerr << "MQuaLix: -3 döndü. (DERLEME::ÇIKTI_DOSYA_AÇILAMADI)" << std::endl;
        return -3;
    }

    std::string token;
    size_t size = 0;
    while (input >> token) {
        if (error) break;

        if (token == ";") {
            ifade_no++;

            tokenize();

            size = 0;
            tokens.clear();
            continue;
        }

        bool var = false;
        for (auto & i : var_name) {
            if (token == i) {
                var = true;
                tokens.push_back({.type = "VARNAME", .value = i});
                break;
            }
        }

        if (var) {}
        else if (token == "var") tokens.push_back({.type = "KEYWORD", .value = "var"});
        else if (token == "delete") tokens.push_back({.type = "KEYWORD", .value = "delete"});

        else if (token == "=") tokens.push_back({.type = "OPERATOR", .value = "="});
        else if (token == "==") tokens.push_back({.type = "OPERATOR", .value = "=="});
        else if (token == "+=") tokens.push_back({.type = "OPERATOR", .value = "+="});
        else if (token == "-=") tokens.push_back({.type = "OPERATOR", .value = "-="});
        else if (token == "/=") tokens.push_back({.type = "OPERATOR", .value = "/="});
        else if (token == "*=") tokens.push_back({.type = "OPERATOR", .value = "*="});
        else if (token.front() == '#') tokens.push_back({.type = "DEFINITE", .value = token.substr(1)});

        else if (token.substr(0, 2) == "0x") {
            token.erase(0, 2);
            for (const auto& i : token) {
                if (!(i == '0' ||
                i == '1' ||
                i == '2' ||
                i == '3' ||
                i == '4' ||
                i == '5' ||
                i == '6' ||
                i == '7' ||
                i == '8' ||
                i == '9' ||
                i == 'A' ||
                i == 'a' ||
                i == 'B' ||
                i == 'b' ||
                i == 'C' ||
                i == 'c' ||
                i == 'D' ||
                i == 'd' ||
                i == 'E' ||
                i == 'e' ||
                i == 'F' ||
                i == 'f')) {
                error = true;
                break;
                }
            }
            
            if (error) {
                return_error("Onaltılıkta geçersiz harfler.", -4, "SAYI::ONALTILIK_GEÇERSİZ");
                continue;
            }

            tokens.push_back({.type = "HEXNUMBER", .value = token});
        }

        else if (token.front() == '1' ||
            token.front() == '2' ||
            token.front() == '3' ||
            token.front() == '4' ||
            token.front() == '5' ||
            token.front() == '6' ||
            token.front() == '7' ||
            token.front() == '8' ||
            token.front() == '9') {
            for (const auto& i : token) {
                if (!(i == '0' ||
                i == '1' ||
                i == '2' ||
                i == '3' ||
                i == '4' ||
                i == '5' ||
                i == '6' ||
                i == '7' ||
                i == '8' ||
                i == '9')) {
                error = true;
                break;
                }
            }
            
            if (error) {
                return_error("Onlukta geçersiz harfler.", -5, "SAYI::ONLUK_GEÇERSİZ");
                continue;
            }
            
            tokens.push_back({.type = "DECNUMBER", .value = token});
        }

        else if (token.front() == '0') {
            token.erase(0, 1);
            for (const auto& i : token) {
                if (!(i == '0' ||
                i == '1' ||
                i == '2' ||
                i == '3' ||
                i == '4' ||
                i == '5' ||
                i == '6' ||
                i == '7')) {
                error = true;
                break;
                }
            }

            if (error) {
                return_error("Sekizlikte geçersiz harfler.", -6, "SAYI::SEKİZLİK_GEÇERSİZ");
                continue;
            }

            tokens.push_back({.type = "OCTNUMBER", .value = token});
        }

        else if (token.substr(0, 2) == "0b") {
            token.erase(0, 2);
            for (const auto& i : token) {
                if (!(i == '0' ||
                i == '1')) {
                error = true;
                break;
                }
            }

            if (error) {
                return_error("İkilikte geçersiz harfler.", -7, "SAYI::İKİLİK_GEÇERSİZ");
                continue;
            }

            tokens.push_back({.type = "BINNUMBER", .value = token});
        }

        else tokens.push_back({.type = "UNKNOW", .value = token});

        output << "İfade: " << ifade_no << " " << tokens[size].type << " | " << tokens[size].value << std::endl;
        size++;
    }

    input.close();
    output.close();

    if (error) {
        std::cerr << "MQuaLix DBG: " << error_info << std::endl;
        std::cerr << "MQuaLix: " << ret << " döndü. " << spesifik_ret << std::endl;
        return ret;
    } else {
        std::cout << "MQuaLix DBG: Hata bulunamadı." << std::endl;
        std::cout << "MQuaLix: 0 döndü.";
        return 0;
    }
}