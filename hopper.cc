#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>

using namespace std;

void get_input_values(int n, vector<vector<int>>& v);
void print(vector<vector<int>>& numbers);
void calculate_neighbours(vector<vector<int>>& numbers, int D, int M);

int main()
{
	int n, D, M;
	cin >> n >> D >> M;

	vector<vector<int>> numbers(n);
	get_input_values(n, numbers);

	calculate_neighbours(numbers, D, M);

	print(numbers);

	// depth first search
	// select a start node (one with highest branching factor?)
	for (int start{}; start<numbers.size(); ++start) {

		// log all nodes as unvisited except start
		unordered_map<int, bool> visited{};
		visited[start] = true;

		stack<int> s; s.push(start);
		while (!q.empty()) {
			// pop element from the stack
			int curr = s.pop()

			// if visited : break? // or while (visited[curr] { curr = s.pop()});

			//visited[curr] = true
			//

			// push all neighbours to the stack
			for (int neighs{}; neighs<numbers[curr].size(); ++neighs) {
				q.push(numbers[curr][neigh])
			}
		}
	}
}

// outer vector is filled with the given values
void get_input_values(int n, vector<vector<int>>& numbers)
{
	for (int i{}; i<n;++i)
	{
		int num;
		cin >> num;
		numbers[i].push_back(num);
	}
}

// inner vectors are filled with indexes to whatever indexes the hopper can jump to
void calculate_neighbours(vector<vector<int>>& numbers, int D, int M) {
	long unsigned int nr_size {numbers.size()};

	for (int i{}; i<nr_size; ++i)
	{
		for (int k{i+1}; k<=i+D; ++k)
		{
			if (k < nr_size && M >= abs(numbers[i][0] - numbers[k][0]))
			{
				numbers[i].push_back(k);
				numbers[k].push_back(i);
			}
		}
	}
}

// prints numbers and neighbours
void print(vector<vector<int>>& numbers)
{
	cout << endl << setw(5) << left << "idx" << setw(5) << left << "value"
		<< setw(12) << right << "neighbours" << endl;

	for (int row{}; row<numbers.size(); ++row)
	{
		cout << " " << row << ":" << setw(5) << numbers[row][0] << "    ";

		for (int k{1}; k<numbers[row].size();++k)
		{
			cout << numbers[row][k] << " ";
		}
		cout << endl;
	}
}
