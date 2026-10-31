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
void saveModel(const string& filepath, int clusterSize, int embeddingSize,
    vector<vector<float>>& embeddings,
    vector<pair<string, string>>& merges, 
    map<string, int>& stoi, 
    vector<Pnode>& paraNodes,
    vector<vector<float>>& centeroids){
    
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

}