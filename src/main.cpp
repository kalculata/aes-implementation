#include <iostream>
#include "include/aes.h"

#define LOG(x) std::cout << x << std::endl;

int main() {
	std::string message = "la cryptographie n'est pas facile.";
	std::string key = "key";
	LOG(AES::aes128(key, message));
}