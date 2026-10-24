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
    float similarity = sum / (uval * vval);
    return similarity;

}
vector<vector<float>> assignCenteroids(int k, vector<Pnode>& paraNodes, 
    vector<vector<float>>& embeddings){

    if(k > paraNodes.size()){
        return {};
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, embeddings.size());

    unordered_set<int> uniqueNums;

    while (uniqueNums.size() < k) {
        uniqueNums.insert(dist(gen));
    }
    vector<int> indices(uniqueNums.begin(), uniqueNums.end());

    vector<vector<float>> centeroids;
    for(int i: indices){
        centeroids.push_back(embeddings[i]);
    }
    return centeroids;
}
int runKmeans(int k, vector<Pnode>& paraNodes, 
    vector<vector<float>>& embeddings){
    
        vector<vector<float>> centeroids = assignCenteroids(k, paraNodes, embeddings);

}