#include <iostream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>

using namespace std;


vector<vector<float>> computeSVD_Eigen(const vector<vector<float>>& matrix, int k) {
    int V = matrix.size();
    k = min(k, V);

    Eigen::MatrixXf M(V, V);
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            M(i, j) = matrix[i][j];
        }
    }

    // 2. Compute SVD: M = U * Sigma * V^T
    // BDCSVD is Eigen's fastest SVD solver for large/medium matrices
    Eigen::BDCSVD<Eigen::MatrixXf> svd(M, Eigen::ComputeThinU | Eigen::ComputeThinV);

    Eigen::MatrixXf U = svd.matrixU();
    Eigen::VectorXf S = svd.singularValues();

    // 3. Construct Word Embeddings: W = U_k * sqrt(Sigma_k)
    vector<vector<float>> embeddings(V, vector<float>(k, 0.0f));
    for (int i = 0; i < V; i++) {
        for (int d = 0; d < k; d++) {
            embeddings[i][d] = U(i, d) * sqrt(S(d));
        }
    }

    // cout << "Top 5 Singular Values:\n";
    // for (int d = 0; d < min(5, (int)S.size()); d++) {
    //     cout << "  dim " << d + 1 << ": " << S(d) << "\n";
    // }

    return embeddings;
}

void pmi(vector<vector<float>>& matrix, vector<int>& tokenCounts, int totaltokens,
    int totalPairs){
    int vocabSize = matrix.size();
    
    for(int i = 0; i < vocabSize; i++){
        for(int j = 0; j < vocabSize; j++){
            if(matrix[i][j] == 0 || tokenCounts[i] == 0 || tokenCounts[j] == 0) {
                matrix[i][j] = 0.0f;
                continue;
            }
            float pij = (float) matrix[i][j] / totalPairs;
            float pi = (float) tokenCounts[i] / totaltokens;
            float pj = (float) tokenCounts[j] / totaltokens;
            float denominator = pi * pj;
            float pmi = log2(pij / denominator);
            matrix[i][j] = max(pmi, 0.0f);
        }
    }
}
vector<vector<float>> matrixBuilder(const int vocabSize, vector<int>& encodedTokens, int embeddingSize){
    vector<vector<float>> matrix(vocabSize, vector<float>(vocabSize, 0.0f));
    vector<int> tokenCounts(vocabSize, 0);
    int totaltokens = encodedTokens.size();
    int totalPairs = 0;

    for(int token : encodedTokens){
        tokenCounts[token]++;
    }

    int range = 10;
    int n = encodedTokens.size();
    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < min(i + range, n); j++){
            matrix[encodedTokens[i]][encodedTokens[j]]++;
            matrix[encodedTokens[j]][encodedTokens[i]]++;
            totalPairs += 2;
        }
    }
    cout << "original matrix size " << matrix.size() << " X " << matrix.size() << endl; 
    pmi(matrix, tokenCounts, totaltokens, totalPairs);
    vector<vector<float>> embeddings = computeSVD_Eigen(matrix, embeddingSize);
    cout << "svd matrix size " << embeddings.size() << " X " << embeddings[0].size() << endl; 

    return embeddings;
}
