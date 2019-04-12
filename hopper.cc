#include <iostream>
#include <iomanip>
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

	// tracks the longest found exploration sequence
	int max_steps{};

	// idea: choose a start node with highest branch factor (only need to look at an index interval of 1..7)
	// 	 you can also always pop the node with highest branch factor. is it worth it?

	// DFS
	// log all nodes as unvisited except start
	unordered_map<int, bool> visited{};
	visited[0] = true;

	// loop through all unvisited nodes in the graph
	for (int start{}; start<numbers.size(); ++start) { if (!visited[start] || start == 0)
	{
		cout << endl << "start = " << start << endl;

		int curr_steps{};
		stack<int> s; s.push(start);
		while (!s.empty())
		{
			int curr = s.top(); s.pop();
			cout << "\tcurr = " << curr << endl;

			visited[curr] = true;
			curr_steps++;

			// push all unvisited neighbours to the stack
			for (int neighbour{1}; neighbour<numbers[curr].size(); ++neighbour)
			{
				if (!visited[ numbers[curr][neighbour] ]) {
					s.push(numbers[curr][neighbour]);
					visited[ numbers[curr][neighbour] ] = true;

					cout << "\t\tpushed " << numbers[curr][neighbour] << endl;
				}
			}
		}
		cout << "\tsteps = " << curr_steps << endl;

		if (curr_steps > max_steps)
			max_steps = curr_steps;

		if (max_steps == numbers.size())
			break;
	}}

	cout << max_steps << endl;
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
