#pragma once

#include "training.h"
#include <vector>

using namespace std;

vector<vector<float>> assignCenteroids(int k, vector<Pnode>& paraNodes);

vector<vector<float>> updateCenteroids(int k, vector<Pnode>& paraNodes);

void assignNodesToCluster(vector<Pnode>& paraNodes, vector<vector<float>>& centeroids);

float runKmeans(int k, vector<Pnode>& paraNodes, vector<vector<float>>& centeroids, int maxIterations = 30);

int findOptimalK(vector<Pnode>& paraNodes, int maxK = 8, int maxIterations = 30);
