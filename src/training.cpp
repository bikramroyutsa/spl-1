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
void loadFile(const string& filepath, vector<string>& allWords, vector<Pnode>& paraNodes) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "[Error] Failed to open file: " << filepath << endl;
        return;
    }

    cout << "Loading " << filepath << "..." << endl;
    string line;
    int lineCount = 0;
    string currPara;

    while (getline(file, line)) {
        vector<string> parsed = parseIntoWords(line);

        if(isBlankLine(line)){
            if(!currPara.empty()){
                paraNodes.push_back(Pnode(currPara, {}, filepath));
            }
            currPara.clear();
        }else{
            if (!currPara.empty()) {
                currPara += " "; 
            }
            currPara += line;
        }

        allWords.insert(allWords.end(), parsed.begin(), parsed.end());
        lineCount++;
    }
    if (!currPara.empty()) {
        paraNodes.push_back(Pnode(currPara, {}, filepath));
    }

    cout << "  Finished " << filepath << " (" << lineCount << " lines loaded)" << endl;
}

// vector<string> loadData(const vector<string>& filepaths, vector<string>& allWords, vector<Pnode>& paraNodes) {
//     for (const string& path : filepaths) {
//         loadFile(path, allWords, paraNodes);
//     }
//     cout << "Total training words loaded: " << allWords.size() << endl;
// }

// vector<string> loadDefaultTrainingData() {
//     vector<string> defaultFiles = {
//         "data/wikitext2.txt"
//     };
//     return loadData(defaultFiles);
// }

void loadDataFromDir(const string& folderpath,
    const string& extension,vector<string>& allWords, vector<Pnode>& paraNodes){
    vector<string> filepaths;
    for(const auto& entry: fs::directory_iterator(folderpath)){
        if(entry.is_regular_file() && entry.path().extension() == extension){
            filepaths.push_back(entry.path().string());
        }
    }
    ;
    for (const string& path : filepaths) {
        loadFile(path, allWords, paraNodes);
    }
    cout << "Total training words loaded: " << allWords.size() << endl;
}

void embedParagraphs(vector<Pnode>& paraNodes, vector<pair<string, string>>& merges, map<string, int>& stoi, vector<vector<float>>& embeddings, int embeddingSize){
    for(Pnode& p: paraNodes){
        p.emb = embedText(p.text, merges, stoi, embeddings, embeddingSize);
    }
}