#include <iomanip>
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <iterator>

using namespace std;

int compareRows(vector<vector<double>>& paymentMatrix, int firstIndex, int secondIndex, list<int>& columns)
{
	int checkSumEqual	 = 0;
	int checkSumNotEqual = 0;
	
	for(int column : columns)
	{
		if(paymentMatrix[firstIndex][column] == paymentMatrix[secondIndex][column])
		{
			checkSumEqual++;
		}
		else
		{
			if(paymentMatrix[firstIndex][column] > paymentMatrix[secondIndex][column])
			{
				checkSumNotEqual++;
			}
			else
			{
				checkSumNotEqual--;
			}
		}
	}
	
	if(abs(checkSumNotEqual) + checkSumEqual == columns.size())
	{
		if(checkSumNotEqual >= 0)
		{
			return secondIndex;
		}
		else
		{
			return firstIndex;
		}
	}
	else
	{
		return -1;
	}
}

int compareColumns(vector<vector<double>>& paymentMatrix, int firstIndex, int secondIndex, list<int>& rows)
{
	int checkSumEqual	 = 0;
	int checkSumNotEqual = 0;
	
	for(int row : rows)
	{
		if(paymentMatrix[row][firstIndex] == paymentMatrix[row][secondIndex])
		{
			checkSumEqual++;
		}
		else
		{
			if(paymentMatrix[row][firstIndex] < paymentMatrix[row][secondIndex])
			{
				checkSumNotEqual++;
			}
			else
			{
				checkSumNotEqual--;
			}
		}
	}
	
	if(abs(checkSumNotEqual) + checkSumEqual == rows.size())
	{
		if(checkSumNotEqual >= 0)
		{
			return secondIndex;
		}
		else
		{
			return firstIndex;
		}
	}
	else
	{
		return -1;
	}
}

bool cutRows(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns)
{
	int indexForDeletion;
	int increment = 0;
	list<int>::iterator outerIter;
	list<int>::iterator innerIter;
	bool isPerformed = false;
	
	outerIter = rows.begin();

	while(outerIter != rows.end())
	{
		innerIter = outerIter;
		innerIter++;

		while(innerIter != rows.end())
		{
			indexForDeletion = compareRows(paymentMatrix, *outerIter, *innerIter, columns);
			
			if(indexForDeletion != -1)
			{
				if(indexForDeletion == *outerIter)
				{
					outerIter = rows.erase(outerIter);
					isPerformed = true;
					increment = -1;
					break;
				}
				else
				{
					innerIter = rows.erase(innerIter);
					isPerformed = true;
				}
			}
			else
			{
				innerIter++;
			}
		}

		advance(outerIter, increment + 1);
		increment = 0;
	}

	return isPerformed;
}

bool cutColumns(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns)
{
	int indexForDeletion;
	int increment = 0;
	list<int>::iterator outerIter;
	list<int>::iterator innerIter;
	bool isPerformed = false;
	
	outerIter = columns.begin();

	while(outerIter != columns.end())
	{
		innerIter = outerIter;
		innerIter++;

		while(innerIter != columns.end())
		{
			indexForDeletion = compareColumns(paymentMatrix, *outerIter, *innerIter, rows);
			
			if(indexForDeletion != -1)
			{
				if(indexForDeletion == *outerIter)
				{
					outerIter = columns.erase(outerIter);
					isPerformed = true;
					increment = -1;
					break;
				}
				else
				{
					innerIter = columns.erase(innerIter);
					isPerformed = true;
				}
			}
			else
			{
				innerIter++;
			}
		}

		advance(outerIter, increment + 1);
		increment = 0;
	}

	return isPerformed;
}

void cutMatrix(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns)
{
	bool isPerformedForRows		= true;
	bool isPerformedForColumns	= true;
	
	for(int i = 0; i < paymentMatrix.size(); i++)
	{
		rows.push_back(i);
	}

	for(int j = 0; j < paymentMatrix[0].size(); j++)
	{
		columns.push_back(j);
	}

	while(isPerformedForRows || isPerformedForColumns)
	{
		isPerformedForRows		= cutRows(paymentMatrix, rows, columns);
		isPerformedForColumns	= cutColumns(paymentMatrix, rows, columns);
	}
}

