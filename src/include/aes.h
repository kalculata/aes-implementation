#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>

typedef std::vector<std::string> Vector;
typedef std::vector <Vector> Matrix;

class AES {
public:
	AES(std::string key, int key_size=AES::USE_AES_128);
	
	std::string encrypt(const std::string &plaintext);
	std::string decrypt(const std::string &cipher);

	static std::string make_128b_key();
	static std::string make_192b_key();
	static std::string make_256b_key();

	static std::string sub_byte_box(std::string value) {
		std::string a(1, value[0]);
		std::string b(1, value[1]);

		std::map<std::string, Vector> sbox = {
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
	static Vector getRoundConst(int index) {
		std::map<int, Vector> consts = {
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
	static Matrix getStandardMatrix() {
		return {
			{"02","03","01","01"},
			{"01","02","03","01"},
			{"01","01","02","03"},
			{"03","01","01","02"},
		};
	}
	static std::vector <std::string> getKeyRefs() {
		return {
			"0","1","2","3","4","5","6","7","8","9",
			"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
			"+","-","/","*","!","?",".", "#","@","(",")","[","]","%","|","\\","$","-","&", ";", ",", "="
		};
	}
	static std::string irreducable_polyn() { return std::string("84310"); };
	std::vector<Matrix> getSubKeys() { return sub_keys; }

	static const int USE_AES_128 = 128;
	static const int USE_AES_192 = 192;
	static const int USE_AES_256 = 256;

	static void test() {
		std::string a, b, c;
		a = "6f";
		b = "73";
		c = "n";
		a = hexstr_2_binstr(a);
		b = hexstr_2_binstr(b);

		std:

		std::cout << a << std::endl;
		
		// 1(7210) = 8321
		// 7210 8321 = 8730


		//std::cout << AES::xor_f(a, b);
	}
	static void logMatrix(Matrix& matrix) {
		for (int i = 0; i < matrix.size(); i++) {
			for (int j = 0; j < matrix[0].size(); j++) {
				std::cout << matrix[i][j] << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << "---------------------" << "\n";
	}

private:
	// converter
	static Matrix str_2_matrix(const std::string &str, int row = 4, int col = 4);
	static std::string str_2_hexstr(const std::string &str);
	static std::string hexstr_vec_2_binstr(const Vector &vec);
	static std::string vec_2_str(const Vector &vec);
	static std::string matrix_2_str(const Matrix &matrix);
	static std::string binstr_2_hexstr(const std::string &binstr);
	static std::string hexchar_2_binstr(char &c);
	static std::string hexstr_2_binstr(std::string &hexstr);
	static std::string binstr_2_polynrep(std::string& binstr);
	static std::string polynrep_2_binstr(std::string& polynrep);
	static std::string binstr_in_gf28(const std::string& binstr);


	// utils
	static Matrix transposed(const Matrix &matrix);
	static Matrix multiply(Matrix& a, Matrix& b);
	static std::string format_bin(std::string &binstr, int length=8);
	static std::string xor_f(std::string binstr_a, std::string binstr_b);
	static Matrix xor_f(const Matrix &a, const Matrix &b);
	static Vector xor_f(const Vector &a, const Vector &b);
	static Vector g(const Vector &vec, int round);
	static std::string dot_multiplication(std::string &binstr_a, std::string &binstr_b);

	// key
	static std::vector<int> get_random_key_refs(int number);
	static std::string generate_key(int size);
	Matrix format_key();


	// key expansion
	static Matrix generate_sub_key(const Matrix &key, int round);
	static Vector rot_word(const Vector &word, int rot = 1);
	static Vector sub_word(const Vector &word);
	static Vector add_round_const(const Vector &word, int round);
	void key_expansion();


	// round ops
	static void byte_sub(Matrix &matrix);
	static void shift_rows(Matrix& matrix);
	static void mix_columns(Matrix& matrix);
	void add_round_key(Matrix& matrix, int round);
	Matrix round_ops(Matrix &state_array, int round);


	int key_size = 0;
	int round = 0;
	std::string key;
	std::vector<Matrix> sub_keys;
	std::string plaintext;
	std::string a[2];
};