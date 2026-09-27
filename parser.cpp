#include<iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "parser.h" 
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
                if(c != ' '){
                    words.push_back(string(1, c));
                }
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
void getPairWiseCounts(vector<BrokenWord>& wordsBrokenDown, map<pair<string, string>, int>& pairWiseCounts){
    pairWiseCounts = {};
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
}
void merge(vector<pair<string, string>>& merges, vector<BrokenWord>& wordsBrokenDown
    , map<pair<string, string>, int>& pairWiseCounts,
    vector<string>& vocab
){
    pair<string, string> bestpair;
    int maxCount = 0;
    for(auto& it: pairWiseCounts){  
        if(it.second > maxCount){
            bestpair = it.first;
            maxCount = it.second;
        }
    }
    if(maxCount < 1){
        return;
    }
    merges.push_back(bestpair);
    vocab.push_back(bestpair.first + bestpair.second);
    for(BrokenWord& it: wordsBrokenDown){
        vector<string> newLetters;
        int len = it.letters.size();

        for(int i = 0; i < len; i++){
            if(i < len - 1 && it.letters[i] == bestpair.first && it.letters[i + 1] == bestpair.second){
                newLetters.push_back(it.letters[i] + it.letters[i + 1]);
                i++; 
            } else {
                newLetters.push_back(it.letters[i]);
            }
        }
        it.letters = newLetters;
    }


}
void bpe(vector<string>& allWords, vector<string> &vocab,
    vector<pair<string, string>>& merges){

    unordered_map<string, int> distinctWordCounts;

    vector<BrokenWord> wordsBrokenDown;
    map<pair<string, string>, int> pairWiseCounts;

    unordered_set<string> uniqueChars;
    for(string& s: allWords){
        for(char& c: s){
            uniqueChars.insert(string(1, c));
        }
        // auto it = distinctWordCounts.find(s);
        // if (it != distinctWordCounts.end()) {
        //     distinctWordCounts[s] += 1;
        // }else{
        //     distinctWordCounts[s] = 1;
        // }
        distinctWordCounts[s] += 1;
    }
    for(string s: uniqueChars){
        vocab.push_back(s);
    }
    vocab.push_back("$");
    for(auto& it: distinctWordCounts){
        vector<string> letters;
        for(char c: it.first){
            letters.push_back(string(1, c));
        }
        letters.push_back("$");
        wordsBrokenDown.push_back(BrokenWord(it.first, letters, it.second));
    }
    for(int i = 0; i < 100; i++){
        getPairWiseCounts(wordsBrokenDown, pairWiseCounts);
        merge(merges, wordsBrokenDown, pairWiseCounts, vocab);
    }
    // for(auto& it: wordsBrokenDown){
    //     for(string& s: it.letters){
    //         cout << s << ", ";
    //     }
    //     cout << endl;
    // }

}

vector<int> encode(vector<string> allWords, vector<pair<string, string>>& merges, 
    map<string, int>& stoi){
         
    vector<int> encodedTokens;
    for(string word: allWords){
        vector<string> letters;
        for(char c: word){
            letters.push_back(string(1, c));
        }
        letters.push_back("$");

        for(auto& it: merges){
            vector<string> newLetters;
            for(int i = 0; i < letters.size(); i++){
                if(i < letters.size() - 1 && letters[i] == it.first && letters[i + 1]  == it.second){
                    newLetters.push_back(letters[i] + letters[i + 1]);
                    i++;
                }else{
                    newLetters.push_back(letters[i]);
                }
            }
            letters = newLetters;
        }
        // printVector(letters);
        for(string& s: letters){
            encodedTokens.push_back(stoi[s]);
        }
    }

    return encodedTokens;
}
string decode(const vector<int>& encodedTokens, map<int, string>& itos) {
    string text = "";

    for (int t : encodedTokens) {
        string piece = itos[t];
        
        if (piece[piece.size() - 1]== '$') {
            text += piece.substr(0, piece.size() - 1) + " ";
        } else {
            text += piece; 
        }
    }
    cout << text;
    return text;
}
