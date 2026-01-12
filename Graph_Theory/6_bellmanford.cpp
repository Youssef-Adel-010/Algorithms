#include<bits/stdc++.h>
using namespace std;

struct Edge {
  int from, to, weight;
};
int n;
vector<Edge> edges;
vector<long long> dist;
const long long INF = LLONG_MAX;

void bellmanFord(int src) {
  dist.assign(n, INF);
  dist[src] = 0;

  for (int i = 0; i < n - 1; ++i) {
    for(auto &e: edges) {
      if(dist[e.from] < INF && dist[e.to] > dist[e.from] + e.weight) {
        dist[e.to] = dist[e.from] + e.weight;
      }
    }
  }
  for (int i = 0; i < n - 1; ++i) {
    for(auto &e: edges) {
      if(dist[e.from] < INF && dist[e.to] > dist[e.from] + e.weight) {
        dist[e.to] = -INF;
      }
    }
  }

}

int main() {
  /*
    Graph:
      0 → 1 (1)
      1 → 2 (1)
      2 → 3 (1)
      3 → 1 (-4)   ← negative cycle (1 → 2 → 3 → 1)
      2 → 4 (2)

    src = 0
  */

  n = 5;

  edges.push_back({0, 1, 1});
  edges.push_back({1, 2, 1});
  edges.push_back({2, 3, 1});
  edges.push_back({3, 1, -4}); // negative cycle
  edges.push_back({2, 4, 2});

  bellmanFord(0);

  cout << "Distances from source 0:\n";
  for (int i = 0; i < n; ++i) {
    if (dist[i] == INF)
      cout << "Node " << i << ": INF (unreachable)\n";
    else if (dist[i] == -INF)
      cout << "Node " << i << ": -INF (affected by negative cycle)\n";
    else
      cout << "Node " << i << ": " << dist[i] << "\n";
  }

  return 0;
}