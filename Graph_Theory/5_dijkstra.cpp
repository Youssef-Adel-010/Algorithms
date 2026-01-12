#include<bits/stdc++.h>
using namespace std;


/*
  * Single Source Shortest Path Algorithm → From one source to all others.
  *
  * Dijkstra only works for positive weights.
  *
  * A shortest path between two vertices contains other shortest paths within it.
  *
  * In a graph of N nodes, longest path will have N-1 edges.
  *
  * A shortest path will never have a cycle (+ve or -ve).
  *
  * Some algorithms could handle -ve cycles (e.g. Floyd, Bellman), Others Can't (e.g. Dijkstra)
  *
  * Say we have graph of n nodes. We want to remove node X, but we need the graph doesn't miss any information about paths!
  * If node has A edges entering it, and B edges outing it -> Add A * B edges, then we don't miss any info.
  *
  * Dijkstra → Greedy + Relaxation
  *
  * if(d > dist[u]) continue; → Used to optimize the algorithm to prevent processing the processed nodes.
  * this line can be replaced with using a visited array.
  *
*/
vector<vector<pair<int, int>>> graph;
vector<int> dist;
vector<int> prevv;
const int INF = INT_MAX;
int n;

void dijkstra(int src) {
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

  dist.assign(n, INF);
  dist[src] = 0;
  prevv.assign(n, -1);
  pq.push({0, src});

  while(!pq.empty()) {
    auto [d,u] = pq.top();
    pq.pop();

    if(d > dist[u]) continue; // can be replaced with visited array

    for(auto &[v,w] : graph[u]) {
      if(dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
        prevv[v] = u;
        pq.push({dist[v], v});
      }
    }
  }
}

void printPath(int d) {
  if(prevv[d] == -1) {
    cout << d << " ";
    return;
  }
  printPath(prevv[d]);
  cout << d << " ";
}

int main() {
  n = 6;
  graph.assign(n, {});

  auto addEdge = [&](int u, int v, int w) {
    graph[u].push_back({v, w});
    graph[v].push_back({u, w});
  };

  // A = 0
  // B = 1
  // C = 2
  // D = 3
  // E = 4
  // F = 5

  addEdge(0, 1, 2);  // A - B
  addEdge(0, 3, 8);  // A - D

  addEdge(1, 3, 5);  // B - D
  addEdge(1, 4, 6);  // B - E

  addEdge(3, 4, 3);  // D - E
  addEdge(3, 5, 2);  // D - F

  addEdge(4, 5, 1);  // E - F
  addEdge(4, 2, 9);  // E - C

  addEdge(5, 2, 3);  // F - C

  int src = 0; // A
  dijkstra(src);

  cout << "Shortest distances from A:\n";
  for (int i = 0; i < n; ++i) {
    cout << "A -> " << i << " = " << dist[i] << "\n";
  }

  cout << "\nShortest path from A to C:\n";
  printPath(2);
  cout << "\n";

  return 0;
}
