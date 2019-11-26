#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <unordered_map>
#include <iomanip>

#define verbose true

using namespace std;

void get_input(vector<vector<int>>& v);
void get_graph(vector<vector<int>>& numbers, int D, int M);
int dfs(vector<vector<int>>& numbers);

void analyze(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, int& largest_nw, vector<int>& bottle_nodes);
int get_max_path(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, unordered_map<int, bool>& visited, int node_idx);
void exclude_nodes(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, int back, int i, int rec_count);
void exclude_network(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, int node_idx);
int common_neighbour(vector<vector<int>>& numbers, int node_1, int node_2);
void remove_neighbour(vector<vector<int>>& numbers, int node_A, int node_B);
void copy_neighbours(vector<vector<int>>& numbers, int source_node, int destination_node, bool move, int dont_cp = -1);
void remove_duplicate_neighbours(vector<int>& neighbours);

void print(vector<vector<int>>& numbers);

class path {
public:
	path() : nodes{}, record_depth{} {}

	void add_node(pair<int, int>& parent_child);

	bool contains(int node_idx) { return pathed_nodes[node_idx]; }

	int get_record() { return record_depth; }

	int get_depth() { return nodes.size(); }

	void print() {cout<<nodes[0];for(int i{1};i<nodes.size();i++)cout<<"->"<<nodes[i];}

private:
	vector<int> nodes{};

	unordered_map<int, bool> pathed_nodes{};

	int record_depth{};
};

int main()
{
	int n, D, M; cin >> n >> D >> M;

	vector<vector<int>> numbers(n);

	get_input(numbers);

	get_graph(numbers, D, M);

	if (verbose) print(numbers);

	cout << dfs(numbers) << endl;
}

// performs a dfs search for the longest path in the graph
int dfs(vector<vector<int>>& numbers)
{
	int max_hops{};

	bool found{};

	unordered_map<int, bool> candidates{};

	int largest_nw{};

	vector<int> bottle_nodes{};

	analyze(candidates, numbers, largest_nw, bottle_nodes);

	if (verbose) cout << "largest nw: " << largest_nw << endl;

	for (int start{}; start<numbers.size(); ++start) if ( candidates[start] == true ) {
	{
		path path{};

		pair<int, int> p{-1, start};
		stack<pair<int, int>> s{}; s.push(p);

		while (!s.empty())
		{
			pair<int, int> pathpair{ s.top() }; s.pop();

			path.add_node(pathpair);

			int curr = pathpair.second;

			// pushing so that the closest neighbour is popped first
			for (unsigned long int neighbour{ numbers[curr].size() - 1 }; neighbour > 0; --neighbour)
			{
				if (!path.contains( numbers[curr][neighbour] ))
				{
					pair<int, int> p { curr, numbers[curr][neighbour] };

					s.push(p);
				}
			}

			if (path.get_record() > max_hops) max_hops = path.get_record();

			if (max_hops == largest_nw) {found = true; break;}
		}

		if (found) break;
	}}

	return max_hops;
}

