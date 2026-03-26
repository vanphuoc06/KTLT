#ifndef PROCESSING_H
#define PROCESSING_H

#include "io.h"
#include<vector>
#include<string>
#include<cmath>

void processing_missing_data(Portfolio &portfolio);

void calculate_Statistics(Portfolio &portfolio);

void calculate_MA(Portfolio &portfolio, int k);

double calculate_MaxProfit(Stock &stock, string &start_date, string &end_date);

int Longest_Increasing_Subarray(Stock &stock, string &start_date, string &end_date);

int findShortestWindowForTargetProfit(Stock &stock, double &target, string &start_date, string &end_date);

vector <double> Prefix_Sum(Stock &stock);

void Query_PrefixSum(vector<double> &Prefix_Sum, int left, int right, double &result);

#endif