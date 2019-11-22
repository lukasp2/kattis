#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>

using namespace std;

void get_input_values(vector<vector<int>>& v);
void calculate_neighbours(vector<vector<int>>& numbers, int D, int M);
int depth_first_search(vector<vector<int>>& numbers);

class path {
public:
	path() : nodes{}, record_depth{} {}

	void add_node(pair<int, int>& parent_child)
	{
		if (parent_child.first != -1)
		{
			while ( parent_child.first != nodes.back() )
			{
				pathed_nodes[nodes.back()] = false; nodes.pop_back();
			}
		}

		pathed_nodes[parent_child.second] = true; nodes.push_back(parent_child.second);

		update_depth();
	}

	bool contains(int node_idx) { return pathed_nodes[node_idx]; }

	int get_record() { return record_depth; }

private:
	void update_depth() { if (nodes.size() > record_depth) { record_depth = nodes.size(); } };

	vector<int> nodes{};

	unordered_map<int, bool> pathed_nodes{};

	int record_depth{};
};

int main()
{
	int n, D, M; cin >> n >> D >> M;

	vector<vector<int>> numbers(n);

	get_input_values(numbers);

	calculate_neighbours(numbers, D, M);

	cout << depth_first_search(numbers) << endl;
}

int depth_first_search(vector<vector<int>>& numbers)
{
	int max_hops{};

	for (int start{}; start<numbers.size(); ++start)
	{
		path path{};

		stack<pair<int, int>> s{}; s.push(make_pair<int, int>(-1, move(start)));

		while (!s.empty())
		{
			pair<int, int> pathpair{ s.top() }; s.pop();

			path.add_node(pathpair);

			int curr = pathpair.second;

			for (int neighbour{ 1 }; neighbour<numbers[curr].size(); ++neighbour)
			{
				if (!path.contains( numbers[curr][neighbour] ))
				{
					pair<int, int> p { curr, numbers[curr][neighbour] };
					s.push(p);
				}
			}
		}

		if (path.get_record() > max_hops) max_hops = path.get_record();

		if (max_hops == numbers.size())	break;
	}

	return max_hops;
}

// numbers[1..n][0] are the values from the user input
void get_input_values(vector<vector<int>>& numbers)
{
	for (int i{}; i<numbers.size(); ++i)
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
