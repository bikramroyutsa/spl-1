#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

struct Pnode {
    string text; 
    vector<float> emb;
    string parentFilePath;
    int cluster;
    Pnode(string text, vector<float> emb, string parentFilePath){
        this->text = text;
        this->emb =emb;
        this->parentFilePath = parentFilePath;
        this->cluster = 0;
    }
};

void loadFileIntoWords(const string& filepath, vector<string>& allWords);

vector<string> loadData(const vector<string>& filepaths);

vector<string> loadDefaultTrainingData();
void loadDataFromDir(const string& folderpath,
    const string& extension, vector<string>& allWords, vector<Pnode>& paraNodes);

void embedParagraphs(vector<Pnode>& paraNodes, vector<pair<string, string>>& merges, map<string, int>& stoi, vector<vector<float>>& embeddings, int embeddingSize);