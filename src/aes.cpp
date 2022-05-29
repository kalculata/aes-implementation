#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "include/aes.h"


AES::AES(std::string key, int key_size) {
    if (key_size != 128 && key_size != 192 && key_size != 256) {
        std::cout << "invalid key size";
        return;
    }
    if (key.length() < key_size / 8) {
        std::cout << "key is to short " << std::endl;
        return;
    }
    if (key.length() > key_size / 8) {
        std::cout << "key must be of length: " << key_size / 8 << std::endl;
        std::cout << "subject key: " << key.substr(0, key_size/8) << std::endl;
        key = key.substr(0, key_size / 8);
    }
    switch (key_size) {
        case 128:
            round = 10;
            break;
        case 192:
            round = 12;
            break;
        case 256:
            round = 14;
            break;
    }
    
    this->key_size = key_size;
    this->key = str_2_hexstr(key);

    this->sub_keys.push_back(format_key());
    key_expansion();
}

std::string AES::encrypt(const std::string &plaintext){
    std::string hexstr = str_2_hexstr(plaintext);
    Matrix matrix_rep = str_2_matrix(hexstr);
    Matrix state = xor_f(matrix_rep, sub_keys[0]);

    // Encryption
    for (int i = 0; i < round; i++)
        round_ops(state, i+1);

    return matrix_2_str(state);
}

std::string AES::make_128b_key() { return AES::generate_key(128 / 8); }

std::string AES::make_192b_key() { return AES::generate_key(192 / 8); }

std::string AES::make_256b_key() { return AES::generate_key(256 / 8); }

// converters
Matrix AES::str_2_matrix(const std::string &str, int row, int col) {
    Vector empty_row(col);
    Matrix matrix(row, empty_row);

    int cursor = 0;
    std::string t;

    for (int j = 0; j < col; j++) {
        for (int i = 0; i < row; i++) {
            t = "";
            t += str[cursor];
            t += str[cursor + 1];
            matrix[i][j] = t;
            cursor += 2;
        }
    }

    return matrix;
}
std::string AES::str_2_hexstr(const std::string &str) {
    std::stringstream sstrm;

    for (const auto& item : str) 
        sstrm << std::hex << int(item);

    return sstrm.str();
}
std::string AES::vec_2_str(const Vector &vec) {
    std::string str = "";

    for (int i = 0; i < vec.size(); i++) 
        str += vec[i];

    return str;
}
std::string AES::hexstr_vec_2_binstr(const Vector &vec) {
    std::string hex_str = vec_2_str(vec);
    std::string bin = "";

    for (int i = 0; i != hex_str.length(); i++)
        bin += hexchar_2_binstr(hex_str[i]);
    return bin;
}
std::string AES::matrix_2_str(const Matrix& matrix) {
    std::string str = "";

    for (int j = 0; j < matrix[0].size(); j++) {
        for (int i = 0; i < matrix.size(); i++)
            str += matrix[i][j];
    }
    
    return str;
}
std::string AES::binstr_2_hexstr(const std::string& binstr) {
    std::string hexstr, tmp = "0000";

    for (size_t j = 0; j < binstr.size(); j += 4) {
        tmp = binstr.substr(j, 4);
        if (!tmp.compare("0000")) hexstr += "0";
        else if (!tmp.compare("0001")) hexstr += "1";
        else if (!tmp.compare("0010")) hexstr += "2";
        else if (!tmp.compare("0011")) hexstr += "3";
        else if (!tmp.compare("0100")) hexstr += "4";
        else if (!tmp.compare("0101")) hexstr += "5";
        else if (!tmp.compare("0110")) hexstr += "6";
        else if (!tmp.compare("0111")) hexstr += "7";
        else if (!tmp.compare("1000")) hexstr += "8";
        else if (!tmp.compare("1001")) hexstr += "9";
        else if (!tmp.compare("1010")) hexstr += "a";
        else if (!tmp.compare("1011")) hexstr += "b";
        else if (!tmp.compare("1100")) hexstr += "c";
        else if (!tmp.compare("1101")) hexstr += "d";
        else if (!tmp.compare("1110")) hexstr += "e";
        else if (!tmp.compare("1111")) hexstr += "f";
        else continue;
    }

    return hexstr;
}
std::string AES::hexchar_2_binstr(char &c) {
    switch (toupper(c)) {
    case '0': return "0000";
    case '1': return "0001";
    case '2': return "0010";
    case '3': return "0011";
    case '4': return "0100";
    case '5': return "0101";
    case '6': return "0110";
    case '7': return "0111";
    case '8': return "1000";
    case '9': return "1001";
    case 'A': return "1010";
    case 'B': return "1011";
    case 'C': return "1100";
    case 'D': return "1101";
    case 'E': return "1110";
    case 'F': return "1111";
    default: return "";
    }
}
std::string AES::hexstr_2_binstr(std::string& hexstr) {
    std::string binstr = "";

    for (int i = 0; i < hexstr.size(); i++)
        binstr += hexchar_2_binstr(hexstr[i]);
    
    return binstr;
}
std::string AES::binstr_2_polynrep(std::string& binstr) {
    std::string polynrep = "";
    int cursor = binstr.length() - 1;
    for (int i = 0; i<binstr.length(); i++) {
        if (binstr[i] == '1') polynrep += std::to_string(cursor);
        cursor--;
    }
    
    return polynrep;
}
std::string AES::polynrep_2_binstr(std::string& polynrep) {
    std::string binstr = "";
    int highpower = std::stoi(std::string(1, polynrep[0])), tmp;
    Vector output(highpower + 1, "0");

    for (int i = 0; i < polynrep.length(); i++) {
        if (polynrep[i] == '0') 
            output[output.size() - 1] = "1";
        else {
            tmp = std::stoi(std::string(1,polynrep[i]));
            output[(output.size() - 1) - tmp] = "1";
        }
    }
    binstr = vec_2_str(output);
    if (binstr.length() < 8) binstr = format_bin(binstr);

    return binstr;
}
std::string AES::binstr_in_gf28(const std::string& binstr) {
    std::string irreducable_polyn = AES::irreducable_polyn();
    std::string res = xor_f(polynrep_2_binstr(irreducable_polyn), binstr);
    return format_bin(res);
}


