#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>

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

	static std::string subBox(std::string value) {
		std::string a(1, value[0]);
		std::string b(1, value[1]);

		std::map<std::string, std::vector<std::string>> sbox = {
					//0    1     2	   3     4     5     6     7     8     9     a     b	 c     d     e     f
			{"0", { "63", "7c", "77", "7b", "f2", "6b", "6f", "c5", "30", "01", "67", "2b", "fe", "d7", "ab", "76" }},
			{"1", { "ca", "82", "c9", "7d", "fa", "59", "47", "f0", "ad", "d4", "a2", "af", "9c", "a4", "72", "c0" }},
			{"2", { "b7", "fd", "93", "26", "36", "3f", "f7", "cc", "34", "a5", "e5", "f1", "71", "d8", "31", "15" }},
			{"3", { "04", "c7", "23", "c3", "18", "96", "05", "9a", "07", "12", "80", "e2", "eb", "27", "b2", "75" }},
			{"4", { "09", "83", "2c", "1a", "1b", "6e", "5a", "a0", "52", "3b", "d6", "b3", "29", "e3", "2f", "84" }},
			{"5", { "53", "d1", "00", "ed", "20", "fc", "b1", "5b", "6a", "cb", "be", "39", "4a", "4c", "58", "cf" }},
			{"6", { "d0", "ef", "aa", "fb", "43", "4d", "33", "85", "45", "f9", "02", "7f", "50", "3c", "9f", "a8" }},
			{"7", { "51", "a3", "40", "8f", "92", "9d", "38", "f5", "bc", "b6", "da", "21", "10", "ff", "f3", "d2" }},
			{"8", { "cd", "0c", "13", "ec", "5f", "97", "44", "17", "c4", "a7", "7e", "3d", "64", "5d", "19", "73" }},
			{"9", { "60", "81", "4f", "dc", "22", "2a", "90", "88", "46", "ee", "b8", "14", "de", "5e", "0b", "db" }},
			{"a", { "e0", "32", "3a", "0a", "49", "06", "24", "5c", "c2", "d3", "ac", "62", "91", "95", "e4", "79" }},
			{"b", { "e7", "c8", "37", "6d", "8d", "d5", "4e", "a9", "6c", "56", "f4", "ea", "65", "7a", "ae", "08" }},
			{"c", { "ba", "78", "25", "2e", "1c", "a6", "b4", "c6", "e8", "dd", "74", "1f", "4b", "bd", "8b", "8a" }},
			{"d", { "70", "3e", "b5", "66", "48", "03", "f6", "0e", "61", "35", "57", "b9", "86", "c1", "1d", "9e" }},
			{"e", { "e1", "f8", "98", "11", "69", "d9", "8e", "94", "9b", "1e", "87", "e9", "ce", "55", "28", "df" }},
			{"f", { "8c", "a1", "89", "0d", "bf", "e6", "42", "68", "41", "99", "2d", "0f", "b0", "54", "bb", "16" }}
		};

		return sbox[a][std::stoi(b, 0, 16)];
	}
	static std::vector<std::string> rconsts(int index) {
		std::map<int, std::vector<std::string>> consts = {
			{1, {"01","00","00","00"}},
			{2, {"02","00","00","00"}},
			{3, {"04","00","00","00"}},
			{4, {"08","00","00","00"}},
			{5, {"10","00","00","00"}},
			{6, {"20","00","00","00"}},
			{7, {"40","00","00","00"}},
			{8, {"80","00","00","00"}},
			{9, {"1b","00","00","00"}},
			{10,{"36","00","00","00"}},
		};

		return consts[index];
	}

private:
	static std::vector<int> getRandomRefsIndexs(int number);
	static std::vector <std::string> keyRefs() {
		static const std::vector <std::string> refs{
		"0","1","2","3","4","5","6","7","8","9",
		"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
		"+","-","/","*","!","?",".", "#","@","(",")","[","]","%","|","\\","$","-","&", ";", ",", "="
		};
		return refs;
	}
	static std::string generateKey(int size);
	static std::string getSubString(std::string str, int startindex, int lastindex);

	static std::vector<std::vector<std::string>> generateSubKey(std::vector<std::vector<std::string>> key, int round);
	static std::vector<std::string> g(std::vector<std::string> v, int round);
	static std::vector<std::vector<std::string>> transposed(std::vector<std::vector<std::string>> m);
	static std::vector<std::string> xor_f(std::vector<std::string> a, std::vector<std::string> b);

	static std::string stringToHex(std::string str);
	static const char* hex_char_to_bin(char c);
	static std::string bin_str_to_hex(std::string bin_str);
	static std::string vecToBin(std::vector<std::string> vec);
	static std::string vecToString(std::vector<std::string> vec);
	static std::vector<std::string> rotWord(std::vector<std::string> word);
	static std::vector<std::string> subWord(std::vector<std::string> word);
	static std::vector<std::string> addRoundConst(std::vector<std::string> word, int round);

	std::vector<std::vector<std::string>> formatKey();
	void keyExpansion();

	int keySize = 0;
	int mode = 0;
	int round = 0;
	std::string key;
	std::vector<std::vector<std::string>> keyMatrix;
	std::vector<std::vector<std::vector<std::string>>> subKeys;
	std::string plaintext;
	std::string a[2];
};