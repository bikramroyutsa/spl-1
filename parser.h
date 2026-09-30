#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

struct BrokenWord {
    string word;
    vector<string> letters;
    int count;
    BrokenWord(string word, vector<string> letters, int count)
        : word(word), letters(letters), count(count) {}
};

vector<string> parseIntoWords(string line);
void bpe(vector<string>& allWords, vector<string>& vocab, vector<pair<string, string>>& merges);
vector<int> encode(vector<string> allWords, vector<pair<string, string>>& merges, map<string, int>& stoi);
string decode(const vector<int>& encodedTokens, map<int, string>& itos);
