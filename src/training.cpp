#include "training.h"
#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;

void loadFileIntoWords(const string& filepath, vector<string>& allWords) {
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

vector<string> loadData(const vector<string>& filepaths) {
    vector<string> allWords;
    for (const string& path : filepaths) {
        loadFileIntoWords(path, allWords);
    }
    cout << "Total training words loaded: " << allWords.size() << endl;
    return allWords;
}

vector<string> loadDefaultTrainingData() {
    vector<string> defaultFiles = {
        "data/wikitext2.txt"
    };
    return loadData(defaultFiles);
}
