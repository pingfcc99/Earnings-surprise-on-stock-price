
#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include "curl.h"
#include <fstream>
#include <stack>
#include <vector>
#include <iomanip> //// std::setw
#include "stock.h"
#include <algorithm>////lower_bound()

using namespace std;

////operator overloading
typedef vector<double> V;
V operator+(const V &v1, const V &v2)
{
	int m = v1.size();
	V temp(m);
	for (int i = 0; i < m; i++)
		temp[i] = v1[i] + v2[i];
	return temp;
}
V operator/(const V &v1, const int &N)
{
	int m = v1.size();
	V temp(m);
	for (int i = 0; i < m; i++)
		temp[i] = v1[i] / N;
	return temp;
}

struct MemoryStruct {
	char *memory;
	size_t size;
};

void *myrealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
	NULL pointers, so we take care of it here */
	if (ptr)
		return realloc(ptr, size);//hold the memory, but change the size, in C dynamic is more efficient than C++
	else
		return malloc(size);//malloc~~new in C++, dynamic allocation
}

int write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;//
	struct MemoryStruct *mem = (struct MemoryStruct *)data;

	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1); //mem->size is occupied, 1:'\0'
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}
/*
void write_data(char* buf, int size, int nmemb, string * data)//size_t size...
{
	for (int c = 0; c < size*nmemb; c++)
	{
		data->push_back(buf[c]);
	}
}
*/
string url_generate(vector<string> time, Stock &s)   //time: SPY.stock_Date, s: each stock
{
	string target; //the type of date extracted from yahoo finance is 2016-01-20, but the type of date extracted from Bloomberg is 20160120
	target = (s.release_date).substr(0, 4) + "-" + (s.release_date).substr(4, 2) + "-" + (s.release_date).substr(6);

	int pos = find(time.begin(), time.end(), target) - time.begin();//*********match the time of SPY
	
	if (pos == time.size()) 
		pos = lower_bound(time.begin(), time.end(), target) - time.begin();
	//pos = upper_bound(time.begin(), time.end(), target) - time.begin();
	s.starting_Point = pos - 30;
	

	/*
	long int da_te = strtol((s.release_date).substr(6).c_str(), NULL, 10) + 1;
	while (pos == time.size())//when release date is not trading day*********************************************
	{
		target = (s.release_date).substr(0, 4) + "-" + (s.release_date).substr(4, 2) + "-" + to_string(da_te);
		pos = find(time.begin(), time.end(), target) - time.begin();
		da_te = da_te + 1;
	}*/
	string url = "http://real-chart.finance.yahoo.com/table.csv?";
	vector<string> line;
	stringstream temp1(time[pos - 30]);
	for (string val; getline(temp1, val, '-');)
	{
		line.push_back(val);
	}
	stringstream temp2(time[pos + 60]);
	for (string val; getline(temp2, val, '-');)
	{
		line.push_back(val);
	}
	url += "s=" + s.ticker + "&a=" + to_string(strtol(line[1].c_str(), NULL, 10) - 1) + "&b=" + line[2] + "&c=" + 
		line[0] + "&d=" + to_string(strtol(line[4].c_str(), NULL, 10) - 1) + "&e=" + line[5] + "&f=" + line[3] + "&g=d&ignore=.csv";
	//month-1
	return url;
}

void Stock::url_(vector<string> time)
{
	url = url_generate(time, *this);
}

void Stock::getdata()
{
	CURL *curl;
	CURLcode res;

	struct MemoryStruct data;//data:instance
	data.memory = NULL;
	data.size = 0;
	curl = curl_easy_init();

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());//c_str: change url to a pointer to an array
		curl_easy_setopt(curl, CURLOPT_ENCODING, "");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
		res = curl_easy_perform(curl);
		stringstream sData;
		sData.str(data.memory);//treat sdata as string
		string sValue;
		double dValue = 0;
		stack<double> store_Price;
		stack<string> store_Date;

		for (string line; getline(sData, line); ) {
			sValue = line.substr(line.find_last_of(',') + 1);
			dValue = strtod(sValue.c_str(), NULL);
			sValue = line.substr(0, line.find_first_of(','));

			if (sValue != "Date") {
				store_Price.push(dValue);
				store_Date.push(sValue);
			}
		}
		double temp = store_Price.top();
		string temp1 = store_Date.top();
		adj_Price.push_back(temp);
		stock_Date.push_back(temp1);
		store_Price.pop();
		store_Date.pop();
		while (!store_Price.empty()) {
			return_Vector.push_back((store_Price.top() - temp) / temp);
			temp = store_Price.top();
			temp1 = store_Date.top();
			store_Price.pop();
			store_Date.pop();
			adj_Price.push_back(temp);
			stock_Date.push_back(temp1);
		}
	}
	if (res == CURLE_OK)
	{
		curl_easy_cleanup(curl);
		//curl_global_cleanup();
	}
}

