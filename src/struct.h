#ifndef STRUCT_H
#define STRUCT_H
#include <bits/stdc++.h>
#include <vector>
#include <string>

using namespace std;

//Cấu trúc lưu trữ dữ liệu giao dịch của 1 cổ phiếu trong 1 phiên(1 ngày)
typedef struct Session
{
    string date;
    double close;
    long volume;
    double ma;
} Session;

//Cấu trúc lưu trữ các chỉ số thống kê của 1 mã cổ phiếu
typedef struct PortfolioStats
{
    double min_price;
    double max_price;
    double mean;
    double std;

} PortfolioStats;

//Cấu trúc lưu trữ thông tin của 1 mã cổ phiếu
typedef struct Stock
{
    string ticker;
    vector<Session> sessions;
    PortfolioStats stats;
} Stock;
//Cấu trúc lưu trữ toàn bộ danh mục đầu tư(bao gồm các mã cổ phiếu)
typedef struct Portfolio
{
    vector<Stock> stocks;
    string name;
}Portfolio;

#endif