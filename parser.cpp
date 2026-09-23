#include <fstream>
#include<iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

struct BrokenWord{
    string word;
    vector<string> letters;
    int count;
    BrokenWord(string word, vector<string> letters, int count){
        this->word = word;
        this->letters = letters;
        this->count = count;
    }
};
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
    vector<BrokenWord> wordsBrokenDown;
    map<pair<string, string>, int> pairWiseCounts;

    for(string& s: allWords){
        for(char& c: s){
            vocab.insert(c);
        }
        auto it = distinctWordCounts.find(s);
        if (it != distinctWordCounts.end()) {
            distinctWordCounts[s] += 1;
        }else{
            distinctWordCounts[s] = 1;
        }
    }
    for(auto& it: distinctWordCounts){
        vector<string> letters;
        for(char c: it.first){
            letters.push_back(string(1, c));
        }
        letters.push_back("$");
        wordsBrokenDown.push_back(BrokenWord(it.first, letters, it.second));
    }
    for(BrokenWord& bw: wordsBrokenDown){
        int len = bw.letters.size();
        for(int i = 0; i < len - 1; i++){
            pair<string, string> currPair = {bw.letters[i], bw.letters[i+1]};
            auto it = pairWiseCounts.find(currPair);
            if(it != pairWiseCounts.end()){
                pairWiseCounts[currPair] += bw.count;
            }else{
                pairWiseCounts[currPair] = bw.count;
            }
        }
    }
    vector<pair<string, string>> merges;
    for(BrokenWord& bw: wordsBrokenDown){
        
    }
    for(auto& it: pairWiseCounts){
        cout << "{" << it.first.first << "," << it.first.second << "}" << ":" << it.second << endl;
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