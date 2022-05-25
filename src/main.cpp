#include <fstream>
#include <vector>
#include <bitset>
#include "include/aes.h"
#include <iostream>
#include <string>

////typedef unsigned char BYTE;

void logMatrix(std::vector<std::vector<std::string>> v);

//std::string bintxt_2_hextxt(const std::string& bin)
//{
//    std::stringstream reader(bin);
//    std::stringstream result;
//
//    while (reader)
//    {
//        std::bitset<8> digit;
//        reader >> digit;
//        result << std::hex << digit.to_ulong();
//    }
//
//    return result.str();
//}

int main()
{
    std::string key = "satishcjisboring";

    AES aes = AES(key);


  logMatrix( aes.getKeyMatrix());

    return 0;
}
//std::vector<BYTE> readFile(const char* filename)
//{
//    // open the file:
//    std::streampos fileSize;
//    std::ifstream file(filename, std::ios::binary);
//
//    // get its size:
//    file.seekg(0, std::ios::end);
//    fileSize = file.tellg();
//    file.seekg(0, std::ios::beg);
//
//    // read the data:
//    std::vector<BYTE> fileData(fileSize);
//    file.read((char*)&fileData[0], fileSize);
//    return fileData;
//}

void logMatrix(std::vector<std::vector<std::string>> v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            std::cout << v[i][j] << "\t";
        }
        std::cout << std::endl;
    }

}