#include <iostream>
#include "src/include/aes.h"


void example1(std::string key, std::string plaintext);


int main() {
    example1("Thats my Kung Fu", "Two One Nine Two");
}

void example1(std::string key, std::string plaintext) {
	AES aes = AES(key);

    std::cout << "Plain text    : " << plaintext << "\n";
    std::cout << "Cipher (hex)  : " << aes.encrypt(plaintext) << "\n";
}