pair<double, double> createFirstStrategy(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns)
{
	double p1;
	double I;
	double h11 = paymentMatrix[rows.front()][columns.front()];
	double h12 = paymentMatrix[rows.front()][columns.back()];
	double h21 = paymentMatrix[rows.back()][columns.front()];
	double h22 = paymentMatrix[rows.back()][columns.back()];

	p1 = (h22 - h21) / (h11 + h22 - h12 - h21);
	I = (h11 - h21) * p1 + h21;

	return make_pair(p1, I);
}

pair<double, double> createSecondStrategy(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns)
{
	double q1;
	double I;
	double h11 = paymentMatrix[rows.front()][columns.front()];
	double h12 = paymentMatrix[rows.front()][columns.back()];
	double h21 = paymentMatrix[rows.back()][columns.front()];
	double h22 = paymentMatrix[rows.back()][columns.back()];

	q1 = (h22 - h12) / (h11 + h22 - h12 - h21);
	I = (h11 - h12) * q1 + h12;

	return make_pair(q1, I);
}

int findMaximumInColumn(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns)
{
	int column = columns.back();
	double valueAtIndexOfMaximum = paymentMatrix[rows.front()][column];
	int indexOfMaximum = rows.front();

	for(int item : rows)
	{
		if(paymentMatrix[item][column] > valueAtIndexOfMaximum)
		{
			valueAtIndexOfMaximum = paymentMatrix[item][column];
			indexOfMaximum = item;
		}
	}

	return indexOfMaximum;
}

int findMinimumInRow(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns)
{
	int row = rows.back();
	double valueAtIndexOfMinimum = paymentMatrix[row][columns.front()];
	int indexOfMinimum = columns.front();

	for(int item : columns)
	{
		if(paymentMatrix[row][item] < valueAtIndexOfMinimum)
		{
			valueAtIndexOfMinimum = paymentMatrix[row][item];
			indexOfMinimum = item;
		}
	}

	return indexOfMinimum;
}

pair<int, pair<double, double>> findIntersectionForRoof(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns, int currStrategy, double pointOfIntersection)
{
	int newStrategy;
	double newPointOfIntersection = 1;
	double tempPointOfIntersection;
	double h11;
	double h12;
	double h21;
	double h22;

	h11 = paymentMatrix[currStrategy][columns.front()];
	h12 = paymentMatrix[currStrategy][columns.back()];

	for(int item : rows)
	{
		h21 = paymentMatrix[item][columns.front()];
		h22 = paymentMatrix[item][columns.back()];

		tempPointOfIntersection = (h22 - h12) / (h11 + h22 - h12 - h21);

		if(tempPointOfIntersection > pointOfIntersection && tempPointOfIntersection < newPointOfIntersection)
		{
			newPointOfIntersection = tempPointOfIntersection;
			newStrategy = item;
		}
	}

	return make_pair(newStrategy, make_pair(newPointOfIntersection, (h11 - h12) * newPointOfIntersection + h12));
}

pair<int, pair<double, double>> findIntersectionForFloor(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns, int currStrategy, double pointOfIntersection)
{
	int newStrategy;
	double newPointOfIntersection = 1;
	double tempPointOfIntersection;
	double h11;
	double h12;
	double h21;
	double h22;

	h11 = paymentMatrix[rows.front()][currStrategy];
	h21 = paymentMatrix[rows.back()][currStrategy];

	for(int item : columns)
	{
		h12 = paymentMatrix[rows.front()][item];
		h22 = paymentMatrix[rows.back()][item];

		tempPointOfIntersection = (h22 - h21) / (h11 + h22 - h21 - h12);

		if(tempPointOfIntersection > pointOfIntersection && tempPointOfIntersection < newPointOfIntersection)
		{
			newPointOfIntersection = tempPointOfIntersection;
			newStrategy = item;
		}
	}

	return make_pair(newStrategy, make_pair(newPointOfIntersection, (h11 - h21) * newPointOfIntersection + h21));
}

