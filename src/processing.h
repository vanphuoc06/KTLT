#ifndef PROCESSING_H
#define PROCESSING_H

#include "io.h"
#include<vector>
#include<string>
#include<cmath>
using namespace std;

//Xử lý missing data
void processing_missing_data(Portfolio &portfolio);

//Tính toán thông kê(Min,Max,Mean,Std) và chuẩn hóa giá trị (Min-Max Normalization)
void calculate_Statistics(Portfolio &portfolio);

//Tính toán Moving Average cho từng phiên
void calculate_MA(Portfolio &portfolio, int k);

//Tìm lợi nhuận cao nhất có thể đạt được(Thuật toán Kadane)
double calculate_MaxProfit(Stock &stock, string &start_date, string &end_date);

//Tìm chiều dài của chuỗi ngày tăng giá liên tiếp dài nhất
int Longest_Increasing_Subarray(Stock &stock, string &start_date, string &end_date);

//Tìm khoảng thời gian ngắn nhất mà tổng giá trị cổ phoeeis >=mục tiêu S(Sliding Window)
int findShortestWindowForTargetProfit(Stock &stock, double &target, string &start_date, string &end_date);

//Tìm mảng cộng dồn(Prefix Sum) để truy vấn tổng nhanh chóng
vector <double> Prefix_Sum(Stock &stock);

//Truy vấn tổng giá trị trong một khoảng [left,right] dùng Prefix Sum
void Query_PrefixSum(vector<double> &Prefix_Sum, int left, int right, double &result);

#endif