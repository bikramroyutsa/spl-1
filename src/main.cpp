#include "training.h"
#include "occurence.h"
#include "parser.h" 
#include "kmeans.h"
#include "saves.h"
#include <iostream>
#include <unordered_set>
#include <cmath>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

void trainAndEmbed(vector<Pnode>& paraNodes, vector<pair<string, string>>& merges,
                   map<string, int>& stoi, vector<vector<float>>& embeddings,
                   int embeddingSize, vector<float>& tokenWeights){
    vector<string> allWords;
    vector<string> vocab;
    map<int, string> itos;
    int mergeIteration = 2000;
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
    
    // idf
    int N = paraNodes.size();
    vector<int> df(vocab.size(), 0);
    for (const auto& p : paraNodes) {
        vector<string> pWords = parseIntoWords(p.text);
        vector<int> pTokens = encode(pWords, merges, stoi);
        unordered_set<int> uniqueTokens(pTokens.begin(), pTokens.end());
        for (int t : uniqueTokens) {
            if (t >= 0 && t < (int)vocab.size()) df[t]++;
        }
    }

    tokenWeights.assign(vocab.size(), 1.0f);
    for (size_t i = 0; i < vocab.size(); i++) {
        float idfVal = log((float)(N + 1) / (float)(df[i] + 1));
        tokenWeights[i] = max(idfVal * idfVal, 0.05f);
    }

    embedParagraphs(paraNodes, merges, stoi, embeddings, embeddingSize, tokenWeights); // embeddings for paragraph nodes are generated
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
    vector<vector<float>> embeddings;
    vector<pair<string, string>> merges;
    map<string, int> stoi;
    vector<float> tokenWeights;

    vector<Pnode> paraNodes;
    vector<vector<float>> centeroids;

    string modelFile = "model.bin";

    if(!loadModel(modelFile, clusterSize, embeddingSize, embeddings, merges, stoi, paraNodes, centeroids, tokenWeights)){
        trainAndEmbed(paraNodes, merges, stoi, embeddings, embeddingSize, tokenWeights);
        cluster(clusterSize, paraNodes, centeroids);
        saveModel(modelFile, clusterSize, embeddingSize, embeddings, merges, stoi, paraNodes, centeroids, tokenWeights);
    }else{
        cout << "model loaded from " << modelFile << " (" << paraNodes.size() << " paragraphs, " << clusterSize << " clusters)" << endl;
    }
    bool loop = true;
    while(loop){
        cout << "Enter your query: " << endl;
        cout << "=> ";
        string s;
        if (!getline(cin, s) || s == "exit" || s.empty()) {
            break;
        }
        vector<float> query = embedText(s, merges, stoi, embeddings, embeddingSize, tokenWeights);
        float similarityScore = -1;
        Pnode matchedP;
        for(Pnode& p: paraNodes){
            float sim =  cosineSimilarity(query, p.emb);
            if(sim > similarityScore){
                matchedP = p;
                similarityScore = sim;
            }
        }
        fs::path p(matchedP.parentFilePath);
        cout << "file " << matchedP.parentFilePath << "\n";
        cout << "score " << similarityScore << "\n";
        cout << "relevant text -> \n" << matchedP.text << "\n\n";
    }
}