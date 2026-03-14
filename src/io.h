#ifndef IO_H
#define IO_H
//Header Guard
#include <iostream>
#include <string>
#include "struct.h"
using namespace std;
void read_configTXT(string file_path,double &k,double &S);

void read_PortfolioCSV(string file_path,Portfolio &portfolio);

void Analysis(string file_path);

void Signals(string file_path);
void BestPeriod(string file_path);
#endif 