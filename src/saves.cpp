#include "saves.h"
#include <fstream>
#include <iostream>
#include <map>
#include "training.h"
using namespace std;
void writeString(ofstream& outputFile, const string& s){
    int size = s.size();
    outputFile.write((char *)& size, sizeof(size));
    outputFile.write(s.data(), size);
}
void writeFloatVector(ofstream& outputFile, vector<float>& v){
    int size = v.size();
    outputFile.write((char *)& size, sizeof(size));
    
    outputFile.write((char *)v.data(), size * sizeof(float));

}

string readString(ifstream& inputFile){
    int size = 0;
    inputFile.read((char *)& size, sizeof(size));
    string s(size, '\0');
    inputFile.read(&s[0], size);
    return s;
}

vector<float> readFloatVector(ifstream& inputFile){
    int size = 0;
    inputFile.read((char *)& size, sizeof(size));
    vector<float> v(size);
    inputFile.read((char *)v.data(), size * sizeof(float));
    return v;
}
void saveModel(const string& filepath, int clusterSize, int embeddingSize,
    vector<vector<float>>& embeddings,
    vector<pair<string, string>>& merges, 
    map<string, int>& stoi, 
    vector<Pnode>& paraNodes,
    vector<vector<float>>& centeroids,
    vector<float>& tokenWeights){
    
    ofstream outputFile(filepath, ios::binary);
    if(!outputFile.is_open()){
        cout << "Failed to open" << filepath << " for writing" << endl;
        return;
    }
    outputFile.write((char *)& clusterSize, sizeof(clusterSize));
    outputFile.write((char *)& embeddingSize, sizeof(embeddingSize));

    int numEmbeddings = embeddings.size();
    outputFile.write((char*)&numEmbeddings, sizeof(numEmbeddings));
    for (vector<float>& row : embeddings) {
        writeFloatVector(outputFile, row);
    }

    int mergesSize = merges.size();
    outputFile.write((char *)& mergesSize, sizeof(mergesSize));
    for(const auto& m: merges){
        writeString(outputFile, m.first);
        writeString(outputFile, m.second);
    }
    
    int stoiSize = stoi.size();
    outputFile.write((char *)& stoiSize, sizeof(stoiSize));

    for(const auto& p: stoi){
        writeString(outputFile, p.first);
        outputFile.write((char *)& p.second, sizeof(p.second));
    }

    int numNodes = paraNodes.size();
    outputFile.write((char*)&numNodes, sizeof(numNodes));
    for (Pnode & p : paraNodes) {
        writeString(outputFile, p.text);
        writeString(outputFile, p.parentFilePath);
        outputFile.write((char*)&p.cluster, sizeof(p.cluster));
        writeFloatVector(outputFile, p.emb);
    }

    int numCenteroids = centeroids.size();
    outputFile.write((char*)&numCenteroids, sizeof(numCenteroids));
    for (vector<float>& row : centeroids) {
        writeFloatVector(outputFile, row);
    }

    writeFloatVector(outputFile, tokenWeights);
}

bool loadModel(const string& filepath, int& clusterSize, int& embeddingSize,
    vector<vector<float>>& embeddings,
    vector<pair<string, string>>& merges, 
    map<string, int>& stoi, 
    vector<Pnode>& paraNodes,
    vector<vector<float>>& centeroids,
    vector<float>& tokenWeights){

    ifstream inputFile(filepath, ios::binary);
    if(!inputFile.is_open()){
        cout << "Failed to open " << filepath << " for reading" << endl;
        return false;
    }

    inputFile.read((char *)& clusterSize, sizeof(clusterSize));
    inputFile.read((char *)& embeddingSize, sizeof(embeddingSize));

    int numEmbeddings = 0;
    inputFile.read((char*)&numEmbeddings, sizeof(numEmbeddings));
    embeddings.resize(numEmbeddings);
    for (int i = 0; i < numEmbeddings; i++) {
        embeddings[i] = readFloatVector(inputFile);
    }

    int mergesSize = 0;
    inputFile.read((char *)& mergesSize, sizeof(mergesSize));
    merges.resize(mergesSize);
    for(int i = 0; i < mergesSize; i++){
        merges[i].first = readString(inputFile);
        merges[i].second = readString(inputFile);
    }

    int stoiSize = 0;
    inputFile.read((char *)& stoiSize, sizeof(stoiSize));
    stoi.clear();
    for(int i = 0; i < stoiSize; i++){
        string word = readString(inputFile);
        int id = 0;
        inputFile.read((char *)& id, sizeof(id));
        stoi[word] = id;
    }

    int numNodes = 0;
    inputFile.read((char*)&numNodes, sizeof(numNodes));
    paraNodes.clear();
    for (int i = 0; i < numNodes; i++) {
        string text = readString(inputFile);
        string parentFilePath = readString(inputFile);
        int cluster = 0;
        inputFile.read((char*)&cluster, sizeof(cluster));
        vector<float> emb = readFloatVector(inputFile);

        Pnode p(text, emb, parentFilePath);
        p.cluster = cluster;
        paraNodes.push_back(p);
    }

    int numCenteroids = 0;
    inputFile.read((char*)&numCenteroids, sizeof(numCenteroids));
    centeroids.resize(numCenteroids);
    for (int i = 0; i < numCenteroids; i++) {
        centeroids[i] = readFloatVector(inputFile);
    }

    tokenWeights = readFloatVector(inputFile);

    return true;
}