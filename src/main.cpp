#include <iostream>
#include "include/aes.h"

using namespace std;


void logMatrix(vector<vector<std::string>>);

int main()
{
    string key = "satishcjisboring";
   
    AES aes = AES(key);

    logMatrix(aes.getKeyMatrix());
   
    return 0;
}

void logMatrix(vector<vector<std::string>> v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            std::cout << v[i][j] << "\t";
        }
        std::cout << std::endl;
    }

}