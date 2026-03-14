#ifndef PROCESSING_H
#define PROCESSING_H
#include "io.h"
#include<vector>
#include<string>
#include<cmath>
using namespace std;
void processing_missing_data(Portfolio &portfolio);
void calculate_Statistics(Portfolio &portfolio);
void calculate_MA(Portfolio &portfolio, int k);
int calculate_MaxProfit();
void Longest_Increasing_Subarray();
void findShortestWindowForTargetProfit();
void calculate_RollingMean();
vector <double> Prefix_Sum();
void Query_PrefixSum();
#endif