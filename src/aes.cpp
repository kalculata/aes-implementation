#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include "include/aes.h"


AES::AES(std::string key, int keySize, int mode) {
    if (keySize != 128 && keySize != 192 && keySize != 256) {
        std::cout << "invalid key size";
        return;
    }
    if (key.length() < keySize / 8) {
        std::cout << "key is to short " << std::endl;
        return;
    }
    if (key.length() > keySize / 8) {
        std::cout << "key must be of length: " << keySize / 8 << std::endl;
        std::cout << "subject key: " << getSubString(key, 0, keySize/8) << std::endl;
        key = getSubString(key, 0, keySize / 8);
    }
    switch (keySize) {
        case 128:
            round = 10;
            break;
        case 192:
            round = 12;
            break;
        case 256:
            round = 14;
            break;
    }
    
    this->keySize = keySize;
    this->key = stringToHex(key);
    this->keyMatrix = formatKey();
    keyExpansion();
}

std::vector<int> AES::getRandomRefsIndexs(int number) {
    srand(time(0));

    std::vector<int> indexs(number);

    for (int i = 0; i < number; i++) {
        indexs[i] = rand() % keyRefs().size();
    }
    return indexs;
}

std::vector<std::vector<std::string>> AES::formatKey() {
    std::vector<std::string> row(keySize / 32);
    std::vector<std::vector<std::string>> fkey(4, row);
    std::string t;
    int cursor = 0;

    for (int j=0; j<keySize/32; j++) {
        for (int i=0; i<4; i++) {
            t = "";
            t += key[cursor];
            t += key[cursor + 1];
            fkey[i][j] = t;
            cursor+=2;
        }
    }

    return fkey;
}

std::string AES::stringToHex(std::string str) {
    std::stringstream sstrm;

    for (const auto& item : str)
    {
        sstrm << std::hex << int(item);
    }
    return sstrm.str();
}

std::string AES::vecToString(std::vector<std::string> vec) {
    std::string str = "";

    for (int i = 0; i < vec.size(); i++) {
        str += vec[i];
    }
    return str;
}

std::string AES::vecToBin(std::vector<std::string> vec) {
    std::string hex_str = vecToString(vec);
    std::string bin;

    for (unsigned i = 0; i != hex_str.length(); i++)
        bin += hex_char_to_bin(hex_str[i]);
    return bin;
}

const char* AES::hex_char_to_bin(char c){
    switch (toupper(c)){
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
}

std::string AES::bin_str_to_hex(std::string bin_str) {
    std::string binToHex, tmp = "0000";

    for (size_t j = 0; j < bin_str.size(); j += 4) {
        tmp = bin_str.substr(j, 4);
        if (!tmp.compare("0000")) binToHex += "0";
        else if (!tmp.compare("0001")) binToHex += "1";
        else if (!tmp.compare("0010")) binToHex += "2";
        else if (!tmp.compare("0011")) binToHex += "3";
        else if (!tmp.compare("0100")) binToHex += "4";
        else if (!tmp.compare("0101")) binToHex += "5";
        else if (!tmp.compare("0110")) binToHex += "6";
        else if (!tmp.compare("0111")) binToHex += "7";
        else if (!tmp.compare("1000")) binToHex += "8";
        else if (!tmp.compare("1001")) binToHex += "9";
        else if (!tmp.compare("1010")) binToHex += "a";
        else if (!tmp.compare("1011")) binToHex += "b";
        else if (!tmp.compare("1100")) binToHex += "c";
        else if (!tmp.compare("1101")) binToHex += "d";
        else if (!tmp.compare("1110")) binToHex += "e";
        else if (!tmp.compare("1111")) binToHex += "f";
        else continue;
    }

    return binToHex;
}

std::vector<std::vector<std::string>> AES::transposed(std::vector<std::vector<std::string>> m) {
    std::vector<std::string> row(m[0].size());
    std::vector<std::vector<std::string>> m_t(m.size(), row);

    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++) {
            m_t[i][j] = m[j][i];
        }
    }
    return m_t;
}

std::vector<std::string> AES::g(std::vector<std::string> v, int round) {
   return addRoundConst(subWord(rotWord(v)), round);
}

std::vector<std::string> AES::xor_f(std::vector<std::string> a, std::vector<std::string> b) {
    std::string a_bin = vecToBin(a), b_bin = vecToBin(b), bin_rep = "", hex_rep = "", t = "";
    std::vector < std::string> vec;

    for (int i = 0; i < a_bin.size(); i++) {
        if (a_bin[i] != b_bin[i])
            bin_rep += "1";
        else 
            bin_rep += "0";
    }

    hex_rep = bin_str_to_hex(bin_rep);

    for (int i = 0; i < hex_rep.length(); i+=2) {
        t = "";
        t += hex_rep[i];
        t += hex_rep[i+1];
        vec.push_back(t);
    }

    return vec;
}

std::vector<std::vector<std::string>> AES::generateSubKey(std::vector<std::vector<std::string>> key, int round) {
    std::vector<std::string> row(4);
    std::vector<std::vector<std::string>> words(key[0].size(), row);
    std::vector<std::vector<std::string>> key_t = transposed(key);

    words[0] = xor_f(key_t[0], g(key_t[key_t.size() - 1], round));

    for (int i = 1; i < key_t.size(); i++) {
        words[i] = xor_f(key_t[i], words[i - 1]);
    }

    return transposed(words);
}

std::vector<std::string> AES::rotWord(std::vector<std::string> word) {
    std::vector < std::string> vec(word.size());

    for (int i = 1; i < word.size(); i++) 
        vec[i-1] = word[i];
    vec[vec.size()-1] = word[0];

    return vec;
}

std::vector<std::string> AES::subWord(std::vector<std::string> word) {
    std::vector <std::string> vec;

    for (int i= 0; i < word.size(); i++)
        vec.push_back(subBox(word[i]));
    
    return vec;
}

std::vector<std::string> AES::addRoundConst(std::vector<std::string> word, int round) {
    return xor_f(word, rconsts(round));
}

void AES::keyExpansion() {
    std::vector<std::vector<std::string>> key = keyMatrix;

    for (int i = 0; i < round; i++) {
        std::vector<std::vector<std::string>> newSubKey = generateSubKey(key, i+1);

        // display key
        for (int i = 0; i < newSubKey.size(); i++) {
            for (int j = 0; j < newSubKey[0].size(); j++) {
                std::cout << newSubKey[i][j] << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << "----------------------------------------------" << "\n";

        subKeys.push_back(newSubKey);
        key = newSubKey;
    }

}

std::string AES::getSubString(std::string str, int startindex, int lastindex) {
    std::string output = "";
    for (int i = startindex; i < lastindex; i++) {
        output += str[i];
    }
    return output;
}

std::string AES::generateKey(int size) {
    std::string key = "";
    std::vector<int> indexs = getRandomRefsIndexs(size);

    for (int i = 0; i<indexs.size(); i++) {
        key += keyRefs()[indexs[i]];
    }
    return key;
}

std::string AES::generateKey128() { return AES::generateKey(128 / 8); }

std::string AES::generateKey192() { return AES::generateKey(192 / 8); }

std::string AES::generatekey256() { return AES::generateKey(256 / 8); }