// gather constraints to make searching less time complex
void analyze(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers,
	int& record_path, vector<int>& bottle_nodes)
{
	// setting all nodes to be a candidate
	for (int i{}; i<numbers.size(); ++i)
	{
		candidates[i] = true;
	}

	if (verbose) cout << "excluding neighbours to edge nodes:" << endl;

	// exclude neighbours to edge nodes
	for (int i{}; i<numbers.size(); ++i)
	{
		if (numbers[i].size() == 2)
		{
			int rec_count{};

			exclude_nodes(candidates, numbers, i, i, rec_count);
		}
	}

	if (verbose) cout << endl;

	// estimate a max-length of the longest path of connected nodes
	unordered_map<int, bool> visited{};
	for (int i{}; i<numbers.size(); ++i)
	{
		if (!visited[i] && candidates[i])
		{
			int longest_path = get_max_path(candidates, numbers, visited, i);

			if (longest_path > record_path)
			{
				record_path = longest_path;
			}
		}
	}

	if (verbose) cout << "excluding networks:" << endl;

	// exclude all networks whose max-length path is below 75% of the est. longest path
	visited = unordered_map<int, bool>{};
	for (int i{}; i<numbers.size(); ++i)
	{
		if (!visited[i])
		{
			int longest_path = get_max_path(candidates, numbers, visited, i);

			if (longest_path < record_path * 0.75)
			{
				exclude_network(candidates, numbers, i);
			}
		}
	}

	if (verbose) cout << endl;

	// rework complete sub graph (stands for majority of performance improvement in dense graphs)
	for (int node_A{}; node_A<numbers.size(); ++node_A)
	{
		if (numbers[node_A].size() > 2)
		{
			// go through our neighbours, see if they are neighbours with eachother
			vector<int> cpy{ numbers[node_A] };
			for (int i{ 1 }; i<cpy.size(); ++i)
			{
				int node_B = numbers[node_A][i];

				int node_C = common_neighbour(numbers, node_A, node_B);

				// there is a complete graph with node_A, node_B, and node_C
				if (node_C != -1)
				{
					if (verbose) cout << "node " << node_A << ", " << node_B << ", and " << node_C
					<< " are eachothers neighbours" << endl;

					// move neighbours from node B to node A
					// copy from C to A
					// copy from A to C
					// cut edge between A to C

					if (verbose) cout << "move from node " << node_B << " to node " << node_A << endl;
					copy_neighbours(numbers, node_B, node_A, true, node_C);

					if (verbose) cout << "new table" << endl;
					if (verbose) print(numbers);

					if (verbose) cout << "copy from node " << node_C << " to node " << node_A << endl;
					copy_neighbours(numbers, node_C, node_A, false, node_B);

					if (verbose) cout << "new table" << endl;
					if (verbose) print(numbers);

					if (verbose) cout << "copy from node " << node_A << " to node " << node_C << endl;
					copy_neighbours(numbers, node_A, node_C, false, node_B);

					if (verbose) cout << "new table" << endl;
					if (verbose) print(numbers);

					if (verbose) cout << "remove connection between nodes " << node_A << " and " << node_C << endl;
					remove_neighbour(numbers, node_A, node_C);
					remove_neighbour(numbers, node_C, node_A);

					if (verbose) cout << "new table" << endl;
					if (verbose) print(numbers);

					if (verbose) cout << "add neighbours " << " to " << node_B << endl;
					numbers[node_B].push_back(node_A);
					numbers[node_B].push_back(node_C);

					sort(numbers[node_A].begin() + 1, numbers[node_A].end());
					sort(numbers[node_C].begin() + 1, numbers[node_C].end());

					remove_duplicate_neighbours(numbers[node_A]);
					remove_duplicate_neighbours(numbers[node_C]);

					if (verbose) cout << "new table []" << endl;
					if (verbose) print(numbers);

					break;
				}
			}
		}
	}
	if (verbose) cout << "final table" << endl;
	if (verbose) print(numbers);
}

void remove_duplicate_neighbours(vector<int>& neighbours)
{
	vector<int> tripples{};

	for (int i{ 1 }; i<neighbours.size(); ++i)
	{
		int nums = count(neighbours.begin(), neighbours.end(), neighbours[i]);

		if (nums == 3)
		{
			tripples.push_back(neighbours[i]);
			neighbours.erase(neighbours.begin() + i);
		}
	}

	auto last = unique(neighbours.begin(), neighbours.end());
	neighbours.erase(last, neighbours.end());

//	for (int i : tripples)
	{
		// add i between node_A and node_B
		// move neighbours numbers[i][1..k] to node A and B
	}
}

void remove_neighbour(vector<vector<int>>& numbers, int node_A, int node_B)
{
	for (int i{}; i<numbers[node_A].size(); ++i)
	{
		if (numbers[node_A][i] == node_B)
		{
			numbers[node_A].erase(numbers[node_A].begin() + i);
		}
	}
}

