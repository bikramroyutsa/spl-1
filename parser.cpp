#include <fstream>
#include<iostream>
#include <unordered_map>
#include <unordered_set>
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

void bpe(vector<string>& allWords){

    unordered_map<string, int> distinctWordCounts;
    unordered_set<char> vocab;
    vector<vector<string>> wordsBrokenDown;

    for(string& s: allWords){
        for(char& c: s){
            vocab.insert(c);
        }
        auto it = distinctWordCounts.find(s);
        if (it != distinctWordCounts.end()) {
            distinctWordCounts[s] += 1;
        }else{
            vector<string> brokenWord;
            for(char& c: s){
                brokenWord.push_back(string(1, c));
            }
            brokenWord.push_back("$");
            wordsBrokenDown.push_back(brokenWord);
            distinctWordCounts[s] = 1;
        }
    }
    for(vector<string>& s: wordsBrokenDown){
        cout << "< ";
        for(string c: s){
            cout << c << ", ";
        }
        cout << ">" << endl;
    }
}
int main(){
    ifstream file("input.txt");
    if(!file.is_open()){
        cout << "failed to open file" << endl;
    }
    string line;
    vector<string> allWords;
    while(getline(file, line)){
        vector<string> parsed = parseIntoWords(line);
        allWords.insert(allWords.end(), parsed.begin(), parsed.end());
        // printVector(parsed); 
    }
    // printVector(allWords);
    bpe(allWords);
    return 0;
}