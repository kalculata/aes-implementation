#pragma once
#include <vector>

class AES {
public:
	AES(std::string key, int keySize=AES::USE_AES_128, int mode=USE_MODE_EBC);
	std::string encrypt(std::string plaintext);
	std::string decrypt(std::string cipher);

	static std::string generateKey128();
	static std::string generateKey192();
	static std::string generatekey256();

	static const int USE_MODE_CBC = 0;
	static const int USE_MODE_EBC = 1;
	static const int USE_AES_128 = 128;
	static const int USE_AES_192 = 192;
	static const int USE_AES_256 = 256;

	std::vector<std::vector<std::string>> getKeyMatrix() { return keyMatrix; }

private:
	static std::vector<int> getRandomRefsIndexs(int number);
	static std::vector <std::string> keyRefs();
	static std::string generateKey(int size);
	static std::string getSubString(std::string str, int startindex, int lastindex);

	std::vector<std::vector<std::string>> formatKey();
	std::string stringToHex(std::string str);

	int keySize = 0;
	int mode = 0;
	std::string key;
	std::vector<std::vector<std::string>> keyMatrix;
	std::string plaintext;
};