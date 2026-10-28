#include "training.h"
#include "occurence.h"
#include "parser.h" 
#include "kmeans.h"
#include<iostream>
using namespace std;

void trainAndEmbed(vector<Pnode>& paraNodes, vector<pair<string, string>>& merges,
                   map<string, int>& stoi, vector<vector<float>>& embeddings,
                   int embeddingSize){
    vector<string> allWords;
    vector<string> vocab;
    map<int, string> itos;
    int mergeIteration = 800;
    vector<int> encodedTokens;

    loadTrainingData("training", ".txt", allWords);
    bpe(allWords, vocab, merges, mergeIteration); // this build the vocabulary and merge rules
    for(int i = 0; i < vocab.size(); i++){
        stoi[vocab[i]] = i;
    }
    for(auto& it: stoi){
        itos[it.second] = it.first;
    }
    encodedTokens = encode(allWords, merges, stoi); // directory texts are encoded
    embeddings = matrixBuilder(vocab.size(), encodedTokens, embeddingSize); // embeddings are generated

    loadSearchData("search", ".txt", paraNodes);
    
    embedParagraphs(paraNodes, merges, stoi, embeddings, embeddingSize); // embeddings for paragraph nodes are generated
    cout << "total paragraphs " << paraNodes.size() << endl;
}
void cluster(int& clusterSize, vector<Pnode>& paraNodes, vector<vector<float>>& centeroids){
    clusterSize = findOptimalK(paraNodes, 15); // we find optimal clusterSize for clustering
    cout << "optimal clusterSize = " << clusterSize << endl;
    cout << "kmeans with clusterSize = " << clusterSize << "..." << endl;
    runKmeans(clusterSize, paraNodes, centeroids, 30); // clustering is done


    vector<int> counts(clusterSize, 0);
    for(const auto& p : paraNodes){
        counts[p.cluster]++;
    }
    for(int i = 0; i < clusterSize; i++){
        cout << "cluster " << i << ": " << counts[i] << " paragraphs" << endl;
    }

}
int main(){
    int clusterSize;
    int embeddingSize = 64;
    vector<Pnode> paraNodes;
    vector<pair<string, string>> merges;
    map<string, int> stoi;
    vector<vector<float>> embeddings;
    vector<vector<float>> centeroids;

    trainAndEmbed(paraNodes, merges, stoi, embeddings, embeddingSize);
    cluster(clusterSize, paraNodes, centeroids);
}