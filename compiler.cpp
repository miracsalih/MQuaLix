//Copyright (c) 2026 Miraç Salih İşler. This project is licensed under the LGPL-2.1 license.

// 1-3: Derleme hataları
// 4-7: Sayı format hataları
// 8-10: Değişken hataları
// 11: Tanımlanma hataları
// 12-19: Koşul hataları

/*
-1  DERLEME::YETERSİZ_ARGÜMAN
-2  DERLEME::GİRDİ_DOSYA_AÇILAMADI
-3  DERLEME::ÇIKTI_DOSYA_AÇILAMADI

-4  SAYI::ONALTILIK_GEÇERSİZ
-5  SAYI::ONLUK_GEÇERSİZ
-6  SAYI::SEKİZLİK_GEÇERSİZ
-7  SAYI::İKİLİK_GEÇERSİZ

-8  DEĞİŞKEN::ZATEN_TANIMLANMIŞ
-9 DEĞİŞKEN::TANIMLANMAMIŞ
-10  DEĞİŞKEN::SAYISAL_OLAMAZ

-11 TANIM::TANIMSIZ

-12 SAYI::DÖNÜŞTÜRME_HATASI
-13 KOŞUL::ATAMA_YAPILAMAZ
-14 KOŞUL::TOPLAMA_YAPILAMAZ
-15 KOŞUL::ÇIKARMA_YAPILAMAZ
-16 KOŞUL::BÖLME_YAPILAMAZ
-17 KOŞUL::ÇARPMA_YAPILAMAZ
-18 KOŞUL::KOŞUL_SAĞLANMAMIŞ
-19 KOŞUL::ELSE_HATASI
*/

#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

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

typedef enum TypeToken {
    unknow = 0,
    keyword = 1,
    varname = 2,
    number = 3,
    operato = 4,
    definite = 5
} TypeToken;

typedef struct Token {
    TypeToken typetoken;
    std::string value;
} Token;

static std::vector<Token> tokens;
static std::vector<bool> equal;
static std::map<std::string, Token> var;

static int get_number(Token token) {
    try {
        return stoi(token.value, 0, 10);
    } catch (std::invalid_argument) {
        return 0;
    }
}

static std::string get_snumber(Token token) {
    try {
        return std::to_string(stoi(token.value, 0, 10));
    } catch (std::invalid_argument) {
        return "";
    }
}

static std::string type(Token token) {
    if (token.typetoken == TypeToken::unknow) return "UNKNOW";
    else if (token.typetoken == TypeToken::keyword) return "KEYWORD";
    else if (token.typetoken == TypeToken::varname) return "VARNAME";
    else if (token.typetoken == TypeToken::operato) return "OPERATOR";
    else if (token.typetoken == TypeToken::definite) return "DEFINITE";
    else return "";
}

