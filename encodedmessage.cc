// https://open.kattis.com/problems/encodedmessage

#include <cmath>
#include <iostream>
#include <string>
using namespace std;

int main() {
        int cases;
        cin >> cases;

        while(cases--) {
                string s;
                cin >> s;

                int size = sqrt(s.size());

                for (int i{size-1};i>=0;i--)
                        for (int k{0};k<size;k++)
                                cout << s[i+k*size];
                cout << endl;
        }
}
