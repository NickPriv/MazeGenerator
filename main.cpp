#include <iostream>
#include "graph.h"

using namespace std;

int main()
{
    size_t n, m, index(0); // create a nxm graph

    cout << "Enter dimensions for maze (both numbers must be greater than 0, and their product should be at most 1,000): " << endl;
    cin >> n >> m;

    graph G;
    for (size_t x=0; x<n; x++)
    {
        for (size_t y=0; y<m; y++)
        {
            G.add_vertex(index, x, y);
            for (auto v: G.V())
            {
                if (G.adjacent(index, v))
                    G.add_edge(index,v);
            }
            ++index;
        }
    }
    cout << "Start at the S and find your way to the question mark." << endl;
    G.print(G.dfs(0,0,0),n,m);

    return 0;
}
