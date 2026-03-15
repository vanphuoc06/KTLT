#include "processing.h"
#include <algorithm>
void processing_missing_data(Portfolio &portfolio)
{
}

void calculate_Statistics(Portfolio &portfolio)
{
    double global_max = -10 ^ 9;
    double global_min = 10 ^ 9;
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        Stock &stock = portfolio.stocks[i];
        double min_stock = 10 ^ 9;
        double max_stock = -10 ^ 9;
        double sum_stock = 0;
        double variance_sum = 0;
        int count = 0;
        for (int j = 0; j < stock.sessions.size(); j++)
        {
            if (stock.sessions[j].close > max_stock)
            {
                max_stock = stock.sessions[j].close;
            }
            if (stock.sessions[j].close < min_stock)
            {
                min_stock = stock.sessions[j].close;
            }
            count++;
            sum_stock += stock.sessions[j].close;
        }
        stock.stats.min_price = min_stock;
        stock.stats.max_price = max_stock;
        stock.stats.mean = sum_stock / count;

        for (int k = 0; k < stock.sessions.size(); k++)
        {
            variance_sum += pow(stock.sessions[k].close - stock.stats.mean, 2);
        }
        stock.stats.std = sqrt(variance_sum / count);
        if (min_stock < global_min)
        {
            global_min = min_stock;
        }
        if (max_stock > global_max)
        {
            global_max = max_stock;
        }
    }
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        for (int j = 0; j < portfolio.stocks[i].sessions.size(); j++)
        {
            Session &Session = portfolio.stocks[i].sessions[j];
            Session.close = (Session.close - global_min) / (global_max - global_min);
        }
    }
}
void calculate_MA(Portfolio &portfolio, int k)
{
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        for (int j = 0; j < portfolio.stocks[i].sessions.size(); j++)
        {
            double sum = 0;
            int count = 0;
            if (j - k >= 0)
            {
                for (int k = j; k > j - k && k >= 0; k--)
                {
                    sum += portfolio.stocks[i].sessions[k].close;
                    count++;
                }
            }
            portfolio.stocks[i].sessions[j].ma = sum / count;
        }
    }
}
int calculate_MaxProfit(Stock &stock)
{
    int start = 0;
    int end = 0;
    int temp_start;
    double Max_Profit = -1e9;
    double MaxEndingHere = 0;
    for (int i = 1; i < stock.sessions.size(); i++)
    {
        double diff = stock.sessions[i].close - stock.sessions[i - 1].close;
        if (MaxEndingHere < 0)
        {
            MaxEndingHere = diff;
            temp_start = i - 1;
        }
        else
        {
            MaxEndingHere += diff;
        }
        if (Max_Profit < MaxEndingHere)
        {
            Max_Profit = MaxEndingHere;
            start=temp_start;
            end=i;
        }
    }
    
}
void Longest_Increasing_Subarray()
{
}
void findShortestWindowForTargetProfit()
{
}
void calculate_RollingMean()
{
}

void Query_PrefixSum()
{
}