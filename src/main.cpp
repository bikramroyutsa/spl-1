#include<iostream>
#include "occurence.h"
#include "parser.h" 
#include<fstream>
int main(){
    ifstream file("data/wikitext2.txt");
    if(!file.is_open()){
        cout << "failed to open data/input.txt" << endl;
        return 1;
    }
    string line;
    vector<string> allWords;
    string text;

    // we read the files
    while(getline(file, line)){
        text += line;
        vector<string> parsed = parseIntoWords(line);
        allWords.insert(allWords.end(), parsed.begin(), parsed.end());
    }

    vector<string> vocab;
    vector<pair<string, string>> merges;
    // building the vocab and merge rules
    bpe(allWords, vocab, merges, 300);

    map<string, int> stoi;
    map<int, string> itos;

    for(int i = 0; i < vocab.size(); i++){
        stoi[vocab[i]] = i;
    }
    for(auto& it: stoi){
        itos[it.second] = it.first;
    }

    // we encode the files
    vector<int> encodedTokens = encode(allWords, merges, stoi);
    vector<vector<float>> embeddings = matrixBuilder(vocab.size(), encodedTokens);
}