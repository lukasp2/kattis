#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <array>

using namespace std;

#define forest_state size_t

struct Step
{
    forest_state prev_value{ 0 };
    size_t operation{};
};

struct Pair
{
    Pair(size_t s) : first{ s }, second{ static_cast<size_t>(pow(2, s)) } {}
    size_t first;
    size_t second;
};

int main()
{
    size_t num_trees;
    size_t num_connections;
    size_t con_A;
    size_t con_B;

    vector<string> result;

    while (cin >> num_trees >> num_connections)
    {
        if (num_trees == 0 && num_connections == 0) break;

        //start == 11..num_trees..11 but in decimal
        forest_state start{ static_cast<size_t>(pow(2, num_trees)) - 1 };

        //Create Adjecency List (a vector of trees with a vector of respective trees neighbours)
        array<vector<Pair>, 21> forest;
        for (size_t i{}; i < num_connections; ++i)
        {
            cin >> con_A >> con_B;
            forest[con_A].push_back(Pair{ con_B });
            forest[con_B].push_back(Pair{ con_A });
        }

        if (num_trees - 1 == num_connections)
        {
            //Log all nodes as unvisited except start
            unordered_map < forest_state, bool > visited{};
            visited[start] = true;

            //Create pathmap
            unordered_map< forest_state, Step> path{};
            path[start].operation = -1;

            forest_state new_curr{ 0 };

            queue< forest_state > q; q.push(start);
            while (!q.empty())
            {
                forest_state curr{ q.front() }; q.pop();
                forest_state save{ curr };

                //For all trees that might contain a monkey
                for (size_t i{}; i < num_trees; ++i)
                {
                    //If tree contains monkey
                    if ((curr >> i) & 1 )
                    {
                        //Tree is shot, cannot contain monkey
                        curr &= ~(1 << i);

                        //Check victory
                        if (!curr) //curr == 0
                        {
                            path[curr] = Step{ save, i };
                            q = queue< forest_state >();
                            break;
                        }

                        //Go through trees; if tree can contain monkey; move monkey to all adjacent trees
                        for (size_t k{}; k < num_trees; ++k)
                        {
                            //if tree can contain monkey 
                            if ( (curr >> k) & 1)
                            {
                                //go through neighbours 
                                size_t f_size{ forest[k].size() };
                                for (size_t a{}; a < f_size; ++a)
                                {
                                    new_curr |= 1 << forest[k][a].first;
                                }
                            }
                        }

                        //If the result is unique, i.e. it is unvisited
                        if (!visited[new_curr])
                        {
                            visited[new_curr] = true;
                            q.push(new_curr);
                            path[new_curr] = Step{ save, i };
                        }

                        new_curr = 0;
                        curr = save;
                    }
                }
            }

            //If prev_value of the state where no monkeys are left in the forest is EMPTY, there is no solution
            if (path[0].prev_value != 0)
            {
                vector<string> v{};
                for (forest_state f{ 0 }; path[f].operation != -1; f = path[f].prev_value)
                    v.push_back(to_string(path[f].operation));

                string step_string{};
                for (size_t i{ 1 }; i <= v.size(); ++i)
                    step_string += " " + v[v.size() - i];

                result.push_back(to_string(v.size()) + ":" + step_string);
            }
            else
                result.push_back("Impossible");
        }
        else
            result.push_back("Impossible");
    }

    for (string& s : result)
        cout << s << endl;

    return 0;
}
