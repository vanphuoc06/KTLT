#include "io.h"
#include "processing.h"
#include <sstream>
#include <fstream>
#include <string>

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
        getline(ss, close, ',');
        getline(ss, ticker, ',');
        getline(ss, volume, ',');
        getline(ss, date, '\n');
        
        date.erase(remove(date.begin(), date.end(), '\r'), date.end());

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
void Signals(string file_path, Portfolio &portfolio, double S)
{
    string start_date_lis, end_date_lis;
    string start_date_sw, end_date_sw;
    ofstream output_file(file_path);
    if (!output_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
    output_file << " SIGNALS \n\n";
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        int max_len = Longest_Increasing_Subarray(portfolio.stocks[i], start_date_lis, end_date_lis);
        int shortest_window = findShortestWindowForTargetProfit(portfolio.stocks[i], S, start_date_sw, end_date_sw);
        output_file << "Ma co phieu [" << portfolio.stocks[i].ticker << "]\n";
        output_file << "  Chuoi tang lien tiep dai nhat : " << max_len << " phien (" << start_date_lis << " -> " << end_date_lis << ")\n";
        output_file << "  Chuoi ngay ngan nhat dat loi nhuan >= " << S << " : ";
        if (shortest_window == -1)
            output_file << "Khong dat duoc muc tieu\n";
        else
            output_file << shortest_window << " phien (" << start_date_sw << " -> " << end_date_sw << ")\n";
        output_file << "\n";
    }
    output_file.close();
}
void BestPeriod(string file_path, Portfolio &portfolio)
{
    ofstream output_file(file_path);
    if (!output_file.is_open())
    {
        cout << "File not found \n";
        return;
    }
    output_file << " BEST PERIOD \n\n";
    for (int i = 0; i < portfolio.stocks.size(); i++)
    {
        string start_max_profit, end_max_profit;
        double max_profit = calculate_MaxProfit(portfolio.stocks[i], start_max_profit, end_max_profit);
        output_file << "Ma co phieu [" << portfolio.stocks[i].ticker << "]\n";
        output_file << "  Loi nhuan cao nhat : " << max_profit << "\n";
        output_file << "  Ngay mua: " << start_max_profit << "\n";
        output_file << "  Ngay ban: " << end_max_profit << "\n";
        output_file << "\n";
    }
    output_file.close();
}