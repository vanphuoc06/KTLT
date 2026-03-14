#ifndef STRUCT_H
#define STRUCT_H
#include <bits/stdc++.h>
#include <vector>
#include <string>
using namespace std;
typedef struct Session
{
    string date;
    double close;
    long volume;
    double ma;
} Session;

typedef struct PortfolioStats
{
    double min_price;
    double max_price;
    double mean;
    double std;

} PortfolioStats;

typedef struct Stock
{
    string ticker;
    vector<Session> sessions;
    PortfolioStats stats;
} Stock;

typedef struct Portfolio
{
    vector<Stock> stocks;
    string name;
}Portfolio;

#endif