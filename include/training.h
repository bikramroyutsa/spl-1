#pragma once

#include <string>
#include <vector>

using namespace std;

// Load words from a single file and append them to allWords
void loadFileIntoWords(const string& filepath, vector<string>& allWords);

// Load words from multiple files into a single vector
vector<string> loadData(const vector<string>& filepaths);

// Load default training files (e.g. data/wikitext2.txt)
vector<string> loadDefaultTrainingData();
vector<string> loadDataFromDir(const string& folderpath,
    const string& extension = ".txt");