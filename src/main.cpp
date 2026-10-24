#include "training.h"
#include "occurence.h"
#include "parser.h" 

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
}