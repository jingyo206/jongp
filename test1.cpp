#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ifstream fin;
    fin.open("encoded.txt", ifstream::in);
    
    string a;

    while (getline(fin, a)) cout << a << endl;

    fin.close();
    
    return 0;
}