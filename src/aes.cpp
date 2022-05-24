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
}

std::vector <std::string>  AES::keyRefs() {
    static const std::vector <std::string> refs{
        "0","1","2","3","4","5","6","7","8","9",
        "a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
        "+","-","/","*","!","?",".", "#","@","(",")","[","]","%","|","\\","$","-","&", ";", ",", "="
    };
    return refs;
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

std::string AES::vecToBin(std::vector<std::string> vec) {
    // implementation
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

std::vector<std::string> AES::g(std::vector<std::string> v) {
   return roundConst(subWord(rotWord(v)));
}

std::vector<std::string> AES::xor_f(std::vector<std::string> a, std::vector<std::string> b) {
    std::string a_bin = vecToBin(a);
    std::string b_bin = vecToBin(b);

    // implementation
}

std::vector<std::vector<std::string>> AES::generateSubKeys(std::vector<std::vector<std::string>> key) {
    std::vector<std::string> row(4);
    std::vector<std::vector<std::string>> words(key[0].size(), row);

    std::vector<std::vector<std::string>> key_t = transposed(key);

    words[0] = xor_f(key_t[0], g(key_t[key_t.size() - 1]));

    for (int i = 1; i < key_t.size(); i++) {
        words[i] = xor_f(key_t[i], words[i - 1]);
    }

    return transposed(words);
}

std::vector<std::string> rotWord(std::vector<std::string> word) {
    // implementation
}
std::vector<std::string> subWord(std::vector<std::string> word) {
    // implementation
}

std::vector<std::string> roundConst(std::vector<std::string> word) {
    // implementation
}

void AES::keyExpansion(){
    std::vector<std::vector<std::string>> key = keyMatrix;

    for (int i = 0; i < round; i++) {
        std::vector<std::vector<std::string>> newSubKey = generateSubKeys(key);
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