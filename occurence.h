#pragma once
#include <map>
#include <vector>
#include <string>
using namespace std;

void pmi(vector<vector<float>>& matrix, vector<int>& tokenCounts, int totaltokens);
vector<vector<float>> matrixBuilder(const int vocabSize, vector<int>& encodedTokens);