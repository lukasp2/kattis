#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
using namespace std;

void get_input_values(int n, vector<vector<int>>& v);
void print(vector<vector<int>>& numbers);
void calculate_neighbours(vector<vector<int>>& numbers);

int main()
{
	int n, D, M;
	cin >> n >> D >> M;

	vector<vector<int>> numbers(n);
	get_input_values(n, numbers);

	calculate_neighbours(numbers);

	print(numbers);
}

// outer vector is filled with the given values
void get_input_values(int n, vector<vector<int>>& numbers)
{
	for (int i{}; i<n;i++)
	{
		int num;
		cin >> num;
		numbers[i].push_back(num);
	}
}

// inner vector is filled with indexes to whatever indexes the hopper can access.
void calculate_neighbours(vector<vector<int>>& numbers) {


}

// prints numbers and neighbours
void print(vector<vector<int>>& numbers)
{
	cout << endl << setw(5) << left << "idx" << setw(5) << left << "value" << setw(12) << right << "neighbours" << endl;

	for (int row{}; row<numbers.size(); row++)
	{
		cout << " " << row << ":" << setw(4) << "[" << numbers[row][0] << "," << setw(4) << " [";

		for (int k{1}; k<numbers[row].size();k++)
		{
			cout << numbers[row][k] << ", ";
		}
		cout << "]]" << endl;
	}
}
