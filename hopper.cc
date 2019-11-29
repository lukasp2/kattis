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

void remove_neighbour(vector<vector<int>>& numbers, int node1, int node2);
void add_neighbour(vector<vector<int>>& numbers, int node1, int node2);

void copy_neighbours(vector<vector<int>>& numbers, int source_node, int destination_node, bool move, int dont_cp = -1);
vector<int> remove_duplicate_neighbours(vector<vector<int>>& numbers, int node, int node_B);
void add_to_chain(vector<vector<int>>& numbers, vector<int>& tripples, int node_A, int node_B, int node_C);

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

				// there is a complete graph with node_A,
				// node_B, and node_C
				if (node_C != -1)
				{
					if (verbose) cout << "node A, B, C (" << node_A
					<< ", " << node_B << ", " << node_C
					<< ") are eachothers neighbours" << endl;

					// move neighbours from node B to node A
					// copy from C to A
					// copy from A to C
					// cut edge between A to C

					// move from node_B to node A (not node C)
					copy_neighbours(numbers, node_B, node_A, true, node_C);

					if (verbose) { cout << "step 1 complete, table:" << endl;
						print(numbers); cout << endl;
					}

					// copy from node_C to node_A (not node B)
					copy_neighbours(numbers, node_C, node_A, false, node_B);
					if (verbose) { cout << "step 2 complete, table:" << endl;
						print(numbers); cout << endl;
					}

					// copy from node_A to node_C (not node B)
					copy_neighbours(numbers, node_A, node_C, false, node_B);

					if (verbose) { cout << "step 3 complete, table:" << endl;
						print(numbers); cout << endl;
					}

					// remove connection between node_A and node_C
					remove_neighbour(numbers, node_A, node_C);

					// add neighbours A and C to node_B
					add_neighbour(numbers, node_B, node_A);
					add_neighbour(numbers, node_B, node_C);
					//numbers[node_B].push_back(node_A);
		 			//numbers[node_B].push_back(node_C);

					// removing duplicates from node_A and node_C
					remove_duplicate_neighbours(numbers, node_A, node_B);
					vector<int> tripples = remove_duplicate_neighbours(numbers, node_C, node_B);

					if (verbose) { cout << "first step complete, table:" << endl;
						print(numbers); cout << endl;
					}

					if (verbose) {
						cout << "these tripples were found:" << endl;
						for (int i : tripples) cout << i << " ";
						cout << endl;
					}

					if (verbose) cout << "add tripples to chain" << endl;
					add_to_chain(numbers, tripples, node_A, node_B, node_C);

					if (verbose) cout << "sub-finished table";
					if (verbose) { print(numbers); cout << endl; }

					break;
				}
			}
		}
	}

	if (verbose) cout << "final table" << endl;
	if (verbose) print(numbers);

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
}

bool contains(vector<int>& neighbours, int node)
{
	for (int i { 1 }; i<neighbours.size(); ++i) { if (neighbours[i] == node) return true; }

	return false;
}

void add_to_chain(vector<vector<int>>& numbers, vector<int>& tripples, int node_A, int node_B, int node_C)
{
	// get the neighbours of the tripples and add them to node A and C
	for (int i{}; i<tripples.size(); ++i)
	{
		for (int k{ 1 }; k<numbers[tripples[i]].size(); ++k)
		{
			int neigh = numbers[tripples[i]][k];

			if (neigh != node_A && neigh != node_C && !contains(numbers[node_A], neigh))
			{
				add_neighbour(numbers, node_A, neigh);
				add_neighbour(numbers, node_C, neigh);

				// neighbour of the tripple removes the tripple as its neighbour
				remove_neighbour(numbers, neigh, tripples[i]);
			}
		}

		// remove all the tripples neighbours
		//int value = numbers[tripples[i]][0];
		//numbers[tripples[i]].clear();
		//numbers[tripples[i]].push_back(value);

		for (long unsigned int k { numbers[tripples[i]].size() - 1 }; k>0; --k) {
			remove_neighbour(numbers, tripples[i], numbers[tripples[i]][k]);
		}
	}

	if (verbose) cout << "the neighbours of the tripples have been moved to node A and C" << endl;
	if (verbose) { print(numbers); cout << endl; }

	// form the chain : add i to i+1's neighbour, i+1 to i+2, etc.
	for (int i{}; i<tripples.size(); ++i)
	{
		// A and B removes each other as neighbours
		remove_neighbour(numbers, node_A, node_B);

		//remove_neighbour(numbers[node_C], tripples[i]);
		remove_neighbour(numbers, node_C, tripples[i]);

		if (verbose) cout << "A and B removes eeach other as neighbours" << endl;
		if (verbose) { print(numbers); cout << endl; }

		// tripple adds A and B as neighbours
		//numbers[tripples[i]].push_back(node_A);
		//numbers[tripples[i]].push_back(node_B);
		add_neighbour(numbers, tripples[i], node_A);
		add_neighbour(numbers, tripples[i], node_B);

		if (verbose) cout << "tripple adds A and B as neighbours" << endl;
		if (verbose) { print(numbers); cout << endl; }

		// B: adds tripple as neighbour
		//numbers[node_B].push_back(tripples[i]);
		add_neighbour(numbers, node_B, tripples[i]);

		if (verbose) cout << "A and B adds tripple as neighbour" << endl;
		if (verbose) { print(numbers); cout << endl; }

		// numbers[tripples[0]] is the new node_B
		node_B = tripples[i];
	}
}

vector<int> remove_duplicate_neighbours(vector<vector<int>>& numbers, int node, int node_B)
{
	vector<int> tripples{};

	sort(numbers[node].begin() + 1, numbers[node].end());

	for (int i{ 1 }; i<numbers[node].size(); ++i)
	{
		int nums = count(numbers[node].begin() + 1, numbers[node].end(), numbers[node][i]);

		//remove node B from neigh here
		if (nums > 2)
		{
			tripples.push_back(numbers[node][i]);
			remove_neighbour(numbers, node, numbers[node][i]);
			remove_neighbour(numbers, node_B, numbers[node][i]);
		}
	}

	auto last = unique(numbers[node].begin() + 1, numbers[node].end());
	numbers[node].erase(last, numbers[node].end());

	return tripples;
}

void remove_neighbour(vector<vector<int>>& numbers, int node1, int node2)
{
	numbers[node1].erase(find(numbers[node1].begin() + 1, numbers[node1].end(), node2));
	numbers[node2].erase(find(numbers[node2].begin() + 1, numbers[node2].end(), node1));
}

void add_neighbour(vector<vector<int>>& numbers, int node1, int node2)
{
	if (!contains(numbers[node1], node2) && node1 != node2)
		numbers[node1].push_back(node2);

	if (!contains(numbers[node2], node1) && node1 != node2)
		numbers[node2].push_back(node1);
}

void copy_neighbours(vector<vector<int>>& numbers, int source_node, int destination_node, bool move, int dont_cp)
{
	for (int i{ 1 }; i<numbers[source_node].size(); ++i)
	{
		// dont copy the destination index to the destination vector
		if (numbers[source_node][i] != dont_cp)
		{
			add_neighbour(numbers, destination_node, numbers[source_node][i]);
		}
	}

	// optionally erases the source vector
	if (move)
	{
		int val = numbers[source_node][0];
		numbers[source_node].clear();
		numbers[source_node].push_back(val);

		//for (long unsigned int k { numbers[source_node].size() - 1 }; k>0; --k) {
		//	remove_neighbour(numbers, source_node, numbers[source_node][k]);
		//}

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
