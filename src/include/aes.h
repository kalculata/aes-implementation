#pragma once
#include <vector>

class AES {
public:
	AES(std::string key, std::string plaintext, int keySize=128);
	static std::string generateKey128();
	static std::string generateKey192();
	static std::string generatekey256();

private:
	static std::vector<int> getRandomRefsIndexs(int number);
	static std::string generateKey(int size);
	static std::vector <std::string> keyRefs();

	std::string stringToHex(std::string str);
	std::vector<std::vector<std::string>> initStringMatrix(int row, int col);
	// std::vector<std::vector<std::string>> stringToMatrix(std::string str, int row);

	int keySize;
	std::string key;
	std::string plaintext;
	std::vector<std::vector<std::string>> keyMatrix;

	
};