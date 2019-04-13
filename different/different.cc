#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

typedef vector<char> vchar;
typedef long unsigned int ulong;

vchar& subtract(vchar& a, vchar& b);
void steal(vchar& a, int i);
void print(vchar& a);
bool operator > (vchar& a, vchar& b);
bool operator == (vchar& a, vchar& b);

int main() {
	string line{};
	vector<vchar> answers{};

	while (getline(cin, line)) {
		vchar n1;
		vchar n2;

		auto space = find(line.begin(), line.end(), ' ');
		copy(line.begin(), space, back_inserter(n1));
		copy(space + 1, line.end(), back_inserter(n2));

		vchar result = (n1 > n2) ? subtract(n1, n2) : subtract(n2, n1);

		answers.push_back(result);
	}

	for (vchar v : answers) {
		print(v);
		cout << endl;
	}
}

vchar& subtract(vchar& a, vchar& b) {
	if (a == b) {
		a.clear();
		a.push_back('0');
		return a;
	}

	for (int i{1};i<=b.size();i++) {
		ulong a_idx = a.size() - i;
		ulong b_idx = b.size() - i;

                if (b[b_idx] > a[a_idx])
                        steal(a, a_idx);

 	        a[a_idx] = char(a[a_idx] - b[b_idx] + 48);
        }
	return a;
}

void steal(vchar& a, int give) {
	ulong take = give - 1;

	if (a[take] == '0')
		steal(a, give - 1);

        a[take] = char(a[take] - 1);
        a[give] = char(a[give] + 10);
}

void print(vchar& a) {
	bool num_start = (a.size() > 1) ? false : true;
	for (int i{};i<a.size();i++)
	{
		if (a[i] != '0')
			num_start=true;

		if (num_start == true)
			cout << a[i];
	}
}

bool operator > (vchar& a, vchar& b) {
	ulong as{ a.size() };
	ulong bs{ b.size() };

	if (as > bs)
		return true;
	if (bs > as)
		return false;

	for (int i{};i<a.size();i++)
	{
		if (a[i] > b[i])
			return true;
	}

	return false;
}

bool operator == (vchar& a, vchar& b) {
	return !(a > b) && !(b > a);
}

