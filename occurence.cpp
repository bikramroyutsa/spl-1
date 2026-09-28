#include <map>
#include "occurence.h"
#include<algorithm>
using namespace std;

vector<vector<int>> matrixBuilder(const int vocabSize, vector<int>& encodedTokens){
    vector<vector<int>> matrix(vocabSize, vector<int>(vocabSize, 0));
    vector<int> tokenCounts(vocabSize, 0);
    int totaltokens = encodedTokens.size();

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
            matrix[encodedTokens[j]][encodedTokens[i]]++;
        }
    }
    // pmi(matrix, tokenCounts, totaltokens);
    return matrix;
}
