#include <fstream>
#include <vector>
#include "include/aes.h"
#include <iostream>

int main(){
    //std::string key = "Thats my Kung Fu";
    std::string key = AES::make_128b_key();
    std::string message = "Two One Nine Two";

    AES aes = AES(key);
    std::cout << "Plain text    : " << message << "\n";
    std::cout << "Cipher (hex)  : " << aes.encrypt(message) << "\n";

    return 0;
}
