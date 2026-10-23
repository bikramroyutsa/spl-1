#include <iostream>
#include <fstream>
using namespace std;


int getUTF8Length(unsigned char c) {
    if ((c & 0b10000000) == 0b00000000) return 1;
    if ((c & 0b11100000) == 0b11000000) return 2;
    if ((c & 0b11110000) == 0b11100000) return 3;
    if ((c & 0b11111000) == 0b11110000) return 4;
    return 1;
}
void getWords(){
    
}
int main(){
    ifstream file("input.txt");
    if(!file.is_open()){
        cout << "Failed to open file" << endl;
        return 0;
    }
    string text;
    string line;
    while(getline(file, line)){
        text += line;
    }
    cout << text << endl;
}
