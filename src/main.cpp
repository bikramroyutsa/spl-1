#include "training.h"
#include "occurence.h"
#include "parser.h" 
#include<iostream>
using namespace std;
int main(){
    vector<string> allWords;
    vector<Pnode> paraNodes;
    loadDataFromDir("data", ".txt", allWords, paraNodes);

    vector<string> vocab;
    vector<pair<string, string>> merges;
    int mergeIteration = 800;
    bpe(allWords, vocab, merges, mergeIteration);

    map<string, int> stoi;
    map<int, string> itos;
    for(int i = 0; i < vocab.size(); i++){
        stoi[vocab[i]] = i;
    }
    for(auto& it: stoi){
        itos[it.second] = it.first;
    }

    vector<int> encodedTokens = encode(allWords, merges, stoi);
    int embeddingSize = 64;
    vector<vector<float>> embeddings = matrixBuilder(vocab.size(), encodedTokens, embeddingSize);

    for(Pnode& p: paraNodes){
        vector<string> words = parseIntoWords(p.text);
        vector<int> tokens = encode(words, merges, stoi);
        if(tokens.empty()){
            continue;
        }
        vector<float> avgEmb(embeddingSize, 0.0f);

        for (int token : tokens) {
            for (int d = 0; d < embeddingSize; d++) {
                avgEmb[d] += embeddings[token][d];
            }
        }
        for (int d = 0; d < embeddingSize; d++) {
            avgEmb[d] /= tokens.size();
        }
        p.emb = avgEmb;
    }
    cout << "total paragraphs " << paraNodes.size() << endl;
}