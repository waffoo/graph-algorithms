#include <cassert>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Edge {
    int to, cap, rev;
};

class Dinic {
    vector<vector<Edge>>& graph;
    vector<int> level;
    constexpr static int INF = INT_MAX;

    int dfs(int v, int t, int f) {
        cout << "vertex " << v << std::endl;
        if (v == t) {
            cout << "return " << f << endl << endl;
            return f;
        }

        for (Edge& e : graph[v]) {
            if (level[v] < level[e.to] and e.cap > 0) {
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

    void bfs(int s) {
        for (size_t i = 0; i < level.size(); i++) level[i] = i == 0 ? 0 : -1;
        queue<int> q;
        q.push(s);
        while (not q.empty()) {
            int v = q.front();
            q.pop();
            for (auto e : graph[v]) {
                if (e.cap > 0 and level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
    }

public:
    Dinic(vector<vector<Edge>>& g) : graph(g) {}

    int compute(int source, int sink) {
        int flow = 0;
        level = vector<int>(graph.size());

        while (bfs(source), level[sink] >= 0) {
            int f = -1;
            while (f != 0) {
                f = dfs(source, sink, INF);
                flow += f;
            }
        }
        return flow;
    }
};

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;

    vector<vector<Edge>> graph(n);
    int from, to, cap;

    for (int i = 0; i < m; i++) {
        cin >> from >> to >> cap;
        graph[from].emplace_back(Edge{to, cap, (int)graph[to].size()});
        graph[to].emplace_back(Edge{from, 0, (int)graph[from].size() - 1});
    }

    Dinic dinic{graph};
    int flow = dinic.compute(s, t);

    int ans;
    cin >> ans;
    cout << "\nAlgorithm returns " << flow << endl;
    cout << "Answer is " << ans << endl;
    assert(ans == flow);
    return 0;
}