// utils
Matrix AES::transposed(const Matrix& matrix) {
    Vector row(matrix.size());

    Matrix new_matrix(matrix[0].size(), row);

    for (int i = 0; i < new_matrix.size(); i++) {
        for (int j = 0; j < new_matrix[0].size(); j++) {
            new_matrix[i][j] = matrix[j][i];
        }
    }
    return new_matrix;
}
Matrix AES::multiply(Matrix& a, Matrix& b) {
    std::string tmp, tmp2;

    Vector empty_row(b[0].size());
    Matrix res_m(a.size(), empty_row);
    std::string t_s, binstr_a, binstr_b;
    std::stringstream stream;

    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b[0].size(); j++) {
            tmp = "";
            for (int k = 0; k < b.size(); k++) {
                binstr_a = hexstr_2_binstr(a[i][k]);
                binstr_b = hexstr_2_binstr(b[k][j]);
                tmp2 = dot_multiplication(binstr_a, binstr_b);
                ////std::cout << a[i][k] << " * " << b[k][j] << " = " << tmp2 << "\n";
                tmp = xor_f(tmp, tmp2);
            }
            tmp = binstr_2_hexstr(tmp);
            res_m[i][j] =  tmp;
        }
    }
    
    return res_m;
}
std::string AES::format_bin(std::string &binstr, int length) {
    if (length < -1) length *= -1;
    if (binstr.length() == length) return binstr;
    int padding = length - binstr.length();

    if (binstr.length() > length) {
        while (true) {
            if (binstr[0] == '0') binstr.erase(binstr.begin());
            if (binstr.length() == length || binstr[0] != '0') break;
        }
    }
    else {
        for (int i = 0; i < padding; i++) {
            binstr = "0" + binstr;
        }
    }
    return binstr;
}
std::string AES::xor_f(std::string binstr_a, std::string binstr_b) {
    if (binstr_a.empty()) return binstr_b;
    if (binstr_b.empty()) return binstr_a;

    if (binstr_a.length() != binstr_b.length()) {
        format_bin(binstr_a);
        format_bin(binstr_b);
    }

    std::string binstr = "";

    for (int i = 0; i < binstr_a.size(); i++) {
        if (binstr_a[i] != binstr_b[i])
            binstr += "1";
        else
            binstr += "0";
    }
    return binstr;
}
Vector AES::xor_f(const Vector& a, const Vector& b) {
    std::string hexstr = binstr_2_hexstr(xor_f(hexstr_vec_2_binstr(a), hexstr_vec_2_binstr(b)));
    std::string t = "";

    Vector vec;

    for (int i = 0; i < hexstr.length(); i += 2) {
        t = "";
        t += hexstr[i];
        t += hexstr[i + 1];
        vec.push_back(t);
    }

    return vec;
}
Matrix AES::xor_f(const Matrix& a, const Matrix& b) {
    Matrix new_matrix;

    for (int i = 0; i < a.size(); i++)
        new_matrix.push_back(xor_f(a[i], b[i]));

    return new_matrix;
}
Vector AES::g(const Vector& vec, int round) {
    return add_round_const(sub_word(rot_word(vec)), round);
}
std::string AES::dot_multiplication(std::string &binstr_a, std::string &binstr_b) {
    std::string polynrep_a = binstr_2_polynrep(binstr_a);
    std::string polynrep_b = binstr_2_polynrep(binstr_b);
    std::string res, res_1 = "", t = "0";
    int high_power = 0;

    if (polynrep_b.empty()) return format_bin(t);

    if (polynrep_a == "0") {
        return binstr_b;
    } else if (polynrep_a == "1") {
        for (int i = 0; i < polynrep_b.size(); i++) 
             res += std::to_string(std::stoi(std::string(1, polynrep_b[i])) + 1);
    } else if (polynrep_a == "10") {
        for (int i = 0; i < polynrep_b.size(); i++)
            res_1 += std::to_string(std::stoi(std::string(1, polynrep_b[i])) + 1);

        for (int i = 0; i < res_1.size(); i++) {
            t = res_1[i];
            if (polynrep_b.find(t) == std::string::npos) res += t;
        }

        for (int i = 0; i < polynrep_b.size(); i++) {
            t = polynrep_b[i];
            if(res_1.find(t) == std::string::npos) res += t;
        }
    } else {
        std::cout << "a must be less than 3";
    }

    std::sort(res.begin(), res.end());
    std::reverse(res.begin(), res.end());
    
    int highpower = std::stoi(std::string(1, res[0]));

    res = polynrep_2_binstr(res);

    if (highpower > 7) res = binstr_in_gf28(res);
    
    return res;
}


