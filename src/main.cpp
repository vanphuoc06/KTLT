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
    read_PortfolioCSV("Input/portfolio1.csv", portfolio); 
    processing_missing_data(portfolio); 
    calculate_Statistics(portfolio); 
    calculate_MA(portfolio, k);         
    Analysis("Output/Analysis.csv", portfolio);
    Signals("Output/Signals.csv",portfolio);
    BestPeriod("Output/BestPeriod.csv", portfolio, S);
    cout<<"Chuong trinh da chay xong";
    return 0;
}
