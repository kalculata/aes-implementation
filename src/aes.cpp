#include <iostream>
#include <sstream>

#include "include/aes.h"


std::string AES::stringToHex(std::string str) {
    std::stringstream sstrm;

    for (const auto& item : str)
    {
        sstrm << std::hex << int(item);
    }
    return sstrm.str();
}

AES::AES(std::string key, std::string message) {
	this->key = key;
}

std::string AES::makeStringKey() {

}