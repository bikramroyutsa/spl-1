#include "kmeans.h"
#include "training.h"
#include <cmath>
#include <random>
#include <unordered_set>
#include<vector>
using namespace std;


float cosineSimilarity(vector<float>& u, vector<float>& v){
    if(u.size() != v.size()){
        return 0;
    }
    int n = u.size();
    float sum = 0;
    float uval = 0;
    float vval = 0;
    for(int i = 0; i < n; i++){
        sum += u[i] * v[i];
        uval += u[i] * u[i];
        vval += v[i] * v[i];
    }
    uval = sqrtf(uval);
    vval = sqrtf(vval);
    if (uval == 0.0f || vval == 0.0f) return 0.0f;
    float similarity = sum / (uval * vval);
    return similarity;

}
vector<vector<float>> assignCenteroids(int k, vector<Pnode>& paraNodes){

    if(k > paraNodes.size()){
        return {};
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, paraNodes.size() - 1);

    unordered_set<int> uniqueNums;

    while (uniqueNums.size() < k) {
        uniqueNums.insert(dist(gen));
    }
    vector<int> indices(uniqueNums.begin(), uniqueNums.end());

    vector<vector<float>> centeroids;
    for(int i: indices){
        centeroids.push_back(paraNodes[i].emb);
    }
    return centeroids;
}
vector<vector<float>> updateCenteroids(int k, vector<Pnode>& paraNodes){
    vector<vector<float>> clusterAvgs(k, vector<float>(paraNodes[0].emb.size(), 0.0f));
    vector<int> clusterCounts(k, 0);
    for(auto& p: paraNodes){
        for (int d = 0; d < p.emb.size(); d++) {
            clusterAvgs[p.cluster][d] += p.emb[d];
        }
        clusterCounts[p.cluster] += 1;
    }
    for(int i = 0; i < clusterAvgs.size(); i++){
        for (int d = 0; d < clusterAvgs[i].size(); d++) {
            if(clusterCounts[i] == 0){
                continue;
            }
            clusterAvgs[i][d] /= clusterCounts[i];
        }
    }
    return clusterAvgs;
}
void assignNodesToCluster(vector<Pnode>& paraNodes, 
        vector<vector<float>>& centeroids){

    for(auto& p: paraNodes){
        vector<float> similarityScores;
        for(auto& c: centeroids){
            similarityScores.push_back(cosineSimilarity(p.emb, c));
        }
        float maxScore = similarityScores[0];
        int maxIndex = 0;
        for(int i = 0; i < similarityScores.size(); i++){
            if(similarityScores[i] > maxScore){
                maxIndex = i;
                maxScore = similarityScores[i];
            }
        }

        p.cluster = maxIndex;
    }
}
void runKmeans(int k, vector<Pnode>& paraNodes, int maxIterations){
    vector<vector<float>> centeroids = assignCenteroids(k, paraNodes);
    for(int i = 0; i < maxIterations; i++){
        assignNodesToCluster(paraNodes , centeroids);
        centeroids = updateCenteroids(k, paraNodes);
    }

}