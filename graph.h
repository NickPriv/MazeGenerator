#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>

class graph
{
public:
    typedef std::size_t T;
    typedef std::unordered_set<T> AdjacentType;
    typedef std::pair<T, T> Edge;
    typedef std::pair<std::size_t, std::size_t> coord;

    graph()
    {
    }

    // pre: none
    // post: returns number of vertices in this graph
    std::size_t n() const
    {
        return _t.size();
    }

    // pre: none
    // post: returns number of edges in this graph
    std::size_t m() const
    {
        std::size_t ans(0);

        for (auto e: _t)
            ans += e.second.second.size();

        return ans/2;

    }

    // pre: v is a vertex of this graph
    // post: returns the number of vertices adjacent to v in this graph
    std::size_t deg(const T & v) const
    {
        assert(_t.find(v) != _t.end());
        return _t.at(v).second.size();
        // t[v].size()
    }

    coord coor(const T & v) const
    {
        return std::make_pair(_t.at(v).first.first, _t.at(v).first.second);
    }

    // post: returns name of vertex located at that coordinate
    T index(coord & c, int n, int m) const
    {
        return m*c.first+c.second;
    }

    bool adjacent(const T & v, const T & w)
    {
        int x_amt = abs(coor(v).first - coor(w).first);
        int y_amt = abs(coor(v).second - coor(w).second);
        return (x_amt == 1 && y_amt == 0) || (y_amt == 1 && x_amt == 0);
    }

    // pre:  v is NOT a vertex of this graph
    // post: new vertex v has been added to this graph
    void add_vertex(const T & v, std::size_t x, std::size_t y)
    {
        if(_t.find(v) != _t.end())
            return;
        _t[v].first.first = x;
        _t[v].first.second = y;
        _t[v].second = AdjacentType();
    }

    // pre: s, d are vertices of this graph and {s, d} is not an edge
    // post: {s, d} has been added to this graph
    void add_edge(const T & s, const T & d)
    {
        if (!(_t.find(s) != _t.end() && _t.find(d) != _t.end()))
            return;
        _t[s].second.insert(d);
        _t[d].second.insert(s);
    }

    // pre: s and d are vertices and {s, d} is an edge
    // post: {s, d} has been removed from this graph
    void remove_edge(const T & s, const T & d)
    {
        assert(_t.find(s) != _t.end() && _t.find(d) != _t.end());
        _t[s].second.erase(d);
        _t[d].second.erase(s);
    }

    // pre: v is a vertex
    // post: v and edges with v as an endpoint have been removed from this graph
    void remove_vertex(const T & v)
    {
        assert(_t.find(v) != _t.end());
        for (auto e: _t)
            e.second.second.erase(v);
        _t.erase(v);
    }

    // pre: none
    // post: returns a vector containing the vertices of this graph
    std::vector<T> V() const
    {
        std::vector<T> ans;

        for (auto e: _t)
            ans.push_back(e.first);

        return ans;
    }

    // pre: none
    // post: returns a vector containing the edges of this graph
    std::vector<Edge> E() const
    {
        std::vector<Edge> ans;

        for (auto v: V())
        {
            for (auto n: Adj(v))
            {
                Edge reverse;
                reverse.first = n;
                reverse.second = v;
                if (std::find(ans.begin(), ans.end(), reverse) == ans.end())
                {
                    Edge current;
                    current.first = v;
                    current.second = n;
                    ans.push_back(current);
                }
            }
        }
        return ans;
    }

    // pre: v is a vertex of this graph
    // post: the vertices adjacent to v are returned
    AdjacentType Adj(const T & v) const
    {
        assert(_t.find(v) != _t.end());

        return _t.at(v).second;
    }

