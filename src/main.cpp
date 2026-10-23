#include "training.h"
#include "occurence.h"
#include "parser.h" 

int main(){
    // Load dataset files into allWords
    vector<string> allWords = loadData({
        "data/input.txt",
        "data/shakespeare.txt",
        "data/wikitext2.txt"
    });

    vector<string> vocab;
    vector<pair<string, string>> merges;
    // building the vocab and merge rules
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

    // // we encode the files
    // vector<string> encodeThisWords = loadData(
    //     {"data/input.txt"}
    // );
    vector<int> encodedTokens = encode(allWords, merges, stoi);
    int embeddingSize = 64;
    vector<vector<float>> embeddings = matrixBuilder(vocab.size(), encodedTokens, embeddingSize);
}