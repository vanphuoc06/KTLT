#include "processing.h"
#include <algorithm>

using namespace std;

//Hàm kiểm tra mã cổ phiếu có bị rỗng hay không 
int check_empty(Stock &stock, string &start_date, string &end_date)
{
    if (stock.sessions.empty())
    {
        start_date = "NA";
        end_date = "NA";
        return 0; 
    }
    return 1;
}

//Hàm xử lý missing data
void processing_missing_data(Portfolio &portfolio)
{
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        for (int j = 0; j < portfolio.stocks[i].sessions.size(); j++)
        {
            if (portfolio.stocks[i].sessions[j].close == -1)
            {
                double prev_val = -1;
                double next_val = -1;

                if (j > 0)
                {
                    prev_val = portfolio.stocks[i].sessions[j - 1].close;
                }
                for (int k = j + 1; k < portfolio.stocks[i].sessions.size(); k++)
                {
                    if (portfolio.stocks[i].sessions[k].close != -1)
                    {
                        next_val = portfolio.stocks[i].sessions[k].close;
                        break;
                    }
                }

                if (prev_val != -1 && next_val != -1)
                {
                    portfolio.stocks[i].sessions[j].close = (prev_val + next_val) / 2.0;
                }
                else if (prev_val == -1 && next_val != -1)
                {
                    portfolio.stocks[i].sessions[j].close = next_val;
                }
                else if (prev_val != -1 && next_val == -1)
                {
                    portfolio.stocks[i].sessions[j].close = prev_val;
                }
                else
                {
                    portfolio.stocks[i].sessions[j].close = 0;
                }
            }
        }
    }
}

//Hàm tính toán các chỉ số thống kê và chuẩn hóa giá cổ phiếu(Min-Max Scaling)
void calculate_Statistics(Portfolio &portfolio)
{
    double global_max_stock = -1e9;
    double global_min_stock = 1e9;
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        Stock &stock = portfolio.stocks[i];
        double min_stock = 1e9;
        double max_stock = -1e9;
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
        if (count > 0)
        {
            stock.stats.mean = sum_stock / count;

            for (int k = 0; k < stock.sessions.size(); k++)
            {
                variance_sum += pow(stock.sessions[k].close - stock.stats.mean, 2);
            }
            stock.stats.std = sqrt(variance_sum / count);
        }
        else
        {
            stock.stats.mean = 0;
            stock.stats.std = 0;
        }
        if (min_stock < global_min_stock)
        {
            global_min_stock = min_stock;
        }
        if (max_stock > global_max_stock)
        {
            global_max_stock = max_stock;
        }
    }
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        for (int j = 0; j < portfolio.stocks[i].sessions.size(); j++)
        {
            Session &session = portfolio.stocks[i].sessions[j];
            if (global_max_stock > global_min_stock)
            {
                session.close = (session.close - global_min_stock) / (global_max_stock - global_min_stock);
            }
            else
            {
                session.close = 0;
            }
        }
    }
}

//Hàm tính Moving Average(MA) cho k ngày
void calculate_MA(Portfolio &portfolio, int k)
{
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        vector<double> prefix = Prefix_Sum(portfolio.stocks[i]);
        for (int j = 0; j < portfolio.stocks[i].sessions.size(); j++)
        {
            double sum = 0;
            int left = max(0, j - k + 1);
            int right = j;
            Query_PrefixSum(prefix, left, right, sum);
            if (sum != -1 && right >= left)
            {
                portfolio.stocks[i].sessions[j].ma = sum / (right - left + 1);
            }
            else
            {
                portfolio.stocks[i].sessions[j].ma = 0;
            }
        }
    }
}

//Hàm tìm lợi nhuận cao nhất (Kadane)
double calculate_MaxProfit(Stock &stock, string &start_date, string &end_date)
{
    if (check_empty(stock, start_date, end_date) == 0)
    {
        return -1;
    }
    if (stock.sessions.size() <= 1)
    {
        if (stock.sessions.empty())
        {
            start_date = "NA";
            end_date = "NA";
        }
        else
        {
            start_date = stock.sessions[0].date;
            end_date = stock.sessions[0].date;
        }
        return 0;
    }
    int start = 0;
    int end = 0;
    int temp_start = 0;
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
            start = temp_start;
            end = i;
        }
    }
    start_date = stock.sessions[start].date;
    end_date = stock.sessions[end].date;
    return Max_Profit;
}

//Hàm tìm chuỗi ngày tăng giá liên tiếp dài nhất(Longest Increasing Subarray)
int Longest_Increasing_Subarray(Stock &stock, string &start_date, string &end_date)
{
    if (check_empty(stock, start_date, end_date) == 0)
    {
        return -1;
    }
    int max_len = 1;
    int current_len = 1;
    int start = 0;
    int end = 0;
    int temp_start = 0;
    for (int i = 1; i < stock.sessions.size(); i++)
    {
        if (stock.sessions[i].close > stock.sessions[i - 1].close)
        {
            current_len++;
        }
        else
        {
            temp_start = i;
            current_len = 1;
        }
        if (current_len > max_len)
        {
            max_len = current_len;
            end = i;
            start = temp_start;
        }
    }
    start_date = stock.sessions[start].date;
    end_date = stock.sessions[end].date;
    return max_len;
}

//Hàm Sliding Window để tìm cửa sổ ngắn nhất có tổng giá trị >=giá trị mục tiêu S
int findShortestWindowForTargetProfit(Stock &stock, double &target, string &start_date, string &end_date)
{
    if (check_empty(stock, start_date, end_date) == 0)
    {
        return -1;
    }
    int left = 0;

    int min_len = 1e9;
    double current_sum = 0;
    start_date = "NA";
    end_date = "NA";
    
    vector<double> prefix = Prefix_Sum(stock);

    for (int right = 0; right < stock.sessions.size(); right++)
    {
        Query_PrefixSum(prefix, left, right, current_sum);
        while (current_sum >= target && left <= right)
        {
            if (min_len > right - left + 1)
            {
                min_len = right - left + 1;
                start_date = stock.sessions[left].date;
                end_date = stock.sessions[right].date;
            }
            left++;
            if (left <= right) {
                Query_PrefixSum(prefix, left, right, current_sum);
            }
        }
    }
    if (min_len == 1e9)
    {
        min_len = -1;
    }
    return min_len;
}

//Hàm tạo mảng cộng dồn(Prefix Sum)
vector<double> Prefix_Sum(Stock &stock)
{
    vector<double> prefix(stock.sessions.size() + 1, 0);
    for (int i = 1; i <= stock.sessions.size(); i++)
    {
        prefix[i] = prefix[i - 1] + stock.sessions[i - 1].close;
    }
    return prefix;
}

//Hàm truy vấn tổng trong đoạn [left,right] dựa trên Prefix Sum
void Query_PrefixSum(vector<double> &Prefix_Sum, int left, int right, double &result)
{
    if (left < 0 || right < left || right >= Prefix_Sum.size() - 1)
    {
        result = -1;
        return;
    }
    result = Prefix_Sum[right + 1] - Prefix_Sum[left];
}