//Copyright (c) 2026 Miraç Salih İşler. This project is licensed under the LGPL-2.1 license.

// Kendime not: Maks. hata: -22 - 0 arası.

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

typedef struct Token {
    std::string type;
    std::string value;
} Token;

static std::vector<Token> tokens;
static std::vector<bool> equal;
static std::map<std::string, Token> var;

static int get_number(Token token) {
    if (token.type == "HEXNUMBER") return stoi(token.value, 0, 16);
    else if (token.type == "DECNUMBER") return stoi(token.value, 0, 10);
    else if (token.type == "OCTNUMBER") return stoi(token.value, 0, 8);
    else if (token.type == "BINNUMBER") return stoi(token.value, 0, 2);
    else return_error("Sayı dönüştürme hatası.", -16, "SAYI::DÖNÜŞTÜRME_HATASI");
    return 0;
}

static std::string tokenize() {
    if (tokens.empty()) return "";
    
    // endif işleme
    if (tokens.size() == 1 && tokens[0].type == "KEYWORD" && tokens[0].value == "endif") {
        if (equal.empty()) return_error("Koşul sağlanmamış!", -19, "KOŞUL::KOŞUL_SAĞLANMAMIŞ");
        else equal.pop_back();
        return "";
    }
    
    // else işleme
    if (tokens.size() == 1 && tokens[0].type == "KEYWORD" && tokens[0].value == "else") {
        if (equal.empty()) return_error("Else eşleşmiyor!", -20, "KOŞUL::ELSE_HATASI");
        else equal.back() = !equal.back();
        return "";
    }
    
    // Koşul kontrolü: Eğer koşul false ise bu ifadeyi atla
    if (!equal.empty() && !equal.back()) return "";

    if (tokens[0].type == "VARNAME" && tokens.size() == 1) {
        return tokens[0].value + " = " + var[tokens[0].value].value;
    }

    else if (tokens[0].type == "KEYWORD" && tokens[0].value == "print") {
        tokens.erase(tokens.begin());
        for (const auto& i : tokens) {
            if (i.type == "VARNAME") std::cout << var[i.value].value << " ";
            else std::cout << i.value << " ";
        }
        std::cout << std::endl;
        return "";
    }

    else if (tokens.size() == 2) {
        if (tokens[0].type == "KEYWORD") {
            if (tokens[0].value == "var") {
                if (tokens[1].type == "UNKNOW") var[tokens[1].value] = {.type = "VARNAME", .value = "n"};
                else if (tokens[1].type == "VARNAME") return_error("Değişken zaten tanımlanmış!", -8, "DEĞİŞKEN::ZATEN_TANIMLANMIŞ");
                else if (tokens[1].type == "HEXNUMBER") return_error("Değişken ismi onaltılık olamaz!", -9, "DEĞİŞKEN::ONALTILIK_OLAMAZ");
                else if (tokens[1].type == "DECNUMBER") return_error("Değişken ismi onluk olamaz!", -10, "DEĞİŞKEN::ONLUK_OLAMAZ");
                else if (tokens[1].type == "OCTNUMBER") return_error("Değişken ismi sekizlik olamaz!", -11, "DEĞİŞKEN::SEKİZLİK_OLAMAZ");
                else if (tokens[1].type == "BINNUMBER") return_error("Değişken ismi ikilik olamaz!", -12, "DEĞİŞKEN::İKİLİK_OLAMAZ");
            }
            
            else if (tokens[0].value == "del") {
                if (tokens[1].type == "VARNAME") var.erase(tokens[1].value);
                else if (tokens[1].type == "UNKNOW") return_error("Tanımsız bir değişken silinemez!", -14, "DEĞİŞKEN::TANIMLANMAMIŞ");
                else if (tokens[1].type == "HEXNUMBER") return_error("Değişken ismi onaltılık olamaz!", -9, "DEĞİŞKEN::ONALTILIK_OLAMAZ");
                else if (tokens[1].type == "DECNUMBER") return_error("Değişken ismi onluk olamaz!", -10, "DEĞİŞKEN::ONLUK_OLAMAZ");
                else if (tokens[1].type == "OCTNUMBER") return_error("Değişken ismi sekizlik olamaz!", -11, "DEĞİŞKEN::SEKİZLİK_OLAMAZ");
                else if (tokens[1].type == "BINNUMBER") return_error("Değişken ismi ikilik olamaz!", -12, "DEĞİŞKEN::İKİLİK_OLAMAZ");
            }
            
            else return_error("Anahtar kullanımı anlaşılamadı.", -13, "ANAHTAR::KULLANIM_ANLAŞILAMADI");
        }
    }
    
    else if (tokens.size() == 3) {
        if (tokens[0].type == "VARNAME") {
            if (tokens[1].type == "OPERATOR") {
                if (tokens[1].value == "=") {
                    if (tokens[2].type == "UNKNOW") return_error("Tanımsız bir şey...", -15, "TANIM::TANIMSIZ");
                    else if (tokens[2].type == "HEXNUMBER") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(tokens[2]))};
                    else if (tokens[2].type == "DECNUMBER") var[tokens[0].value] = {.type = "DECNUMBER", .value = std::to_string(get_number(tokens[2]))};
                    else if (tokens[2].type == "OCTNUMBER") var[tokens[0].value] = {.type = "OCTNUMBER", .value = std::to_string(get_number(tokens[2]))};
                    else if (tokens[2].type == "BINNUMBER") var[tokens[0].value] = {.type = "BINNUMBER", .value = std::to_string(get_number(tokens[2]))};
                    else if (tokens[2].type == "VARNAME") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[2].value]))};
                }

                else if (tokens[1].value == "+") {
                    if (tokens[2].type == "UNKNOW") return_error("Tanımsız bir şey...", -15, "TANIM::TANIMSIZ");
                    else if (tokens[2].type == "HEXNUMBER") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) + get_number(tokens[2]))};
                    else if (tokens[2].type == "DECNUMBER") var[tokens[0].value] = {.type = "DECNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) + get_number(tokens[2]))};
                    else if (tokens[2].type == "OCTNUMBER") var[tokens[0].value] = {.type = "OCTNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) + get_number(tokens[2]))};
                    else if (tokens[2].type == "BINNUMBER") var[tokens[0].value] = {.type = "BINNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) + get_number(tokens[2]))};
                    else if (tokens[2].type == "VARNAME") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) + get_number(var[tokens[2].value]))};
                }

                else if (tokens[1].value == "-") {
                    if (tokens[2].type == "UNKNOW") return_error("Tanımsız bir şey...", -15, "TANIM::TANIMSIZ");
                    else if (tokens[2].type == "HEXNUMBER") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) - get_number(tokens[2]))};
                    else if (tokens[2].type == "DECNUMBER") var[tokens[0].value] = {.type = "DECNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) - get_number(tokens[2]))};
                    else if (tokens[2].type == "OCTNUMBER") var[tokens[0].value] = {.type = "OCTNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) - get_number(tokens[2]))};
                    else if (tokens[2].type == "BINNUMBER") var[tokens[0].value] = {.type = "BINNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) - get_number(tokens[2]))};
                    else if (tokens[2].type == "VARNAME") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) - get_number(var[tokens[2].value]))};
                }

                else if (tokens[1].value == "/") {
                    if (tokens[2].type == "UNKNOW") return_error("Tanımsız bir şey...", -15, "TANIM::TANIMSIZ");
                    else if (tokens[2].type == "HEXNUMBER") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) / get_number(tokens[2]))};
                    else if (tokens[2].type == "DECNUMBER") var[tokens[0].value] = {.type = "DECNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) / get_number(tokens[2]))};
                    else if (tokens[2].type == "OCTNUMBER") var[tokens[0].value] = {.type = "OCTNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) / get_number(tokens[2]))};
                    else if (tokens[2].type == "BINNUMBER") var[tokens[0].value] = {.type = "BINNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) / get_number(tokens[2]))};
                    else if (tokens[2].type == "VARNAME") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) / get_number(var[tokens[2].value]))};
                }

                else if (tokens[1].value == "*") {
                    if (tokens[2].type == "UNKNOW") return_error("Tanımsız bir şey...", -15, "TANIM::TANIMSIZ");
                    else if (tokens[2].type == "HEXNUMBER") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) * get_number(tokens[2]))};
                    else if (tokens[2].type == "DECNUMBER") var[tokens[0].value] = {.type = "DECNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) * get_number(tokens[2]))};
                    else if (tokens[2].type == "OCTNUMBER") var[tokens[0].value] = {.type = "OCTNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) * get_number(tokens[2]))};
                    else if (tokens[2].type == "BINNUMBER") var[tokens[0].value] = {.type = "BINNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) * get_number(tokens[2]))};
                    else if (tokens[2].type == "VARNAME") var[tokens[0].value] = {.type = "HEXNUMBER", .value = std::to_string(get_number(var[tokens[0].value]) * get_number(var[tokens[2].value]))};
                }
            }
        }
    }
    
    else if (tokens.size() == 4) {
        if (tokens[0].type == "KEYWORD") {
            if (tokens[0].value == "if") {
                tokens.erase(tokens.begin());
                for (size_t size = 0; size < tokens.size(); size++) {
                    if (tokens[size].type == "VARNAME") {
                        int deger = get_number(var[tokens[size].value]);
                        tokens[size].value = std::to_string(deger);
                        tokens[size].type = "DECNUMBER";
                    }
                }

                if (tokens[1].value == "=") return_error("Atama koşul karşılaştırmasında yapılamaz!", -18, "KOŞUL::ATAMA_YAPILAMAZ");
                else if (tokens[1].value == "+") return_error("Toplama koşul karşılaştırmasında yapılamaz!", -19, "KOŞUL::TOPLAMA_YAPILAMAZ");
                else if (tokens[1].value == "-") return_error("Çıkarma koşul karşılaştırmasında yapılamaz!", -20, "KOŞUL::ÇIKARMA_YAPILAMAZ");
                else if (tokens[1].value == "/") return_error("Bölme koşul karşılaştırmasında yapılamaz!", -21, "KOŞUL::BÖLME_YAPILAMAZ");
                else if (tokens[1].value == "*") return_error("Çarpma koşul karşılaştırmasında yapılamaz!", -22, "KOŞUL::ÇARPMA_YAPILAMAZ");
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
                tokens.push_back({.type = "VARNAME", .value = isim});
                break;
            }
        }

        if (finded) {}
        else if (token == "variable" || token == "var") tokens.push_back({.type = "KEYWORD", .value = "var"});
        else if (token == "delete" || token == "del") tokens.push_back({.type = "KEYWORD", .value = "del"});
        else if (token == "print") tokens.push_back({.type = "KEYWORD", .value = "print"});

        else if (token == "if") tokens.push_back({.type = "KEYWORD", .value = "if"});
        else if (token == "else") tokens.push_back({.type = "KEYWORD", .value = "else"});
        else if (token == "endif") tokens.push_back({.type = "KEYWORD", .value = "endif"});

        else if (token == "=") tokens.push_back({.type = "OPERATOR", .value = "="});
        else if (token == "==") tokens.push_back({.type = "OPERATOR", .value = "=="});
        else if (token == "+") tokens.push_back({.type = "OPERATOR", .value = "+"});
        else if (token == "-") tokens.push_back({.type = "OPERATOR", .value = "-"});
        else if (token == "/") tokens.push_back({.type = "OPERATOR", .value = "/"});
        else if (token == "*") tokens.push_back({.type = "OPERATOR", .value = "*"});

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
