#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>

#define verbose true

using namespace std;
void get_input_values(int n, vector<vector<int>>& v);
void print(vector<vector<int>>& numbers);
void calculate_neighbours(vector<vector<int>>& numbers, int D, int M);
void depth_first_search(vector<vector<int>>& numbers);

int main()
{
	int n, D, M;
	cin >> n >> D >> M;

	vector<vector<int>> numbers(n);
	get_input_values(n, numbers);

	calculate_neighbours(numbers, D, M);
	if (verbose) print(numbers);
	depth_first_search(numbers);
}

// performs a DFS search for the longest exploration sequence
void depth_first_search(vector<vector<int>>& numbers)
{
	/*
	 * improvements (?):
	 * - choose a start node with highest branch factor (only need to look
	 *   at an index interval of [1,7] (because of the constraint on D).
	 *
	 * - always pop the node with highest branch factor. (worth it?)
	 */

	// the longest found exploration sequence
	int max_steps{};

	unordered_map<int, bool> visited{};

	// loop through all unvisited nodes in the graph
	for (int start{}; start<numbers.size(); ++start) { if (!visited[start])
	{
		if (verbose) cout << endl << "node: " << start << endl;

		int curr_steps{};
		stack<int> s{}; s.push(start);
		while (!s.empty())
		{
			int curr{ s.top() }; s.pop();
			if (verbose) cout << "\tpopped: " << curr << endl;

			visited[curr] = true;
			curr_steps++;

			// push all unvisited neighbours to the stack
			for (int neighbour{1}; neighbour<numbers[curr].size(); ++neighbour)
			{
				if (!visited[ numbers[curr][neighbour] ]) {
					s.push(numbers[curr][neighbour]);
					visited[ numbers[curr][neighbour] ] = true;

					if (verbose) cout << "\t\tpushed: " << numbers[curr][neighbour] << endl;
				}
			}
		}
		if (verbose) cout << "\tlongest path: " << curr_steps << endl;

		if (curr_steps > max_steps)
			max_steps = curr_steps;

		if (max_steps == numbers.size())
			break;
	}}

	cout << max_steps << endl;
}

// numbers[1..n][0] are the values from the user input
void get_input_values(int n, vector<vector<int>>& numbers)
{
	for (int i{}; i<n; ++i)
	{
		int num;
		cin >> num;
		numbers[i].push_back(num);
	}
}

// numbers[A][1..k] are the indexes of the neighbours of index A
void calculate_neighbours(vector<vector<int>>& numbers, int D, int M) 
{
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
