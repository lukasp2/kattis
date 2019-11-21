#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool fits_in_room(pair<int,int>& room, pair<int,int>& minion) {
	return minion.first <= room.second;
}

int main() {

	int n;
	cin >> n;

	vector<pair<int,int>> minions{};

	while(n--) {
		pair<int, int> p{};
		cin >> p.first >> p.second;
		minions.push_back(p);
	}

	sort(minions.begin(), minions.end(), [](pair<int, int>& p1, pair<int, int>& p2) {
		if (p1.second == p2.second)
			return p1.first < p2.first;
		return p1.second < p2.second;
	});

	int rooms{ 1 };
	pair<int,int> curr_room = minions[0];

	for (auto p : minions) {
		if (!fits_in_room(curr_room, p)) {
			rooms++;
			curr_room = p;
		}
	}

	cout << rooms << endl;

	return 0;
}
