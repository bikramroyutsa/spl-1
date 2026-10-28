#include "training.h"
#include "parser.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include<filesystem>
using namespace std;
namespace fs = std::filesystem;


bool isBlankLine(const string& str) {
    for(unsigned char ch: str){
        if(!isspace(ch)){
            return false;
        }
    }
    return true;
}
void loadTrainingFile(const string& filepath, vector<string>& allWords) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "[Error] Failed to open file: " << filepath << endl;
        return;
    }

    cout << "Loading " << filepath << "..." << endl;
    string line;
    int lineCount = 0;

    while (getline(file, line)) {
        vector<string> parsed = parseIntoWords(line);
        allWords.insert(allWords.end(), parsed.begin(), parsed.end());
        lineCount++;
    }

    cout << "  Finished " << filepath << " (" << lineCount << " lines loaded)" << endl;
}

void loadSearchFile(const string& filepath, vector<Pnode>& paraNodes) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "[Error] Failed to open file: " << filepath << endl;
        return;
    }

    string line;
    string currPara;

    while (getline(file, line)) {
        if(isBlankLine(line)){
            if(!currPara.empty()){
                paraNodes.push_back(Pnode(currPara, {}, filepath));
                currPara.clear();
            }
        }else{
            if (!currPara.empty()) {
                currPara += " "; 
            }
            currPara += line;
        }
    }
    if (!currPara.empty()) {
        paraNodes.push_back(Pnode(currPara, {}, filepath));
    }
}

void loadTrainingData(const string& folderpath,
    const string& extension, vector<string>& allWords){
    vector<string> filepaths;
    for(const auto& entry: fs::directory_iterator(folderpath)){
        if(entry.is_regular_file() && entry.path().extension() == extension){
            filepaths.push_back(entry.path().string());
        }
    }
    for (const string& path : filepaths) {
        loadTrainingFile(path, allWords);
    }
    cout << "Total training words loaded: " << allWords.size() << endl;
}

void loadSearchData(const string& folderpath,
    const string& extension, vector<Pnode>& paraNodes){
    vector<string> filepaths;
    for(const auto& entry: fs::directory_iterator(folderpath)){
        if(entry.is_regular_file() && entry.path().extension() == extension){
            filepaths.push_back(entry.path().string());
        }
    }
    for (const string& path : filepaths) {
        loadSearchFile(path, paraNodes);
    }
}

void embedParagraphs(vector<Pnode>& paraNodes, vector<pair<string, string>>& merges, map<string, int>& stoi, vector<vector<float>>& embeddings, int embeddingSize){
    for(Pnode& p: paraNodes){
        p.emb = embedText(p.text, merges, stoi, embeddings, embeddingSize);
    }
}