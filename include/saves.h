#pragma once

#include <string>
#include <vector>
#include <map>
#include "training.h"

using namespace std;

void saveModel(const string& filepath, int clusterSize, int embeddingSize,
    vector<vector<float>>& embeddings,
    vector<pair<string, string>>& merges, 
    map<string, int>& stoi, 
    vector<Pnode>& paraNodes,
    vector<vector<float>>& centeroids,
    vector<float>& tokenWeights);

bool loadModel(const string& filepath, int& clusterSize, int& embeddingSize,
    vector<vector<float>>& embeddings,
    vector<pair<string, string>>& merges, 
    map<string, int>& stoi, 
    vector<Pnode>& paraNodes,
    vector<vector<float>>& centeroids,
    vector<float>& tokenWeights);


