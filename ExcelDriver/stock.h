#ifndef stock_h
#define stock_h
#include <stdio.h>
#include <vector>
#include <string>
#include <map>
using namespace std;


class Stock
{
public:
	string ticker;
	string release_date;
	double EPS_estimate;
	//string buf;
	string url;
	double EPS_actual;
	double surprise;
	vector<double> adj_Price;
	vector<string> stock_Date;
	vector<double> return_Vector;
	vector<double> AR;
	int starting_Point;

	Stock() {}
	Stock(string ticker_, string release_date_, string EPS_estimate_, string EPS_actual_, string surprise_) :
		ticker(ticker_), release_date(release_date_)
	{
		EPS_estimate = strtod(EPS_estimate_.c_str(), NULL);
		EPS_actual = strtod(EPS_actual_.c_str(), NULL);
		surprise = strtod(surprise_.c_str(), NULL);
	}
	void getdata();
	void url_(vector<string> time);
	void Pullinfo();//for menu purpose
};

class Group
{
public:
	map <string, Stock> onegroup;
	vector<double> AAR, CAAR;
	int NO;

	Group() {}
	Group(int NO_) :NO(NO_) {}
	void Loadgroup(vector<string> time);
	void TickerList();//for menu purpose
	int StockSearch(string ticker);//for menu purpose
	void Computation(Stock & SPX_Info);
	void GenerateAAR();
	void GenerateCAAR();
};

#endif /* Stock_hpp */
#pragma once

