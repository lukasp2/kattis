#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#define ARR_SIZE 10
#define IDX_JMP 7
#define VAL_JMP 1

using namespace std;
int main() {
	std::srand(std::time(nullptr));
    	int random_variable = std::rand();

	vector<int> input{};
	for (int i{}; i<ARR_SIZE; ++i) {
		int rand_n = 1 + rand()/((RAND_MAX + 1u)/10);
		input.push_back(rand_n);
	}

	cout << ARR_SIZE << " " << IDX_JMP << " " << VAL_JMP << endl;

	for (int i{}; i<input.size(); ++i)
	{
		cout << " " << input[i];
	}
}
