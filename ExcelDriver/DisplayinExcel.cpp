#include "stdafx.h"
#include "BitsAndPieces\StringConversions.hpp"
#include "ExcelDriver.hpp"
#include "ExcelMechanisms.hpp"
#include "VectorsAndMatrices\NumericMatrix.cpp" // Template
#include "ExceptionClasses\DatasimException.hpp"
#include "DisplayinExcel.h"
#include <string>
#include <list>
using namespace std;

Vector<double, long> ConvertToLong(vector<double> vec)
{
	Vector<double, long> temp(vec.size());
	int i = 0;
	for (long n = temp.MinIndex(); n <= temp.MaxIndex(); n++)
	{
		temp[n] = vec[i];
		i++;
	}
	return temp;
}


void DisplayinExcel(Group *group1, Group *group2, Group *group3)
{
	long N = 90; //generate xarry
	Vector<double, long> xarr(N);
	double h1 = 1.0;
	xarr[xarr.MinIndex()] = -29.0;
	for (long i = xarr.MinIndex() + 1; i <= xarr.MaxIndex(); i++)
	{
		xarr[i] = xarr[i - 1] + h1;
	}
	list<Vector<double, long> >  CAAR_;
	CAAR_.push_back(ConvertToLong(group1->CAAR));
	CAAR_.push_back(ConvertToLong(group2->CAAR));
	CAAR_.push_back(ConvertToLong(group3->CAAR));


	list<Vector<double, long> >  AAR_;
	AAR_.push_back(ConvertToLong(group1->AAR));
	AAR_.push_back(ConvertToLong(group2->AAR));
	AAR_.push_back(ConvertToLong(group3->AAR));

	list<string> labels;
	labels.push_back("Miss");
	labels.push_back("Meet");
	labels.push_back("Beat");
	string Title1("CAAR");
	string Title2("AAR");
	string X("time");
	string Y("return");

	printInExcel(xarr, labels, CAAR_, Title1, X, Y);
	printInExcel(xarr, labels, AAR_, Title2, X, Y);
}

void displaymatrix(vector<vector<double>> Mat)
{
	int n = Mat.size(); //2
	int k = Mat[0].size();//90
	cout << setw(30);
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << Mat[j][i] << setw(30);
		}
		cout << endl;
	}
	cout << endl;
}