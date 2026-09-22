#include <fstream>
#include<iostream>
using namespace std;

string to_lowercase(string s){
    for(char& c: s){
        c = tolower(c);
    }
    return s;
}
vector<string> parseIntoWords(string line){
    line = to_lowercase(line);
    vector<string> words;
    string s = "";
    for(char c: line){
        if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
            s += c;
        }else{
            if(s != ""){
                words.push_back(s);
                s = "";
            }
        }
    }
    if(s != ""){
        words.push_back(s);
    }
    return words;
}
void printVector(vector<string>& vect){
    for(string s: vect){
        cout << s << " , ";
    }
    cout << endl;
}
int main(){
    ifstream file("input.txt");
    if(!file.is_open()){
        cout << "failed to open file" << endl;
    }
    string line;
    vector<string> vocabs;
    while(getline(file, line)){
        vector<string> parsed = parseIntoWords(line);
        vocabs.insert(vocabs.end(), parsed.begin(), parsed.end());
        // printVector(parsed);
    }
    printVector(vocabs);
    return 0;
}