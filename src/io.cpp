#include "io.h"
#include "processing.h"
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;
void read_configTXT(string file_path, int &k, double &S)
{
    ifstream input_file(file_path);
    if (!input_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
    input_file >> k >> S;
    input_file.close();
    return;
}

void read_PortfolioCSV(string file_path, Portfolio &portfolio)
{
    ifstream input_file(file_path);
    if (!input_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
    string line;
    string ticker, date, close, volume;
    getline(input_file, line);
    while (getline(input_file, line))
    {
        Session session;
        stringstream ss(line);
        if (line.empty())
        {
            continue;
        }
        getline(ss, date, ',');
        getline(ss, close, ',');
        getline(ss, volume, ',');
        getline(ss, ticker, '\n');
        session.close = (close.empty() || close == "NA") ? -1 : stod(close);
        session.volume = (volume.empty() || volume == "NA") ? -1 : stol(volume);
        session.date = (date.empty() || date == "NA") ? "Khong tim thay" : date;
        int found = 0;
        for (int i = 0; i < portfolio.stocks.size(); i++)
        {
            if (portfolio.stocks[i].ticker == ticker)
            {
                portfolio.stocks[i].sessions.push_back(session);
                found = 1;
                break;
            }
        }
        if (!found)
        {
            Stock New_Stock;
            New_Stock.ticker = ticker;
            New_Stock.sessions.push_back(session);
            portfolio.stocks.push_back(New_Stock);
        }
    }
    input_file.close();
    return;
}
void Analysis(string file_path, Portfolio &portfolio)
{
    ofstream output_file(file_path);
    if (!output_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
    output_file << "Ticker,Date,Close,Volume,Ma\n";
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        for (int j = 0; j < portfolio.stocks[i].sessions.size(); j++)
        {
            Session s = portfolio.stocks[i].sessions[j];
            output_file << portfolio.stocks[i].ticker << "," << s.date << "," << s.close << "," << s.volume << "," << s.ma << "\n";
        }
    }
    output_file.close();
}
void Signals(string file_path, Portfolio &portfolio)
{
    string start_date, end_date;
    ofstream output_file(file_path);
    if (!output_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
    output_file << "Ticker,Chuoi_ngay_tang_dai_nhat,Ngay_bat_dau,Ngay_ket_thuc\n";
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        int max_len = Longest_Increasing_Subarray(portfolio.stocks[i], start_date, end_date);
        output_file << portfolio.stocks[i].ticker << "," << max_len << "," << start_date << "," << end_date << "\n";
    }
    output_file.close();
}
void BestPeriod(string file_path, Portfolio &portfolio, double S)
{
    ofstream output_file(file_path);
    if (!output_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
    output_file << "Ticker,Max_Profit,Ngay_mua,Ngay_ban,Loi_nhuan_muc_tieu,Ngay_mua_dat_muc_tieu,Ngay_ban_dat_muc_tieu\n";
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        string start_max_profit, end_max_profit, start_S, end_S;
        double max_profit = calculate_MaxProfit(portfolio.stocks[i], start_max_profit, end_max_profit);
        int max_len = findShortestWindowForTargetProfit(portfolio.stocks[i], S, start_S, end_S);
        output_file << portfolio.stocks[i].ticker << "," << max_profit << "," << start_max_profit << "," << end_max_profit << "," << S << "," << start_S << "," << end_S << "\n";
    }

    output_file.close();
}