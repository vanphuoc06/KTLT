#include "io.h"
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
        getline(ss, ticker, ',');
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
    output_file << "date,close,volume,ma \n";
    for (int i = 0; i < portfolio.stocks.size(); i++)

    {
        for (int j = 0; j < portfolio.stocks[i].sessions.size(); j++)
        {
            Session s = portfolio.stocks[i].sessions[j];
            output_file << s.date << "," << s.close << "," << s.volume << "," << s.ma << "\n";
        }
    }
    output_file.close();
}
void Signals(string file_path,Portfolio &portfolio)
{
     ofstream output_file(file_path);
    if (!output_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
}
void BestPeriod(string file_path)
{
     ofstream output_file(file_path);
    if (!output_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
}
int main()
{
}