#pragma once
#include <vector>

class AES {
public:
	AES(std::string key, std::string plaintext);
	std::string aes128();
	std::string aes192();
	std::string aes256();

private:
	std::string makeStringKey();
	std::string stringToHex(std::string str);

	std::string key;
	std::string plaintext;
	std::vector<std::vector<std::string>> keyMatrix;
};