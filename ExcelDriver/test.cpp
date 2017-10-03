#include "stdafx.h"
#include "BitsAndPieces\StringConversions.hpp"
#include "ExcelDriver.hpp"
#include "ExcelMechanisms.hpp"
#include "VectorsAndMatrices\NumericMatrix.cpp" // Template
#include "ExceptionClasses\DatasimException.hpp"

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "curl.h"
#include <fstream>
#include <vector>
#include <map>
#include "stock.h"
#include "DisplayinExcel.h"
#include <iomanip>
#include <thread>
using namespace std;

void thread_LoadGroup(Group *mygroup, vector<string> time) {
	mygroup->Loadgroup(time);
}

//void thread_Computation(Group *mygroup, Stock & SPY) {
//	mygroup->Computation(SPY);
//}

void Showmenu()
{
	cout << endl << "Which option would you like to choose?" << endl;
	cout << "1. Retrieve historical price data for all stocks." << endl;
	cout << "2. Pull information for one stock from one group." << endl;
	cout << "3. Show AAR or CAAR for one group." << endl;
	cout << "4. Show the Excel graph with CAAR for all 3 groups." << endl;
	cout << "5. Exit this program." << endl;
	cout << "Please type a number:" << endl;
}

int main()
{
	curl_global_init(CURL_GLOBAL_ALL);//**
	Stock SPY;
	vector<string> time;
	SPY.url = "http://real-chart.finance.yahoo.com/table.csv?s=SPY&a=00&b=1&c=2015&d=03&e=30&f=2017&g=d&ignore=.csv";
	SPY.getdata();
	time = SPY.stock_Date;

	std::thread t[3];

	string ticker_;
	int NO;
	Group group1(1), group2(2), group3(3);

	vector<vector<vector<double>>> Matrix(3);
	int d = 90; //group1.AAR.size()
	for (int i = 0; i < 3; i++)  Matrix[i].resize(2);
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			Matrix[i][k].resize(d);
		}
	}

	cout << "Welcome! O(^_^)O" << endl << endl;
	int n, k;
	int con = 9;
	cout << "Please choose 1 to load data first!" << endl << endl;

	Showmenu();
	cin >> n;

	while (con != 0)
	{
		switch (n)
		{

		case 1:
			cout << "Please wait..." << endl;

			t[0] = std::thread(thread_LoadGroup, &group1, time);
			t[1] = std::thread(thread_LoadGroup, &group2, time);
			t[2] = std::thread(thread_LoadGroup, &group3, time);

			for (int i = 0; i < 3; i++) {
				t[i].join();
			}

			//group1.Loadgroup(time);	group2.Loadgroup(time);	group3.Loadgroup(time);

			cout << "Done! o( ^V^ ) /" << endl << endl;
			Showmenu();
			cin >> n;
			break;


		case 2:

			if (group1.onegroup.empty())
			{
				cout << endl << "Please load the data first!" << endl;
				Showmenu();
				cin >> n;
				break;
			}
			cout << endl << "Which group, 1, 2 or 3?" << endl;
			cout << "Group: "; cin >> NO;
			if (NO == 1)
			{
				group1.TickerList();
				cout << "Which stock?" << endl;
				cin >> ticker_;
				if (group1.StockSearch(ticker_) == 1)
				{
					Showmenu();
					cin >> n;
					break;
				}
				else cout << "Invalid Ticker! Please use capitalized letters!" << endl;
			}
			else if (NO == 2)
			{
				group2.TickerList();
				cout << "Which stock?" << endl;
				cin >> ticker_;
				if (group2.StockSearch(ticker_) == 1)
				{
					Showmenu();
					cin >> n;
					break;
				}
				else cout << "Invalid Ticker! Please use capitalized letters!" << endl;
			}
			else if (NO == 3)
			{
				group3.TickerList();
				cout << "Which stock?" << endl;
				cin >> ticker_;
				if (group3.StockSearch(ticker_) == 1)
				{
					Showmenu();
					cin >> n;
					break;
				}
				else cout << "Invalid Ticker! Please use capitalized letters!" << endl;
			}
			else
			{
				cout << "Invalid Group Number!" << endl;
			}

			Showmenu();
			cin >> n;
			break;


		case 3:
			if (group1.onegroup.empty())
			{
				cout << endl << "Please load the data first!" << endl;
				Showmenu();
				cin >> n;
				break;
			}
			if (group1.AAR.empty() && group2.AAR.empty() && group3.AAR.empty()) {

				group1.Computation(SPY);
				group2.Computation(SPY);
				group3.Computation(SPY);

			}

			Matrix[0][0] = group1.AAR;
			Matrix[0][1] = group1.CAAR;
			Matrix[1][0] = group2.AAR;
			Matrix[1][1] = group2.CAAR;
			Matrix[2][0] = group3.AAR;
			Matrix[2][1] = group3.CAAR;
			cout << "Which group?" << endl;
			cout << "Group1: Miss; Group2: Meet; Group3: Beat. Choose from 1 to 3." << endl;
			cin >> NO;
			cout << "AAR or CAAR? Type 1 for AAR, 2 for CAAR and 3 for both." << endl;
			if (NO == 1)
			{
				cin >> k;
				if (k == 1) group1.GenerateAAR();
				else if (k == 2) group1.GenerateCAAR();
				else {
					cout << "Group" << NO << endl;
					cout << setw(30) << "AAR" << setw(30) << "CAAR" << endl;
					displaymatrix(Matrix[0]);
				}
			}
			else if (NO == 2)
			{
				cin >> k;
				if (k == 1) group2.GenerateAAR();
				else if (k == 2) group2.GenerateCAAR();
				else {
					cout << "Group" << NO << endl;
					cout << setw(30) << "AAR" << setw(30) << "CAAR" << endl;
					displaymatrix(Matrix[1]);
				}
			}
			else if (NO == 3)
			{
				cin >> k;
				if (k == 1) group3.GenerateAAR();
				else if (k == 2) group3.GenerateCAAR();
				else {
					cout << "Group" << NO << endl;
					cout << setw(30) << "AAR" << setw(30) << "CAAR" << endl;
					displaymatrix(Matrix[2]);
				}
			}
			else cout << "Invalid Group Number!" << endl;

			Showmenu();
			cin >> n;
			break;

		case 4:
			if (group1.onegroup.empty())
			{
				cout << endl << "Please load the data first!" << endl;
				Showmenu();
				cin >> n;
				break;
			}
			if (group1.AAR.empty())
			{
				cout << endl << "Please choose 3 to compute the CAAR first." << endl;
				Showmenu();
				cin >> n;
				break;
			}
			DisplayinExcel(&group1, &group2, &group3);
			Showmenu();
			cin >> n;
			break;

		case 5:
			cout << endl << "Are you sure you want to exit this program? (#'o'#)" << endl;
			cout << "Press 0 to exit and any key else to show menu" << endl;
			cin >> con;
			if (con == 0) break;
			else
			{
				Showmenu();
				cin >> n;
			}
			break;

		default:
			cout << endl << "Invalid choice! Please choose from 1 to 5!" << endl << endl;
			Showmenu();
			cin >> n;
			break;
		}
	}
	return 0;
}