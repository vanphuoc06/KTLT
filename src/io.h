#ifndef IO_H
#define IO_H

#include <iostream>
#include <string>
#include "struct.h"
using namespace std;
void read_configTXT(string file_path, int &k, double &S);

void read_PortfolioCSV(string file_path,Portfolio &portfolio);

void Analysis(string file_path, Portfolio &portfolio);

void Signals(string file_path, Portfolio &portfolio);

void BestPeriod(string file_path, Portfolio &portfolio, double S);
#endif 