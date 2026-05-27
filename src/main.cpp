#include "io.h"
#include "processing.h"
#include <iostream>
using namespace std;
int main()
{
    int k;
    double S;
    Portfolio portfolio;
    read_configTXT("Input/config.txt", k, S);
    read_PortfolioCSV("Input/test_edge_1_row.csv", portfolio); 
    processing_missing_data(portfolio);       
    calculate_MA(portfolio, k);
    Signals("Output/signals.txt", portfolio, S);
    BestPeriod("Output/best_period.txt", portfolio);
    calculate_Statistics(portfolio);         
    Analysis("Output/Analysis.csv", portfolio);
    cout<<"Chuong trinh da chay xong";
    return 0;
}