void Stock::Pullinfo()
{
	cout << "Ticker: " << ticker << endl;
	cout << "Earning Release Date: " << release_date << endl;
	cout << "EPS Estimate: " << EPS_estimate << endl;
	cout << "Actual EPS:  " << EPS_actual << endl;
	cout << "Surprise:  " << surprise << endl;
	cout << "Historical Price: " << endl;
	for (int i = 0; i < stock_Date.size(); i++)
	{
		cout << stock_Date[i] << "   " << adj_Price[i] << endl;
	}
}

void Group::Loadgroup(vector<string> time)
{
	int adjust = 40 * (NO - 1);
	string ticker, release_date, estim_ate, actu_al, sur_prise;
	size_t index; 

	ifstream file("./data_bloomberg_0420.csv");//*relative path
	while (file.good())
	{
		int i = 0;
		for (string line; getline(file, line);)
		{
			if (i > adjust && i <= NO * 40)
			{
				ticker = line.substr(0, line.find(','));
				index = line.find(',');
				release_date = line.substr(index + 1, line.find(',', index + 1) - index - 1);
				index = line.find(',', index + 1);
				estim_ate = line.substr(index + 1, line.find(',', index + 1) - index - 1);
				index = line.find(',', index + 1);
				actu_al = line.substr(index + 1, line.find(',', index + 1) - index - 1);
				index = line.find(',', index + 1);
				sur_prise = line.substr(index + 1, line.find(',', index + 1) - index - 1);
				Stock *ptr = new Stock(ticker, release_date, estim_ate, actu_al, sur_prise);//dynamic allocation
				onegroup[ticker] = ptr;
				//onegroup[ticker] = Stock(ticker, release_date, estim_ate, actu_al, sur_prise); //warning: Stock would be destoryed
			}
			i += 1;
		}
	}
	map<string, Stock>::iterator itr;
	for (itr = onegroup.begin(); itr != onegroup.end(); itr++)
	{
		(itr->second).url_(time);
		(itr->second).getdata();
	}
}

void Group::TickerList()
{
	map<string, Stock>::iterator itr2;
	for (itr2 = onegroup.begin(); itr2 != onegroup.end(); itr2++)
	{
		cout << itr2->first << endl;
	}
}
int Group::StockSearch(string ticker)
{
	map<string, Stock>::iterator itr4;
	for (itr4 = onegroup.begin(); itr4 != onegroup.end(); itr4++)
	{
		if ((itr4->second).ticker == ticker)
		{
			itr4->second.Pullinfo();
			return 1;
		}
	}
	return 0;
}


void Group::Computation(Stock & SPY_Info)
{
	map<string, Stock>::iterator itr = onegroup.begin();
	int N = (itr->second).return_Vector.size();//90
	AAR.resize(N); CAAR.resize(N);
	while (itr != onegroup.end())
	{
		for (int i = 0; i < (itr->second).return_Vector.size(); i++)
		{
			(itr->second).AR.push_back((itr->second).return_Vector[i] - SPY_Info.return_Vector[i + (itr->second).starting_Point]);  
		}
		if (itr == onegroup.begin())     AAR = (itr->second).AR;
		else     AAR = (itr->second).AR + AAR; //operator overloading
		itr++;
	}
	AAR = AAR / 40; //operator overloading
	CAAR[0] = AAR[0];
	for (int t = 1; t < N; t++)
		CAAR[t] = AAR[t] + CAAR[t - 1];
}

void Group::GenerateAAR()
{
	for (int i = 0; i < AAR.size(); i++)
	{
		cout << AAR[i] << endl;
	}
}

void Group::GenerateCAAR()
{
	for (int i = 0; i < CAAR.size(); i++)
	{
		cout << CAAR[i] << endl;
	}
}



