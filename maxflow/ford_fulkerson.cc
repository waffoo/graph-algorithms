#include <cassert>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    int to, cap, rev;
};

class FordFulkerson {
    vector<vector<Edge>>& graph;
    vector<bool> used;
    constexpr static int INF = INT_MAX;

    int dfs(int v, int t, int f) {
        cout << "vertex " << v << std::endl;
        if (v == t) {
            cout << "return " << f << endl << endl;
            return f;
        }
        used[v] = true;

        for (Edge& e : graph[v]) {
            if (not used[e.to] and e.cap > 0) {
                int d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    graph[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

public:
    FordFulkerson(vector<vector<Edge>>& g) : graph(g) {}

    int compute(int source, int sink) {
        int flow = 0;
        while (true) {
            used = vector<bool>(graph.size());
            int f = dfs(source, sink, INF);
            if (f == 0) return flow;
            flow += f;
        }
    }
};

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;

    int from, to, cap;
    vector<vector<Edge>> graph(n);

    for (int i = 0; i < m; i++) {
        cin >> from >> to >> cap;
        graph[from].emplace_back(Edge{to, cap, (int)graph[to].size()});
        graph[to].emplace_back(Edge{from, 0, (int)graph[from].size() - 1});
    }

    FordFulkerson ford{graph};
    int flow = ford.compute(s, t);

    int ans;
    cin >> ans;
    cout << "\nAlgorithm returns " << flow << endl;
    cout << "Answer is " << ans << endl;
    assert(ans == flow);
    return 0;
}