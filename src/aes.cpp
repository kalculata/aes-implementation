#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include "include/aes.h"


AES::AES(std::string key, std::string plaintext, int keySize) {
    if (keySize != 128 && keySize != 192 && keySize != 256) {
        std::cout << "invalid key size";
        return;
    }

    this->keySize = keySize;
    this->key = stringToHex(key);
    this->plaintext = plaintext;
    // this->keyMatrix = stringToMatrix(this->key);

}

std::string AES::stringToHex(std::string str) {
    std::stringstream sstrm;

    for (const auto& item : str)
    {
        sstrm << std::hex << int(item);
    }
    return sstrm.str();
}

std::vector<std::vector<std::string>> AES::initStringMatrix(int row, int col) {
    std::vector<std::vector<std::string>> matrix;
    for (int i = 0; i < row; i++) {
        matrix.push_back(std::vector<std::string>(col, ""));
    }
    return matrix;
}

std::vector <std::string>  AES::keyRefs() {
    static const std::vector <std::string> refs{
        "0","1","2","3","4","5","6","7","8","9","10",
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