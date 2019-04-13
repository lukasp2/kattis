#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>

using namespace std;
int main()
{
    int jack_cds, jill_cds;
    vector<int> result;

    while (cin >> jack_cds >> jill_cds)
    {
        if (jack_cds == 0 && jill_cds == 0)
            break;

        int x;
        vector<int> v;

        for (int i{}; i < jack_cds + jill_cds; ++i)
        {
            cin >> x;
            v.push_back(x);
        }
        
        sort(v.begin(), v.end());
        auto last = std::unique(v.begin(), v.end());
        result.push_back(distance(last, v.end()));
    }


    for (int i : result)
        cout << i << endl;

    return 0;
}
