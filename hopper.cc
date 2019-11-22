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
int depth_first_search(vector<vector<int>>& numbers);

class path {
public:
	path() : nodes{}, record_depth{}, nodes_since_last_branch{} {}
	void add_node(int node_idx, vector<vector<int>>& n) {
		nodes.push_back(node_idx);
		if (n[node_idx].size()-1 > 2) nodes_since_last_branch=0; else nodes_since_last_branch++;
	}
	int get_size() {return nodes.size();}
	void backtrack() {while(nodes_since_last_branch--) nodes.pop_back();}
	void print() {cout<<nodes[0];for(int i{1};i<nodes.size();i++)cout<<"->"<<nodes[i];}
	bool contains(int node_idx) {for(int i:nodes){if(i==node_idx)return true;}return false;};
	void log_depth() {if(nodes.size()>record_depth){record_depth=nodes.size();}};
	int get_record() {return record_depth;}
	int get_nodes_since_last_branch() {return nodes_since_last_branch;}
private:
	vector<int> nodes{};
	int record_depth{};
	int nodes_since_last_branch{};
};

int main()
{
	int n, D, M;
	cin >> n >> D >> M;

	vector<vector<int>> numbers(n);
	get_input_values(n, numbers);

	calculate_neighbours(numbers, D, M);
	if (verbose) print(numbers);
	int hops = depth_first_search(numbers);
	cout << hops << endl;
}

// performs a DFS search for the longest exploration sequence
int depth_first_search(vector<vector<int>>& numbers)
{
	int max_steps{};
	bool found{};

	// loop through all unvisited nodes in the graph
	for (int start{}; start<numbers.size(); ++start)
	{
		if (verbose) cout << endl << "node: " << start << endl;

		path path{};
		unordered_map<int, bool> visited{};

		stack<int> s{}; s.push(start);
		while (!s.empty())
		{
			int curr{ s.top() }; s.pop();

			path.add_node(curr, numbers);
			visited[curr] = true;
			bool dead_end{true};

			if (verbose) { cout << "\tpopped: " << curr << ", depth " << path.get_size() << endl;
					cout << "current path: "; path.print(); cout << endl; }

			// push all unvisited neighbours to the stack
			for (int neighbour{1}; neighbour<numbers[curr].size(); ++neighbour)
			{
				//if (!visited[ numbers[curr][neighbour] ]) {
				if (!path.contains(numbers[curr][neighbour]) || !visited[numbers[curr][neighbour]]) {
					s.push(numbers[curr][neighbour]);
					visited[ numbers[curr][neighbour] ] = true;
					dead_end=false;
					if (verbose) { cout << "\t\tpushed: " << numbers[curr][neighbour] << endl; }
				}
			}

			if (dead_end) {
				path.log_depth();

				if (path.get_nodes_since_last_branch() != 0) { path.backtrack(); }
				else { found = true; break; }

				//cout << "backtrack!" << endl;
			}
		}

		if (verbose) cout << "\trecord depth: " << path.get_record() << endl;

		if (path.get_record() > max_steps)
			max_steps = path.get_record();

		if (found || max_steps == numbers.size())
			break;
	}

	return max_steps;
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
