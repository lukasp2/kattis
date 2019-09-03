#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <unordered_map>

using namespace std;

void get_input_values(vector<vector<int>>& v);
void calculate_neighbours(vector<vector<int>>& numbers, int D, int M);
int depth_first_search(vector<vector<int>>& numbers);
bool BFS_set_candidate(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, int node_idx, int search_type);
bool check_nw_size(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, int node_idx);
void analyze(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, int& largest_nw);

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

	unordered_map<int, bool> candidates{};
	int largest_nw{};
	analyze(candidates, numbers, largest_nw);

	for (int start{}; start<numbers.size(); ++start) if ( candidates[start] == true ) {
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

		if (max_hops == largest_nw) break;
	}}

	return max_hops;
}

void check_nw_size(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers,
	unordered_map<int, bool>& visited, int node_idx, int& nw_size) {

	visited[node_idx] = true;

	queue<int> q{}; q.push(node_idx);
	while(!q.empty()) {
		int curr { q.front() }; q.pop();

		nw_size++;

		for (int i{1}; i < numbers[curr].size(); i++) {
			if (!visited[numbers[curr][i]]) {
				q.push(numbers[curr][i]);
				visited[numbers[curr][i]] = true;
			}
		}
	}
}

bool BFS_set_candidate(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers,
	unordered_map<int, bool>& visited, int node_idx, int search_type) {

	visited[node_idx] = true;

	queue<int> q{}; q.push(node_idx);
	while(!q.empty()) {
		int curr { q.front() }; q.pop();

		if (search_type == 0) { if (candidates[curr]) { return true; } }
		if (search_type == 1) { candidates[curr] = true; }

		for (int i{1}; i < numbers[curr].size(); i++) {
			if (!visited[numbers[curr][i]]) {
				q.push(numbers[curr][i]);
				visited[numbers[curr][i]] = true;
			}
		}
	}
	return false;
}

// excludes nodes that cannot be a starting node in the longest path
void analyze(unordered_map<int, bool>& candidates, vector<vector<int>>& numbers, int& largest_nw)
{
	for (int i{}; i<numbers.size(); ++i) {
		if ( numbers[i].size() == 2 ) {
			candidates[i] = true;
		}
	}

	unordered_map<int, bool> visited{};
	for (int i{}; i<numbers.size(); ++i) {
		// for each network we have not yet visited
		if (!visited[i]) {
			// if there is no node that is a candidate in the nw
			if (!BFS_set_candidate(candidates, numbers, visited, i, 0)) {
				//set all nodes in the nw to be a candidate
				BFS_set_candidate(candidates, numbers, visited, i, 1);
			}
		}
	}

	visited = unordered_map<int, bool>{};
	for (int i{}; i<numbers.size(); ++i) {
		int nw_size{};
		if (!visited[i]) {
			check_nw_size(candidates, numbers, visited, i, nw_size);
		}

		if (nw_size > largest_nw) {
			largest_nw = nw_size;
		}
	}
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
