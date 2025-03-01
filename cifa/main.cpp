#include <string>
#include <variant>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
enum TYPE {
    IDN,        // ��ʶ��
    DEC,        // ʮ����
    OCT,        // �Ϸ��˽���
    HEX,        // �Ϸ�ʮ������

    // �������ָ���
    ADD, SUB, MUL, DIV, GT, LT, EQ, GE, LE, NEQ, SLP, SRP, SEMI,

    // �ؼ���
    IF, THEN, ELSE, WHILE, DO, BEGIN, END,

    //�Ƿ��˽��ƺ�ʮ������
    ILOCT, ILHEX
};

struct SPECIES {
    TYPE type = IDN;
    std::variant<int, std::string> value = '-'; // ����ֵ (�������������ַ���)
};

// �ؼ����б�
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
    //����Ƿ�Ϊ�ؼ���
    auto keywordsIt = keywords.find(word);
    if (keywordsIt != keywords.end()) {
        species.type = keywordsIt->second;
        return species;
    }
    //����Ƿ�Ϊ��ʶ��
    if (isalpha(word[0]) || word[0] == '_') {
        species.type = IDN;
        species.value = word;
        return species;
    }
    //�������ּ��
    if (isalnum(word[0])) {
        //ʮ����
        if ((word[0] == '0' && word.length() == 1) || (word[0] != '0')) {
            species.type = DEC;
            species.value = std::stoi(word);
            return species;
        }
        //�˽��ƺͷǷ��˽���
        if (word[0] == '0' && word.length() > 1 && word[1] != 'x' && word[1] != 'X') {
            std::string octPart = word.substr(1); // ȥ�� "0"
            for (char c : octPart) {//�Ƿ����
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
        //ʮ�����ƺͷǷ�ʮ������
        if (word[0] == '0' && word.length() > 1 && (word[1] == 'x' || word[1] == 'X')) {
            std::string hexPart = word.substr(2); // ȥ�� "0x" or "0X"
            for (char c : hexPart) {//�Ƿ����
                if ((c >= 'g' && c <= 'z') || (c >= 'G' && c <= 'Z')) {
                    species.type = ILHEX;
                    species.value = '-';
                    return species;
                }
            }
            //ʮ������
            species.type = HEX;
            species.value = std::stoi(word, nullptr, 16);
            return species;

        }
    }
    // �������ָ���
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
//��������
std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    // �ȴ������룬ȥ����ͷ�ͽ�β�Ŀհ��ַ�
    std::string cleanedInput = std::regex_replace(input, std::regex("^\\s+|\\s+$"), "");

    // �Ľ�������ʽ��ȷ��ÿ�����ֱ���ȷ����
    std::regex regex(R"(\s*(>=|<=|<>|[;<>=+\-*/()]|0[xX][0-9a-fA-F]+|[0-9]+|[a-zA-Z_]\w*)\s*)");

    auto words_begin = std::sregex_iterator(cleanedInput.begin(), cleanedInput.end(), regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string matched = i->str();
        // ȥ��ǰ��Ŀհ׷�
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

    std::cout << "�������ݣ� " << std::endl;

    while (std::getline(std::cin, input)) {
        // ����û�������У�ֹͣ��ȡ
        if (input.empty()) {
            break;
        }
        fullInput += input + "\n"; // ��ÿ������׷�ӵ����ַ�����
    }

    // �����ռ�������������
    std::vector<std::string> words = tokenize(fullInput);

    std::cout << "������ݣ� " << std::endl;
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
            default: std::cout << "�������ͣ�" << '???' << std::endl; break;
            }
        }
    }

    return 0;
}