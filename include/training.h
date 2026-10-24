#pragma once

#include <string>
#include <vector>

using namespace std;

struct Pnode {
    string text; 
    vector<float> emb;
    string parentFilePath;

    Pnode(string text, vector<float> emb, string parentFilePath){
        this->text = text;
        this->emb =emb;
        this->parentFilePath = parentFilePath;
    }
};

void loadFileIntoWords(const string& filepath, vector<string>& allWords);

vector<string> loadData(const vector<string>& filepaths);

vector<string> loadDefaultTrainingData();
void loadDataFromDir(const string& folderpath,
    const string& extension, vector<string>& allWords, vector<Pnode>& paraNodes);