#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>

#define verbose false

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
	// the longest found exploration sequence
	int max_steps{};

	// loop through all unvisited nodes in the graph
	for (int start{}; start<numbers.size(); ++start)
	{
		if (verbose) cout << endl << "node: " << start << endl;

		unordered_map<int, int> map{{start, 1}};
		unordered_map<int, bool> visited{};

		stack<int> s{}; s.push(start);
		while (!s.empty())
		{
			int curr{ s.top() }; s.pop();
			if (verbose) cout << "\tpopped: " << curr << ", depth " << map[curr] << endl;

			visited[curr] = true;

			// push all unvisited neighbours to the stack
			for (int neighbour{1}; neighbour<numbers[curr].size(); ++neighbour)
			{
				if (!visited[ numbers[curr][neighbour] ]) {
					s.push(numbers[curr][neighbour]);
					visited[ numbers[curr][neighbour] ] = true;
					map[numbers[curr][neighbour]] = map[curr] + 1;

					if (verbose) cout << "\t\tpushed: " << numbers[curr][neighbour]
							<< ", depth " << map[numbers[curr][neighbour]]
							<< endl;
				}
			}
		}

		int high_steps{};
		for (auto i : map) {
			if (i.second > high_steps) high_steps = i.second;
		}
		if (verbose) cout << "\tlongest path: " << high_steps << endl;

		if (high_steps > max_steps)
			max_steps = high_steps;

		if (max_steps == numbers.size())
			break;
	}

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