// key
std::vector<int> AES::get_random_key_refs(int number) {
    srand(time(0));
    std::vector<int> indexs;

    for (int i = 0; i < number; i++) 
        indexs.push_back(rand() % getKeyRefs().size());
    
    return indexs;
}
std::string AES::generate_key(int size) {
    std::string key = "";
    std::vector<int> indexs = get_random_key_refs(size);

    for (int i = 0; i < indexs.size(); i++)
        key += getKeyRefs()[indexs[i]];
    
    return key;
}
Matrix AES::format_key() {
    return str_2_matrix(key, 4, key_size / 32);
}


// key expansion
Matrix AES::generate_sub_key(const Matrix &prev_key, int round) {
    Vector row(4);
    Matrix new_key_T(prev_key[0].size(), row);
    Matrix prev_key_T = transposed(prev_key);

    new_key_T[0] = xor_f(prev_key_T[0], g(prev_key_T[prev_key_T.size() - 1], round));

    for (int i = 1; i < prev_key_T.size(); i++)
        new_key_T[i] = xor_f(prev_key_T[i], new_key_T[i - 1]);

    return transposed(new_key_T);
}
Vector AES::rot_word(const Vector &word, int rot) {
    if (rot == 0) return word;

    std::vector < std::string> vec(word.size());

    if (rot == 1) {
        vec[0] = word[1];
        vec[1] = word[2];
        vec[2] = word[3];
        vec[3] = word[0];
    }
    else if (rot == 2) {
        vec[0] = word[2];
        vec[1] = word[3];
        vec[2] = word[0];
        vec[3] = word[1];
    }
    else if (rot == 3) {
        vec[0] = word[3];
        vec[1] = word[0];
        vec[2] = word[1];
        vec[3] = word[2];
    }

    return vec;
}
Vector AES::sub_word(const Vector &word) {
    std::vector <std::string> vec;

    for (int i = 0; i < word.size(); i++)
        vec.push_back(sub_byte_box(word[i]));

    return vec;
}
Vector AES::add_round_const(const Vector &word, int round) {
    return xor_f(word, getRoundConst(round));
}
void AES::key_expansion() {
    Matrix prev_key = sub_keys[0];


    for (int i = 0; i < round; i++) {
        Matrix new_key = generate_sub_key(prev_key, i + 1);
        /*std::cout << "KEY Number " << i+1 << std::endl;
        AES::logMatrix(new_key);*/
        sub_keys.push_back(new_key);
        prev_key = new_key;
    }
}


// round ops
void AES::byte_sub(Matrix& matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++)
            matrix[i][j] = sub_byte_box(matrix[i][j]);
    }
}
void AES::shift_rows(Matrix& matrix) {
    for (int i = 0; i < matrix.size(); i++)
        matrix[i] = rot_word(matrix[i], i);
}
void AES::mix_columns(Matrix& matrix) {
    Matrix standard_m = getStandardMatrix();
    Matrix matrix_T = transposed(matrix);
    Matrix res, tmp;

    for (int i = 0; i < matrix[0].size(); i++) {
        tmp.clear();
        tmp.push_back(matrix_T[i]);
        tmp = transposed(tmp);
        tmp = multiply(standard_m, tmp);
        tmp = transposed(tmp);
        res.push_back(tmp[0]);
    }
    res = transposed(res);
    matrix = res;
}
void AES::add_round_key(Matrix& matrix, int round) {
    matrix = xor_f(matrix, sub_keys[round]);
}
Matrix AES::round_ops(Matrix& state, int round) {
    byte_sub(state);
    shift_rows(state);
    if (round != this->round) mix_columns(state);
    add_round_key(state, round);
    
    return state;
}