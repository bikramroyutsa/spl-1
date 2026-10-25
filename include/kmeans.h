#pragma once

#include "training.h"
#include <vector>

using namespace std;

vector<vector<float>> assignCenteroids(int k, vector<Pnode>& paraNodes);

vector<vector<float>> updateCenteroids(int k, vector<Pnode>& paraNodes);

void assignNodesToCluster(vector<Pnode>& paraNodes, vector<vector<float>>& centeroids);

void runKmeans(int k, vector<Pnode>& paraNodes, int maxIterations = 30);
