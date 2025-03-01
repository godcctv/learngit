#include <string>
#include <variant>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
enum TYPE {
    IDN,        // 标识符
    DEC,        // 十进制
    OCT,        // 合法八进制
    HEX,        // 合法十六进制

    // 运算符与分隔符
    ADD, SUB, MUL, DIV, GT, LT, EQ, GE, LE, NEQ, SLP, SRP, SEMI,

    // 关键字
    IF, THEN, ELSE, WHILE, DO, BEGIN, END,

    //非法八进制和十六进制
    ILOCT, ILHEX
};

struct SPECIES {
    TYPE type = IDN;
    std::variant<int, std::string> value = '-'; // 属性值 (可以是整数或字符串)
};

// 关键字列表
std::unordered_map<std::string, TYPE> keywords = {
    {"if", IF},
    {"then", THEN},
    {"else", ELSE},
    {"while", WHILE},
    {"do", DO},
    {"begin", BEGIN},
    {"end", END}
};

SPECIES anlysics(const std::string& word) {
    SPECIES species;
    //检查是否为关键字
    auto keywordsIt = keywords.find(word);
    if (keywordsIt != keywords.end()) {
        species.type = keywordsIt->second;
        return species;
    }
    //检查是否为标识符
    if (isalpha(word[0]) || word[0] == '_') {
        species.type = IDN;
        species.value = word;
        return species;
    }
    //进入数字检查
    if (isalnum(word[0])) {
        //十进制
        if ((word[0] == '0' && word.length() == 1) || (word[0] != '0')) {
            species.type = DEC;
            species.value = std::stoi(word);
            return species;
        }
        //八进制和非法八进制
        if (word[0] == '0' && word.length() > 1 && word[1] != 'x' && word[1] != 'X') {
            std::string octPart = word.substr(1); // 去掉 "0"
            for (char c : octPart) {//非法检测
                if (c == '8' || c == '9') {
                    species.type = ILOCT;
                    species.value = '-';
                    return species;
                }
            }
            species.type = OCT;
            species.value = std::stoi(word, nullptr, 8);
            return species;
        }
        //十六进制和非法十六进制
        if (word[0] == '0' && word.length() > 1 && (word[1] == 'x' || word[1] == 'X')) {
            std::string hexPart = word.substr(2); // 去掉 "0x" or "0X"
            for (char c : hexPart) {//非法检测
                if ((c >= 'g' && c <= 'z') || (c >= 'G' && c <= 'Z')) {
                    species.type = ILHEX;
                    species.value = '-';
                    return species;
                }
            }
            //十六进制
            species.type = HEX;
            species.value = std::stoi(word, nullptr, 16);
            return species;

        }
    }
    // 运算符与分隔符
    if (word == "+") species.type = ADD;
    else if (word == "-") species.type = SUB;
    else if (word == "*") species.type = MUL;
    else if (word == "/") species.type = DIV;
    else if (word == ">") species.type = GT;
    else if (word == "<") species.type = LT;
    else if (word == "=") species.type = EQ;
    else if (word == ">=") species.type = GE;
    else if (word == "<=") species.type = LE;
    else if (word == "<>") species.type = NEQ;
    else if (word == "(") species.type = SLP;
    else if (word == ")") species.type = SRP;
    else if (word == ";") species.type = SEMI;

    return species;
}
//处理输入
std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    // 先处理输入，去掉开头和结尾的空白字符
    std::string cleanedInput = std::regex_replace(input, std::regex("^\\s+|\\s+$"), "");

    // 改进正则表达式以确保每个部分被正确解析
    std::regex regex(R"(\s*(>=|<=|<>|[;<>=+\-*/()]|0[xX][0-9a-fA-F]+|[0-9]+|[a-zA-Z_]\w*)\s*)");

    auto words_begin = std::sregex_iterator(cleanedInput.begin(), cleanedInput.end(), regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string matched = i->str();
        // 去掉前后的空白符
        matched = std::regex_replace(matched, std::regex("^\\s+|\\s+$"), "");
        if (!matched.empty()) {
            tokens.push_back(matched);
        }
    }
    return tokens;
}

int main() {
    std::string input;
    std::string fullInput;

    std::cout << "输入数据： " << std::endl;

    while (std::getline(std::cin, input)) {
        // 如果用户输入空行，停止读取
        if (input.empty()) {
            break;
        }
        fullInput += input + "\n"; // 将每行输入追加到大字符串中
    }

    // 处理收集到的所有输入
    std::vector<std::string> words = tokenize(fullInput);

    std::cout << "输出数据： " << std::endl;
    for (const auto& word : words) {
        if (!word.empty()) {
            SPECIES species = anlysics(word);
            switch (species.type) {
            case IDN: std::cout << "IDN: " << word << std::endl; break;
            case DEC: std::cout << "DEC: " << std::get<int>(species.value) << std::endl; break;
            case OCT: std::cout << "OCT: " << std::get<int>(species.value) << std::endl; break;
            case HEX: std::cout << "HEX: " << std::get<int>(species.value) << std::endl; break;
            case ADD: std::cout << "ADD: " << '-' << std::endl; break;
            case SUB: std::cout << "SUB: " << '-' << std::endl; break;
            case MUL: std::cout << "MUL: " << '-' << std::endl; break;
            case DIV: std::cout << "DIV: " << '-' << std::endl; break;
            case GT: std::cout << "GT: " << '-' << std::endl; break;
            case LT: std::cout << "LT: " << '-' << std::endl; break;
            case EQ: std::cout << "EQ: " << '-' << std::endl; break;
            case GE: std::cout << "GE: " << '-' << std::endl; break;
            case LE: std::cout << "LE: " << '-' << std::endl; break;
            case NEQ: std::cout << "NEQ: " << '-' << std::endl; break;
            case SLP: std::cout << "SLP: " << '-' << std::endl; break;
            case SRP: std::cout << "SRP: " << '-' << std::endl; break;
            case SEMI: std::cout << "SEMI: " << '-' << std::endl; break;
            case IF: std::cout << "if: " << '-' << std::endl; break;
            case THEN: std::cout << "then: " << '-' << std::endl; break;
            case ELSE: std::cout << "else: " << '-' << std::endl; break;
            case WHILE: std::cout << "while: " << '-' << std::endl; break;
            case DO: std::cout << "do: " << '-' << std::endl; break;
            case BEGIN: std::cout << "begin: " << '-' << std::endl; break;
            case END: std::cout << "end: " << '-' << std::endl; break;
            case ILOCT: std::cout << "ILOCT: " << '-' << std::endl; break;
            case ILHEX: std::cout << "ILHEX: " << '-' << std::endl; break;
            default: std::cout << "其他类型：" << '???' << std::endl; break;
            }
        }
    }

    return 0;
}