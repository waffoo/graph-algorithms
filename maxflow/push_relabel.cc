#include <cassert>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Edge {
    int to, cap, rev;
};

class PushRelabel {
    vector<vector<Edge>>& graph;
    vector<int> level;
    vector<int> excess;
    queue<int> q;
    int flow;
    int start;
    int goal;
    constexpr static int INF = INT_MAX - 10000;

    void push(int from, Edge& e) {
        int f = min(excess[from], e.cap);
        e.cap -= f;
        graph[e.to][e.rev].cap += f;
        excess[from] -= f;
        excess[e.to] += f;
        if (e.to == goal) flow += f;
        if (e.to != start and e.to != goal) q.push(e.to);
    }

    void relabel(int v) {
        bool need_relabel = true;
        int min_level = INF;
        for (Edge& e : graph[v]) {
            if (e.cap > 0) {
                if (level[v] > level[e.to]) {
                    need_relabel = false;
                    break;
                } else {
                    min_level = min(min_level, level[e.to]);
                }
            }
        }
        if (need_relabel) level[v] = min_level + 1;
    }

public:
    PushRelabel(vector<vector<Edge>>& g) : graph(g) {}

    int compute(int source, int sink) {
        q = {};
        flow = 0;
        start = source;
        goal = sink;
        level = vector<int>(graph.size());
        excess = vector<int>(graph.size());
        level[source] = graph.size();

        for (Edge& e : graph[source]) {
            q.push(e.to);
            int f = e.cap;
            graph[e.to][e.rev].cap += f;
            excess[e.to] = f;
            e.cap = 0;
        }

        while (not q.empty()) {
            int v = q.front();
            q.pop();
            if (not(excess[v] > 0)) continue;

            relabel(v);

            for (Edge& e : graph[v])
                if (e.cap > 0 and level[v] == level[e.to] + 1) this->push(v, e);

            if (excess[v] > 0) q.push(v);
        }

        return flow;
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

    PushRelabel pr{graph};
    int flow = pr.compute(s, t);

    int ans;
    cin >> ans;
    cout << "\nAlgorithm returns " << flow << endl;
    cout << "Answer is " << ans << endl;
    assert(ans == flow);
    return 0;
}