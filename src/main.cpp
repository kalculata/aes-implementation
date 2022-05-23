#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    string str = "satishcjisboring";
    string str1;
    stringstream sstrm;

    for (const auto& item : str)
    {
        sstrm << hex << int(item);
    }
    str1 = sstrm.str();

    cout << "Hexadecimal Equivalent of String =  " << str << " is :" << str1;

    return 0;
}