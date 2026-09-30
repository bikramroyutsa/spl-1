#include <map>
#include "occurence.h"
#include<algorithm>
#include <cmath> 
using namespace std;

void pmi(vector<vector<float>>& matrix, vector<int>& tokenCounts, int totaltokens,
    int totalPairs){
    int vocabSize = matrix.size();
    
    for(int i = 0; i < vocabSize; i++){
        for(int j = 0; j < vocabSize; j++){
            float pij = (float) matrix[i][j] / totalPairs;
            float pi = (float) tokenCounts[i] / totaltokens;
            float pj = (float) tokenCounts[j] / totaltokens;
            float denominator = pi * pj;
            float pmi = log2(pij / (pi * pj));
            matrix[i][j] = max(pmi, 0.0f);
        }
    }
}
vector<vector<float>> matrixBuilder(const int vocabSize, vector<int>& encodedTokens){
    vector<vector<float>> matrix(vocabSize, vector<float>(vocabSize, 0.0f));
    vector<int> tokenCounts(vocabSize, 0);
    int totaltokens = encodedTokens.size();
    int totalPairs = 0;

    for(int i = 0; i < vocabSize; i++){
        tokenCounts[encodedTokens[i]]++;
        for(int j = 0; j < vocabSize; j++){
            matrix[i][j] = 0;
        }
    }
    int range = 10;
    int n = encodedTokens.size();
    for(int i = 0; i < n; i++){
        for(int j = i; j < min(i + range, n); j++){
            matrix[encodedTokens[i]][encodedTokens[j]]++;
            if(i != j){
                matrix[encodedTokens[j]][encodedTokens[i]]++;
                totalPairs += 2;
            } else {
                totalPairs += 1;
            }
        }
    }
    pmi(matrix, tokenCounts, totaltokens, totalPairs);
    return matrix;
}
