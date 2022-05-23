#include <iostream>
#include "include/aes.h"

using namespace std;

int main()
{
    string key128 = AES::generateKey128();
    string key192 = AES::generateKey192();
    string key256 = AES::generatekey256();

    /*string key = "satishcjisboring";
    string plaintext = "hello friend";

    AES blender = AES(key, plaintext);*/

    std::cout << "key text 128: " << key128 << std::endl;
    std::cout << "key text 192: " << key192 << std::endl;
    std::cout << "key text 256: " << key256 << std::endl;
   
    return 0;
}