   graph dfs(const T & v, std::size_t x, std::size_t y)
   {
       std::stack<T> s;
       std::unordered_map<T, T> parent;
       std::unordered_map<T, std::size_t> pre, post;
       graph ans;
       std::srand(std::time(NULL));

       std::size_t time(1);

       s.push(v);
       ans.add_vertex(v, x, y);
       std::unordered_set<T> visited;
       visited.insert(v);

       while (!s.empty())
       {
           T top = s.top();
           if (top != 0)
                ans.add_edge(parent[top],top);
           if (pre.find(top) == pre.end()) // has not been discovered
           {
               pre[top] = time++;
               AdjacentType neighbors = Adj(top);
               std::vector<T> nb;
               for (auto n: neighbors)
                   if (pre.find(n) == pre.end())
                   {
                       nb.push_back(n);
                       parent[n] = top;
                       ans.add_vertex(n, coor(n).first, coor(n).second);
                   }
               std::random_shuffle(nb.begin(),nb.end());
               for (auto e: nb)
                   s.push(e);
           }
           else
           {
               if (post.find(top) == post.end())
                   post[top] = time++;
               s.pop();
           }
       }
       return ans;
   }

    // post: Returns farthest vertex from start
    T diameter() const
    {

        std::queue<T> q;
        std::unordered_map<T, std::size_t> distance_a, distance_b;

        q.push(0);
        distance_a[q.front()] = 0;

        T last;

        while(!q.empty())
        {
            T front = q.front();
            for (auto v: Adj(front))
            {
                if (distance_a.find(v) == distance_a.end())
                {
                    distance_a[v] = distance_a[front]+1;
                    q.push(v);
                }
            }
            last = q.front();
            q.pop();
        }

        /*q.push(last);
        distance_b[q.front()] = 0;

        while(!q.empty())
        {
            T front = q.front();
            for (auto v: Adj(front))
            {
                if (distance_b.find(v) == distance_b.end())
                {
                    distance_b[v] = distance_b[front]+1;
                    q.push(v);
                }
            }
            last = q.front();
            q.pop();
        }*/
        return last;
        //return distance_b[last];
    }

    void print(graph G, int n, int m)
    {
        int len = 2*n+1;
        int wid = 2*m+1;
        char a[len][wid];
        for (int i=0; i<len; i++)
            for (int j=0; j<wid; j++)
                a[i][j] = 'X';
        for (auto v: G.V())
            a[2*G.coor(v).first+1][2*G.coor(v).second+1] = ' ';
        for (auto e: G.E())
        {
            T v = e.first;
            T w = e.second;
            int x_amt = abs(G.coor(v).first - G.coor(w).first);
            int y_amt = abs(G.coor(v).second - G.coor(w).second);
            int x, y;
            if (x_amt == 1 && y_amt == 0) // If horizontally adjacent
            {
                if (G.coor(v).first > G.coor(w).first)
                    x = 2*G.coor(v).first;
                else
                    x = 2*G.coor(v).first + 2;
                y = 2*G.coor(v).second+1;
            }
            else if (x_amt == 0 && y_amt == 1) // If vertically adjacent
            {
                if (G.coor(v).second > G.coor(w).second)
                    y = 2*G.coor(v).second;
                else
                    y = 2*G.coor(v).second + 2;
                x = 2*G.coor(v).first+1;
            }
            a[x][y] = ' ';
        }
        a[1][1] = 'S';
        T last = G.diameter();
        a[2*coor(last).first+1][2*coor(last).second+1] = '?';

        for (int i=0; i<wid; i++)
        {
            for (int j=0; j<len; j++)
            {
                std::cout << a[j][i] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    std::unordered_map<T, std::pair<coord, AdjacentType>> _t;
};

std::ostream & operator << (std::ostream & os, const graph & G)
{
    os << G.n() << " " << G.m() << std::endl;
    for (auto v: G.V())
        os << v << " ";
    os << std::endl;
    for (auto v: G.V())
    {
        os << v << ": ";
        for (auto e: G.Adj(v))
            os << e << " ";
        os << std::endl;
    }
    return os;
}
/*std::istream & operator >> (std::istream & is, graph & G)
{
    std::size_t n, m, x, y;
    is >> n >> m;
    std::string v, w;

    for (std::size_t i = 1; i <= n; ++i)
    {
        is >> v >> x >> y;
        G.add_vertex(v, x, y);
    }
    for (std::size_t i = 1; i <= m; ++i)
    {
        is >> v >> w;
        G.add_edge(v, w);
    }
    return is;
}*/

#endif // GRAPH_H