void copy_neighbours(vector<vector<int>>& numbers, int source_node, int destination_node, bool move, int dont_cp)
{
	for (int i{ 1 }; i<numbers[source_node].size(); ++i)
	{
		// dont copy the destination index to the destination vector
		if (numbers[source_node][i] != destination_node && numbers[source_node][i] != dont_cp)
		{
			numbers[destination_node].push_back(numbers[source_node][i]);
		}
	}

	// optionally erases the source vector
	if (move)
	{
		int val = numbers[source_node][0];
		numbers[source_node].clear();
		numbers[source_node].push_back(val);
	}
}

int common_neighbour(vector<vector<int>>& numbers, int node_1, int node_2)
{
	for (int i{ 1 }; i<numbers[node_1].size(); ++i)
	{
		for (int k{ 1 }; k<numbers[node_2].size(); ++k)
		{
			if (numbers[node_1][i] == numbers[node_2][k])
			{
				return numbers[node_1][i];
			}
			if (numbers[node_1][i] < numbers[node_2][k])
			{
				break;
			}
		}
	}
	return -1;
}

// Estimate how long the longest path can potentially be (never underestimates the path)
int get_max_path(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers,
	unordered_map<int, bool>& visited, int node_idx)
{
	int edges{};

	int nodes{};

	visited[node_idx] = true;

	queue<int> q{}; q.push(node_idx);

	while(!q.empty())
	{
		int curr { q.front() }; q.pop();

		nodes++;

		if (numbers[curr].size() == 2) edges++;

		for (int i{1}; i < numbers[curr].size(); i++)
		{
			if (!visited[numbers[curr][i]])
			{
				q.push(numbers[curr][i]);
				visited[numbers[curr][i]] = true;
			}
		}
	}

	if (edges >= 2) return nodes - edges + 2;

	return nodes;
}

// excludes nodes that cannot be a starting node in the longest path
void exclude_nodes(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers,
	int back, int i, int rec_count)
{
	if (rec_count++ > 2000) return; // prevent kattis memory overuse (1024 MB)

	int neigh{1};
	while (numbers[i][neigh] == back) neigh++;

	if (numbers[numbers[i][neigh]].size() == 3)
	{
		candidates[numbers[i][neigh]] = false;

		exclude_nodes(candidates, numbers, i, numbers[i][neigh], rec_count);

		if(verbose) cout << "-"<< numbers[i][neigh] << " ";
	}

	if ( numbers[numbers[i][neigh]].size() > 3 )
	{
		candidates[numbers[i][neigh]] = false;

		if(verbose) cout << "-"<< numbers[i][neigh] << " ";
	}
}

// exclude all nodes in a network
void exclude_network(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, int node_idx)
{
	unordered_map<int, bool> visited{};

	visited[node_idx] = true;

	queue<int> q{}; q.push(node_idx);

	while(!q.empty())
	{
		int curr { q.front() }; q.pop();

		candidates[curr] = false;

		if(verbose) cout << "-"<< curr << " ";

		for (int i{1}; i < numbers[curr].size(); i++)
		{
			if (!visited[numbers[curr][i]])
			{
				q.push(numbers[curr][i]);
				visited[numbers[curr][i]] = true;
			}
		}
	}
}

// numbers[1..n][0] are the values from the user input
void get_input(vector<vector<int>>& numbers)
{
	for (int i{}; i<numbers.size(); ++i)
	{
		int num;
		cin >> num;
		numbers[i].push_back(num);
	}
}

// numbers[A][1..k] are the indexes of the neighbours of index A
void get_graph(vector<vector<int>>& numbers, int D, int M)
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

// prints graph
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

// path member functions
void path::add_node(pair<int, int>& parent_child)
{
	if (verbose && parent_child.first != -1 && parent_child.first != nodes.back()) {cout << "path: "; print(); cout << endl; }

	if (parent_child.first != -1)
	{
		while ( parent_child.first != nodes.back() )
		{
			pathed_nodes[nodes.back()] = false; nodes.pop_back();
		}
	}

	pathed_nodes[parent_child.second] = true; nodes.push_back(parent_child.second);

	if (nodes.size() > record_depth)
	{
		record_depth = nodes.size();
	}
}