static std::string tokenize() {
    if (tokens.empty()) return "";
    
    if (tokens.size() == 1 && tokens[0].typetoken == TypeToken::keyword && tokens[0].value == "endif") {
        if (equal.empty()) return_error("Koşul sağlanmamış!", -18, "KOŞUL::KOŞUL_SAĞLANMAMIŞ");
        else equal.pop_back();
        return "";
    }

    if (tokens.size() == 1 && tokens[0].typetoken == TypeToken::keyword && tokens[0].value == "else") {
        if (equal.empty()) return_error("Else eşleşmiyor!", -19, "KOŞUL::ELSE_HATASI");
        else equal.back() = !equal.back();
        return "";
    }
    
    if (!equal.empty() && !equal.back()) return "";

    if (tokens[0].typetoken == TypeToken::varname && tokens.size() == 1) {
        return tokens[0].value + " = " + var[tokens[0].value].value;
    }

    else if (tokens[0].typetoken == TypeToken::keyword && tokens[0].value == "print") {
        tokens.erase(tokens.begin());
        for (const auto& i : tokens) {
            if (i.typetoken == TypeToken::varname) std::cout << var[i.value].value << " ";
            else std::cout << i.value << " ";
        }
        std::cout << std::endl;
        return "";
    }

    else if (tokens.size() == 2) {
        if (tokens[0].typetoken == TypeToken::keyword) {
            if (tokens[0].value == "var") {
                if (tokens[1].typetoken == TypeToken::unknow) var[tokens[1].value] = {.typetoken = TypeToken::varname, .value = "n"};
                else if (tokens[1].typetoken == TypeToken::varname) return_error("Değişken zaten tanımlanmış!", -8, "DEĞİŞKEN::ZATEN_TANIMLANMIŞ");
                else if (tokens[1].typetoken == TypeToken::number) return_error("Değişken ismi sayısal olamaz!", -10, "DEĞİŞKEN::SAYISAL_OLAMAZ");
            }
            
            else if (tokens[0].value == "del") {
                if (tokens[1].typetoken == TypeToken::varname) var.erase(tokens[1].value);
                else if (tokens[1].typetoken == TypeToken::unknow) return_error("Tanımsız bir değişken silinemez!", -9, "DEĞİŞKEN::TANIMLANMAMIŞ");
                else if (tokens[1].typetoken == TypeToken::number) return_error("Değişken ismi sayısal olamaz!", -10, "DEĞİŞKEN::SAYISAL_OLAMAZ");
            }
        }
    }
    
    else if (tokens.size() == 3) {
        if (tokens[0].typetoken == TypeToken::varname) {
            if (tokens[1].typetoken == TypeToken::operato) {
                if (tokens[1].value == "=") {
                    if (tokens[2].typetoken == TypeToken::unknow) return_error("Tanımsız bir şey...", -11, "TANIM::TANIMSIZ");
                    else if (tokens[2].typetoken == TypeToken::number) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = get_snumber(tokens[2])};
                    else if (tokens[2].typetoken == TypeToken::varname) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = get_snumber(var[tokens[2].value])};
                }

                else if (tokens[1].value == "+") {
                    if (tokens[2].typetoken == TypeToken::unknow) return_error("Tanımsız bir şey...", -11, "TANIM::TANIMSIZ");
                    else if (tokens[2].typetoken == TypeToken::number) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = std::to_string(get_number(var[tokens[0].value]) + get_number(tokens[2]))};
                    else if (tokens[2].typetoken == TypeToken::varname) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = std::to_string(get_number(var[tokens[0].value]) + get_number(var[tokens[2].value]))};
                }

                else if (tokens[1].value == "-") {
                    if (tokens[2].typetoken == TypeToken::unknow) return_error("Tanımsız bir şey...", -11, "TANIM::TANIMSIZ");
                    else if (tokens[2].typetoken == TypeToken::number) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = std::to_string(get_number(var[tokens[0].value]) - get_number(tokens[2]))};
                    else if (tokens[2].typetoken == TypeToken::varname) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = std::to_string(get_number(var[tokens[0].value]) - get_number(var[tokens[2].value]))};
                }

                else if (tokens[1].value == "/") {
                    if (tokens[2].typetoken == TypeToken::unknow) return_error("Tanımsız bir şey...", -11, "TANIM::TANIMSIZ");
                    else if (tokens[2].typetoken == TypeToken::number) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = std::to_string(get_number(var[tokens[0].value]) / get_number(tokens[2]))};
                    else if (tokens[2].typetoken == TypeToken::varname) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = std::to_string(get_number(var[tokens[0].value]) / get_number(var[tokens[2].value]))};
                }

                else if (tokens[1].value == "*") {
                    if (tokens[2].typetoken == TypeToken::unknow) return_error("Tanımsız bir şey...", -11, "TANIM::TANIMSIZ");
                    else if (tokens[2].typetoken == TypeToken::number) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = std::to_string(get_number(var[tokens[0].value]) * get_number(tokens[2]))};
                    else if (tokens[2].typetoken == TypeToken::varname) var[tokens[0].value] = {.typetoken = TypeToken::number, .value = std::to_string(get_number(var[tokens[0].value]) * get_number(var[tokens[2].value]))};
                }
            }
        }
    }

    else if (tokens.size() == 4) {
        if (tokens[0].typetoken == TypeToken::keyword) {
            if (tokens[0].value == "if") {
                tokens.erase(tokens.begin());
                for (size_t size = 0; size < tokens.size(); size++) {
                    if (tokens[size].typetoken == TypeToken::varname) {
                        int deger = get_number(var[tokens[size].value]);
                        tokens[size].value = std::to_string(deger);
                        tokens[size].typetoken = TypeToken::number;
                    }
                }

                if (tokens[1].value == "=") return_error("Atama koşul karşılaştırmasında yapılamaz!", -13, "KOŞUL::ATAMA_YAPILAMAZ");
                else if (tokens[1].value == "+") return_error("Toplama koşul karşılaştırmasında yapılamaz!", -14, "KOŞUL::TOPLAMA_YAPILAMAZ");
                else if (tokens[1].value == "-") return_error("Çıkarma koşul karşılaştırmasında yapılamaz!", -15, "KOŞUL::ÇIKARMA_YAPILAMAZ");
                else if (tokens[1].value == "/") return_error("Bölme koşul karşılaştırmasında yapılamaz!", -16, "KOŞUL::BÖLME_YAPILAMAZ");
                else if (tokens[1].value == "*") return_error("Çarpma koşul karşılaştırmasında yapılamaz!", -17, "KOŞUL::ÇARPMA_YAPILAMAZ");
                else if (tokens[1].value == "==") {
                    int sol = get_number(tokens[0]);
                    int sag = get_number(tokens[2]);
                    equal.push_back(sol == sag);
                    return (sol == sag ? "True!" : "False!");
                }
            }
        }
    }

    return "";
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

            std::string out = tokenize();
            output << "MQuaLix DBG: " << (out.empty() ? "(dönmedi)" : out) << std::endl << std::endl;

            size = 0;
            tokens.clear();
            continue;
        }

        bool finded = false;
        for (const auto& [isim, veri] : var) {
            if (token == isim) {
                finded = true;
                tokens.push_back({.typetoken = TypeToken::varname, .value = isim});
                break;
            }
        }

        if (finded) {}
        else if (token == "variable" || token == "var") tokens.push_back({.typetoken = TypeToken::keyword, .value = "var"});
        else if (token == "delete" || token == "del") tokens.push_back({.typetoken = TypeToken::keyword, .value = "del"});
        else if (token == "print") tokens.push_back({.typetoken = TypeToken::keyword, .value = "print"});

        else if (token == "if") tokens.push_back({.typetoken = TypeToken::keyword, .value = "if"});
        else if (token == "else") tokens.push_back({.typetoken = TypeToken::keyword, .value = "else"});
        else if (token == "endif") tokens.push_back({.typetoken = TypeToken::keyword, .value = "endif"});

        else if (token == "=") tokens.push_back({.typetoken = TypeToken::operato, .value = "="});
        else if (token == "==") tokens.push_back({.typetoken = TypeToken::operato, .value = "=="});
        else if (token == "+") tokens.push_back({.typetoken = TypeToken::operato, .value = "+"});
        else if (token == "-") tokens.push_back({.typetoken = TypeToken::operato, .value = "-"});
        else if (token == "/") tokens.push_back({.typetoken = TypeToken::operato, .value = "/"});
        else if (token == "*") tokens.push_back({.typetoken = TypeToken::operato, .value = "*"});

        else if (token.front() == '#') tokens.push_back({.typetoken = TypeToken::definite, .value = token.substr(1)});

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

            tokens.push_back({.typetoken = TypeToken::number, .value = std::to_string(stoi(token, 0, 16))});
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
            
            tokens.push_back({.typetoken = TypeToken::number, .value = std::to_string(stoi(token, 0, 10))});
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

            tokens.push_back({.typetoken = TypeToken::number, .value = std::to_string(stoi(token, 0, 8))});
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

            tokens.push_back({.typetoken = TypeToken::number, .value = std::to_string(stoi(token, 0, 2))});
        }

        else tokens.push_back({.typetoken = TypeToken::unknow, .value = token});

        output << "İfade: " << ifade_no << " " << type(tokens[size]) << " | " << tokens[size].value << std::endl;
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