pair<pair<int, int>, double> findRoof(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns, int currStrategy, double pointOfIntersection)
{
	pair<int, pair<double, double>> newStrategyAndNewPointOfIntersection;
	pair<pair<int, int>, double> potentialActiveStrategies;

	rows.remove(currStrategy);
	newStrategyAndNewPointOfIntersection = findIntersectionForRoof(paymentMatrix, rows, columns, currStrategy, pointOfIntersection);
	potentialActiveStrategies.second = newStrategyAndNewPointOfIntersection.second.second;
	
	if(newStrategyAndNewPointOfIntersection.second.first != 1)
	{
		potentialActiveStrategies = findRoof(paymentMatrix, rows, columns, newStrategyAndNewPointOfIntersection.first, newStrategyAndNewPointOfIntersection.second.first);
	}

	if(newStrategyAndNewPointOfIntersection.second.second < potentialActiveStrategies.second)
	{
		potentialActiveStrategies.second = newStrategyAndNewPointOfIntersection.second.second;
		potentialActiveStrategies.first.first = currStrategy;
		potentialActiveStrategies.first.second = newStrategyAndNewPointOfIntersection.first;
	}

	return potentialActiveStrategies;
}

pair<pair<int, int>, double> findFloor(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns, int currStrategy, double pointOfIntersection)
{
	pair<int, pair<double, double>> newStrategyAndNewPointOfIntersection;
	pair<pair<int, int>, double> potentialActiveStrategies;

	columns.remove(currStrategy);
	newStrategyAndNewPointOfIntersection = findIntersectionForFloor(paymentMatrix, rows, columns, currStrategy, pointOfIntersection);
	potentialActiveStrategies.second = newStrategyAndNewPointOfIntersection.second.second;
	
	if(newStrategyAndNewPointOfIntersection.second.first != 1)
	{
		potentialActiveStrategies = findFloor(paymentMatrix, rows, columns, newStrategyAndNewPointOfIntersection.first, newStrategyAndNewPointOfIntersection.second.first);
	}

	if(newStrategyAndNewPointOfIntersection.second.second > potentialActiveStrategies.second)
	{
		potentialActiveStrategies.second = newStrategyAndNewPointOfIntersection.second.second;
		potentialActiveStrategies.first.first = currStrategy;
		potentialActiveStrategies.first.second = newStrategyAndNewPointOfIntersection.first;
	}

	return potentialActiveStrategies;
}

void findActiveStrategies(vector<vector<double>>& paymentMatrix, list<int>& rows, list<int>& columns)
{
	pair<int, int> activeStrategies;
	int beginningStrategy;
	
	if(rows.size() != 2)
	{
		beginningStrategy = findMaximumInColumn(paymentMatrix, rows, columns);
		activeStrategies = findRoof(paymentMatrix, rows, columns, beginningStrategy, 0).first;
		rows.resize(0);
		rows.push_back(activeStrategies.first);
		rows.push_back(activeStrategies.second);
		rows.sort();
	}
	else
	if(columns.size() != 2)
	{
		beginningStrategy = findMinimumInRow(paymentMatrix, rows, columns);
		activeStrategies = findFloor(paymentMatrix, rows, columns, beginningStrategy, 0).first;
		columns.resize(0);
		columns.push_back(activeStrategies.first);
		columns.push_back(activeStrategies.second);
		columns.sort();
	}
}

int main()
{
	int n1;
	int n2;
	vector<vector<double>> paymentMatrix;
	list<int> rows;
	list<int> columns;
	pair<double, double> firstStrategy;
	pair<double, double> secondStrategy;
	
	ifstream fin("input.txt");
	fin >> n1 >> n2;

	paymentMatrix.resize(n1);
	for(vector<double>& vector : paymentMatrix)
	{
		vector.resize(n2);
		for(double& item : vector)
		{
			fin >> item;
		}
	}

	fin.close();

	cutMatrix(paymentMatrix, rows, columns);
	findActiveStrategies(paymentMatrix, rows, columns);
	firstStrategy  = createFirstStrategy(paymentMatrix, rows, columns);
	secondStrategy = createSecondStrategy(paymentMatrix, rows, columns);

	ofstream fout("output.txt");

	fout << "p" <<  rows.front() + 1 << " = " << firstStrategy.first	 << endl;
	fout << "p" <<  rows.back()  + 1 << " = " << 1 - firstStrategy.first << endl;
	fout << endl;
	fout << "q" <<  columns.front() + 1 << " = " << secondStrategy.first	 << endl;
	fout << "q" <<  columns.back()  + 1 << " = " << 1 - secondStrategy.first << endl;
	fout << endl;
	fout << "I" << " = " << firstStrategy.second;

	return 0;
}