#pragma once
#ifndef DisplayInExcel_h
#define DisplayInExcel_h
#include "stdafx.h"
#include "BitsAndPieces\StringConversions.hpp"
#include "ExcelDriver.hpp"
#include "ExcelMechanisms.hpp"
#include "VectorsAndMatrices\NumericMatrix.cpp" 
#include "ExceptionClasses\DatasimException.hpp"
#include "stock.h"
#include <string>
#include <iomanip>

Vector<double, long> ConvertToLong(vector<double> vec);
//transfer vector<double> to Vector<double, long> to apply to the Excel driver format

void DisplayinExcel(Group *group1, Group *group2, Group *group3);
//Display CAAR performance of three groups in EXCEL
void displaymatrix(vector<vector<double>> Mat);
//Display matrix
#endif // !DisplayInExcel_h